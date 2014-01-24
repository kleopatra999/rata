### What is this?

This is a 2-dimensional sidescrolling action-adventure video game.  It uses
low-resolution pixel-art graphics, but with an unorthodox material-lighting
model.

### Compiling (on Linux)

There are a few dependencies for compiling this project.
 - OpenGL libraries (libgl1-mesa-dev on my system)
 - GLFW (libglfw-dev in debian-like systems)
 - SOIL (libsoil-dev in debian-like systems)
 - Box2D SVN r252
    - $ cd lib
    - $ svn checkout http://box2d.googlecode.com/svn/trunk/ box2d-read-only -r 252
    - Then follow that project's installation instructions.  Copy libBox2D.a
       into lib and make sure the Box2D headers are in lib/Box2D (such that the
       file lib/Box2D/Box2D/Box2D.h exists).
 - Perl >= 5.8 (for the build script)

Once all dependencies have been satisfied, you can build the main executable with
    $ ./make.pl
for debugging or
    $ ./make.pl --build=release
for an optimized release.  Then run the executable with
    $ ./rata

For very verbose OpenGL API tracing, compile with
    $ ./make.pl --compiler-opts=-DCORE_OPENGL_VERY_DEBUG

### Windows?

Windows support is not currently implemented, but is planned for the future.

### Internal documentation

This codebase is written in C++11, with OpenGL for graphics, and a JSON-like
data language called HACC for runtime data management.

Here is a list of the overall code modules, in rough order from high-level
to low-level.
 - shell: global integration, menus, editing, in-game console, initial state
 - world: contains images, sounds, levels, the material palette
 - ent: actors both player-controlled and AI, other game objects
 - geo: tilemaps, room loading system, camera control
 - phys: integration with Box2D physics engine, physics-related mixins
 - vis: graphics, text, shaders, segmented models
 - core: window management, input listeners, main loop, OpenGL integration
 - util: misc stuff for math, geometry, containers, logging, OS integration
 - hacc: HACC language integration, memory management, file IO
 - tap: testing (Test Anything Protocol)

For more in-depth documentation on one of these modules, see the README in
that module's directory, if it exists.


