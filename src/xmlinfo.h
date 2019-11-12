#ifndef XMLINFO_H
#define XMLINFO_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <string>

namespace rrt {

class XMLInfo {
 public:
  XMLInfo();
  XMLInfo(std::string type, std::string date, std::string number);

  std::string type() const;
  boost::gregorian::date date() const;
  std::string number() const;

 private:
  std::string type_;
  boost::gregorian::date date_;
  std::string number_;
};

}  // namespace rrt

#endif  // XMLINFO_H
