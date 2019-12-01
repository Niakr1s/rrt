#ifndef XMLZIP_TESTS_H
#define XMLZIP_TESTS_H

#include <gtest/gtest.h>
#include <rrtzip.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <string>

namespace bf = boost::filesystem;

extern std::string PATH;
static const std::string ZIP = PATH + "Response-80-64365663.zip";

TEST(xmlzip, case1) {
  auto zip = rrt::Zip(bf::path(ZIP));
  rrt::Zip::paths_t paths;
  zip.extractFromZip(".xml", paths);
  ASSERT_EQ(paths.size(), 1);
  ASSERT_TRUE(bf::exists(paths.front()));
  ASSERT_EQ(paths.front().filename(),
            "kv_1b87dab2-8378-4a82-a702-8f5d5d4dd2f0.xml");
  bf::remove(paths.front());
}

#endif  // XMLZIP_TESTS_H
