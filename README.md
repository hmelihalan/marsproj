# MarsPath — Phase 1 desktop skeleton

Native Qt Quick / C++17 application with Exploration, Mission Planning and Scientific Discovery placeholders, authoritative PDS-label metadata display and explicit scientific limitations.

## Windows build and run

Requires Visual Studio 2022 Desktop development with C++, Windows SDK, CMake 3.20+, Qt 6.5+ (Core, Gui, Qml, Quick, QuickControls2, Test), GDAL and PROJ, all built for x64 MSVC. Install dependencies with:

```powershell
C:/vcpkg/vcpkg.exe install 'gdal[core]:x64-windows' 'qtbase[core,gui,widgets]:x64-windows' qtdeclarative:x64-windows
```

Run from the repository root:

```powershell
$cmake='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
$ctest='C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/ctest.exe'
$env:PROJ_DATA='C:/vcpkg/installed/x64-windows/share/proj'
$env:QT_PLUGIN_PATH='C:/vcpkg/installed/x64-windows/Qt6/plugins'
$env:QML_IMPORT_PATH='C:/vcpkg/installed/x64-windows/Qt6/qml'
& $cmake -S . -B build -G 'Visual Studio 17 2022' -A x64 -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
& $cmake --build build --config Release
& $ctest --test-dir build -C Release --output-on-failure
./build/Release/marspath.exe --data-root .
```

Use actual plugin/QML directories if your Qt installation differs. This is a development setup; standalone distribution requires the matching `windeployqt --qmldir qml`, GDAL/PROJ DLLs and runtime data. Review dependency licenses before distribution.

`--data-root` defaults to the working directory. Registry labels reside in `data/metadata/ctx-dem.xml` and `ctx-ortho.xml`; pilot files reside in `pilot_v1/pilot_v1/`. Refresh checks local file presence only. Phase 0 pixel identity is historical evidence, not a claim of current revalidation. Missing files and malformed labels display unavailable/error states.

## Architecture and scope

`src/app` contains QObject services; `src/data` contains the metadata registry; `src/gis` is the native GDAL/PROJ boundary; `src/analysis`, `src/routing`, `src/science` contain interfaces only. `qml` contains three views, reusable Theme and ModuleView components. No geospatial processing occurs in QML. No analysis, routes, scores, maps or globe are implemented.

Existing Phase 0 targets/tests remain intact. Configure a separate build directory with `-DMARSPATH_BUILD_APP=OFF` to build only those tools. Parent identity tests require downloaded parent TIFFs at configure time. Application Qt Tests cover initialization, registry failure/refresh behavior, QML navigation and resize.

The precise CTX vertical datum and independent feature registration remain unresolved. The imagery label declares zero missing although its TIFF has no NoData tag; Phase 0 found 8,277 missing pilot pixels. Original rasters are preserved. See [Phase 0 findings](docs/phase0-validation.md) and [Phase 1 report](docs/phase1-implementation.md).

## Project skills (original installation notes)

Copy the `.agents` folder into the **root of your existing MarsPath repository**. These files are instruction-only and contain no executable scripts.

## Included
- `qt-cpp-development`
- `planetary-gis`
- `nasa-data-validation`
- `mcda-terrain-analysis`
- `pathfinding-algorithms`
- `scientific-data-integrity`

## Windows PowerShell install
From the folder where you extracted the archive:

```powershell
Copy-Item -Path ".\.agents" -Destination "C:\Users\swind\Desktop\marsexpo" -Recurse -Force
```

If your repository is elsewhere, replace the destination path.

After copying, check:

```powershell
Get-ChildItem .\.agents\skills\*\SKILL.md
```

Run this verification **from your repository directory**. Restart Codex if skill discovery has not refreshed.

## Suggested Codex invocation

`Use the nasa-data-validation and planetary-gis skills for Phase 0. Inspect available Jezero datasets, verify actual product metadata, and report evidence. Do not start Qt UI work yet.`

## Notes
- Skills are scoped to the repository and complement your existing `AGENTS.md`; do not overwrite it.
- These are project-specific skills, not official NASA-certified methods.
- Tool installation (Qt, GDAL, PROJ, compiler) is separate.
- Codex docs: https://learn.chatgpt.com/docs/build-skills
