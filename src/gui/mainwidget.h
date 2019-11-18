#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QLabel>
#include <QTableView>
#include <QTreeView>
#include <QWidget>

class MainWidget : public QWidget {
  Q_OBJECT
 public:
  explicit MainWidget(QWidget* parent = nullptr);

 signals:

 public slots:

 private:
  QTreeView* treeView_;
  QLabel* resultLabel_;

 private:
  QTreeView* createTreeView();
  QLabel* createresultLabel();
};

#endif  // MAINWIDGET_H
