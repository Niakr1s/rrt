#include "cadastralnumber.h"

#include <fmt/core.h>
#include <boost/algorithm/string.hpp>
#include <stdexcept>

namespace rrt {

CadastralNumber::CadastralNumber() {}

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

std::vector<std::string> CadastralNumber::strings() const {
  std::vector<std::string> res;
  boost::split(res, str_, boost::is_any_of(":"));
  return res;
}

std::string CadastralNumber::safeString() const {
  std::string res(str_);
  boost::replace_all(res, ":", "-");
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
