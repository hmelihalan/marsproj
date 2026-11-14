#include "PlanetaryContext.h"
#include <gdal.h>
#include <proj.h>
QString MarsPath::geospatialRuntime(){return QString("GDAL %1 / PROJ %2").arg(GDALVersionInfo("RELEASE_NAME"),proj_info().version);}
