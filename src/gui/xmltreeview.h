#ifndef XMLTREEVIEW_H
#define XMLTREEVIEW_H

#include <QTreeView>

class XMLTreeView : public QTreeView {
  Q_OBJECT

 public:
  XMLTreeView(QWidget* parent = nullptr);
};

#endif  // XMLTREEVIEW_H
