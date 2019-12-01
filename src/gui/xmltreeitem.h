#ifndef XMLTREEITEM_H
#define XMLTREEITEM_H

#include <QModelIndex>
#include <QVariant>
#include <QVector>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include "spatial.h"
#include "xmlspatial.h"

class XMLTreeItem {
  friend class XMLTreeModel;
  friend class XMLRootTreeItem;

 public:
  enum Column : int { CadastralNumber = 0, MAX };

  explicit XMLTreeItem(const std::string& strID = "",
                       XMLTreeItem* parentItem = nullptr);
  ~XMLTreeItem();

  void appendChild(XMLTreeItem* child);

  XMLTreeItem* child(int row);

  int childCount() const;
  int columnCount() const;
  int spatialChildCount(bool onlyIntersected = false) const;
  QVariant data(int column) const;
  int row() const;
  XMLTreeItem* parentItem();
  std::ostream& dumpInfo(std::ostream& out = std::cout,
                         std::string sep = "") const;

  void appendSpatial(rrt::xmlSpatial_t spatial, bool fromDB);
  rrt::xmlSpatial_t spatial() const;
  QString tooltipData() const;
  bool intersects(const rrt::Spatial& spatial);

  bool intersectsFlag() const;
  bool anyChildIntersectsFlag() const;
  void turnOffIntersectsFlag();
  bool newFlag() const;
  void setNewFlag(bool newFlag);

  bool insertChildren(int row, int count, int columns);

  bool setData(int col, const QVariant& value);

  std::string strID() const;
  QString qstrID() const;

 signals:
  void dataChanged(QModelIndex*);

 private:
  QVector<XMLTreeItem*> childs_;
  XMLTreeItem* parent_;

  std::string strID_;
  rrt::xmlSpatial_t spatial_;

  bool intersectsFlag_ = false;
  bool newFlag_ = false;

 private:
  void forEach(std::function<void(XMLTreeItem*)> fn,
               bool onlyIntersected = false);
  void forEach(std::function<void(XMLTreeItem*)> fn,
               bool onlyIntersected = false) const;
};

#endif  // XMLTREEITEM_H
