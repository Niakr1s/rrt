#ifndef VECSTR_H
#define VECSTR_H

#include <QVector>

template <typename String>
class VecStr {
 public:
  VecStr(const QVector<String>& vecStr) : vecStr_(vecStr) {}

  String sepBySemicolon() const { return sepBy(";"); }

  String sepByNewLine() const { return sepBy("\n"); }

  String sepBy(const char* sep) const {
    String res;
    for (int i = 0; i != vecStr_.size(); ++i) {
      res.append(vecStr_[i]);
      if (i != vecStr_.size() - 1) {
        res.append(sep);
      }
    }
    return res;
  }

 private:
  QVector<String> vecStr_;
};

#endif  // VECSTR_H
