#pragma once
#include <QString>
namespace MarsPath {
// Dependency/service boundary only. No raster operations run in the UI.
QString geospatialRuntime();
}
