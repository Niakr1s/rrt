#ifndef DXF_H
#define DXF_H

#include <memory>
#include <string>
#include "dx_data.h"
#include "dx_iface.h"
#include "spatial.h"

namespace rrt {

class DXF {
  static const char* IGNORED;  // layer with this name will be ignored

 public:
  DXF();

  std::shared_ptr<Spatial> spatial();
  void drawSpatial(std::string cadastralNumber,
                   std::shared_ptr<Spatial> spatial);

  void fileImport(const std::string& path);
  void fileExport(const std::string& path);

 private:
  dx_iface dxIface_;
  dx_data dxData_;
  std::shared_ptr<Spatial> spatial_;

 private:
  void dataToSpatial();
  void appendDRWCircle(DRW_Circle* e);
  void appendDRWLine(DRW_Line* e);
  void appendDRWPolyline(DRW_Polyline* e);
  void appendDRWLWPolyline(DRW_LWPolyline* e);

  // NOTE: all toPoint must implicially changes x and y
  static Point toPoint(const DRW_Vertex2D& p);
  static Point toPoint(const DRW_Coord& p);
  static Point toPoint(const DRW_Point& p);
  static Point toPoint(const DRW_Circle& p);

  // for drawSpatial
  void draw(std::vector<Spatial::point_t>::const_iterator begin,
            std::vector<Spatial::point_t>::const_iterator end,
            const std::string& layer);
  void draw(Point circle, const std::string& layer);
};

}  // namespace rrt

#endif  // DXF_H
