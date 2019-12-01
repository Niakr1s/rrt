#include "rrtzip.h"

#include <fcntl.h>
#include <io.h>
#include <share.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <boost/filesystem.hpp>
#include <iostream>
#include <stdexcept>

namespace rrt {

Zip::Zip(bf::path p) : path_(p), parentPath_(p.remove_filename()) {}

Zip::Zip(boost::filesystem::path p, boost::filesystem::path parentPath)
    : path_(p), parentPath_(parentPath) {}

Zip::~Zip() {}

// ext should be with dot, for example ".xml"
void Zip::extractFromZip(const std::string& ext, paths_t& acc) {
  zip_source_t* src;
  zip_t* za;
  zip_error_t error;

  zip_error_init(&error);
  /* create source from buffer */
  if ((src = zip_source_win32w_create(path_.wstring().c_str(), 0, -1,
                                      &error)) == nullptr) {
    fprintf(stderr, "can't create source: %s\n", zip_error_strerror(&error));
    zip_error_fini(&error);
    return;
  }

  /* open zip archive from source */
  if ((za = zip_open_from_source(src, 0, &error)) == nullptr) {
    fprintf(stderr, "can't open zip from source: %s\n",
            zip_error_strerror(&error));
    zip_source_free(src);
    zip_error_fini(&error);
    return;
  }
  zip_error_fini(&error);

  for (int i = 0, sz = zip_get_num_files(za); i != sz; ++i) {
    auto name = zip_get_name(za, i, 0);
    bf::path innerPath(name);
    if (innerPath.extension() == ".zip") {
      bf::path tmpDir = bf::temp_directory_path() / "rrt";
      if (!bf::exists(tmpDir)) {
        bf::create_directories(tmpDir);
      }
      bf::path tmpPath = tmpDir / innerPath;
      extract(za, i, tmpPath);
      Zip tmpZip(tmpPath, parentPath_);
      tmpZip.extractFromZip(ext, acc);
    } else if (innerPath.extension() == ext) {
      bf::path newPath = parentPath_ / innerPath;
      extract(za, i, newPath);
      acc.push_back(newPath);
    }
  }
}

void Zip::extract(zip_t* za,
                  zip_uint64_t index,
                  boost::filesystem::path targetPath) {
  struct zip_stat st;
  zip_stat_init(&st);
  zip_stat_index(za, index, 0, &st);
  char* contents = new char[st.size];
  zip_file* f = zip_fopen(za, zip_get_name(za, index, 0), 0);
  zip_fread(f, contents, st.size);
  zip_fclose(f);
  bf::ofstream(targetPath, bf::ofstream::binary).write(contents, st.size);
  delete[] contents;
}

}  // namespace rrt
