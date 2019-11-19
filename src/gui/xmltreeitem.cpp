#include "xmltreeitem.h"

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

XMLTreeItem* XMLTreeItem::child(const std::string& strID) {
  for (auto& child : childs_) {
    if (child->strID_ == strID) {
      return child;
    }
  }
  return nullptr;
}

int XMLTreeItem::childCount() const {
  return childs_.count();
}

int XMLTreeItem::columnCount() const {
  return Column::MAX;
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

void XMLTreeItem::appendSpatial(std::shared_ptr<rrt::XMLSpatial> spatial) {
  if (spatial_ == nullptr ||
      spatial->xmlInfo().date() > spatial_->xmlInfo().date()) {
    spatial_ = spatial;
  }
}

std::shared_ptr<rrt::XMLSpatial> XMLTreeItem::spatial() const {
  return spatial_;
}

QString XMLTreeItem::tooltipData() const {
  if (spatial_ == nullptr) {
    return "";
  }
  QString res =
      QString("XML: %1, %2, %3\nParent: %4: %5")
          .arg(QString::fromStdString(spatial_->xmlInfo().type()))
          .arg(QString::fromStdString(spatial_->xmlInfo().orderNumber()))
          .arg(QString::fromStdString(spatial_->xmlInfo().dateString()))
          .arg(QString::fromStdString(
              spatial_->xmlInfo().rootSpatialInfo().type()))
          .arg(QString::fromStdString(spatial_->xmlInfo()
                                          .rootSpatialInfo()
                                          .cadastralNumber()
                                          .string()));
  return res;
}

bool XMLTreeItem::intersects(const rrt::Spatial& spatial) {
  if (spatial_ == nullptr) {
    return false;
  }
  intersectsFlag_ = spatial_->spatial()->intersects(spatial);
  return intersectsFlag_;
}

bool XMLTreeItem::intersectsFlag() const {
  return intersectsFlag_;
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
