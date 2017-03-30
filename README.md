# GJK/EPA
Implementation of GJK (Gilbert–Johnson–Keerthi) and EPA (Expanding Polytope Algorithm) algorithm's in 2D (WIP)

2D implementation of the [Gilbert–Johnson–Keerthi distance algorithm](https://en.wikipedia.org/wiki/Gilbert%E2%80%93Johnson%E2%80%93Keerthi_distance_algorithm)

Note: A lot of the functions are not optimized. I'll try to mark them at some point, but in general be aware that this is not an optimal implementation, just an implementation.

Note 2: Have not added support for concave shapes, although you can draw them.

## Controls:
 * Q - switches between drawing and selecting mode. When drawing, a little red dot will follow the cursor
 * Drawing Mode:
   * Left Click - place vertex
   * Space - links the vertices in the order they were placed. Note: There is no testing for non-simple polygons yet, so KISS
 * Selecting Mode:
   * Left Click and Drag - move a shape around
   * Hold Left Click + Delete/Backspace - delete shape
   * Ctrl + Left Click - marks a shape. If 2 shapes are selected B replaces A and the new shape becomes B
   * Space - Calculates if intersecting
   
## Requirements
  * SFML for rendering
## References:
* GJK
  * Casey Muratori, "Implementing GJK" - [molleyrocket.com](https://mollyrocket.com/849)
  * Allen Chou, "Game Physics: Collision Detection – GJK" - [allenchou.net](http://allenchou.net/2013/12/game-physics-collision-detection-gjk/)
* EPA
  * EPA (Expanding Polytope Algorithm) - [http://www.dyn4j.org/](http://www.dyn4j.org/2010/05/epa-expanding-polytope-algorithm/ )
  * Allen Chou, "Game Physics: Contact Generation – EPA" - [allenchou.net](http://allenchou.net/2013/12/game-physics-contact-generation-epa/)

## See Also:
* [Minkowski sum calculator](https://github.com/inzombiak/MinkowskiSum)
* [Orbital physics engine](https://github.com/inzombiak/Orbitals)
