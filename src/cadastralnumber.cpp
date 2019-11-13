#include "cadastralnumber.h"

#include <fmt/core.h>
#include <boost/algorithm/string.hpp>
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

std::string CadastralNumber::underscoredString() const {
  std::string res(str_);
  boost::replace_all(res, ":", "_");
  return res;
}

void CadastralNumber::getNumbers(const std::string& str) {
  std::vector<std::string> splitted;
  boost::split(splitted, str, boost::is_any_of(":"));
  for (auto& chunk : splitted) {
    numbers_.push_back(std::stoi(chunk));
  }
}

std::ostream& operator<<(std::ostream& out, const CadastralNumber& rhs) {
  return out << rhs.string();
}

}  // namespace rrt
