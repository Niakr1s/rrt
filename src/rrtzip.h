#ifndef RRTZIP_H
#define RRTZIP_H

#include <zip.h>
#include <boost/filesystem/path.hpp>
#include <string>
#include <vector>

namespace bf = boost::filesystem;

namespace rrt {

class Zip {
 public:
  using paths_t = std::vector<bf::path>;

  Zip(bf::path p);
  Zip(bf::path p, bf::path parentPath);
  ~Zip();

  void extractFromZip(const std::string& ext, paths_t& acc);

 private:
  bf::path path_;
  bf::path parentPath_;
  static void extract(zip_t* za, zip_uint64_t index, bf::path targetPath);
};

}  // namespace rrt

#endif  // RRTZIP_H
