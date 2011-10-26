


sf::Sprite drawing_sprite;
void draw_image (int16 imagei, Vec p, int sub=0, bool flip=false, bool cam=false, float scale=1.0) {
	if (imagei < 0) return;
	img::Image* image = &img::def[imagei];
	//if (!cam)
	//if (x < camera.x - 10 - img->w
	// || y < camera.y - 7.5 - img->h
	// || x > camera.x + 10 + img->w
	// || y > camera.y + 7.5 + img->h) return;
	uint iw = image->sfi.GetWidth();
	uint ih = image->sfi.GetHeight();
	sub %= image->numsubs();
	
	sf::IntRect sr;
	if (image->w == 0 || image->h == 0)
		sr = sf::IntRect(0, ih, iw, 0);
	else
		sr = sf::IntRect(
			sub % (iw/image->w) * image->w,
			sub / (iw/image->w) * image->h + image->h,
			sub % (iw/image->w) * image->w + image->w,
			sub / (iw/image->w) * image->h
		);
	drawing_sprite.SetScale(PX*scale, PX*scale);
	drawing_sprite.SetImage(image->sfi);
	drawing_sprite.SetSubRect(sr);
	drawing_sprite.FlipX(flip);
	drawing_sprite.FlipY(true);
	uint h = image->h;
	uint w = image->w;
	if (h == 0) h = ih;
	if (w == 0) w = iw;
	float xpx = p.x*UNPX - (flip ? w-image->x : image->x);
	float ypx = p.y*UNPX + image->y;
//	uint xpos = flip && image->w ? image->w - image->x : image->x;
	if (!cam)
		drawing_sprite.SetPosition(
			(round(xpx)-.5)*PX,
			(round(ypx)-.5)*PX
		);
	else
		drawing_sprite.SetPosition(
			window_view.GetRect().Left + xpx*PX,
			window_view.GetRect().Top + ypx*PX
		);
	window->Draw(drawing_sprite);
};
void draw_rect (float l, float t, float r, float b, sf::Color color=sf::Color(31, 31, 31, 127), bool cam=false) {
	if (cam)
		window->Draw(sf::Shape::Rectangle(
			viewleft() + l, viewbottom() + t,
			viewleft() + r, viewbottom() + b,
			color
		));
	else
		window->Draw(sf::Shape::Rectangle(
			round(l*UNPX)*PX, round(t*UNPX)*PX,
			round(r*UNPX)*PX, round(b*UNPX)*PX,
			color
		));
};
