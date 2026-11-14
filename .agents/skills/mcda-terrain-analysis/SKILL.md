---
name: mcda-terrain-analysis
description: >
  Use when generating, modifying, explaining, or verifying raster-derived slope, roughness, terrain-cost grids, objective-aware scientific suitability layers, MCDA normalization, no-go masks, or criteria weights for MarsPath.
---

# MCDA terrain and scientific suitability workflow

## Architectural rule
Keep **terrain traversal cost** independent from **scientific value**. Scientific interest must never override traversal exclusion constraints. Include a criterion only when the source product supports the physical interpretation.

## Procedure
1. Confirm a verified, correctly registered DEM and any additional criterion-specific data, with known units, resolution and coverage.
2. Calculate slope from elevation using horizontal distances in meters. State finite-difference method and raster boundary handling; calculate slope over each possible step when direction matters.
3. Define roughness explicitly (for example local elevation standard deviation or terrain ruggedness index), including window size, native resolution and limitations; do not equate image texture to physical roughness.
4. Normalize available terrain **cost** criteria to [0, 1] using specified bounds and clamping; larger cost means worse traversal. NoData stays unknown and is never normalized to zero.
5. Keep a separate validity mask, hard no-go mask, and optional uncertain/insufficient-resolution mask.
6. Validate criterion weights are finite, nonnegative, and sum to 1 after normalization. If a criterion is unavailable, report whether the operation is disabled or weights renormalized across remaining **valid** criteria.
7. For valid cells compute `terrainCost = sum(weight[k] * normalizedCost[k])` and retain per-criterion contribution grids for explainability.
8. For objective-specific science suitability, use verified vector/raster evidence with independent weights and provenance; do not include science rewards directly in the physical safety exclusion mask.
9. Publish grid CRS, pixel size, min/max/meaning of cost, coverage, masks, original layers and processing settings.

## Critical tests
- All available criteria cost 0 => cost 0, **not** zero travel distance/cost.
- A fully excluded cell must never be traversable.
- Missing required elevations => invalid, never safe.
- Weights 0 or negative/NaN inputs handled explicitly; reject nonsensical values.
- Coarse data resampled to fine grid retains coarse uncertainty and precision statements.
- Compare cost outputs against a tiny hand-calculated raster fixture.

## References
- https://gdal.org/en/stable/
- https://pro.arcgis.com/en/pro-app/latest/tool-reference/spatial-analyst/how-weighted-overlay-works.htm
