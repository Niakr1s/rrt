#ifndef XMLTREESORTFILTERPROXYMODEL_H
#define XMLTREESORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class XMLTreeSortFilterProxyModel : public QSortFilterProxyModel {
 public:
  XMLTreeSortFilterProxyModel(QObject* parent = nullptr);

  // QSortFilterProxyModel interface
 protected:
  bool lessThan(const QModelIndex& source_left,
                const QModelIndex& source_right) const override;
};

#endif  // XMLTREESORTFILTERPROXYMODEL_H
