#ifndef XMLTREESORTFILTERPROXYMODEL_H
#define XMLTREESORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class XMLTreeSortFilterProxyModel : public QSortFilterProxyModel {
 public:
  XMLTreeSortFilterProxyModel(QObject* parent = nullptr);
};

#endif  // XMLTREESORTFILTERPROXYMODEL_H
