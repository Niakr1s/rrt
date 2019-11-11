#ifndef DXF_H
#define DXF_H

#include <boost/filesystem/path.hpp>
#include "dx_data.h"
#include "dx_iface.h"
#include "spatial.h"

namespace bf = boost::filesystem;

namespace rrt {

class DXF {
  static const char* IGNORED;  // layer with this name will be ignored

 public:
  DXF(const std::string& path);

  Spatial& spatial();
  const Spatial& spatial() const;

 private:
  bf::path path_;
  dx_iface dxIface_;
  dx_data dxData_;
  Spatial spatial_;

 private:
  void parse();
  void appendDRWCircle(DRW_Circle* e);
  void appendDRWLine(DRW_Line* e);
  void appendDRWPolyline(DRW_Polyline* e);
  void appendDRWLWPolyline(DRW_LWPolyline* e);

  // NOTE: all toPoint must implicially changes x and y
  static Point toPoint(const DRW_Vertex2D& p);
  static Point toPoint(const DRW_Coord& p);
  static Point toPoint(const DRW_Point& p);
  static Point toPoint(const DRW_Circle& p);
};

}  // namespace rrt

#endif  // DXF_H
