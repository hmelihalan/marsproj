#include <gdal_priv.h>
#include <cpl_conv.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>

static std::string escapeJson(const char* s) {
  std::string out;
  if (!s) return out;
  for (const unsigned char c : std::string(s)) {
    if (c == '\\') out += "\\\\";
    else if (c == '"') out += "\\\"";
    else if (c == '\n') out += "\\n";
    else if (c == '\r') out += "\\r";
    else if (c == '\t') out += "\\t";
    else if (c < 32) out += "?";
    else out += static_cast<char>(c);
  }
  return out;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << "Usage: marspath_data_probe <local-raster-file>\n";
    return 2;
  }
  GDALAllRegister();
  auto* ds = static_cast<GDALDataset*>(GDALOpen(argv[1], GA_ReadOnly));
  if (!ds) { std::cerr << "Cannot open raster: " << argv[1] << "\n"; return 1; }
  std::cout << std::setprecision(12);
  std::cout << "{\n  \"file\": \"" << escapeJson(argv[1]) << "\",\n";
  std::cout << "  \"driver\": \"" << escapeJson(ds->GetDriverName()) << "\",\n";
  std::cout << "  \"raster_size\": [" << ds->GetRasterXSize() << ", " << ds->GetRasterYSize() << "],\n";
  std::cout << "  \"bands\": " << ds->GetRasterCount() << ",\n";
  std::cout << "  \"projection_wkt\": \"" << escapeJson(ds->GetProjectionRef()) << "\",\n";
  double gt[6];
  if (ds->GetGeoTransform(gt) == CE_None) {
    std::cout << "  \"geotransform\": [";
    for (int i=0;i<6;++i) std::cout << (i ? ", " : "") << gt[i];
    std::cout << "],\n";
  } else std::cout << "  \"geotransform\": null,\n";
  std::cout << "  \"band_info\": [\n";
  for (int i=1;i<=ds->GetRasterCount();++i) {
    auto* band=ds->GetRasterBand(i);
    int hasNodata=0;
    double nodata=band->GetNoDataValue(&hasNodata);
    std::cout << "    {\"band\": " << i << ", \"type\": \""
              << GDALGetDataTypeName(band->GetRasterDataType()) << "\", \"nodata\": ";
    if (hasNodata) std::cout << nodata; else std::cout << "null";
    // Read a bounded native-resolution window, including its validity mask.
    const int w=std::min(64,band->GetXSize()), h=std::min(64,band->GetYSize());
    const int x=(band->GetXSize()-w)/2, y=(band->GetYSize()-h)/2;
    std::vector<double> values(w*h);
    std::vector<unsigned char> mask(w*h);
    if (band->RasterIO(GF_Read,x,y,w,h,values.data(),w,h,GDT_Float64,0,0,nullptr)!=CE_None ||
        band->GetMaskBand()->RasterIO(GF_Read,x,y,w,h,mask.data(),w,h,GDT_Byte,0,0,nullptr)!=CE_None) {
      GDALClose(ds); return 1;
    }
    int valid=0; double low=0, high=0;
    for (size_t j=0;j<values.size();++j) {
      const double v=values[j];
      if (!mask[j] || !std::isfinite(v) || (hasNodata && v==nodata)) continue;
      if (!valid) low=high=v;
      low=std::min(low,v); high=std::max(high,v); ++valid;
    }
    std::cout << ", \"sample_window\": [" << x << ", " << y << ", " << w << ", " << h
              << "], \"valid_samples\": " << valid << ", \"unknown_samples\": " << w*h-valid;
    if(valid) std::cout << ", \"sample_min\": " << low << ", \"sample_max\": " << high;
    std::cout << "}" << (i<ds->GetRasterCount() ? "," : "") << "\n";
  }
  std::cout << "  ]\n}\n";
  GDALClose(ds);
}
