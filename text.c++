


uint letter_width [256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	3, 2, 6, 6, 6, 6, 6, 3, 4, 4, 6, 6, 3, 4, 3, 6,
	6, 4, 6, 6, 6, 6, 6, 6, 6, 6, 3, 3, 6, 6, 6, 6,
	8, 6, 6, 7, 7, 6, 6, 7, 6, 4, 6, 7, 6, 8, 8, 8,
	6, 8, 6, 6, 6, 8, 8, 8, 8, 8, 6, 4, 6, 4, 6, 6,
	3, 6, 6, 6, 6, 6, 5, 6, 6, 3, 3, 5, 3, 8, 6, 6,
	6, 6, 5, 6, 4, 6, 6, 6, 6, 6, 6, 5, 2, 5, 8, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

uint letter_width_small [256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 4, 4, 4, 4, 4, 4, 5, 4, 4, 4, 4, 4, 6, 5, 4,
	4, 5, 4, 4, 4, 4, 4, 6, 4, 4, 4, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

uint text_width (char* s) {
	uint r;
	for (r = 0; *s; s++) {
		r += letter_width[(uint8)*s];
		if (*s == '\n') break;
	}
	return r;
};

uint text_width_small (char* s) {
	uint r;
	for (r = 0; *s; s++) {
		r += letter_width_small[(uint8)*s];
		if (*s == '\n') break;
	}
	return r;
};



void render_text (char* text, float x, float y, uint lines=2, bool cam=false, bool small=false, float scale=1.0) {
	uint pos = 0;
	uint linecount = 0;
	uint lineheight = small ? 7 : 16;
	img::Image* font = small ? &img::font_small : &img::font_proportional;
	for (;*text != 0 && linecount <= lines; text++) {
		//printf("%c;%d;%d;", *text, *text%16, *text/16);
		if (*text == '\n') {
			linecount++;
			pos = 0;
		}
		else {
			draw_image(font,
				Vec(x + pos*PX*scale, y - linecount*lineheight*PX*scale),
				*text, false, cam, scale
			);
			if (small)
				pos += letter_width_small[(uint8)*text];
			else pos += letter_width[(uint8)*text];
		}
	}
	//printdesc->x, desc->y
}






