============
Dependencies
============

GLFW / OpenGL
=============

1. This library is included as a package for most distributions.
   For me, installing glfw-devel did the trick.
2. Once I learn how to use CMake, I might find a better way to do this.

GLM
===

1. OpenGL Mathematics Library (Likely packaged somewhere)

Assimp
======

1. OpenGL Model Loading Library (Licely packaged somewhere)

NCurses
=======

1. This library is included with most systems out of the box.
2. If not, it is likely packaged.

Building
========

Running :code:`cmake ..` in the build directory will generate the required
build files.

After this, running :code:`cmake --build .` will build the executables and put
them in build/bin
