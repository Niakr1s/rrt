#ifndef XMLTREEDELEGATE_H
#define XMLTREEDELEGATE_H

#include <QStyledItemDelegate>

class XMLTreeDelegate : public QStyledItemDelegate {
 public:
  XMLTreeDelegate(QObject* parent = nullptr);

  void paint(QPainter* painter,
             const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;
};

#endif  // XMLTREEDELEGATE_H
