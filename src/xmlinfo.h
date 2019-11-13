#ifndef XMLINFO_H
#define XMLINFO_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <string>

namespace rrt {

class XMLInfo {
 public:
  XMLInfo();
  XMLInfo(std::string type, std::string date, std::string orderNumber);

  std::string type() const;
  std::string date() const;
  std::string orderNumber() const;

 private:
  std::string type_;
  boost::gregorian::date date_;
  std::string orderNumber_;
};

}  // namespace rrt

#endif  // XMLINFO_H
