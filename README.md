stardazed
=========

A project to create a full 3D game engine using modern C++ coding style.<br>
Requires a C++14 feature-level compiler and standard library.


Project Goals
-------------

* Build a performant, cross-platform, modern 3D game library
* Make all core functionality myself, only use external libraries
  for non-core features (like compression, jpeg decoding, etc.)
* Become more proficient with
  * Large-scale modern C++ projects
    * Obeying game-industry standard limitations such as no exceptions or RTTI
  * 3D-engine math and physics concepts
  * Modern 3D APIs: OpenGL 4.x, DirectX 11/12, Metal, Vulkan
  * Rendering techniques, working towards physically based rendering


Status
------

Nearing completion of render types with GL 4.1 implementation borrowing
from Metal for low-level render interfaces. Audio, input, scene graph and
other sections functional but simple.

(c) 2015-2017 by Arthur Langereis (@zenmumbler), all rights reserved
