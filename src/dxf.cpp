#include "dxf.h"

#include <fmt/core.h>
#include <boost/log/trivial.hpp>
#include <stdexcept>
#include <vector>
#include "libdxfrw.h"

namespace rrt {

DXF::DXF(const std::string& path) : path_(path) {
  if (auto ok = dxIface_.fileImport(path.c_str(), &dxData_); !ok) {
    throw std::invalid_argument(
        fmt::format("DXF: wrong input file: {}", path_.string()));
  }
  parse();
  BOOST_LOG_TRIVIAL(info) << "DXF: succesfully opened and parsed: " << path_;
}

void DXF::parse() {
  for (auto e : dxData_.mBlock->ent) {
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
  spatial_.append(toPoint(*e));
}

void DXF::appendDRWLine(DRW_Line* e) {
  std::vector<Point> toAppend;
  toAppend.push_back(toPoint(e->basePoint));
  toAppend.push_back(toPoint(e->secPoint));
  spatial_.append(toAppend);
}

void DXF::appendDRWPolyline(DRW_Polyline* e) {
  std::vector<Point> toAppend;
  //  toAppend.push_back(toPoint(e->basePoint));  TODO: Check
  for (auto vert : e->vertlist) {
    toAppend.push_back(toPoint(vert->basePoint));
  }
  spatial_.append(toAppend);
}

void DXF::appendDRWLWPolyline(DRW_LWPolyline* e) {
  std::vector<Point> toAppend;
  //  toAppend.push_back(toPoint(e->basePoint));  TODO: Check
  for (auto vert : e->vertlist) {
    toAppend.push_back(Point(vert->x, vert->y));
  }
  spatial_.append(toAppend);
}

Point DXF::toPoint(const DRW_Coord& p) {
  return Point(p.x, p.y);
}

Point DXF::toPoint(const DRW_Point& p) {
  return toPoint(p.basePoint);
}

Point DXF::toPoint(const DRW_Circle& p) {
  return Point(p.basePoint.x, p.basePoint.y, p.radious);
}

}  // namespace rrt
