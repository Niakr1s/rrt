#include "xmltreesortfilterproxymodel.h"

XMLTreeSortFilterProxyModel::XMLTreeSortFilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent) {}

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
