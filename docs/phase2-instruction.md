# Phase 2 instruction: functional 2D planetary map

This explicit Phase 2 instruction supersedes the "Phase 0 only" stage line in `AGENTS.md`, as the Phase 1 instruction did. Read `docs/MASTER_PROMPT.md`, `AGENTS.md`, `docs/phase1-implementation.md`, `docs/science_data_sources.md` and `pilot_v1/pilot_v1/README.md` first. Keep all AGENTS.md rules (no fabrication, missing = unknown, never claim a check that did not run, document exact commands).

## Scope (Phase 2 only)

Replace the placeholder Exploration view with a working 2D map of the Jezero pilot area, using the real rasters in `pilot_v1/pilot_v1/`:

- `ctx_5m.tif` (imagery, visualization only)
- `dem_20m.tif`, `slope_20m_deg.tif`, `roughness_20m.tif`
- `slope_class_rover_ref_v2.tif` (Byte, NoData = 255; expected counts of classes 0/1/2/3 = 59924 / 1353 / 1050 / 173, read class meaning from the README and `thresholds.json`, do not invent labels)

Required features:
1. Custom map item (Qt Quick scene graph / QQuickItem) with pan, zoom (bounded) and correct map-coordinate to screen transform in the raster's own projected Mars CRS. No Earth WGS84 / Web Mercator / Qt Location.
2. Raster loading with GDAL off the GUI thread; display resampling must be labelled as display-only and never change analysis data.
3. Layer panel: visibility and opacity per layer, base imagery distinguished from numeric layers, legend with units and class colours, source and provenance text per layer.
4. Coordinate readout under the cursor: projected x/y in meters and planetocentric lon/lat via PROJ using the CRS read from the files (confirm with `gdalinfo`, do not hard-code values you did not verify). Scale bar.
5. Point inspector: click a location to show elevation, slope, roughness and slope class from the native rasters, with explicit "NoData / unknown" states (never 0, never "safe"), plus the source file and native resolution of each value.
6. Honest status: no "live data" indicators, visible failure state if a file is missing, graceful empty state.

Out of scope: CRISM or any new dataset, geology map, MCDA, routing, Mission and Discovery logic, 3D globe, new analysis. Do not modify any file under `pilot_v1/`. Do not commit rasters elsewhere.

## Required tests (Qt Test, run them)

- Screen/map transform round trip; zoom keeps the point under the cursor fixed.
- Sampling a known pixel of each raster returns the value `gdallocationinfo` / GDAL returns; NoData cell returns "unknown".
- Slope class histogram read through the app code equals 59924 / 1353 / 1050 / 173.
- PROJ round trip of the pilot box corners (lon/lat <-> x/y) within a stated tolerance.
- Existing 4 CTest targets still pass.

## Report

State exactly what was executed and the output, what passed or failed, OS and Qt version, and which features were only exercised by automated tests versus actually seen on screen. List remaining limitations (for example CTX/DEM registration is only partly checked, CRISM not integrated). Stop after Phase 2; do not start Phase 3.
