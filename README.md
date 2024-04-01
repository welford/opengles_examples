# OpenGL ES Examples

Some very simple examples of OpenGL ES on the Raspberry Pi

## Getting Started

1. Get Git (if you don't already have it)
Sudo apt-get install git-core

2. Make a directory somewhere for OpenGL ES Examples

3. Clone the respository
git clone https://github.com/welford/opengles_examples.git
OR
git clone git://github.com/welford/opengles_examples.git

## Building and running

* 2024:   Go into each examples directory and type "make sdl" on the command line

* 2014(?): Go into each examples directory and type "make" on the command line

* Then run ./000.bin etc

## Examples

Press escape to quit any of the examples

000 Creates a window ready for opengl drawing and clears it

001 Creates an uncoloured triangle

002 Same as 002 but using some helper libraries

003 Adds colour to the triangle

004 Creates a triangle that renders on top of the bash 

005 Creates a multi coloured triangle that renders on top of the bash 

006 Rendering in clip space from the bottom left to the top right

007 Demonstates creating a textures and applying various texture parameters to it  (use space to switch between them)

008 Demonstates static push/pop style transforms

009 Demonstates animated push/pop style transforms

010 A simple camera used to view an axis (use arrow keys to rotate the camera)

011 Shows various mip mapping modes (use space to switch between them)

012 Rendering to a framebuffer object

There will not be more to come...
