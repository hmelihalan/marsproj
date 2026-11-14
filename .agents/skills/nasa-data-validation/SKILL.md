---
name: nasa-data-validation
description: >
  Use when discovering, downloading, verifying, cataloging, or integrating NASA Mars Trek, PDS, MOLA, CTX, HiRISE, THEMIS, CRISM, Perseverance, or other planetary-science datasets for MarsPath.
---

# NASA Mars data validation workflow

## Do not assume availability
A referenced mission or data portal does not imply a downloadable product covering the chosen Jezero study area. Never claim a dataset has been downloaded, validated, or integrated without evidence.

## Procedure
1. Define the precise region and intended use: imagery, quantitative elevation, mineral observation, geology, or science waypoint.
2. Identify primary source and concrete product, mission, instrument, identifier, archive/asset URL, published metadata, terms and citation requirements.
3. Check access mechanism (WMTS, downloadable raster, PDS archive, tile endpoint); avoid guessing API paths. Inspect service capabilities when using tiles.
4. Check actual spatial footprint, resolution, CRS, longitude direction and latitude convention, acquisition time, raster bands, units, nodata and format.
5. Check the data can support the intended computation: optical imagery alone is **not** a DEM; coarse MOLA cannot validate meter-scale hazard avoidance; orbital mineral signatures are not direct water access.
6. Preserve original file, source URL, checksum when available, metadata and processing record. Keep raw and processed assets separate.
7. Verify one small sample in GDAL/OGR, including coordinate transformations and sample-value interpretation. Prefer a verified small sample to a large collection of unvalidated links.
8. Register only verified usable layers in the layer catalog. Mark layers missing data as unavailable and explain why.

## Required assessment output
A compact table with: product ID, mission/instrument, URL, format, CRS, native resolution, coverage, temporal metadata, status (`verified`, `candidate`, `unavailable`), scientific role, and known limits.

## Failure handling
- If network access fails, show `not verified`, do not guess metadata or invent fallback data.
- Distinguish science observation provenance from a location inferred by the team.
- Never conflate ancient water evidence with present-day subsurface ice. Confirm SWIM coverage before offering it in Jezero.

## References
- https://trek.nasa.gov/mars/
- https://pds.nasa.gov/
- https://www.uahirise.org/
- https://pds-geosciences.wustl.edu/missions/mgs/mola.html
