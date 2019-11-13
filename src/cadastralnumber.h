#ifndef CADASTRALNUMBER_H
#define CADASTRALNUMBER_H

#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace rrt {

class CadastralNumber {
 public:
  CadastralNumber(const std::string& str);

  std::string string() const;
  std::string underscoredString() const;

 public:
  bool operator<(const CadastralNumber& rhs) const;
  bool operator==(const CadastralNumber& rhs) const;
  bool operator!=(const CadastralNumber& rhs) const;
  friend std::ostream& operator<<(std::ostream& out,
                                  const CadastralNumber& rhs);

 private:
  std::string str_;
  std::vector<int> numbers_;

 private:
  void getNumbers(const std::string& str);
};

std::ostream& operator<<(std::ostream& out, const CadastralNumber& rhs);

}  // namespace rrt

#endif  // CADASTRALNUMBER_H
