#include "xmltreedelegate.h"

#include <QDebug>
#include <QIcon>
#include <QLinearGradient>
#include <QPainter>
#include "xmltreeitem.h"

XMLTreeDelegate::XMLTreeDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {}

void XMLTreeDelegate::paint(QPainter* painter,
                            const QStyleOptionViewItem& option,
                            const QModelIndex& index) const {
  XMLTreeItem* item = static_cast<XMLTreeItem*>(index.internalPointer());
  if (item == nullptr) {
    return;
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
  QStyledItemDelegate::paint(painter, optCopy, index);
}
