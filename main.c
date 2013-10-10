// Copyright 2013 Skookum Digital Works
// Author: Mark Rickert (mark@skookum.com)
// All rights reserved.
//
// The MIT License

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsp.h"
#include "Display_Controller.h"
#include "AsciiImageConverter.h"

int main(int argc, char *argv[])
{
	uint8_t* pOldImg;
	uint8_t* pImg;
	unsigned char FileImage[176][33];
	unsigned char PreloadImage[176][33];

	memset((uint8_t*)&PreloadImage[0][0], 0xff, 176*33);
	pOldImg = (uint8_t*)&PreloadImage[0][0];

	memset((uint8_t*)&FileImage[0][0], 0xff, 176*33);
	pImg = (uint8_t*)&FileImage[0][0];

	bsp_init();

	if (argc > 1)
	{
		char command[100];
		printf("Displaying image from file: %s\n", argv[1]);
		sprintf(command, "convert -monochrome -compress none -gravity center -background white -extent 264x176 %s /tmp/tmp.pbm", argv[1]);
		system(command);
		asciiImage_readImage("/tmp/tmp.pbm", pImg, 176, 33);
		epd_DisplayImg(DISPLAY_IN_USE, pImg, pOldImg);
	}
	else
	{
		printf("You must pass an image path to this utility.\n\n");
	}

	return 1;
}
