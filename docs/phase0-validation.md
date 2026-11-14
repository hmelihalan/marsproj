# Phase 0 validation — 2026-11-14

**Current status:** the follow-up section at the end supersedes the initial access/provenance limitations below. CTX parents and labels have now been retrieved and both pilot crops match their parent pixels exactly. Scientific registration and precise CTX vertical-datum documentation remain unresolved.

## Scope and evidence

Read root AGENTS.md and MASTER_PROMPT.md; root and docs master specifications have identical SHA256 C404CB7FEB31C906D1FB2EA7896BEBE94095B52143E54508D63393C5E85AFA6A. Inspected all six project skill names and applied nasa-data-validation, planetary-gis and scientific-data-integrity. Existing project contains a C++ GDAL probe, synthetic fixture tests, and pilot_v1/pilot_v1 rasters. No later-phase implementation was undertaken.

## Dataset register

| Product | Mission/instrument and source | Resolution / format | Coverage and status | Role / limits |
|---|---|---|---|---|
| M20_JezeroCrater_CTXDEM_20m (claimed parent of dem_20m.tif) | MRO/CTX; [USGS catalog](https://astrogeology.usgs.gov/search/map/mars_2020_science_investigation_ctx_dem_mosaic), attributed to Calef (2021) by supplied README | Local crop: 20 m, Float32 GeoTIFF, 250 x 250 | Local geometry and bounded sample verified; parent provenance candidate | Candidate regional elevation; vertical datum, acquisition dates, source version and parent checksum unverified |
| M20_JezeroCrater_CTXortho_mosaic_5m (claimed parent of ctx_5m.tif) | MRO/CTX; same supplied catalog citation | Local crop: 5.0020646811 x 5.00226297161 m, Byte GeoTIFF, 1001 x 1000 | Local geometry and bounded sample verified; parent provenance candidate | Optical imagery only; no elevation interpretation |
| DTEEC_022680_1985_022746_1985_A01 | MRO/HiRISE; [instrument team DTM page](https://www.uahirise.org/dtm/dtm.php?ID=ESP_022680_1985) | Published 1 m/pixel, 480 MB download; file encoding not inspected | Metadata only: page center 18.23 N, 77.44 E; pilot overlap not verified | Higher-resolution elevation candidate; full CRS, vertical datum, footprint, NoData and temporal metadata still required |
| ESP_022680_1985_RED_C_01_ORTHO | MRO/HiRISE; same DTM page | Listed 44 MB orthoimage; pixel spacing and encoding not inspected | Metadata only; no local sample | Possible registered imagery companion; registration not tested |

The HiRISE page credits NASA/JPL-Caltech/UArizona. The supplied pilot README claims public-domain USGS licensing; this was not independently confirmed because the catalog returned HTTP 403 twice. No original archive raster was downloaded in this run. Local files are user-supplied derivatives with incomplete provenance, not independently authenticated NASA observations. Acquisition times and authoritative latitude/longitude conventions remain unverified.

## Local GDAL results

Both local rasters encode a Mars sphere of radius 3396190 m and equirectangular projection with standard parallel 18.4663 degrees, central meridian 0, zero false offsets and metre units. Datum names differ. No terrestrial CRS was substituted. Parameters agree, but this does not prove scientific registration.

DEM geotransform: [4351000,20,0,1095800,0,-20]. Its outer pixel extent is X 4351000–4356000, Y 1090800–1095800 m, derived directly from dimensions and transform. This is the initial candidate study box; the supplied README locates it approximately at 77.39–77.48 E, 18.40–18.49 N. Geographic coordinates were not independently transformed or checked against control points in this run.

Image geotransform: [4350996.10807,5.0020646811,0,1095800.91463,0,-5.00226297161]. Its grid differs from the DEM and must not be stacked cell-for-cell.

Extended src/data_probe.cpp reads a bounded central window at native pixel spacing, checks GDAL masks, declared NoData and nonfinite values, and reports valid/unknown counts and min/max. It does not overwrite inputs or resample.

| Input | Window x,y,width,height | Valid / unknown | Observed numerical range |
|---|---|---|---|
| Synthetic fixture | 0,0,4,3 | 11 / 1 | 1–12 |
| dem_20m.tif | 93,93,64,64 | 4096 / 0 | -2574.81933594 to -2533.21386719 |
| ctx_5m.tif | 468,468,64,64 | 4096 / 0 | 23–94 |

DEM declares NoData -9999; elevation units in metres come from the supplied README, not a verified vertical reference. Image declares no NoData; successful mask checks cannot establish that every image pixel is scientifically valid. Sample results do not establish whole-file coverage or extrema. Existing slope/roughness rasters were not validated or used.

SHA256 of inspected inputs:

- dem_20m.tif: 485365AFF47345760BB75BCB09C717ABABA691AA07A02D848EA8088729FE4FAC
- ctx_5m.tif: 925C7FBDD0693FBE1F60D7EFE301429C41B08797D5D373FD9B4A039CCC3E4F34

## Executed commands and tests

Windows, existing GDAL 3.12.4 / PROJ 9.9.0 installation. Initial probe runs emitted `Cannot find proj.db`; setting PROJ_DATA resolved the warning. The following exact commands were executed from the repository root:

```powershell
$env:PROJ_DATA='C:/vcpkg/installed/x64-windows/share/proj'
& 'C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe' --build build --config Release
& 'C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/ctest.exe' --test-dir build -C Release --output-on-failure
./build/Release/marspath_data_probe.exe build/fixture.tif
./build/Release/marspath_data_probe.exe pilot_v1/pilot_v1/dem_20m.tif
./build/Release/marspath_data_probe.exe pilot_v1/pilot_v1/ctx_5m.tif
Get-FileHash pilot_v1/pilot_v1/dem_20m.tif,pilot_v1/pilot_v1/ctx_5m.tif
```

Build succeeded. CTest: create_fixture and inspect_fixture passed (2/2, 0.09 seconds). Existing tests are smoke tests; sample counts above were inspected manually, not asserted by CTest. They do not validate NASA coverage. GDAL command-line tools were absent at C:/vcpkg/installed/x64-windows/tools/gdal; the linked C++ API was used instead.

## Claim audit and remaining gates

- Verified locally: raster dimensions, format, projection parameters, grid spacing, declared NoData, checksums and bounded reads.
- Derived: DEM projected bounding box and sample extrema/counts.
- Unsupported pending archive verification: complete provenance, acquisition dates, vertical datum, claimed original resolution, whole-raster valid coverage and scientific alignment.
- Unavailable during this check: USGS catalog body (HTTP 403). HiRISE raster download and metadata labels were not inspected; this is untested access, not an unavailable dataset. CRISM, THEMIS, MOLA, SWIM and rover observation products were not validated or registered.

Recommended next milestone remains Phase 0: retrieve original CTX PDS labels and download links, match the pilot crop to its parent, confirm vertical reference and coordinate conventions, then implement Mars geographic round-trip and overlap checks with independent controls. Assess the HiRISE footprint before choosing it for a finer study area. At 20 m, CTX cannot resolve rock-scale hazards; no safety or route conclusions are justified. No analysis grid reprojection, slope calculation, MCDA or routing was implemented.

## Follow-up: authoritative labels, parent identity and coordinate tests

### Retrieved evidence

Direct HTTPS downloads succeeded after the earlier browser 403 and sandbox network failure. Files retained in `data/metadata/`:

- `ctx-catalog.html`: the USGS catalog URL above, including its original download links.
- `ctx-dem.xml`: [authoritative DEM PDS4 label](https://astrogeology.usgs.gov/ckan/dataset/f8ef46b8-d512-4c46-b825-fe9633c38e1c/resource/5691ea08-1db4-4018-a341-e415e716add7/download/m20_jezerocrater_ctxdem_20m.xml).
- `ctx-ortho.xml`: [authoritative ortho PDS4 label](https://astrogeology.usgs.gov/ckan/dataset/f8ef46b8-d512-4c46-b825-fe9633c38e1c/resource/d139f28f-9976-40b1-aa55-49b78cebf055/download/m20_jezerocrater_ctxortho_mosaic_5m.xml).
- `hirise-directory.html`: [instrument archive listing](https://www.uahirise.org/PDS/DTM/ESP/ORB_022600_022699/ESP_022680_1985_ESP_022746_1985/).
- `hirise-header.bin`: first 32768 bytes only of [DTEEC_022680_1985_022746_1985_A01.IMG](https://www.uahirise.org/PDS/DTM/ESP/ORB_022600_022699/ESP_022680_1985_ESP_022746_1985/DTEEC_022680_1985_022746_1985_A01.IMG). This contains the complete embedded PDS3 label plus some payload; it is NOT a complete usable DTM.

Full CTX parent rasters were downloaded from the catalog's actual links into ignored `data/raw/`:

- [DEM](https://planetarymaps.usgs.gov/mosaic/mars2020_trn/CTX/ScienceInvestigationMaps_JPL/M20_JezeroCrater_CTXDEM_20m.tif), 90,345,300 bytes, SHA256 `BA7F706EA1B5D63A562F28A7DCF7A7F3C79BCE7F048E6974B57E8A33C30D1BA2`.
- [Ortho](https://asc-pds-services.s3.us-west-2.amazonaws.com/mosaic/mars2020_trn/CTX/ScienceInvestigationMaps_JPL/M20_JezeroCrater_CTXortho_mosaic_5m.tif), 359,599,280 bytes, SHA256 `CD9F4B2154FDD43E8C17ABFBAE63210B72507DB2209D1D7DB63F6F855B18D2DF`.

Label SHA256: DEM `DDF03B3DE30FFEE3D04BAEA2C8EBDF4558B5490A84989440C7447658C2F62732`; ortho `617B3D0A0608BBBA6016907CA30040E0C1FA6BCA89E3C27912CF0EF4C39C6A32`; HiRISE partial file `07A50C9DBD44356585138D0AEFFD557F2FF4503F31902917A743C9523A13AA52`. These are locally computed integrity fingerprints, not archive-supplied checksums. Original pilot hashes are unchanged.

### Metadata verification

The CTX labels identify version 1.0 products `urn:nasa:pds:mars2020_ctx_basemap_2021:data:m20_ctxdem_mosaic` and `urn:nasa:pds:mars2020_ctx_basemap_2021:data:m20_ctxortho_mosaic`. Both are derived MRO/CTX products targeting Mars. Start/stop observation times are explicitly nil/inapplicable; 2021 is publication metadata, not an acquisition date. Labels were inspected but not schema-validated. The catalog calls its status “PDS 4 Like”; do not infer formal archive certification from the identifier alone.

Both labels specify planetocentric latitude, positive-east longitude, spherical radius 3396190 m, equirectangular projection, standard parallel 18.4663 degrees, latitude origin and central meridian zero. The catalog specifies longitude domain -180 to 180. Local WKT matches these projection parameters. Catalog radii of 3396160 m conflict with both labels and both TIFFs; use 3396190 m for these products. Catalog minimum/maximum latitude fields are reversed; label south/north values are coherent.

| Parent | Label bounds west,south,east,north (degrees) | Size / grid | Missing value |
|---|---|---|---|
| DEM | 76.99743706146,17.58051991054,78.58256332409,19.29018700687 | 4456 x 5067, 20 m, Float32 GeoTIFF | -3.4028234663852886e38 |
| Ortho | 77.00085397838,17.58139102773,78.58147460364,19.28896260880 | 17766 x 20234, 5.00206468110262659 x 5.00226297161329292 m, Byte GeoTIFF | Label: 0; TIFF: no NoData tag |

Vertical reference: the catalog states added DEMs were vertically localized to MOLA and gives metres. Neither inspected CTX label specifies a precise areoid realization or vertical transformation. Therefore “MOLA-controlled elevation in metres” is source-supported; an exact named vertical datum and epoch remain unverified. Do not substitute the horizontal sphere for an elevation reference. Catalog says the ortho is matched to the DEM and horizontal accuracy is tied to a control net, but gives no independently measured pilot residuals.

### Traceability, coverage and alignment results

The new C++ test reads every pilot pixel and compares it exactly to the corresponding native parent window. It checks integer pixel offsets and equal pixel spacing, and fails on any value mismatch.

| Crop | Parent offset (column,row) | Compared values | Mismatches | Label-aware unknown values |
|---|---|---|---|---|
| DEM | 1100,2381 | 62500 | 0 | 0 |
| Image | 4359,9505 | 1001000 | 0 | 8277 |

This establishes pixel and grid traceability to the retrieved parent versions; it does not reconstruct the creator's processing history. DEM NoData was relabeled locally to -9999, but there are no parent missing values inside this crop. For imagery, the 8277 zeros must remain unknown under the PDS label, despite successful GDAL reads and the absent TIFF NoData tag. Raw files were preserved; the validation test applies the label-aware rule in memory. The general metadata probe still reports TIFF mask semantics only and must not be treated as a label-aware scientific validator.

GDAL/PROJ inverse transforms give DEM bounds 77.3887384279077 E,18.4024557792321 N to 77.4776705566458 E,18.4868088035227 N. Image bounds are 77.3886692043996 E,18.4024330318904 N to 77.4777270252868 E,18.4868242338808 N. The image rectangle covers 100% of the DEM rectangle; valid imagery coverage is incomplete. This calculation is exact for these unrotated equirectangular rectangles; no arbitrary footprint polygon capability is claimed.

Geometric grid alignment: different origins and noninteger 20 m / image-spacing ratio mean the grids do not nest. Each crop is aligned to its own parent. Scientific feature alignment between elevation and imagery is still unverified: no independent ground-control correspondences or registration residuals were measured. Matching WKT, parent identity and bounding boxes are insufficient to prove that alignment.

### HiRISE footprint decision

The actual DTM label states 7514 x 15967 Float32 PDS3 IMG, map scale 1.0097038146737 m/pixel (more precise than the page's rounded 1 m), local projection sphere radius 3394839.8133163 m, center latitude 15 degrees, center longitude 77.44 degrees, planetocentric latitude and positive-east longitude. Its horizontal geometry differs from CTX. Elevations are explicitly metres above the Martian equipotential surface (Mars 2000 Datum), with Smith et al. (2001) cited, scale 1 and offset 0. Missing constant is hex FF7FFFFB.

Label bounds: west 77.37089912, east 77.50346219, south 18.096543803297, north 18.368640005209 degrees. The pilot south edge is 18.4024557792321 degrees. **No overlap:** the gap is 0.0338157740231253 degrees latitude. Disjoint outer bounds suffice to reject overlap regardless of interior NoData. This specific strip is unsuitable for the pilot. Other Jezero HiRISE mosaics may cover it but are not verified here.

### Automated checks and exact execution

Added `tests/planetary_validation.cpp` and CMake tests `planetary_roundtrip_overlap` and conditional `parent_crop_identity` (enabled when both parent files exist). The first test derives geographic CRSs from each Mars raster, explicitly uses longitude/latitude axis order, checks 16 edge/center/pixel-center combinations per raster in both directions, and compares inverse results against an independent spherical equirectangular formula. Maximum round-trip errors: DEM 9.59985336665451e-10 m; image 2.3283064365387e-10 m; threshold 1e-6 m. Formula tolerance is 1e-9 degrees. These are numerical consistency tests, not independent geodetic control measurements.

Overlap assertions include disjoint, touching and partially overlapping synthetic rectangles; actual pilot coverage; and the HiRISE exclusion. Parent comparison also asserts 0 DEM unknown values and 8277 label-aware image unknown values. Tests reject other sphere radii, non-metre units and rotated grids rather than silently approximating them.

Executed downloads used `curl.exe -fLsS -L --max-time 120 <parent URL> -o data/raw/<filename>`. Labels used `curl.exe -fLsS --max-time 45 <label URL> -o data/metadata/ctx-dem.xml` (and ctx-ortho.xml). HiRISE used `curl.exe -fLsS --range 0-32767 --max-filesize 65536 --max-time 45 <IMG URL> -o data/metadata/hirise-header.bin`. URLs above are the exact arguments. Metadata directories were created with `New-Item -ItemType Directory -Force data/metadata` and `data/raw`; initial metadata download failed because its output directory did not yet exist, then succeeded on retry.

```powershell
$env:PROJ_DATA='C:/vcpkg/installed/x64-windows/share/proj'
& 'C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe' --build build --config Release
& 'C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/ctest.exe' --test-dir build -C Release --output-on-failure
./build/Release/marspath_planetary_validation.exe pilot_v1/pilot_v1/dem_20m.tif pilot_v1/pilot_v1/ctx_5m.tif data/raw/M20_JezeroCrater_CTXDEM_20m.tif data/raw/M20_JezeroCrater_CTXortho_mosaic_5m.tif
Get-FileHash data/raw/*.tif,data/metadata/ctx-*.xml,data/metadata/hirise-header.bin,pilot_v1/pilot_v1/dem_20m.tif,pilot_v1/pilot_v1/ctx_5m.tif
```

Both parent TIFFs were additionally inspected with `./build/Release/marspath_data_probe.exe <parent path>`. Build succeeded; final CTest run passed **4/4**, 0.22 seconds. All checks executed on Windows only. PROJ_DATA remains an environment prerequisite on this machine.

### Readiness decision

The project has a traced, numerically validated CTX pilot suitable for a Phase 1 engineering skeleton once explicitly approved. It is **not fully scientifically validated for terrain analysis**: the exact CTX vertical reference and independent feature-registration residuals remain unresolved, and imagery has missing pixels. If Phase 0 acceptance requires those gaps completely closed, that gate remains open. No Phase 1 implementation was started. Next scientific milestone: obtain the producer's precise vertical-reference documentation and independently controlled registration checks, plus identify a different HiRISE product that actually intersects the pilot.
