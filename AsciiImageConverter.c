// Copyright 2013 Skookum Digital Works
// Author: Mark Rickert (mark@skookum.com)
// All rights reserved.
// Adapted from: https://github.com/deoryp/epaper
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
#include <stdint.h>
#include <string.h>

int atoi(char *p) {
	int k = 0;
	while (*p) {
		k = (k*10)+(*p)-'0';
		p++;
	}
	return k;
}

char dot(uint8_t byte, int bit) {
	return ((((1<<bit) & byte) >> bit) > 0) ? '0' : '1';
}

uint8_t tod(char c, int bit) {
	return (uint8_t)((c == '0' ? 1 : 0) << bit);
}

void asciiImage_readImage(char * file, uint8_t * image, int h, int w) {
	FILE * f = fopen(file, "r");
	int i, j, k;
	uint8_t pixelGroup;
	char c;
	int file_h = 0;
	int file_w = 0;

	char line[16];
	char * l = &line[0];
	memset((uint8_t*)&line[0], 0, sizeof(line));

	// read format
	do {
		c = fgetc(f);
		if (c == '\n') break;
		*l++ = c;
 	} while (c != EOF);

	// reset line
 	l = &line[0];
	memset((uint8_t*)&line[0], 0, sizeof(line));

	// read comment
	// do {
	// 	c = fgetc(f);
	// 	if (c == '\n') break;
	// 	*l++ = c;
	// 	} while (c != EOF);

	// // reset line
	// 	l = &line[0];
	// memset((uint8_t*)&line[0], 0, sizeof(line));

	// read width
	do {
		c = fgetc(f);
		if (c == ' ') break;
		*l++ = c;
 	} while (c != EOF);
	file_w = atoi(line);

	// reset line
 	l = &line[0];
	memset((uint8_t*)&line[0], 0, sizeof(line));

	// read height
	do {
		c = fgetc(f);
		if (c == '\n') break;
		*l++ = c;
	} while (c != EOF);
	file_h = atoi(line);

	// reset line
 	l = &line[0];
	memset((uint8_t*)&line[0], 0, sizeof(line));

	// now we are pointing to either newline or first char on image line.

	// printf("Opening file %s [%d, %d]\n", file, file_w, file_h);

	// reset line
 	l = &line[0];
	memset((uint8_t*)&line[0], 0, sizeof(line));

	if (h != file_h) printf("Height mismatch.\n");
	if ((w*8) != file_w) printf("Width mismatch\n");

	//read the lines.
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			k = 0;
			pixelGroup = 0;
			do {
				c = getc(f);
				// Ignore newlines and spaces
				if (c == '\n' || c == ' ') continue;
				pixelGroup += tod(c, 7-k);
				k++;
			} while (c != EOF && k < 8);
			if (c == EOF) return;
			image[i*w+j] = pixelGroup;
		}
	}
	fclose(f);
}
