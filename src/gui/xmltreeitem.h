#ifndef XMLTREEITEM_H
#define XMLTREEITEM_H

#include <QVariant>
#include <QVector>
#include <memory>
#include <string>
#include "xmlspatial.h"

class XMLTreeItem {
 public:
  enum Column : int { CadastralNumber = 0, MAX };

  explicit XMLTreeItem(const std::string& strID = "",
                       XMLTreeItem* parentItem = nullptr);
  ~XMLTreeItem();

  void appendChild(XMLTreeItem* child);

  XMLTreeItem* child(int row);
  XMLTreeItem* child(const std::string& strID);

  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  int row() const;
  XMLTreeItem* parentItem();

  void appendSpatial(std::shared_ptr<rrt::XMLSpatial> spatial);
  std::shared_ptr<rrt::XMLSpatial> spatial() const;
  QString tooltipData() const;

 private:
  QVector<XMLTreeItem*> childs_;
  XMLTreeItem* parent_;

  std::string strID_;
  std::shared_ptr<rrt::XMLSpatial> spatial_;
};

#endif  // XMLTREEITEM_H
