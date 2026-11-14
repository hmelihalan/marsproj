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

## ODE search result (2026-11-14, manual query by operator)

Query: MRO CRISM PDS4 MTRDR, product center within lat 18.2-18.7, lon 77.2-77.7 E (ODE searches by product center point, so this is a deliberately wider box than the pilot area). 8 products found. Read from an ODE screenshot; IDs transcribed by hand.

| Product ID | Obs time (UTC) |
|---|---|
| frt000047a3_07_if166j_mtr3 | 2007-02-26 04:35 |
| frt00005850_07_if167j_mtr3 | 2007-04-21 15:45 |
| frt00005c5e_07_if166j_mtr3 | 2007-05-19 09:31 |
| frt000066a4_07_if166j_mtr3 | 2007-06-27 10:27 |
| frt0001ecba_07_if166j_mtr3 | 2011-07-01 15:12 |
| frt0001fb74_07_if166j_mtr3 | 2011-08-14 19:34 |
| frt00021da6_07_if166j_mtr3 | 2011-12-14 01:27 |
| hrl000040ff_07_if183j_mtr3 | 2007-01-29 10:49 |

## Bounding-box coverage of the pilot box (from ODE metadata)

Pilot box (computed from the README CRS parameters): lon 77.387-77.476 E, lat 18.402-18.487 N. Overlap below uses each product's Min/Max lat/lon from ODE Meta Data, i.e. the **bounding box**. The real footprint is a skewed strip, so true coverage is equal or smaller. Percentages are rough, per-axis.

| Product | Pixel | Lat range | Lon range | Pilot box coverage (bbox) | Incidence | Verdict |
|---|---|---|---|---|---|---|
| frt00005c5e_07_if166j | 18 m | 18.3648-18.6352 | 77.2790-77.5586 | bbox: full. **True footprint (KML): ~80% of pilot area**; SW and SE corners outside, NW and NE inside | 63.8 | best candidate, south part missing |
| frt0001fb74_07_if166j | 18 m | 18.2698-18.5120 | 77.3984-77.6610 | bbox: lat full, lon ~88%. **True footprint (KML): ~52% of pilot area** (only SE corner inside) | 39.1 | partial, better lighting |
| hrl000040ff_07_if183j | 36 m | 18.2766-18.7327 | 77.2752-77.5611 | bbox: full. **True footprint (KML): 100% of pilot area**, all four corners inside | 55.6 | only product fully covering the box, coarser (36 m) |
| frt000047a3_07_if166j | 18 m | 18.4429-18.6881 | 77.3758-77.6420 | lon full, lat ~50% (south missing) | 58.0 | partial |
| frt00005850_07_if167j | 18 m | 18.1125-18.4291 | 77.1923-77.5035 | lon full, lat ~30% (north part only) | 64.1 | partial |
| frt000066a4_07_if166j | 18 m | 18.3425-18.5873 | 77.4690-77.7322 | east sliver ~0.007 deg only | 60.0 | marginal, likely none |
| frt0001ecba_07_if166j | 18 m | 18.2730-18.5079 | 77.6105-77.8662 | none | 45.5 | outside |
| frt00021da6_07_if166j | 18 m | 18.2684-18.4994 | 77.5012-77.7547 | none | 38.7 | outside |

All labels give Map Scale 18 m/pixel for FRT and 36 m/pixel for HRL (confirmed, replaces the earlier recalled value). Incidence angles are large (39-64 deg), so spectral parameters depend on illumination and cannot be compared across observations without care.

Still unverified: true footprint polygons of the other products (bounding boxes overstate coverage), data quality inside the box, bad pixels, and CRS of the .img files.

### True footprint check: frt00005c5e

The ODE product KML (`frt00005c5e_07_if166j_mtr3_xml.kml`, planetocentric lon/lat, 16 vertices) was projected into the pilot CRS (eqc, R = 3396190 m, lat_ts = 18.4663) and intersected with the pilot box (4351000-4356000 x 1090800-1095800 m, 25.0 km2).

- Footprint area 164.4 km2; intersection 20.09 km2 = **80.3%** of the pilot box.
- Pilot corners NW and NE are inside the footprint; SW and SE are outside (the missing part is the southern edge).
- The bounding-box estimate ("full") overstated coverage, as expected.
- Limits of this check: the KML polygon is the product extent, not a valid-pixel mask, so no-data pixels inside it are not accounted for. Geolocation accuracy of CRISM against the CTX/DEM layers is **not measured**.
- Same check not yet done for the two partial products (frt000047a3, frt00005850).

### True footprint check: frt0001fb74 and hrl000040ff

Same method (KML footprint projected to the pilot CRS, intersected with the pilot box).

| Product | Footprint km2 | Intersection km2 | Share of pilot box | Corners inside |
|---|---|---|---|---|
| frt00005c5e (18 m) | 164.4 | 20.09 | 80.3% | NW, NE |
| frt0001fb74 (18 m) | 154.3 | 12.99 | 52.0% | SE only |
| hrl000040ff (36 m) | 303.6 | 25.00 | 100% | all four |

- frt0001fb74: the bounding box suggested ~88% in longitude; the real footprint is skewed and covers about half. Bounding boxes overstate coverage.
- hrl000040ff is the only product that fully covers the pilot box, at half the spatial resolution (36 m vs 18 m; one 36 m pixel spans ~1.8 cells of the 20 m routing grid).
- Union of the two 18 m footprints (frt00005c5e + frt0001fb74) covers **98.4%** of the pilot box. The uncovered part is 0.39 km2 (1.6%), a sliver in the SW corner (about 1.4 km east-west by 0.5 km north-south from the corner). The two footprints overlap each other inside the box over 8.47 km2 (34% of the box), where both observations exist.
- Combining them is not free: the two observations differ in date (2007 vs 2011), illumination (incidence 64 vs 39 deg) and viewing geometry, so parameter values are not directly comparable and a seam would appear in a mosaic. Whether and how to merge is **undecided**.
- Same limits as before: KML is the product extent, not a valid-pixel mask; CRISM-to-CTX/DEM registration is not measured.

## Open items

1. Optional: check valid-pixel coverage and bad bands inside the pilot box once labels/data are read (KML gives extent only). The footprint union for the two 18 m products is done (98.4%).
2. Read the label of each overlapping product: pixel size, CRS, parameter definitions, bad-pixel conventions.
3. Download the SIM 3464 database and measure its offset against `ctx_5m.tif` at several features.
4. Confirm license and citation text for each source.
5. Decide which parameter layers, if any, are shown in Phase 6, and how limitations are displayed.
