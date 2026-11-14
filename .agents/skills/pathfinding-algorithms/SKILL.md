---
name: pathfinding-algorithms
description: >
  Use when implementing, improving, testing, or explaining MarsPath A*, Dijkstra baseline, grid edge weights, route comparison, no-go traversal, mission waypoints, or terrain-aware routing.
---

# MarsPath pathfinding workflow

## Inputs
A validated Mars projected grid, resolution and extent; traversability/NoData masks; normalized terrain cost; valid start/destination; explicit mission constraints. Missing terrain is not safe.

## Cost and heuristic
- Use nonnegative finite terrain penalties and strictly positive physical travel cost for valid steps:
  `edgeCost(i,j) = distanceMeters(i,j) * (1 + lambda * average(terrainCost[i],terrainCost[j]))`
  with `lambda >= 0`, and `terrainCost in [0,1]`.
- Reject transitions touching blocked/invalid cells. For diagonal moves, prohibit corner-cutting past blocked orthogonal neighbors unless a documented traversability policy explicitly permits it.
- When edge cost is at least physical distance, straight-line distance in the **same metric Mars analysis CRS** is an admissible A* heuristic. Where projection distortion is significant, replace it with a rigorously valid lower bound or zero (Dijkstra).
- Use the minimum cost formula consistently for every valid neighbor; preserve the independent distance-only routing mode.

## Implementation steps
1. Validate grid, masks, row-column orientation, CRS, dimension and bounds; snap world coordinates to nearest valid cells only with explicit policy and report offsets.
2. Implement 8-neighbor grid traversal with actual physical neighbor distances (horizontal/vertical/diagonal).
3. Implement A* with priority queue, best g-score and predecessor reconstruction; support cancellation and search-size guardrails.
4. Implement Dijkstra baseline using identical transition costs to verify optimality on small fixtures.
5. Compute metrics from the resulting path: ground distance, accumulated cost, slopes, elevation gain/loss, coverage/uncertainty and violations.
6. Emit map-compatible route geometry in the correct coordinate system with all source and cost model metadata.
7. Compare distance-optimal and terrain-cost-optimal routes; clearly state `lower modeled cost`, not guaranteed safety.

## Tests
- Flat grid returns expected shortest geometric path.
- Impossible/blocked destination returns no route, not a straight-line fallback.
- Invalid data mask and corner-cutting are honored.
- Distinct terrain costs produce different alternatives when expected.
- A* and Dijkstra return same cost with admissible heuristic.
- No path cost is zero for nonzero travel distance.

## References
- https://en.wikipedia.org/wiki/A*_search_algorithm (background; prefer algorithm tests over prose)
- https://doc.qt.io/qt-6/qtconcurrent-index.html
