
#ifdef HEADER

struct Door : Spatial {
	Vec dest;
	void before_move ();
	Door (int16 type, room::Def* loc, Vec pos, Vec dest);
};

#else

void Door::before_move () {
	if (rata->floor)
		rata->propose_action(Rata::action_enter, this, pos, 1);
}
Door::Door (int16 type, room::Def* loc, Vec pos, Vec dest) : Spatial(type, loc, pos), dest(dest) { }



struct Crate : Object {
	virtual char* describe () { return "There's a wooden crate sitting here.\x80\nIt looks like it can be pushed around."; }
	virtual void damage (int d) { Object::damage(d); snd::def[snd::woodhit].play(); }
	Crate (int16 type, room::Def* loc, Vec pos, Vec vel = Vec(0, 0)) : Object(type, loc, pos, vel) {
		life = max_life = 144;
	}
};



struct Heart : Object {
	virtual char* describe () { return "Just as rats live off the refuse of humans,\x80\nYou too can live off of the rats.\x80\nPick this up to restore one heart."; }
	Heart (int16 type, room::Def* loc, Vec pos, Vec vel = Vec(0, 0)) : Object(type, loc, pos, vel) { }
};


struct TileLayer : Actor {
	void draw () {
		int minx = floor(viewleft());
		int miny = floor(viewbottom());
		int maxx = ceil(viewright());
		int maxy = ceil(viewtop());
		for (int x=minx; x < maxx; x++)
		for (int y=miny; y < maxy; y++) {
			int tile = map::at(x, y).id;
			bool flip = (tile < 0);
			if (flip) tile = -tile;
			if (type == type::front_tiles ? tile::def[tile].front : tile::def[tile].back) {
				draw_image(
					img::tiles,
					Vec(x+.5, y+.5),
					tile, flip
				);
			}
		}
	}
	TileLayer (int16 type) : Actor(type) { }
};

struct BulletLayer : Actor {
	void draw () {
		for (uint i=0; i < MAX_BULLETS; i++) {
			bullets[i].draw();
		}
	}
	BulletLayer (int16 type) : Actor(type) { }
};

struct CursorLayer : Actor {
	void draw () {
		if (draw_cursor) {
			float ax = rata->aim_center().x;
			float ay = rata->aim_center().y;
			float cx = cursor.x;
			float cy = cursor.y;
			if (cx+ax > viewright()) {
				cy *= (viewright() - ax) / cx;
				cx = viewright() - ax;
			}
			if (cx+ax < viewleft()) {
				cy *= (viewleft() - ax) / cx;
				cx = viewleft() - ax;
			}
			if (cy+ay > viewtop()) {
				cx *= (viewtop() - ay) / cy;
				cy = viewtop() - ay;
			}
			if (cy+ay < viewbottom()) {
				cx *= (viewbottom() - ay) / cy;
				cy = viewbottom() - ay;
			}
			draw_image(cursor.img, Vec(cx+ax, cy+ay));
		}
	}
	CursorLayer (int16 type) : Actor(type) { }
};

struct Shade : Spatial {
	Vec pos;
	Vec size;
	Color color;
	void draw () {
		draw_rect(
			pos.x, pos.y,
			pos.x + size.x, pos.y + size.y,
			color, false
		);
	}
	Shade (int16 type, room::Def* loc, Vec pos, Vec size, Color color) :
		Spatial(type, loc, pos),
		size(size),
		color(color)
	{ }
};


struct Lifebar : Actor {
	void draw () {
		if (rata) {
			int life = rata->life;
			for (int i = 0; i < (rata->max_life+47)/48; i++) {
				uint subimage;
				if (life >= i*48 + 48)
					subimage = 0;
				else if (life >= i*48 + 36)
					subimage = 1;
				else if (life >= i*48 + 24)
					subimage = 2;
				else if (life >= i*48 + 12)
					subimage = 3;
				else
					subimage = 4;
				draw_image(
					img::heart,
					Vec(19.5 - (i * 12*PX), 14.25),
					subimage,
					false, true
				);
			}
		}
	}
	Lifebar (int16 type) : Actor(type) { }
};

struct BGColor : Spatial {
	Vec size;
	Color color;
	void draw () {
		draw_rect(pos.x, pos.y, pos.x+size.x, pos.y+size.y, color);
	}
	BGColor (int16 type, room::Def* loc, Vec pos, Vec size, Color color) :
		Spatial(type, loc, pos),
		size(size),
		color(color)
	{ }
};









#endif



