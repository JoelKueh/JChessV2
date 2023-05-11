============
Dependencies
============

GLFW / OpenGL
=============

1. This library is included as a package for most distributions.
   For me, installing glfw-devel did the trick.
2. Once I learn how to use CMake, I might find a better way to do this.

NCurses
=======

1. This library is included with most systems out of the box.
2. If not, it is likely packaged.

Bear
====

1. This is not required, but my make-compile-commands script uses it to
   generate the proper configuration file for clangd.
2. Once again, it is likely packaged with whatever distro you are using.

Building
========

Hopefully the build script that is in the root directory just works after the
dependencies are installed. If it doesn't, I wouldn't be supprised because
I don't really know what I'm doing.
