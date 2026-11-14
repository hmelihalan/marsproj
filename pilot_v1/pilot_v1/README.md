# ARES - Pilot Area GIS Layers (v1)

Research/hackathon prototype. Not a certified safety system.

## Area
Jezero Crater, 5 km x 5 km pilot box.
X: 4351000 - 4356000 m, Y: 1090800 - 1095800 m (projected CRS below).
Approx. 77.39-77.48 E, 18.40-18.49 N.

## CRS
Mars equirectangular (Mars 2000 sphere, R = 3396190 m, lat_ts = 18.4663).
proj4: +proj=eqc +lat_ts=18.4663 +lat_0=0 +lon_0=0 +x_0=0 +y_0=0 +R=3396190 +units=m +no_defs
Units: meters. QGIS may show the CRS name as "Unknown" or "Jezero_Crater_Projection"; parameters are identical.

## Routing grid (dem_20m, slope_20m_deg, roughness_20m)
250 x 250 cells, 20 m pixels, same origin (4351000, 1095800), same extent.
Perfectly aligned: stack them cell for cell. No resampling was applied.
Format: GeoTIFF, Float32, NoData = -9999 (no NoData pixels inside this pilot area).

| File | Content | Units |
|---|---|---|
| dem_20m.tif | CTX DEM elevation (clipped from M20_JezeroCrater_CTXDEM_20m) | m |
| slope_20m_deg.tif | Slope, gdaldem (Horn), computed from dem_20m | degrees |
| roughness_20m.tif | Max-min elevation in 3x3 window, gdaldem | m |

Elevation range in pilot: -2616.9 to -2441.6 m. Slope range 0.006-33.7 deg (mean 3.3).

## Optical layer (separate, not part of the routing grid)
ctx_5m.tif: CTX orthomosaic, Byte grayscale, ~5.002 m pixels, 1001 x 1000.
Covers the same area to within ~4 m (pixel grid does not nest in the 20 m grid).
No NoData defined. Kept at native resolution; do not merge with the DEM.

## Limitations
- slope and roughness correlate at r = 0.99 (Pearson) and both come from the same 20 m DEM. Treat roughness as secondary, not independent evidence.
- 20 m pixels smooth out rocks, small craters and steep faces. Slope maxima are underestimated on the scarp. These layers show regional terrain, not rock-scale hazards.
- Hazard thresholds are in thresholds.json (rover reference values, not astronaut limits; sources cited there). slope_class_rover_ref_v2.tif is derived from them.
- Mineral targets (CRISM) and environment data are not included yet.

## Provenance
Source: NASA Mars 2020 Jezero CTX orthomosaic and CTX DEM (original files untouched).
Processing: gdal_translate -projwin, gdaldem slope/roughness -compute_edges, NoData relabel to -9999.

Dataset: Calef, F. (2021). Mars 2020 Science Investigation CTX DEM Mosaic [Data set]. USGS Astrogeology Science Center. https://astrogeology.usgs.gov/search/map/mars_2020_science_investigation_ctx_dem_mosaic
Files used: M20_JezeroCrater_CTXDEM_20m.tif (20 m DEM), M20_JezeroCrater_CTXortho_mosaic_5m.tif (5 m orthomosaic).
License: Public domain (per USGS catalog record); cite authors.
