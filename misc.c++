





struct Crate : Object {
	virtual char* describe () { return "There's a wooden crate sitting here.\x80\nIt looks like it can be pushed around."; }
	virtual void damage (int d) { Object::damage(d); snd::def[snd::woodhit].play(); }
	Crate (obj::Desc* desc) : Object(desc) {
		life = max_life = 144;
	}
};



struct Heart : Object {
	virtual char* describe () { return "Just as rats live off the refuse of humans,\x80\nYou too can live off of the rats.\x80\nPick this up to restore one heart."; }
	Heart (obj::Desc* desc) : Object(desc) { }
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
			if (facing ? tile::def[tile].front : tile::def[tile].back) {
				draw_image(
					img::tiles,
					Vec(x+.5, y+.5),
					tile, flip
				);
			}
		}
	}
	TileLayer (obj::Desc* desc) : Actor(desc) { }
};

struct BulletLayer : Actor {
	void draw () {
		for (uint i=0; i < MAX_BULLETS; i++) {
			bullets[i].draw();
		}
	}
	BulletLayer (obj::Desc* desc) : Actor(desc) { }
};


struct Shade : Actor {
	void draw () {
		if (facing > -1 || paused) {
			draw_rect(
				pos.x, pos.y,
				pos.x + vel.x, pos.y + vel.y,
				data, facing <= 0
			);
		}
	}
	Shade (obj::Desc* desc) : Actor(desc) { }
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
	Lifebar (obj::Desc* desc) : Actor(desc) { }
};















