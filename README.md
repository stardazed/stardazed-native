stardazed
=========

A project to create a full 3D game engine using modern C++ methods and
concepts.<br>
Requires a C++14 feature-level compiler and standard library which
at this point in time means Clang 3.4 or newer with libc++.


Project Goals
-------------

* Build a performant, cross-platform, modern 3D game library
* Become more proficient with
  * Large-scale modern C++ projects
    * Obeying game-industry standard limitations such as no exceptions or RTTI
  * 3D-engine math and physics concepts
  * Modern 3D APIs: OpenGL 4.x, DirectX 11/12, Metal, Mantle and WebGL
  * Rendering techniques, focusing on physically based rendering
  * Integration of external libraries like PhysX


Why
---

In this age where Unreal Engine 4 is now open-source, what is the point of
writing my own engine? It's mainly for my personal education. Reading and
extending existing codebases is useful, but I grok things best when I've
implemented it myself. After that I also feel more confident in evaluating
and extending existing codebases of the same nature.

Also, by keeping the specs limited enough for now and not having to worry
about supporting ancient compilers, OSes or 3D standards and techniques,
I can simply focus on keeping things as simple and tidy as possible.

In other words: _this is what I do to have fun_. Writing neat code and learning
while doing. Also, games.


Status
------

stardazed is in the early development phase. A full simulation construct
with a core OpenGL rendering back-end is working. Only Mac OS X 10.9+ is
supported at this time.

Work is ongoing whenever I have time, I'm updating
[my blog](http://cpphappyfunhouse.com/) regularly with updates about
development. I'm also [on twitter](https://twitter.com/zenmumbler).

----

(c) 2014 by Arthur Langereis, all rights reserved
