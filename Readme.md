# rpi-eink

## Overview

If you have ever tried to display something on an e-ink screen from your Raspberry Pi, you understand what a painful process it can be. If you succeeded, you're probably a rockstar programmer. I am not a rockstar programmer, but I got it to work with a mishmash of bits and pieces of code and a lot of research about how the e-ink display works.

Here's how to do it so I can save you possibly weeks of work. Why? Because I'm a nice guy.

This software will allow you to easily take appropriately sized images on your machine (or from anywhere, really) and display them properly on the e-ink display.

## Hardware

You'll need a bit of hardware to get started.

1. A Raspberry Pi. You can purchase these literally everywhere. Doesn't matter what model.
2. An [Embedded Artists e-ink display](http://www.embeddedartists.com/products/displays/lcd_27_epaper.php). Specifically, this code is made to work with the 2.7" model.

## Software

### Linux Version

I'm using the latest version of [Rasbian](http://www.raspbian.org/), installed using the [NOOBS](http://www.raspberrypi.org/downloads) bootloader.

### Software Dependencies

You'll need the Linux build tools and [ImageMagick](http://www.imagemagick.org/) installed on your Raspberry Pi. You can do that with a simple one-liner:

```bash
sudo apt-get update && sudo apt-get install -y gcc make build-essential imagemagick
```

This will likely take a while. Go grab a cup of :coffee: or a :beer:.

## Putting The Hardware Together

You'll want to [follow the wiring diagram](http://www.embeddedartists.com/sites/default/files/support/displays/epaper/Epaper_RaspberryPi.pdf) from Embedded Artists on how to wire the e-ink display up to the GPIO pins on your Raspberry PI.

## Compiling the Software

1. Clone the repository to your Raspberry Pi and cd into the directory.
2. Run `make`.
3. This will create the compiled binary  `./epaper`.
4. There is no step 4.

## Running the Software

Simply pass an image file that is `264x176` or smaller (the application will automatically pad the image to that size) to the binary like so:

```bash
sudo ./epaper ~/my_awesome_image.png
```

A few things to note here:

1. It doesn't matter what format your image is in. JPG, GIF, TIFF, whatever. ImageMagick will take care of that automatically for us.
2. Don't forget to `sudo` when you run the binary. The application requires `sudo` access in order to use the GPIO pins.

## How it Works


# License

The portions written by Skookum Digital Works are MIT licensed

```
Copyright 2013 Skookum Digital Works

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

Other parts (not written by us) are outlined in their respective files. Please read all licenses before using this project.

```
Copyright(C) 2012, Embedded Artists AB
All rights reserved.
```

```
Copyright 2013 Pervasive Displays, Inc.
```