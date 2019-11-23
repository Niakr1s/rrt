#include "dxf.h"

#include <fmt/core.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/log/trivial.hpp>
#include <stdexcept>
#include <vector>
#include "compare.h"
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

void DXF::drawSpatial(const std::string& cadastralNumber,
                      const std::string& type,
                      std::shared_ptr<Spatial> spatial,
                      Color color) {
  if (spatial->empty()) {
    return;
  }

  std::string layerName(cadastralNumber);
  boost::replace_all(layerName, ":", "_");
  addLayer(layerName);

  drawSpatial(spatial, color, layerName);

  std::string mtext = fmt::format("{}, {}", cadastralNumber, type);
  draw(mtext, layerName, spatial->rect_.centroid(), Color::LIGHTGREY);
}

void DXF::drawSpatial(std::shared_ptr<Spatial> spatial,
                      DXF::Color color,
                      const std::string& layerName) {
  for (auto& polygon : spatial->polygons_) {
    draw(polygon.outer().cbegin(), polygon.outer().cend(), layerName, color);
    for (auto& innerPolygon : polygon.inners()) {
      draw(innerPolygon.cbegin(), innerPolygon.cend(), layerName, color);
    }
  }

  for (auto& linestring : spatial->linestrings_) {
    draw(linestring.cbegin(), linestring.cend(), layerName, color);
  }

  for (auto& circle : spatial->circles_) {
    draw(circle, layerName, color);
  }
}

void DXF::fileImport(const std::string& path) {
  return fileImport(bf::path(path));
}

void DXF::fileImport(const std::wstring& path) {
  return fileImport(bf::path(path));
}

void DXF::fileImport(const boost::filesystem::path& path) {
  BOOST_LOG_TRIVIAL(info) << "DXF::fileImport: starting to open: " << path;
  if (auto ok = dxIface_.fileImport(path, &dxData_); !ok) {
    throw std::invalid_argument("DXF: problem while importing");
  }
  dataToSpatial();
  BOOST_LOG_TRIVIAL(info) << "DXF::fileImport: succesfully opened and parsed : "
                          << path;
}

void DXF::fileExport(const std::string& path, Version version) {
  if (auto ok = dxIface_.fileExport(path, version, false, &dxData_); !ok) {
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

void DXF::addLayer(const std::string& name) {
  auto& layers = dxData_.layers;
  if (auto found = std::find_if(layers.begin(), layers.end(),
                                [&](auto& it) { return it.name == name; });
      found != layers.end()) {
  } else {
    DRW_Layer layer;
    layer.name = name;
    dxIface_.addLayer(layer);
  }
}

void DXF::draw(std::vector<Spatial::point_t>::const_iterator begin,
               std::vector<Spatial::point_t>::const_iterator end,
               const std::string& layer,
               Color color) {
  DRW_LWPolyline drwLWPolyline;
  for (auto it = begin; it != end; ++it) {
    // changing x and y
    drwLWPolyline.addVertex(DRW_Vertex2D(bg::get<1>(*it), bg::get<0>(*it), 0));
  }
  drwLWPolyline.layer = layer;
  drwLWPolyline.color = static_cast<int>(color);
  dxIface_.addLWPolyline(drwLWPolyline);
}

void DXF::draw(Point circle, const std::string& layer, Color color) {
  DRW_Circle drwCircle;
  drwCircle.basePoint.x = circle.y();
  drwCircle.basePoint.y = circle.x();
  drwCircle.radious = circle.r().value_or(0.);
  drwCircle.layer = layer;
  drwCircle.color = static_cast<int>(color);
  dxIface_.addCircle(drwCircle);
}

void DXF::draw(const std::string& text,
               const std::string& layer,
               Spatial::point_t center,
               Color color) {
  DRW_Text drwText;
  drwText.text = text;
  drwText.basePoint.x = center.get<1>();
  drwText.basePoint.y = center.get<0>();
  drwText.secPoint.x = drwText.basePoint.x;
  drwText.secPoint.y = drwText.basePoint.y;
  drwText.layer = layer;
  drwText.height = 1.;
  drwText.alignH = DRW_Text::HAlign::HCenter;
  drwText.alignV = DRW_Text::VAlign::VMiddle;
  drwText.color = static_cast<int>(color);
  dxIface_.addText(drwText);
}

const char* DXF::IGNORED = "ignored";

}  // namespace rrt
