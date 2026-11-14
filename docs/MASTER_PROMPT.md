# MASTER DEVELOPMENT PROMPT
# Interplanetary Survival Guide: Martian Map
# Native Desktop Application — Qt 6 / C++17

## ROLE AND WORKFLOW
Act as a senior C++/Qt desktop engineer, GIS engineer, planetary-data integration engineer, and algorithm developer. Build a scientifically grounded native desktop application for NASA Space Apps Challenge 2026. This document is the **project specification**, not authorization to implement everything in one step.

**Immediate instruction:** Execute **Phase 0 only**. Investigate data access and implement a small, repeatable C++/GDAL geospatial proof of concept. Report results and stop. Do not scaffold the full application, build the 3D globe, or proceed to Phase 1 unless explicitly instructed in a subsequent message. Do not claim successful verification unless the requested checks actually ran.

## 1. PROJECT OVERVIEW
Develop **Interplanetary Survival Guide: Martian Map**, a real-data-driven desktop application for exploration, scientific discovery, and terrain-aware mission planning in Mars's Jezero Crater (approximately 18.44°N, 77.57°E; approximate regional reference, not a verified waypoint).

The application must combine verified NASA/planetary datasets, allow scientific-objective-driven layer selection, visualize a 2D Mars map, inspect scientific observations, generate reproducible terrain calculations, and compare alternative routes. It is **not** a chatbot or AI-generated scientific map.

### Three application modes
1. **Exploration**: browse Mars layers and inspect real data.
2. **Mission Planning**: select start/destination, adjust terrain criteria, calculate and compare routes.
3. **Scientific Discovery**: choose scientific objectives, discover supported target points and evidence, and add waypoints to missions.

These are application modes, **not scientific objectives**. Scientific objectives (e.g., Geological Exploration and Ancient Water Evidence) are configurations that can affect layers, scientific target filters, and explanations across modes.

### Non-negotiable principles
- Real planetary datasets, documented sources, deterministic computations.
- No invented measurements, observations, coordinates, scientific points, layers, or API endpoints.
- Explicitly distinguish raw data, derived values, hypotheses, and temporary development mock data.
- Preserve original data and provenance: mission/instrument/product ID, URL, acquisition time if supplied, resolution, CRS, spatial coverage, processing history, and any applicable attribution.
- Missing values are unknown, **never zero risk**.
- A calculated route is *lower model cost*, not guaranteed physically safe. Avoid NASA-certified safety claims.
- Treat scientifically interesting terrain separately from traversability.
- Document and test every analytical assumption.
- Do not introduce LLMs, chatbots, agents, computer vision, object detection, classification, or AI-generated observations.

## 2. REQUIRED DESKTOP TECHNOLOGY STACK
Use **C++17 or newer** and **Qt 6** as the sole application runtime.

- **Qt Quick / QML**: cinematic desktop UI, navigation, controls, responsive window layouts, transitions.
- **Qt Quick 3D**: real, interactive, textured 3D Mars sphere on the home screen; defer until the scientific MVP works.
- **C++ QObject models / QAbstractListModel**: expose typed backend state and validated data to QML using signals/slots and Q_PROPERTY.
- **Qt Quick Scene Graph / QQuickItem**: custom 2D planetary map renderer; use map-coordinate-to-screen transformations and efficient tile/raster loading. An appropriate C++ geospatial visualization solution may be used if validated. **Do not rely on an Earth-specific Qt Location/WGS84 basemap or Web Mercator behavior for Mars.**
- **GDAL/OGR**: GeoTIFF/raster and vector I/O, windowing, resampling, processing.
- **PROJ**: coordinate transformation using correctly defined Mars planetary CRS; check compatibility with GDAL.
- **Qt Network**: validated, asynchronous data/WMTS tile retrieval when network access is available; local dataset fallback.
- **Qt Concurrent / QThreadPool**: background raster loading, grid preparation, routing; never block the GUI thread.
- **Qt Test**: unit and integration tests.
- **CMake**: reproducible multi-platform build and dependency management.
- **C++ standard library**: A*, graph operations, MCDA, optimization; optional external algorithm library only if justified.
- **Data formats**: GeoTIFF, GeoJSON, JSON/metadata. Optional SQLite/SpatiaLite only if needed; do not introduce a database without justification.

Do **not** use Python, FastAPI, JavaScript frontend frameworks, Electron, PySide/PyQt, Node.js backend, or browser-based mapping UI. QML is acceptable as Qt's declarative UI language, with all application logic, geospatial processing, and algorithms implemented in C++.

## 3. SCIENTIFIC DATA SOURCES AND VALIDATION
Investigate and verify data suitable for Jezero, without assuming that an indexed product is usable or that all products share a grid:

- NASA Mars Trek and WMTS documentation: https://trek.nasa.gov/mars/ and https://trek.nasa.gov/tiles/apidoc/trekAPI.html?body=mars
- MOLA / MGS: https://pds-geosciences.wustl.edu/missions/mgs/mola.html
- CTX / MRO: https://pds-imaging.jpl.nasa.gov/volumes/mro.html
- HiRISE images and DTMs: https://www.uahirise.org/
- THEMIS / Mars Odyssey: https://themis.asu.edu/
- CRISM / MRO: https://pds-geosciences.wustl.edu/missions/mro/crism.htm
- USGS Astrogeology: https://astrogeology.usgs.gov/
- Perseverance mission observations: official NASA/PDS mission products
- HRSC / Mars Express (ESA): https://www.cosmos.esa.int/web/psa/psa-instrument-hosted-data
- SWIM: https://swim.psi.edu/ **only if actual coverage and relevant interpretation are verified**; ice-related mapping must not be conflated with ancient-water geological evidence.
- PDS: https://pds.nasa.gov/

For **each selected product**, verify and record:
1. Exact product identifier and trustworthy source URL.
2. Actual Jezero study-area footprint and valid coverage.
3. Native pixel/ground resolution and useful scale.
4. Data type, units, nodata values, and measurement meaning.
5. Mars CRS/projection/datum, longitude convention (east/west, 0–360 vs ±180), and latitude convention (planetocentric vs planetographic).
6. Format and confirmed download/access method; do not infer a URL or API.
7. Metadata and acquisition date where available.
8. License/attribution and usage requirements.
9. Scientific suitability for each proposed feature.
10. Verification status: verified working, metadata only, inaccessible, or not suitable.

Imagery is not a DEM. Do not calculate physical slopes from a colored terrain visualization or an imagery basemap. Never pretend that resampling a coarse raster yields new fine-scale measurements. The appropriate **numerical DEM/DTM**, not a rendered topographic image, is essential for slope-based routing.

## 4. CORE ARCHITECTURE
Implement a clean, testable, modular **single-process native C++ application**. Separation of concerns:

- **Presentation** (QML): pages, maps, panels, animations, user input and status.
- **Controllers / view models** (QObject): manage UI state and invoke services.
- **Data access**: metadata registry, raw raster/vector loaders, verified optional network fetch and caching.
- **Geospatial**: Mars CRS conversions, coverage intersection, raster alignment, sampling and validity masks.
- **Scientific objectives**: objective configs, evidence-target filtering and availability checks.
- **Terrain analytics**: derivatives, traversability, objective-independent terrain costs.
- **Routing**: A*, shortest path, weighted cost path, validation and comparison.
- **Mission management**: selected waypoints, mission constraints, reports and export.

Suggested CMake repository layout:

```text
marspath/
  CMakeLists.txt
  cmake/
  src/
    main.cpp
    app/
    models/
    data/
      DatasetRegistry.*
      RasterRepository.*
      TileProvider.*
    gis/
      MarsCoordinateSystem.*
      RasterAlignment.*
      CoverageValidator.*
      TerrainAnalysis.*
    objectives/
      ObjectiveRegistry.*
      LayerResolver.*
      TargetFilter.*
    analysis/
      Normalization.*
      McdaEngine.*
      TerrainCostModel.*
      ScientificSuitabilityModel.*
    routing/
      AStar.*
      RouteEvaluator.*
      RouteComparison.*
    science/
      ScientificPointRepository.*
    missions/
      MissionPlanner.*
    ui/
      MapCanvas.*
      MapViewModel.*
  qml/
    Main.qml
    pages/
      Home.qml
      Exploration.qml
      MissionPlanning.qml
      ScientificDiscovery.qml
    components/
      MarsGlobe.qml
      PlanetaryMap.qml
      LayerPanel.qml
      CoordinateInspector.qml
      PointDetails.qml
      RoutePanel.qml
      RouteProfile.qml
      MissionSummary.qml
  resources/
    textures/
    icons/
  data/
    raw/
    processed/
    metadata/
  tools/
    data_probe/
  tests/
  docs/
    datasets.md
    architecture.md
    coordinate_conventions.md
    analysis_methodology.md
    limitations.md
  README.md
```

The exact structure may be adjusted to fit the repository, but keep the separation explicit. Heavy raster assets should not be committed accidentally; use manifests/checksums and document how to obtain them. Never overwrite raw scientific data during preprocessing.

## 5. THREE MODES AND HOME SCREEN

### Home: cinematic Mars exploration
Create a polished, professional native desktop home screen with dark space-blue/near-black background, Mars-orange accents, restrained glow/translucent panels, accessible text and a prominent **3D Mars globe**. Use the supplied visual reference image only as visual inspiration, not as a screenshot background.

The **real Qt Quick 3D object** must:
- Be a sphere with scientifically appropriate Mars texture and provenance.
- Rotate gently around its own axis by default.
- Support mouse drag rotation and bounded mouse-wheel zoom.
- Smoothly resume auto-rotation after manual interaction.
- Use reasonable lighting, optional subtle atmospheric glow and star field.
- Respect reduced-motion settings, minimize GPU load, and avoid expensive rendering while hidden.

Provide persistent navigation and three prominent mode cards:
- Exploration (orange)
- Mission Planning (blue)
- Scientific Discovery (violet)

Main heading: **Explore. Plan. Discover.**
Subtitle: **Your interactive guide for safe and informed exploration of Jezero Crater on Mars.**
Show Jezero regional information only when verified. Do not display a “live data” indicator without an actual service check. The 3D globe is decorative/navigation UI; it must **not replace the precise scientific 2D GIS map**.

### Exploration mode
- Interactive 2D Mars map, pan, zoom, coordinates, scale, legend and source/coverage details.
- Layer visibility and opacity controls; distinguish base maps from numeric scientific layers.
- Base imagery (verified CTX/HiRISE, other suitable layers), numeric terrain derivatives, geology/mineral observations, science points, model cost and route overlays as available.
- Point inspector for validated elevation, slope, other observations and metadata, with clear NoData states.
- No false availability or unverified values.

### Mission Planning mode
- Same shared 2D Mars map engine with start/end picking.
- Weight controls for available terrain criteria; explicit limits and no-go regions.
- Shortest-distance and lowest-MCDA-cost routes; distinct rendering.
- Route distance, model cost, average/max slope, elevation profile and data coverage/limitations.
- Comparative and explainable route selection; mission waypoint management and export.

### Scientific Discovery mode
- Verified scientific points/regions with filters, evidence details, mission/instrument/source citations, resolution and uncertainty.
- Scientific objective selector that updates relevant layers and target filters.
- Add verified science sites to mission as waypoints.
- Keep ancient-water geological evidence distinct from present accessible water or life detection claims.

## 6. OBJECTIVE-AWARE LAYER MANAGEMENT (REQUIRED)
Application modes and **scientific objectives are independent**. Implement a configuration-driven objective registry.

For each objective (start with Geological Exploration and Ancient Water Evidence) define:
- Objective ID, description and interpretation limits.
- Required/optional data categories and validated layer mappings.
- Supported geographic coverage.
- Target types/evidence tags and science filtering logic.
- Enabled analysis operations and disable reasons for missing data.
- Optional suggested basemap and default overlays.

Algorithmic flow:
1. User selects scientific objective and study region.
2. Resolve required datasets from registry.
3. Validate product availability, spatial overlap, CRS and intended scientific meaning.
4. Choose and render available layers; disclose unavailable data.
5. Filter verified scientific points and enable relevant UI.
6. Keep user free to toggle optional overlays manually.
7. If objective changes, refresh layer presets, targets and stale mission-derived results.

Do not scatter objective-specific `if` statements throughout UI; use reusable services/configuration. A required dataset unavailable in the region must result in an explicit unsupported/incomplete state, not fabricated output.

## 7. GEOSPATIAL FUSION AND MULTI-RESOLUTION DATA
Keep original rasters and each source's native resolution. Define a **small local common analysis grid** in an appropriate Mars projected metric CRS for selected Jezero subarea; do not conflate this with the viewport tile grid.

For every raster/vector source:
- Validate coverage and CRS; reconcile longitude conventions.
- Transform using correctly defined Mars CRS through GDAL/PROJ.
- Clip to study area, align grid origin/resolution/extent only as appropriate.
- Preserve NoData/validity mask and source resolution/quality per cell.
- Use bilinear or suitable interpolation for continuous scalar rasters only where scientifically justified.
- Use nearest-neighbor/mode for categorical geology.
- Use spatial joins/geometry intersections for science points and polygons.
- Treat optical imagery primarily as a visualization layer unless a validated numerical extraction method exists; do not invent automatic image interpretation.
- Record processing operations and units.
- Show unsupported or coarse areas as uncertain rather than safe.

The analysis grid's chosen cell size must reflect the DEM's effective quality and spatial resolution. A downsampled preview is acceptable for performance but cannot certify fine terrain clearance.

## 8. SEPARATE SCIENCE VALUE FROM TERRAIN RISK
Create **two independent surfaces**:

### Terrain Cost / Traversability
Uses only scientifically justified terrain criteria, such as valid DTM-derived directional slope, surface roughness where computable, elevation effects if not duplicative, and genuinely supported hazards. Apply hard exclusion masks for no-go conditions before routing. Unknown terrain does not silently become traversable.

### Scientific Suitability / Evidence
Supports target prioritization for the selected scientific objective using verified geology, minerals, documented observations, science tags, coverage and evidence quality. Start with transparent rule-based scoring or filtering; weights are development assumptions, not NASA-calibrated scientific truth.

Scientific score **must not override** hard terrain constraints. A valuable research target can be unreachable. Keep this distinction visible in UI and reports.

## 9. MCDA COST SURFACE
For the available, justified numeric terrain criteria, normalize with recorded methods to [0,1] (higher = more difficult). Allow configurable **nonnegative** weights; normalize active weights to sum to 1. If a layer is missing, remove that criterion only if the remaining model still makes scientific sense, renormalize transparently, and label the reduced model. If essential terrain data is missing, refuse route calculation.

At each valid cell x:

```
M(x) = Σ_k w_k · C_k(x),  0 <= M(x) <= 1
```

Maintain separate states for valid/low cost, valid/high cost, excluded, and unknown. Hard exclusions cannot be compensated by low costs elsewhere. Do not count the same slope information twice under different criterion names without rationale.

Suggested **initial weights are placeholders**, not NASA safety standards. Use only the criteria actually validated and available; do not force fake environmental criteria into the cost function. Document weight sensitivity and assumptions.

## 10. ROUTE PLANNING WITH A*
Represent the valid analysis area as a grid graph with appropriate physical neighbor distances in **meters on Mars**; make diagonal movement consistent, prevent corner-cutting across blocked cells, and account for cell-to-cell elevation changes when measuring slopes.

Compute two alternatives:
1. **Shortest valid-distance route**, using physical edge distance and the same hard constraints.
2. **Lowest terrain-cost route**, using a strictly positive edge cost.

Recommended baseline edge-cost model:

```
edgeCost(i,j) = d(i,j) * [1 + lambda * average(M(i), M(j))]
```

where d(i,j) > 0 is metric horizontal distance, lambda >= 0 is documented terrain sensitivity, and all M values are valid. An invalid/blocked transition is forbidden, not assigned a cheap cost.

Use an **admissible heuristic**, e.g., straight-line metric distance to destination when edgeCost >= physical distance. Verify the bound under any altered formula. Optionally compare with Dijkstra for correctness on small grids.

Calculate and display:
- Both route geometries and physical lengths.
- Accumulated model cost.
- Max/average sampled slope, elevation gain/loss and profile when supported.
- Blocked/unknown regions avoided.
- Resolution, coverage, source products, and limitations.
- Explanations of which criteria materially affected the selected route.

Do not label the result “safest” without qualification; use **lower modeled terrain cost**. Do not use an uncertified hard-coded threshold as a real astronaut capability limit.

## 11. OPTIONAL ADVANCED OPTIMIZATION
Only after a working MVP:
- **Hierarchical A*** for coarse corridor planning followed by high-resolution refinement, clearly preserving data-quality boundaries.
- Multi-target mission optimization under a distance/time budget, combining science value and valid route costs with explicit constraints.
- Additional scientific objective presets and verified thermal/mineral layers.

Do not add complex AHP/fuzzy weighting, D* Lite or RRT* merely for sophistication. Prefer simple, tested, explainable algorithms.

## 12. QT APP ENGINEERING AND UI QUALITY
- Keep the interface responsive by moving network/file I/O and large computations off the GUI thread.
- Expose thread-safe, typed C++ state to QML through minimal view-model APIs.
- Use a shared map core across modes, avoiding duplicate renderers.
- Lazy-load the 3D home scene and expensive raster layers.
- Cache downloaded imagery, honor license/attribution, make failures visible.
- Offer loading, empty, unavailable and failure states.
- Support desktop window resizing, high DPI, accessible labels, keyboard focus and reduced-motion.
- Discard/deallocate heavy Qt Quick 3D assets when not in use.
- Ensure a documented cross-platform build target; report which operating system(s) were actually tested.

## 13. DATA STORAGE AND LOCAL OPERATION
- Use local `data/raw`, `data/processed`, and `data/metadata` directories with platform-appropriate writable paths at runtime (e.g., QStandardPaths); project folders are development artifacts, not assumed install locations.
- Include product manifests and reproducible processing parameters/checksums.
- Store mission settings/waypoints and objective configurations as versioned JSON or another justified local format.
- Support already-downloaded offline data where possible; clearly identify features requiring network access.
- Never write into bundled read-only assets or overwrite raw datasets.
- Avoid a separate server/database unless a demonstrated feature needs it.

## 14. TEST AND ACCEPTANCE CRITERIA
Use Qt Test for C++ units and QML/UI checks where feasible.

**Data/GIS tests**
- Correct interpretation and round-trip transformation of representative Mars CRS coordinates.
- Raster coverage, expected pixel sampling and NoData handling.
- Raster alignment and no cross-source misregistration.
- Native vs processed resolution metadata preserved.
- Geospatial compatibility tests using small real-data samples when available.

**Terrain/MCDA tests**
- Synthetic planar DEM yields expected slope with meter-scale coordinates.
- Directional slope and elevation gain calculations respect units.
- Active weights normalize to 1; invalid weights rejected.
- Missing/blocked cells are not traversed.
- No nonphysical zero movement costs.

**Routing tests**
- A* matches Dijkstra's minimum cost on representative finite grids.
- Shortest route and cost-minimizing route can differ.
- Diagonal corner-cutting is prohibited.
- Unreachable and out-of-coverage points return explicit errors.
- Admissibility checked for the implemented heuristic/cost formulation.

**UI/mission tests**
- Scientific objective changes layers and filters, but not core terrain safety constraints.
- Layer toggles, map pan/zoom and coordinate inspection function.
- Verified science point opens correct source metadata.
- Route rendering agrees with computed coordinates.
- App remains usable while processing.
- Real-time 3D globe rotates and accepts drag/zoom when its phase is reached.
- No successful-state indicators for failed or unavailable sources.

## 15. DEVELOPMENT ROADMAP (STRICT ORDER)
Proceed by independently testable milestones; do not attempt everything in one giant commit.

### Phase 0 — Data Research and Minimal C++/GDAL Proof of Concept (**DO NOW; STOP AFTER THIS**)
1. Review the official challenge and available Mars planetary data documentation.
2. Identify at least one credible Jezero numerical elevation candidate and one suitable imagery source.
3. Verify actual product IDs, coverage, resolution, CRS/conventions, download methods and metadata to the extent access permits.
4. Obtain a small real sample **only if genuinely accessible**; otherwise report the blocker and use metadata-only analysis without pretending it succeeded.
5. Create a tiny C++17 + GDAL/PROJ utility (not the full app) that opens a real raster if obtained, prints dimensions, bands, nodata, geotransform, CRS and ground sampling, and verifies a Jezero subarea or reports incompatibility.
6. Demonstrate safe clipping/window extraction and numerical sample/NoData handling when possible.
7. Save a concise dataset assessment and the commands needed to reproduce the result.

**Required Phase 0 response:**
- Short data-source assessment with exact links/product IDs.
- Jezero coverage verification status (verified vs tentative).
- Formats, native resolutions, CRS/conventions and known limitations.
- Minimal compilable C++ data-probe source/CMake target and actual test results.
- Honest unresolved blockers and recommendation for first validated study subarea.
- **Stop. Do not implement Phase 1 or later without a new user instruction.**

### Phase 1 — CMake / Qt 6 project skeleton
Qt Quick application, modular C++ library targets, models, dependency checks, unit test scaffold; local build verified.

### Phase 2 — Functional 2D planetary GIS map
Verified Mars raster/tile display, precise coordinate handling, pan/zoom, data-source legends, layer controls and point inspection.

### Phase 3 — Terrain data processing
Verified local DEM sample; raster alignment, slope, optional valid roughness, masks, provenance.

### Phase 4 — MCDA cost model
Justified criteria, weight settings, validity/hard masks, cost visualization and test grid validation.

### Phase 5 — A* route planning
Shortest and lower-cost route, statistics/elevation profile and Dijkstra baseline tests.

### Phase 6 — Scientific Objective and Discovery
Objective registry, coverage-aware layer resolution, verified science-site markers, science evidence details and target filtering.

### Phase 7 — Mission workflow
Waypoints, alternative routes, mission summary and file export; error handling and reproducibility.

### Phase 8 — Cinematic Qt Quick 3D homepage
Real interactive rotating Mars globe, navigation cards, smooth UI transitions and optimizations.

### Phase 9 — Polishing and finalization
Data citations, offline/cache behavior, unit/integration/UI tests, performance, packaging, README and demo script.

Phase 8 may be developed in parallel by a different teammate **only after Phase 0 validates the scientific core**, without blocking scientific milestones.

## 16. DELIVERABLES
1. Source code and reproducible CMake build.
2. Working native Qt desktop executable for tested OS(es).
3. Data source register with provenance/coverage/CRS/resolution.
4. Documented GDAL/PROJ preprocessing steps.
5. Transparent MCDA methodology and calibrated-vs-assumed distinctions.
6. Documented A* cost/heuristic, route correctness tests.
7. Three functional application modes.
8. Objective-aware map layer selection and verified science observations.
9. Cinematic interactive 3D Mars home screen once scientific MVP is complete.
10. README, installation, tests, limitations, app demo instructions.

## FINAL INSTRUCTION FOR THIS CODEX RUN
**Implement Phase 0 ONLY.** Keep work small, reproducible and verifiable. Do not start UI design or application scaffolding. Never invent results or present inaccessible sources as validated. Show exactly what was executed, what passed, what failed, and where the science or data remains uncertain. Stop after reporting Phase 0 deliverables.
