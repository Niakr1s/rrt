#ifndef XMLTREEBUTTONS_H
#define XMLTREEBUTTONS_H

#include <QPushButton>
#include <QWidget>

class XMLTreeButtons : public QWidget {
 public:
  XMLTreeButtons(QWidget* parent = nullptr);

  QPushButton* btnExpand() const;
  QPushButton* btnCollapse() const;

 private:
  QPushButton* btnExpand_;
  QPushButton* btnCollapse_;
};

#endif  // XMLTREEBUTTONS_H
