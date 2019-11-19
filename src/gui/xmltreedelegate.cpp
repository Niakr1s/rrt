#include "xmltreedelegate.h"

#include <QDebug>
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
  if (item->intersectsFlag()) {
    auto optCopy = option;
    optCopy.palette.setColor(QPalette::Text, Qt::green);
    return QStyledItemDelegate::paint(painter, optCopy, index);
  }
  QStyledItemDelegate::paint(painter, option, index);
}
