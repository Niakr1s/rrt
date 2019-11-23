#ifndef XMLTREESORTFILTERPROXYMODEL_H
#define XMLTREESORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class XMLTreeSortFilterProxyModel : public QSortFilterProxyModel {
 public:
  XMLTreeSortFilterProxyModel(QObject* parent = nullptr);

  void setFiltering(bool yes);

  // QSortFilterProxyModel interface
 protected:
  bool lessThan(const QModelIndex& source_left,
                const QModelIndex& source_right) const override;

  bool filterAcceptsRow(int source_row,
                        const QModelIndex& source_parent) const override;

 private:
  bool filtering_ = false;
};

#endif  // XMLTREESORTFILTERPROXYMODEL_H
