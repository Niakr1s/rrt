#include "xmltreesortfilterproxymodel.h"

#include <boost/log/trivial.hpp>
#include "xmltreemodel.h"

XMLTreeSortFilterProxyModel::XMLTreeSortFilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent) {
  setDynamicSortFilter(true);
}

bool XMLTreeSortFilterProxyModel::lessThan(
    const QModelIndex& source_left,
    const QModelIndex& source_right) const {
  QVariant leftData = sourceModel()->data(source_left);
  QVariant rightData = sourceModel()->data(source_right);

  if (!(leftData.type() == QVariant::Type::String)) {
    return XMLTreeSortFilterProxyModel::lessThan(source_left, source_right);
  }
  auto left = leftData.toString();
  auto right = rightData.toString();

  auto leftLast = left.split(":").back();
  auto rightLast = right.split(":").back();

  return leftLast.toInt() < rightLast.toInt();
}

bool XMLTreeSortFilterProxyModel::filterAcceptsRow(
    int source_row,
    const QModelIndex& source_parent) const {
  if (!filtering_) {
    return true;
  }
  auto idx = sourceModel()->index(source_row, 0, source_parent);
  auto item = static_cast<XMLTreeModel*>(sourceModel())->getItem(idx);
  bool flag = item->anyChildIntersectsFlag();

  return item == nullptr ? false : flag;
}

void XMLTreeSortFilterProxyModel::setFiltering(bool yes) {
  filtering_ = yes;
  invalidateFilter();
}
