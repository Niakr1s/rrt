#include "cadastralnumber.h"

#include <fmt/core.h>
#include <stdexcept>

namespace rrt {

CadastralNumber::CadastralNumber(const std::string& str) : str_(str) {
  getNumbers(str_);
  if (numbers_.size() == 0 || numbers_.size() > 4) {
    throw(std::invalid_argument(
        fmt::format("CadastralNumber: wrong input str: {}", str_)));
  }
}

bool CadastralNumber::operator<(const CadastralNumber& rhs) const {
  if (numbers_.size() != rhs.numbers_.size()) {
    return numbers_.size() < rhs.numbers_.size();
  }
  return numbers_.back() < rhs.numbers_.back();
}

bool CadastralNumber::operator==(const CadastralNumber& rhs) const {
  if (numbers_.size() != rhs.numbers_.size()) {
    return false;
  }
  return numbers_.back() == rhs.numbers_.back();
}

bool CadastralNumber::operator!=(const CadastralNumber& rhs) const {
  return !(*this == rhs);
}

std::string CadastralNumber::string() const {
  return str_;
}

void CadastralNumber::getNumbers(std::string str) {
  if (auto found = str.find(":"); found != std::string::npos) {
    numbers_.push_back(std::stoi(str.substr(0, found)));
    getNumbers(str.substr(found + 1));
  } else {
    numbers_.push_back(std::stoi(str));
  }
}

std::ostream& operator<<(std::ostream& out, const CadastralNumber& rhs) {
  return out << rhs.string();
}

}  // namespace rrt
