# Science data sources for the Jezero pilot box (Phase 6 preparation)

Status: desk research only. Nothing below has been downloaded, opened or validated against the pilot rasters.

Pilot box: Jezero Crater, X 4351000-4356000 m, Y 1090800-1095800 m (Mars equirectangular, R = 3396190 m), approx. 77.39-77.48 E, 18.40-18.49 N. See `pilot_v1/pilot_v1/README.md`.

## 1. CRISM MTRDR (mineral spectral data)

- What: map-projected targeted hyperspectral cubes derived from CRISM targeted observations, plus spectral summary parameter products and browse products. Spatial resolution is roughly 6-12 times higher than the CRISM multispectral map tiles (MRDR).
- PDS3 data set ID: `MRO-M-CRISM-5-RDR-MPTARGETED-V1.0`. Product names look like `FRT00005C5E_07_IF166J_MTR3`.
- A known MTRDR (FRT00005C5E) covers Jezero crater; whether it overlaps the pilot box is **not yet checked**.
- Coverage lookup: ODE (https://ode.rsl.wustl.edu/mars/coverage/ODE_Coverage.aspx) offers MTRDR footprint coverage files.
- Documentation: CRISM Product Primer and MTRDR SIS on ODE.
- Pixel size: **unverified**. Recalled as ~18 m/pixel for full-resolution targeted observations; confirm from the product label before use.
- Caveat: summary parameters are spectral indices, not mineral abundances. The UI must not present them as detections.

## 2. Geologic map SIM 3464 (Sun and Stack, 2020)

- USGS Scientific Investigations Map 3464, scale 1:75,000, vector database (zip) in a USGS data release. DOI: 10.3133/sim3464.
- Extent per metadata: 76.8-77.8 E, 17.7-18.7 N, which contains the pilot box.
- Registration caveat: the map was made on a semi-controlled CTX mosaic, before the controlled CTX mosaic released July 2020. Our DEM and orthomosaic come from the controlled Calef mosaic, so unit boundaries may be offset from our layers. Offset is **unmeasured**.
- Scale caveat: 1:75,000 is regional; unit boundaries are not suitable for rover-scale decisions.

## 3. Photogeologic map of the Perseverance field site (Stack et al., 2020)

- Space Science Reviews 216, 127. DOI: 10.1007/s11214-020-00739-x. Mapped at 1:5000 on a 25 cm HiRISE orthomosaic with a 1 m HiRISE DTM.
- Whether the map data are available as a separate download: **not verified**.

## 4. Scientific context (not data)

- Olivine-carbonate mineralogy of the Jezero region from CRISM MTRDR (arXiv 1904.11414).
- Carbonate-bearing units are reported in the delta, margin fractured unit and nearby aeolian bedforms (Stack et al., 2020 unit names).

## Open items

1. Run the ODE footprint query for the pilot box and list overlapping CRISM MTRDR observations.
2. Read the label of each overlapping product: pixel size, CRS, parameter definitions, bad-pixel conventions.
3. Download the SIM 3464 database and measure its offset against `ctx_5m.tif` at several features.
4. Confirm license and citation text for each source.
5. Decide which parameter layers, if any, are shown in Phase 6, and how limitations are displayed.
