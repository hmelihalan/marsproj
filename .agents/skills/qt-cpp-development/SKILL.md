---
name: qt-cpp-development
description: >
  Use when implementing, refactoring, debugging, or testing native Qt 6 C++17 desktop application features, QML integration, Qt Quick maps or 3D scenes, models, CMake, or asynchronous UI tasks for MarsPath.
---

# Qt 6 / C++17 desktop workflow

## Scope
- Implement a **native desktop application** using Qt 6, C++17, CMake, Qt Quick/QML, and Qt Quick 3D for the optional homepage globe.
- Do not introduce React, Electron, Python, FastAPI, web servers, or invented third-party map widgets.
- Keep C++ scientific logic independent from UI/QML. Expose minimal QObject / QAbstractListModel facades for UI consumption.

## Procedure
1. Inspect existing CMake targets, Qt modules, build presets, and data structures; avoid unnecessary rewrites.
2. State which UI models, commands, and C++ services are being changed.
3. Put long-running GDAL I/O, raster processing, and pathfinding on a worker using Qt concurrency or a dedicated QThread; marshal results to the UI thread through queued signals.
4. Use RAII and clear ownership for QObjects, GDAL resources, and Qt Quick 3D assets. Do not access QML/UI elements from worker threads.
5. Design QML components to handle loading, no-data, error, and unavailable states.
6. Keep Mars 3D globe for homepage visualization; scientific exploration and route overlays remain accurate 2D planetary GIS.
7. Handle high-DPI screens, resized windows, keyboard navigation, and reduced-motion preference.
8. Never hardcode an Earth CRS or silently transform Mars coordinates with Earth defaults.

## Validation
- Configure and build with CMake and the project's detected Qt 6 toolchain.
- Run Qt Test suite / CTest; smoke test opening, resizing, switching modes, and exiting without leaks or crashes.
- Test worker cancellation and repeated navigation; no UI stalls during long computation.
- Report exact commands, results, unverified UI behavior, and any missing local dependencies.

## Common traps
- QObject used after destruction; repeated signal connections; QML binding loops; blocking network/raster I/O in GUI thread; wrong map tile projection.

## References
- https://doc.qt.io/qt-6/
- https://cmake.org/cmake/help/latest/
