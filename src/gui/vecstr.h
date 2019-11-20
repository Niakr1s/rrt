#ifndef VECSTR_H
#define VECSTR_H

#include <QString>
#include <QVector>

class VecStr {
 public:
  VecStr(const QVector<QString>& vecStr);

  QString sepBySemicolon() const;
  QString sepByNewLine() const;
  QString sepBy(const char* sep) const;

 private:
  QVector<QString> vecStr_;
};

#endif  // VECSTR_H
