#include "xmltreedelegate.h"

#include <QIcon>
#include <QLinearGradient>
#include <QPainter>
#include "xmltreeitem.h"
#include "xmltreesortfilterproxymodel.h"

XMLTreeDelegate::XMLTreeDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {}

void XMLTreeDelegate::paint(QPainter* painter,
                            const QStyleOptionViewItem& option,
                            const QModelIndex& index) const {
  auto idxCopy = index;

  auto model = static_cast<const XMLTreeSortFilterProxyModel*>(idxCopy.model());
  if (model != nullptr) {
    idxCopy = model->mapToSource(idxCopy);
  }

  auto item = static_cast<XMLTreeItem*>(idxCopy.internalPointer());
  if (item == nullptr) {
    return QStyledItemDelegate::paint(painter, option, index);
  }
  auto optCopy = option;
  if (item->intersectsFlag()) {
    optCopy.palette.setColor(QPalette::Text, Qt::green);
  }
  if (item->newFlag()) {
    auto rect = optCopy.rect;
    static QImage newIcon = QImage(":/icons/new");
    rect.setLeft(rect.right() - 25);
    rect.setRight(rect.right() - 2);
    rect.setTop(rect.top() + 2);
    rect.setBottom(rect.bottom() - 2);
    painter->drawImage(rect, newIcon);
  }
  if (item->hasSpatial()) {
    auto spa = item->spatial();
    QImage icon;
    if (spa->spatial()->empty()) {
      icon = QImage(":/icons/empty.svg");
    } else {
      icon = QImage(":/icons/full.svg");
    }
    auto rect = optCopy.rect;
    rect.setRight(rect.left() - 4);
    rect.setTop(rect.top() + 4);
    rect.setBottom(rect.bottom() - 4);
    rect.setLeft(rect.right() - rect.height());
    painter->drawImage(rect, icon);
  }
  return QStyledItemDelegate::paint(painter, optCopy, index);
}
