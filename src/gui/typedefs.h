#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <QHash>
#include <QMetaType>
#include <QString>
#include <QStringList>
#include <memory>
#include "xmlspatial.h"

typedef QHash<QString, QStringList> DXFResult;

Q_DECLARE_METATYPE(DXFResult);
Q_DECLARE_METATYPE(std::shared_ptr<DXFResult>);
Q_DECLARE_METATYPE(rrt::xmlSpatials_t);

#endif  // TYPEDEFS_H
