# GJK
Implementation of GJK algorithm in 2D (WIP)

2D implementaiton of the [Gilbert–Johnson–Keerthi distance algorithm](https://en.wikipedia.org/wiki/Gilbert%E2%80%93Johnson%E2%80%93Keerthi_distance_algorithm)

## Controls:
 * Q - switches between drawing and selecting mode. When drawing a little red dot will follow the cursor
 * Drawing Mode:
   * Left Click - place vertex
   * Space - links the verices in the order they were placed. Note: Theres no testing for non-simple polygons yet, so KISS
 * Selecting Mode:
   * Left Click and Drag - move a shape around
   * Left Click, Hold + Delete/Backspace - delete shape
   * Ctrl + Left Click - marks a shape. If 2 shapes are selected B replaces A and the new shape becomes B
   * Space - Calculates if intersecting
   
## References:
* Casey Muratori, "Implementing GJK" - [molleyrocket.com](https://mollyrocket.com/849)

## See Also:
* [Minkowski sum calculator](https://github.com/inzombiak/MinkowskiSum)
* [Orbital physics engine](https://github.com/inzombiak/Orbitals)
