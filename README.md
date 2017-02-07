# Landlord

A straightforward tile editor designed for jRPG style games.

## Features

Landlord offers the following features:

* Multi-layer editing
* Collision Mask suport
* Transparent tile support
* Placing down single tiles and groups of tiles on any of four tile layers
* Fill tool allows filling a layer with a single tile or a tile pattern
* Eraser tool with an adjustable brush size
* Dynamically generated minimap window
* Draggable minimap/tile palette windows, similar to graphic editing programs

## Why Landlord vs any other editor?

Landlord was built as a lightweigt editor for [NAS2D](https://github.com/lairworks/nas2d-core) based games. Many other editors exist but were either too complicated, too large or didn't provide the functionality needed.

Landlord isn't as fully featured as many of these editors. It's simple, straight forward and to the point without a lot of bells and whistles. That's it's strength -- simple tools often provide the best experiences.

It's also built to be extensible. It's easy to add new features if you need something tailored toward your own game. We use it at LairWorks for many of our 2D tile-based games because it just works.

## Installation

Binaries aren't as easily accessible as we'd like but so long as you have NAS2D and its dependencies, downloading and compiling Landlord should be fairly straight forward.

## Settings

Landlord doesn't offer much in the way of settings. You can change video and audio settings and that's about it.

In order to do so you will need to edit the 'editor.xml' configuration file in the '/data/' folder. Remember, Landlord is a simple program and leaving configuration to the XML file makes it a lot simpler.

## Troubleshooting

As with all NAS2D applications, the first step in troubleshooting problems is to make sure you have the latest versions of all of your device drivers. If all of your drivers are up to date and your video card supports an OpenGL 3.0 context, make sure that Landlord is installed in a directory with write permissions (e.g., installing it in 'c:\Program Files\' will make life difficult for you).

## License & Disclaimer

Copyright © 2014 - 2017 LairWorks Entertainment

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.