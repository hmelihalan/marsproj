# Phase 1 implementation

The user's explicit Phase 1 instruction supersedes the repository's earlier Phase 0-only stage restriction. Scope is a native Qt 6 / C++17 skeleton, with no scientific analysis implementations.

## Changes

- Qt Quick window, three independent placeholder views, keyboard-focusable navigation, dark shared Theme and reusable ModuleView.
- QObject application services own the dataset registry and validate module selection.
- Registry reads the two retained authoritative PDS XML labels and reports missing/malformed metadata, file availability, provenance and unresolved limitations. File presence is explicitly not scientific validation; Phase 0 parent identity remains historical evidence.
- CMake separates registry, services, GIS and interface-only terrain/routing/discovery targets. Explicit Qt, GDAL and PROJ dependencies. Existing Phase 0 targets and tests preserved.
- Qt Tests cover initialization, invalid navigation, missing and malformed metadata, refresh/availability, QML loading, repeated navigation, resize and warnings.

The planetary-gis and scientific-data-integrity skills informed the distinction between metadata, availability and scientific validity. No raster is modified; image zero values remain missing according to the label. CTX vertical datum realization and independent feature registration remain unresolved.

## Validation status

### Linux (Arch/Omarchy)

- Built with Qt 6.12.0, GCC 16.2.1, GDAL 3.13.3 using CMake + Ninja (`cmake -S . -B build -G Ninja`).
- First run: 3 of 4 CTest targets passed. `application_services_registry_ui` failed in `ApplicationTests::qmlNavigation()` because the test located the navigation button with `findChild`, which does not find items created inside QML Repeater/ScrollView content. The application itself was unaffected.
- Fix: `tests/application_tests.cpp` now searches the QML item tree (`childItems`) instead (commit 511c300, "Fix QML nav test lookup").
- After the fix the operator reported the application opens and the tests pass. The full post-fix `ctest` output was not captured in this document; re-run `ctest --test-dir build --output-on-failure` to record it.
- Windows remains unvalidated (see below).

### Windows

Qt installation initially failed on the C: drive with compiler errors C1088/C1085, “No space left on device.” The generated Qt build directory was moved to D: and installation retried there. Application compilation and Qt tests are pending completion of that installation.

The existing Phase 0 executables were tested after the source changes: 4/4 passed in 0.21 seconds. This does not validate the new Qt sources.

Commands attempted:

```powershell
C:/vcpkg/vcpkg.exe install 'qtbase[core,gui,widgets]:x64-windows' qtdeclarative:x64-windows
C:/vcpkg/vcpkg.exe install 'qtbase[core,gui,widgets]:x64-windows' qtdeclarative:x64-windows --x-buildtrees-root=D:/MarsPathBuildtrees --clean-buildtrees-after-build
$env:PROJ_DATA='C:/vcpkg/installed/x64-windows/share/proj'
& 'C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/ctest.exe' --test-dir build -C Release --output-on-failure
```

README includes Windows configuration, execution and runtime-path requirements. The next milestone after Phase 1 validation is Phase 2 planetary map display, subject to explicit instruction and the outstanding scientific limitations. No Phase 2 work is included here.
