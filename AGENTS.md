# MarsPath agent instructions
Read `docs/MASTER_PROMPT.md` before implementing work.

- Current stage: **Phase 0 only**. Do not begin full Qt UI, 3D globe or routing without new instructions. There's an pilot_v1 folder in the repo contains info for phase 0 make sure to check it
- Use native Qt 6 / C++17 for the future application; GDAL and PROJ for planetary GIS.
- Never use Earth WGS84/Web Mercator by default for Mars.
- Treat missing raster values as unknown, never traversable by assumption.
- Distinguish NASA-verified data from synthetic test fixtures.
- Do not fabricate coordinates, metadata or scientific observations.
- Document exact commands and tests that ran; never claim validation without execution.
