---
name: scientific-data-integrity
description: >
  Use when reviewing scientific claims, source traceability, data-quality states, calculations, exports, user-facing labels, NASA attribution, or mock-versus-real data handling in MarsPath.
---

# Scientific integrity and traceability workflow

## Non-negotiable distinctions
- Observation: verified source data or source statement.
- Derived result: output of a documented reproducible calculation over verified inputs.
- Assumption: user-controlled or team-selected model parameter.
- Example/mock: illustrative, never presented as observed or measured.
- Unknown/unavailable: absent or unverifiable data, never silently replaced with zero or safe status.

## Procedure
1. List scientific claims and measurements introduced by the change.
2. Trace each to a concrete source/product identifier and citation, or to input datasets plus computation and parameters.
3. Verify scientific meaning and units: present-day ice vs historical water alteration; elevation datum; slope definition; roughness scale; raw observed vs model-derived temperature.
4. Preserve provenance with source URL, product ID, acquisition time when available, original CRS/resolution, processing log, input checksum when available, and software version.
5. Keep warnings and missing-data states visible in the Qt UI, exported mission report, and layer detail inspector.
6. Mark uncertainty explicitly; if quantitative confidence is not calibrated, do not invent probabilities, percentages or safety labels.
7. Verify documented limits: data gap, coverage, temporal relevance, measurement resolution, unknown terrain features.
8. Inspect example/mocks before delivery; remove or clearly label them and do not quietly substitute them for unavailable real results.

## Review output
Provide a claim audit: claim, underlying source or derivation, status (`verified`, `derived`, `assumption`, `unsupported`), limitation, and required correction.

## Tests
- Source links remain associated with targets, layers and exports.
- Unknown values render `unavailable`, not zero.
- Mission report never claims a traverse is NASA-certified safe.
- Invalid / absent source values block conclusions that depend on them.

## References
- https://pds.nasa.gov/
- https://www.nasa.gov/
