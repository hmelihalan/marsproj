#include <gdal_priv.h>
#include <ogr_spatialref.h>
#include <iostream>
int main(int argc, char** argv) {
  if (argc!=2) return 2;
  GDALAllRegister();
  auto* driver=GetGDALDriverManager()->GetDriverByName("GTiff");
  if (!driver) return 1;
  auto* ds=driver->Create(argv[1],4,3,1,GDT_Float32,nullptr);
  if (!ds) return 1;
  double gt[6]={1000,10,0,2000,0,-10};
  ds->SetGeoTransform(gt);
  // Test-only local projected coordinate system. NOT a Mars observation.
  OGRSpatialReference srs;
  srs.SetLocalCS("SYNTHETIC_TEST_CRS_NOT_MARS");
  char* wkt=nullptr;
  if (srs.exportToWkt(&wkt)==OGRERR_NONE) { ds->SetProjection(wkt); CPLFree(wkt); }
  float pixels[12]={1,2,3,4,5,6,-9999,8,9,10,11,12};
  auto* band=ds->GetRasterBand(1);
  band->SetNoDataValue(-9999);
  const auto status=band->RasterIO(GF_Write,0,0,4,3,pixels,4,3,GDT_Float32,0,0,nullptr);
  GDALClose(ds);
  return status==CE_None ? 0 : 1;
}
