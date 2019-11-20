#include "vecstr.h"

VecStr::VecStr(const QVector<QString>& vecStr) : vecStr_(vecStr) {}

QString VecStr::sepBySemicolon() const {
  return sepBy(";");
}

QString VecStr::sepByNewLine() const {
  return sepBy("\n");
}

QString VecStr::sepBy(const char* sep) const {
  QString res;
  for (auto& str : vecStr_) {
    res.append(str);
    res.append(sep);
  }
  res.remove(res.size() - 1, 1);
  return res;
}
