#include "dxf.h"

#include <fmt/core.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/log/trivial.hpp>
#include <stdexcept>
#include <vector>
#include "libdxfrw.h"

namespace bg = boost::geometry;

namespace rrt {

DXF::DXF() : spatial_(std::make_shared<Spatial>()) {
  // chunk from dx_iface::fileImport
  dxIface_.cData = &dxData_;
  dxIface_.currentBlock = dxIface_.cData->mBlock;
}

std::shared_ptr<Spatial> DXF::spatial() {
  return spatial_;
}

void DXF::drawSpatial(std::string cadastralNumber,
                      std::shared_ptr<Spatial> spatial) {
  //  boost::replace_all(cadastralNumber, ":", "_");
  //  std::string utf8CN = fmt::format("{}", cadastralNumber);
  //  auto& blocks = dxData_.blocks;
  //  if (auto found = std::find_if(blocks.begin(), blocks.end(),
  //                                [&](auto& it) { return it->name == utf8CN;
  //                                });
  //      found != blocks.end()) {
  //    dxIface_.currentBlock = *found;
  //  } else {
  //    DRW_Block block;
  //    block.name = utf8CN;
  //    block.basePoint.x = 0.;
  //    block.basePoint.y = 0.;
  //    block.basePoint.z = 0.;
  //    dxIface_.addBlock(block);
  //  }

  boost::replace_all(cadastralNumber, ":", "_");
  auto& blocks = dxData_.layers;
  if (auto found =
          std::find_if(blocks.begin(), blocks.end(),
                       [&](auto& it) { return it.name == cadastralNumber; });
      found != blocks.end()) {
  } else {
    DRW_Layer layer;
    layer.name = cadastralNumber;
    dxIface_.addLayer(layer);
  }

  for (auto& polygon : spatial->polygons_) {
    draw(polygon.outer().cbegin(), polygon.outer().cend(), cadastralNumber);
    for (auto& innerPolygon : polygon.inners()) {
      draw(innerPolygon.cbegin(), innerPolygon.cend(), cadastralNumber);
    }
  }

  for (auto& linestring : spatial->linestrings_) {
    draw(linestring.cbegin(), linestring.cend(), cadastralNumber);
  }

  for (auto& circle : spatial->circles_) {
    draw(circle, cadastralNumber);
  }

  //  dxIface_.endBlock();
}

void DXF::fileImport(const std::string& path) {
  if (auto ok = dxIface_.fileImport(path, &dxData_); !ok) {
    throw std::invalid_argument(
        fmt::format("DXF: problem while importing: {}", path));
  }
  dataToSpatial();
  BOOST_LOG_TRIVIAL(info) << "DXF::fileImport: succesfully opened and parsed: "
                          << path;
}

void DXF::fileExport(const std::string& path) {
  if (auto ok = dxIface_.fileExport(path, DRW::AC1024, false, &dxData_); !ok) {
    throw std::invalid_argument(
        fmt::format("DXF: problem while exporting: {}", path));
  }
  BOOST_LOG_TRIVIAL(info) << "DXF::fileExport: succesfully exported: " << path;
}

void DXF::dataToSpatial() {
  for (auto e : dxData_.mBlock->ent) {
    if (e->layer == IGNORED) {
      continue;
    }
    switch (e->eType) {
      case DRW::CIRCLE:
        appendDRWCircle(static_cast<DRW_Circle*>(e));
        break;
      case DRW::LINE:
        appendDRWLine(static_cast<DRW_Line*>(e));
        break;
      case DRW::POLYLINE:
        appendDRWPolyline(static_cast<DRW_Polyline*>(e));
        break;
      case DRW::LWPOLYLINE:
        appendDRWLWPolyline(static_cast<DRW_LWPolyline*>(e));
        break;
      default:
        break;
    }
  }
}

void DXF::appendDRWCircle(DRW_Circle* e) {
  spatial_->append(toPoint(*e));
}

void DXF::appendDRWLine(DRW_Line* e) {
  std::vector<Point> toAppend;
  toAppend.push_back(toPoint(e->basePoint));
  toAppend.push_back(toPoint(e->secPoint));
  spatial_->append(toAppend);
}

void DXF::appendDRWPolyline(DRW_Polyline* e) {
  std::vector<Point> toAppend;
  for (auto vert : e->vertlist) {
    toAppend.push_back(toPoint(vert->basePoint));
  }
  spatial_->append(toAppend);
}

void DXF::appendDRWLWPolyline(DRW_LWPolyline* e) {
  std::vector<Point> toAppend;
  for (auto vert : e->vertlist) {
    toAppend.push_back(toPoint(*vert));
  }
  spatial_->append(toAppend);
}

Point DXF::toPoint(const DRW_Vertex2D& p) {
  return Point(p.y, p.x);
}

Point DXF::toPoint(const DRW_Coord& p) {
  return Point(p.y, p.x);
}

Point DXF::toPoint(const DRW_Point& p) {
  return toPoint(p.basePoint);
}

Point DXF::toPoint(const DRW_Circle& p) {
  return Point(p.basePoint.y, p.basePoint.x, p.radious);
}

void DXF::draw(std::vector<Spatial::point_t>::const_iterator begin,
               std::vector<Spatial::point_t>::const_iterator end,
               const std::string& layer) {
  DRW_LWPolyline drwLWPolyline;
  for (auto it = begin; it != end; ++it) {
    // changing x and y
    drwLWPolyline.addVertex(DRW_Vertex2D(bg::get<1>(*it), bg::get<0>(*it), 0));
  }
  drwLWPolyline.layer = layer;
  dxIface_.addLWPolyline(drwLWPolyline);
}

void DXF::draw(Point circle, const std::string& layer) {
  DRW_Circle drwCircle;
  drwCircle.basePoint.x = circle.y();
  drwCircle.basePoint.y = circle.x();
  drwCircle.radious = circle.r().value_or(0.);
  drwCircle.layer = layer;
  dxIface_.addCircle(drwCircle);
}

const char* DXF::IGNORED = "ignored";

}  // namespace rrt
