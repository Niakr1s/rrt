#include "xmltreeitem.h"

#include <QObject>
#include <boost/log/trivial.hpp>

XMLTreeItem::XMLTreeItem(const std::string& strID, XMLTreeItem* parentItem)
    : parent_(parentItem), strID_(strID), spatial_(nullptr) {}

XMLTreeItem::~XMLTreeItem() {
  qDeleteAll(childs_);
}

void XMLTreeItem::appendChild(XMLTreeItem* child) {
  childs_.append(child);
}

XMLTreeItem* XMLTreeItem::child(int row) {
  if (row < 0 || row >= childs_.size())
    return nullptr;
  return childs_.at(row);
}

int XMLTreeItem::childCount() const {
  return childs_.count();
}

int XMLTreeItem::columnCount() const {
  return Column::MAX;
}

int XMLTreeItem::childCountThatHaveSpatial() const {
  int res = 0;
  for (int i = 0; i != childs_.size(); ++i) {
    if (childs_[i]->spatial() != nullptr) {
      ++res;
    } else {
      res += childs_[i]->childCountThatHaveSpatial();
    }
  }
  return res;
}

QVariant XMLTreeItem::data(int column) const {
  QString res;
  switch (column) {
    case (Column::CadastralNumber):
      res = QString::fromStdString(strID_);
      break;
    default:
      break;
  }
  return res;
}

int XMLTreeItem::row() const {
  if (parent_)
    return parent_->childs_.indexOf(const_cast<XMLTreeItem*>(this));

  return 0;
}

XMLTreeItem* XMLTreeItem::parentItem() {
  return parent_;
}

std::ostream& XMLTreeItem::dumpInfo(std::ostream& out, std::string sep) const {
  out << sep << strID() << "\n";
  sep.append("    ");
  for (auto& child : childs_) {
    child->dumpInfo(out, sep);
  }
  return out;
}

void XMLTreeItem::appendSpatial(rrt::xmlSpatial_t spatial, bool fromDB) {
  if (spatial_ == nullptr ||
      spatial->xmlInfo().date() > spatial_->xmlInfo().date()) {
    setNewFlag(!fromDB);
    spatial_ = spatial;
  }
}

rrt::xmlSpatial_t XMLTreeItem::spatial() const {
  return spatial_;
}

QString XMLTreeItem::tooltipData() const {
  QString res;
  if (spatial_ == nullptr) {
    return (
        QObject::tr("%1 spatial elements").arg(childCountThatHaveSpatial()));
  }
  res =
      QString("%6\nXML: %1, %2, %3\nParent: %4: %5\n%7")
          .arg(QString::fromStdString(spatial_->xmlInfo().type()))
          .arg(QString::fromStdString(spatial_->xmlInfo().orderNumber()))
          .arg(QString::fromStdString(spatial_->xmlInfo().dateString()))
          .arg(QString::fromStdString(
              spatial_->xmlInfo().rootSpatialInfo().type()))
          .arg(QString::fromStdString(
              spatial_->xmlInfo().rootSpatialInfo().cadastralNumber().string()))
          .arg(qstrID())
          .arg(QString::fromStdWString(spatial_->xmlInfo().path().wstring()));
  return res;
}

bool XMLTreeItem::intersects(const rrt::Spatial& spatial) {
  if (spatial_ == nullptr) {
    return false;
  }
  intersectsFlag_ = spatial_->spatial()->intersects(spatial);
  BOOST_LOG_TRIVIAL(trace) << strID_ << " intersects = " << intersectsFlag_;
  return intersectsFlag_;
}

bool XMLTreeItem::intersectsFlag() const {
  return intersectsFlag_;
}

bool XMLTreeItem::anyChildIntersectsFlag() {
  bool res = false;
  forEach([&](XMLTreeItem* item) {
    res |= item->intersectsFlag();
  });  // TODO not effective, impl anyOf or smth
  return res;
}

void XMLTreeItem::turnOffIntersectsFlag() {
  intersectsFlag_ = false;
}

void XMLTreeItem::forEach(std::function<void(XMLTreeItem*)> fn) {
  fn(this);
  for (auto& child : childs_) {
    child->forEach(fn);
  }
}

bool XMLTreeItem::insertChildren(int row, int count, int columns) {
  if (row < 0 || row > childs_.size()) {
    return false;
  }
  for (int i = 0; i < count; ++i) {
    XMLTreeItem* item = new XMLTreeItem("", this);
    childs_.insert(row, item);
  }
  return true;
}

bool XMLTreeItem::setData(int col, const QVariant& value) {
  if (col < 0 || col >= Column::MAX) {
    return false;
  }
  strID_ = value.toString().toStdString();
  return true;
}

std::string XMLTreeItem::strID() const {
  return strID_;
}

QString XMLTreeItem::qstrID() const {
  return QString::fromStdString(strID_);
}

bool XMLTreeItem::newFlag() const {
  return newFlag_;
}
void XMLTreeItem::setNewFlag(bool newFlag) {
  auto it = this;
  do {
    it->newFlag_ |= newFlag;
    it = it->parent_;
  } while (it != nullptr);
}
