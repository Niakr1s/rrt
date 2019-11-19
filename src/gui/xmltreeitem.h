#ifndef XMLTREEITEM_H
#define XMLTREEITEM_H

#include <QModelIndex>
#include <QVariant>
#include <QVector>
#include <functional>
#include <memory>
#include <string>
#include "spatial.h"
#include "xmlspatial.h"

class XMLTreeItem {
 public:
  enum Column : int { CadastralNumber = 0, MAX };

  explicit XMLTreeItem(const std::string& strID = "",
                       XMLTreeItem* parentItem = nullptr);
  ~XMLTreeItem();

  void appendChild(XMLTreeItem* child);

  XMLTreeItem* child(int row);

  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  int row() const;
  XMLTreeItem* parentItem();

  void appendSpatial(std::shared_ptr<rrt::XMLSpatial> spatial);
  std::shared_ptr<rrt::XMLSpatial> spatial() const;
  QString tooltipData() const;
  bool intersects(const rrt::Spatial& spatial);

  bool intersectsFlag() const;
  void turnOffIntersectsFlag();
  bool newFlag() const;

  void forEach(std::function<void(XMLTreeItem*)> fn);

  bool insertChildren(int row, int count, int columns);

  bool setData(int col, const QVariant& value);

  std::string strID() const;

 private:
  QVector<XMLTreeItem*> childs_;
  XMLTreeItem* parent_;

  std::string strID_;
  std::shared_ptr<rrt::XMLSpatial> spatial_;

  bool intersectsFlag_ = false;
  bool newFlag_ = false;
};

#endif  // XMLTREEITEM_H
