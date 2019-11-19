#include "xmltreeview.h"

#include "xmltreedelegate.h"
#include "xmltreemodel.h"

XMLTreeView::XMLTreeView(QWidget* parent) : QTreeView(parent) {
  XMLTreeModel* model = new XMLTreeModel();

  setModel(model);
  setSortingEnabled(true);
  setEditTriggers(QTreeView::NoEditTriggers);
  setMinimumHeight(600);

  auto delegate = new XMLTreeDelegate();
  setItemDelegate(delegate);
}
