#include <gdal_priv.h>
#include <ogr_spatialref.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <vector>

void require(bool ok, const char* message) { if(!ok) throw std::runtime_error(message); }
using Dataset=std::unique_ptr<GDALDataset, decltype(&GDALClose)>;
Dataset open(const char* path) {
  Dataset d(static_cast<GDALDataset*>(GDALOpen(path,GA_ReadOnly)),GDALClose);
  require(bool(d),"Cannot open input"); return d;
}
struct Box {double west,south,east,north;};
double overlap(Box a, Box b) {
  return std::max(0.0,std::min(a.east,b.east)-std::max(a.west,b.west))*
         std::max(0.0,std::min(a.north,b.north)-std::max(a.south,b.south));
}
Box inspect(GDALDataset* ds) {
  auto s=ds->GetSpatialRef(); require(s && s->IsProjected(),"Missing projected CRS");
  require(std::abs(s->GetSemiMajor()-3396190)<1e-6 && std::abs(s->GetSemiMinor()-3396190)<1e-6,"Unexpected Mars sphere");
  require(std::abs(s->GetLinearUnits()-1)<1e-12,"Expected metres");
  OGRSpatialReference source(*s);
  source.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
  std::unique_ptr<OGRSpatialReference> geo(source.CloneGeogCS());
  geo->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
  auto destroy=[](OGRCoordinateTransformation* p){if(p) OCTDestroyCoordinateTransformation(p);};
  std::unique_ptr<OGRCoordinateTransformation,decltype(destroy)> inv(OGRCreateCoordinateTransformation(&source,geo.get()),destroy);
  std::unique_ptr<OGRCoordinateTransformation,decltype(destroy)> fwd(OGRCreateCoordinateTransformation(geo.get(),&source),destroy);
  require(inv && fwd,"Cannot create Mars transformations");
  double g[6]; require(ds->GetGeoTransform(g)==CE_None,"Missing affine transform");
  require(g[2]==0 && g[4]==0 && g[1]>0 && g[5]<0,"Only north-up rectangles supported");
  Box box{1e30,1e30,-1e30,-1e30}; double maxError=0;
  const double pi=std::acos(-1.0), radius=3396190;
  for(double px : {0.0,0.5,ds->GetRasterXSize()/2.0,double(ds->GetRasterXSize())})
    for(double py : {0.0,0.5,ds->GetRasterYSize()/2.0,double(ds->GetRasterYSize())}) {
      const double ox=g[0]+px*g[1], oy=g[3]+py*g[5]; double x=ox,y=oy;
      require(inv->Transform(1,&x,&y),"Inverse failed");
      // Independent spherical equirectangular formula from label parameters.
      require(std::abs(x-ox/(radius*std::cos(18.4663*pi/180))*180/pi)<1e-9,"Longitude differs from analytic reference");
      require(std::abs(y-oy/radius*180/pi)<1e-9,"Latitude differs from analytic reference");
      box.west=std::min(box.west,x); box.east=std::max(box.east,x);
      box.south=std::min(box.south,y); box.north=std::max(box.north,y);
      require(fwd->Transform(1,&x,&y),"Forward failed");
      maxError=std::max(maxError,std::hypot(x-ox,y-oy));
    }
  require(maxError<1e-6,"Round trip exceeds one micrometre");
  std::cout<<"bounds_lonlat="<<box.west<<","<<box.south<<","<<box.east<<","<<box.north<<" roundtrip_m="<<maxError<<"\n";
  return box;
}
void compare(GDALDataset* crop, GDALDataset* parent, bool imagery) {
  double c[6],p[6]; require(crop->GetGeoTransform(c)==CE_None && parent->GetGeoTransform(p)==CE_None,"Missing transforms");
  require(c[2]==0 && c[4]==0 && p[2]==0 && p[4]==0,"Rotated crop unsupported");
  require(std::abs(c[1]-p[1])<1e-8 && std::abs(c[5]-p[5])<1e-8,"Spacing changed");
  double dx=(c[0]-p[0])/p[1],dy=(c[3]-p[3])/p[5];
  int x=int(std::round(dx)),y=int(std::round(dy)),w=crop->GetRasterXSize(),h=crop->GetRasterYSize();
  require(std::abs(dx-x)<1e-6 && std::abs(dy-y)<1e-6,"Nonintegral crop offset");
  require(x>=0 && y>=0 && x+w<=parent->GetRasterXSize() && y+h<=parent->GetRasterYSize(),"Crop outside parent");
  std::vector<double> a(w),b(w); size_t mismatches=0,zero=0,unknown=0;
  int hasNodata=0;
  const double nodata=parent->GetRasterBand(1)->GetNoDataValue(&hasNodata);
  for(int row=0;row<h;++row) {
    require(crop->GetRasterBand(1)->RasterIO(GF_Read,0,row,w,1,a.data(),w,1,GDT_Float64,0,0,nullptr)==CE_None,"Crop read failed");
    require(parent->GetRasterBand(1)->RasterIO(GF_Read,x,y+row,w,1,b.data(),w,1,GDT_Float64,0,0,nullptr)==CE_None,"Parent read failed");
    for(int col=0;col<w;++col) {
      if(a[col]!=b[col]) ++mismatches;
      if(b[col]==0) ++zero;
      // The ortho PDS4 label declares 0 missing even though TIFF omits NoData.
      if(!std::isfinite(b[col]) || (hasNodata && b[col]==nodata) || (imagery && b[col]==0)) ++unknown;
    }
  }
  std::cout<<"parent_offset="<<x<<","<<y<<" compared="<<size_t(w)*h<<" mismatches="<<mismatches<<" parent_zero_count="<<zero<<"\n";
  require(mismatches==0,"Parent/crop values differ (including missing values)");
  std::cout<<"label_aware_unknown="<<unknown<<"\n";
  require(imagery ? unknown==8277 : unknown==0,"Pilot validity regression");
}
int main(int argc,char** argv) {
 try {
  GDALAllRegister(); std::cout<<std::setprecision(15);
  require(argc==3 || argc==5,"Usage: validation dem image [parent_dem parent_image]");
  auto dem=open(argv[1]),image=open(argv[2]);
  auto a=inspect(dem.get()),b=inspect(image.get());
  require(overlap(a,b)>0,"No geographic overlap");
  double fraction=overlap(a,b)/((a.east-a.west)*(a.north-a.south));
  std::cout<<"dem_bbox_overlap_fraction="<<fraction<<"\n";
  require(fraction>0.999,"Image does not cover pilot DEM");
  require(overlap({0,0,1,1},{2,2,3,3})==0,"Disjoint control failed");
  require(overlap({0,0,1,1},{1,0,2,1})==0,"Touching control failed");
  require(std::abs(overlap({0,0,2,2},{1,1,3,3})-1)<1e-12,"Partial overlap control failed");
  // Bounds transcribed from archived PDS3 header, not a valid-data polygon.
  Box hirise{77.37089912,18.096543803297,77.50346219,18.368640005209};
  require(overlap(a,hirise)==0,"HiRISE footprint conclusion changed");
  std::cout<<"hirise_bbox_overlap=0 north_gap_degrees="<<a.south-hirise.north<<"\n";
  if(argc==5) {auto pd=open(argv[3]),pi=open(argv[4]); compare(dem.get(),pd.get(),false); compare(image.get(),pi.get(),true);}
  return 0;
 } catch(const std::exception& e) {std::cerr<<e.what()<<"\n"; return 1;}
}
