
#ifdef HEADER

void quit_game ();
void main_init ();
void main_loop ();

#else

void quit_game () {
	window->SetCursorPosition(cursor2.x*UNPX*window_scale, window->GetHeight() - cursor2.y*UNPX*window_scale);
	window->Close();
	throw 0;
}

void set_video () {
	if (window_fullscreen) {
		window->Create(sf::VideoMode(640, 480, 32), "", sf::Style::Fullscreen);
		window->SetView(window_view);
	}
	else {
		window->Create(sf::VideoMode(320*window_scale, 240*window_scale, 32), "");
		window->SetView(window_view);
	}
	window->UseVerticalSync(true);
	//window->SetFramerateLimit(60);
	window->EnableKeyRepeat(false);
	window->ShowMouseCursor(false);
	window->Display();
	frameclock.Reset();
	draw_latency = 0;
}


void main_init () {
	 // SFML
	window = new sf::RenderWindow;
	//window = new sf::Image(320, 240);
	set_video();
	 // box2d
	world = new b2World(
		b2Vec2(0.0, gravity),  // grav
		true  // allow sleep
	);
	world->SetContactListener(new myCL);
	 // Initial room
	//room::testroom.start();
	cursor2.x = 10;
	cursor2.y = 7.5;
}

void toggle_pause () {
	if (paused) {
		paused = false;
		trap_cursor = true;
		window->ShowMouseCursor(false);
		window->SetCursorPosition(window->GetWidth()/2, window->GetHeight()/2);
		n_buttons = 0;
	}
	else {
		paused = true;
		trap_cursor = false;
		window->SetCursorPosition(cursor2.x*UNPX*window_scale, window->GetHeight() - cursor2.y*UNPX*window_scale);
		n_buttons = n_pause_buttons;
		buttons = pause_buttons;
	}
};



void add_phase () {
	 // We must repeat if an object creates a new object in on_create()
	while (Actor* tq = activation_queue) {
		activation_queue = NULL;
		for (Actor* next; tq; tq = next) {
			Actor* c = tq;
			next = c->next_active;
			Actor** a;
			for (a = &active_actors; *a; a = &(*a)->next_active) {
				if (obj::type[c->type].depth > obj::type[(*a)->type].depth) {
					c->next_active = *a;
					*a = c;
					goto done_activating;
				}
			}  // Least deep object
			c->next_active = NULL;
			*a = c;
			done_activating: { }
		}
	}
}

void remove_phase () {
	for (Actor** a = &active_actors; *a;) {
		if (!(*a)->active) {
			Actor* old = *a;
			*a = old->next_active;
		}
		else { a = &(*a)->next_active; }
	}
}


void draw_phase () {
	draw_latency -= 1/FPS;
	if (draw_latency > 1/FPS) {
		dbg(6, "Skipping frame %d.\n", frame_number);
		return;
	}
	if (rata) {
		float focusx = rata->aim_center().x + cursor.x/2.0;
		float focusy = rata->aim_center().y + cursor.y/2.0;
		if (rata->loc > -1) {
			Room* room = (Room*)obj::global[rata->loc];
			room::Desc* r = &room::desc[room->data];
			uint32 walls = r->walls;
			if (walls&LEFT && focusx < room->pos.x + 10)
				focusx = room->pos.x + 10;
			else if (walls&RIGHT && focusx > room->pos.x + r->width - 10)
				focusx = room->pos.x + r->width - 10;
			if (walls&BOTTOM && focusy < room->pos.y + 7.5)
				focusy = room->pos.y + 7.5;
			else if (walls&TOP && focusy > room->pos.y + r->height - 7.5)
				focusy = room->pos.y + r->height - 7.5;
		}
		 // To look smooth in a pixelated environment,
		 //  we need a minimum speed.
		if (camera_jump) {
			camera.x = focusx;
			camera.y = focusy;
			camera_jump = false;
		}
		else {
			if (abs_f(focusx - camera.x) < .25*PX) camera.x = focusx;
			else {
				float newx = (9*camera.x + focusx) / 10;
				if (abs_f(newx - camera.x) < .25*PX) camera.x += .25*PX * sign_f(newx - camera.x);
				else camera.x = newx;
			}
			if (abs_f(focusy - camera.y) < .25*PX) camera.y = focusy;
			else {
				float newy = (9*camera.y + focusy) / 10;
				if (abs_f(newy - camera.y) < .25*PX) camera.y += .25*PX * sign_f(newy - camera.y);
				else camera.y = newy;
			}
		}
		window_view.SetCenter(camera.x+10*(window_scale-1), camera.y+7.5*(window_scale-1));

	}
	window->SetView(window_view);
	 // Draw background
	window->Clear(Color(0x7f7f7fff));
	//else if (rc) {
	//	float w = img::def[rc->bg].sfi.GetWidth()*PX;
	//	float h = img::def[rc->bg].sfi.GetHeight()*PX;
	//	float bg_x = mod_f(-camera.x/2, w);
	//	float bg_y = mod_f(-camera.y/2, h);
	//	for (float x = bg_x + viewleft(); x < viewright(); x += w)
	//	for (float y = bg_y + h + viewtop(); y > viewbottom(); y -= h) {
	//		draw_image(rc->bg, Vec(x, y));
	//	}
	//}
	 // Draw actors
	for (Actor* a = active_actors; a; a = a->next_active) {
		dbg(8, "Drawing 0x%08x\n", a);

		a->draw();
	}

	 // DEBUG DRAWING
	if (debug_mode)
	for (Actor* a=active_actors; a; a = a->next_active) {
		if (a->has_body()) {
			Object* o = (Object*) a;
			for (b2Fixture* f = o->body->GetFixtureList(); f; f = f->GetNext())
			if (f->GetFilterData().categoryBits)
			switch (f->GetType()) {
				case (b2Shape::e_edge): {
					b2EdgeShape* e = (b2EdgeShape*)f->GetShape();
					if (e->m_vertex1.x > viewleft() - 1
					 && e->m_vertex1.x < viewright() + 1
					 && e->m_vertex1.y > viewbottom() - 1
					 && e->m_vertex1.y < viewtop() + 1)
						window->Draw(sf::Shape::Line(
							e->m_vertex1.x, e->m_vertex1.y,
							e->m_vertex2.x, e->m_vertex2.y,
							1.0*PX, Color(0x00ff007f)
						));
					if (rata->pos.x + cursor.x > e->m_vertex1.x - 1.0)
					if (rata->pos.x + cursor.x < e->m_vertex1.x + 1.0)
					if (rata->pos.y + cursor.y+1 > e->m_vertex1.y - 1.0)
					if (rata->pos.y + cursor.y+1 < e->m_vertex1.y + 1.0) {
						window->Draw(sf::Shape::Line(
							e->m_vertex1.x, e->m_vertex1.y,
							e->m_vertex0.x+3*PX, e->m_vertex0.y+3*PX,
							1.0*PX, Color(0x00ff007f)
						));
						//printf("(% 6.2f, % 6.2f) (% 6.2f, % 6.2f) (% 6.2f, % 6.2f) (% 6.2f, % 6.2f)\n",
						//	e->m_vertex0.x, e->m_vertex0.y, e->m_vertex1.x, e->m_vertex1.y, 
						//	e->m_vertex2.x, e->m_vertex2.y, e->m_vertex3.x, e->m_vertex3.y);
					}
					if (rata->pos.x + cursor.x > e->m_vertex2.x - 1)
					if (rata->pos.x + cursor.x < e->m_vertex2.x + 1)
					if (rata->pos.y + cursor.y+1 > e->m_vertex2.y - 1)
					if (rata->pos.y + cursor.y+1 < e->m_vertex2.y + 1)
						window->Draw(sf::Shape::Line(
							e->m_vertex3.x-3*PX, e->m_vertex3.y-3*PX,
							e->m_vertex2.x, e->m_vertex2.y,
							1.0*PX, Color(0x0000ff7f)
						));
					break;
				}
				case (b2Shape::e_polygon): {
					b2PolygonShape* p = (b2PolygonShape*)f->GetShape();
					Color color = f->GetFilterData().categoryBits == 256 ? 0x0000ff4f : 0x00ff007f;
					sf::Shape draw_shape;
					draw_shape.EnableFill(false);
					draw_shape.EnableOutline(true);
					draw_shape.SetOutlineWidth(1.0*PX);
					for (int i=0; i < p->m_vertexCount; i++) {
						draw_shape.AddPoint(p->m_vertices[i].x, p->m_vertices[i].y, Color(0), color);
					}
					draw_shape.SetPosition(o->pos);
					window->Draw(draw_shape);
					break;
				}
				case (b2Shape::e_circle): {
					b2CircleShape* c = (b2CircleShape*)f->GetShape();
					Color color = f->GetFilterData().categoryBits == 256 ? 0x0000ff4f : 0x00ff007f;
					sf::Shape draw_shape = sf::Shape::Circle(Vec(c->m_p)+o->pos, c->m_radius, Color(0), 1.0*PX, color);
					draw_shape.EnableFill(false);
					draw_shape.EnableOutline(true);
					window->Draw(draw_shape);
					break;
				}
				default: { }
			}
		}
		else {  // Debug draw an object without a b2Body
			window->Draw(sf::Shape::Line(
				a->pos, a->pos + a->vel,
				1.0*PX, Color(0xff00007f)
			));
		};
		 // Debug draw rata path.
		if (mag2(rata->pos - oldratapos) > 0.2) {
			debug_path[debug_path_pos % debug_path_size] = rata->pos;
			uint8 whiteshift = rata->float_frames * 255.0 / (rata->stats.float_time*FPS);
			debug_path_color[debug_path_pos % debug_path_size] =
				whiteshift ? Color(255, whiteshift, whiteshift, 127) : 0x0000ff7f;
			oldratapos = rata->pos;
			debug_path_pos++;
		}

		uint i = debug_path_pos>=debug_path_size ? debug_path_pos-debug_path_size+2 : 1;
		for (; i < debug_path_pos; i++) {
			window->Draw(sf::Shape::Line(
				debug_path[(i-1) % debug_path_size], debug_path[i % debug_path_size],
				1.0*PX, debug_path_color[i % debug_path_size]
			));
		}
	}
	else { debug_path_pos = 0; }
	 // Draw cursor
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
	window->ShowMouseCursor(!trap_cursor);
	 // Draw buttons
	if (n_buttons) {
		uint i = n_buttons;
		while (i) {
			buttons[--i].draw();
		}
	}
	 // Draw text message
	if (message) {
		render_text(message_pos, Vec(10, 1), 1, false, true, 0, true);
	}
	if (window_scale > 1.0) {
		//glWindowPos2i(ceil(viewleft()*UNPX)*PX, ceil(viewbottom()*UNPX)*PX);
		glPixelZoom(window_scale, window_scale);
		glCopyPixels(0, 0, 320, 240, GL_COLOR);
		glPixelZoom(1.0, 1.0);
	}
	draw_latency += frameclock.GetElapsedTime();
	frameclock.Reset();
	if (draw_latency < 0) sf::Sleep(-draw_latency);
	dbg(6, "%f\r", draw_latency);
	if (DEBUG >= 6) fflush(stdout);
	window->Display();
}

void input_phase () {
	click_taken = false;
	 // Count frames for each key and button
	for (uint sym = 0; sym < 400; sym++)
	if (key[sym] > 0 and key[sym] < 255) key[sym]++;
	for (uint btn = 0; btn < 10; btn++)
	if (button[btn] > 0 and btn[button] < 255) button[btn]++;
	 // Start event handling
	sf::Event event;
	while (window->GetEvent(event))
	switch (event.Type) {
		case sf::Event::KeyPressed: {
			if (event.Key.Code == sf::Key::Escape) {
				quit_game();
			}
			if (event.Key.Code == sf::Key::Equal) debug_mode = !debug_mode;
			if (event.Key.Code == sf::Key::F11) {
				window_fullscreen = !window_fullscreen;
				set_video();
			}
			//if (event.Key.Code == sf::Key::Num1) enter_room(room::test1, 0);
			//if (event.Key.Code == sf::Key::Num2) enter_room(room::test2, 0);
			//if (event.Key.Code == sf::Key::Num3) enter_room(room::test3, 0);
			//if (event.Key.Code == sf::Key::Num4) enter_room(room::test4, 0);
			//if (event.Key.Code == sf::Key::Num5) enter_room(room::edit1, 0);
			//if (event.Key.Code == sf::Key::Num6) enter_room(room::empty, 0);
			//if (event.Key.Code == sf::Key::Num0) save_save();
			if (event.Key.Code == sf::Key::P) toggle_pause();
			if (event.Key.Code >= 400) break;
			key[event.Key.Code] = 1;
			break;
		}
		case sf::Event::KeyReleased: {
			if (event.Key.Code >= 400) break;
			key[event.Key.Code] = 0;
			break;
		}
		case sf::Event::MouseButtonPressed: {
			if (event.MouseButton.Button >= 10) break;
			button[event.MouseButton.Button] = 1;
			break;
		}
		case sf::Event::MouseButtonReleased: {
			if (event.MouseButton.Button >= 10) break;
			button[event.MouseButton.Button] = 0;
			break;
		}
		case sf::Event::MouseMoved: {
			if (trap_cursor) {
				cursor.x += (event.MouseMove.X - window->GetWidth()/2.0)
					* PX * cursor_scale / window_scale;
				cursor.y += -(event.MouseMove.Y - window->GetHeight()/2.0)
					* PX * cursor_scale / window_scale;
				window->SetCursorPosition(window->GetWidth()/2, window->GetHeight()/2);
				if (cursor.x > 19) cursor.x = 19;
				else if (cursor.x < -19) cursor.x = -19;
				if (cursor.y > 14) cursor.y = 14;
				else if (cursor.y < -14) cursor.y = -14;
			}
			else {
				cursor2.x = event.MouseMove.X*PX/window_scale;
				cursor2.y = (window->GetHeight() - event.MouseMove.Y)*PX/window_scale;
			}
			break;
		}
		case sf::Event::Closed: {
			quit_game();
			break;
		}
	}
};

void move_phase () {
	if (n_buttons) {
		for (uint i=0; i < n_buttons; i++)
		if (cursor2.x > buttons[i].pos.x)
		if (cursor2.y > buttons[i].pos.y)
		if (cursor2.x < buttons[i].pos.x + buttons[i].size.x)
		if (cursor2.y < buttons[i].pos.y + buttons[i].size.y) {
			if (buttons[i].click) (*buttons[i].click)();
			break;
		}
	}
	if (paused) return;
	for (Actor* a = active_actors; a; a = a->next_active)
		a->before_move();

	for (uint i=0; i < MAX_BULLETS; i++)
		bullets[i].move();
	if (world) {
		world->SetAutoClearForces(false);
		world->Step(1/120.0, 10, 10);
		world->SetAutoClearForces(true);
		world->Step(1/120.0, 10, 10);
		//world->Step(1/180.0, 10, 10);
		//world->Step(1/240.0, 10, 10);
	}

	for (Actor* a = active_actors; a; a = a->next_active)
		a->after_move();
}


void main_loop () {
	window->SetCursorPosition(window->GetWidth()/2, window->GetHeight()/2);
	for (;;) {
		frame_number++;
		add_phase();
		draw_phase();
		input_phase();
		move_phase();
		remove_phase();
	}
}


#endif

