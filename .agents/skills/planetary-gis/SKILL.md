---
name: planetary-gis
description: >
  Use when loading, aligning, reprojecting, sampling, or comparing Mars raster and vector datasets with GDAL, OGR, PROJ, DTM/DEM, map tiles, or planetary coordinate systems in the MarsPath C++ application.
---

# Planetary GIS workflow

## Scope
Process **Mars**, not Earth. Treat planetary coordinate conventions as source-specific metadata, not a global assumption.

## Procedure
1. Collect provenance for every input: original URL / product ID, instrument, native CRS and longitude/latitude conventions, horizontal and vertical units, resolution, nodata, and coverage.
2. Identify longitude direction and domain (0–360 vs -180–180), latitude convention (planetocentric vs planetographic), Mars ellipsoid/sphere radii, projection parameters, axis order, datum assumptions, and image/DEM registration.
3. Define one explicit **analysis CRS** for the bounded Jezero workspace, with dimensions in meters. Record the full CRS definition and assumptions; never use terrestrial WGS84/EPSG:4326 or Web Mercator silently.
4. Verify the source footprint intersects the requested workspace before processing. Preserve native datasets unchanged.
5. Create an analysis grid anchored to a documented CRS, origin, pixel size and extent. Choose pixel size from the actual DEM's meaningful resolution, not the imagery resolution.
6. Resample continuous elevations with a defensible method and handle nodata masks; use nearest or mode for categorical classes. Preserve point/polygon features as vector features whenever possible.
7. Keep map imagery separate from analytical DEMs. Visual alignment is not proof of scientific alignment; validate with known georeferenced control locations.
8. Compute geodesic or projected distances with explicitly documented Mars geometry; account for diagonal movement.
9. Record transformations, software versions, input checksums, processing parameters, and uncertainty.

## Validation and failure modes
- Round-trip known coordinates through the transform and compare against independently checked control locations.
- Test dateline handling, pixel centers, partial coverage, nodata boundaries and different resolutions.
- Fail closed if CRS/datum or scale is ambiguous. Do not infer elevations from a colorized map or manufacture higher-resolution precision by interpolation.
- Report coverage, CRS, units, resolution, alignment residuals and limitations alongside derived outputs.

## References
- https://gdal.org/en/stable/
- https://proj.org/en/stable/
- https://pds.nasa.gov/
