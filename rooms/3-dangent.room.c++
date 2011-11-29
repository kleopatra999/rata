
ROOM_BEGIN

ROOM_POS(40, -10)
ROOM_WIDTH(30)
ROOM_HEIGHT(20)
ROOM_TILES(
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  0,  0, -4, -2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  0, -4, -3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1, -4, -3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1, -3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  0,  0,  0,  2,  1,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  5,  0,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  6,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  6,  0,  6,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  0,  0,  0,  0,  5,  0,  0,  0,  0,  6,  0,  6,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
  1,  0,  0,  0,  0,  6,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  6,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  0,  0,  0,  0,  0,  5,  0,  0,  0,  5,  0,  0,  0,  5,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  0,  0,  0,  0,  0,  6,  0,  0,  0,  6,  0,  0,  0,  6,  0,  0,  0,  0,  0,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  0,  0,  0,  0,  0,  6,  0,  0,  0,  6,  0,  0,  0,  6,  0,  0,  0,  0,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1
)

ROOM_N_NEIGHBORS(1)
ROOM_NEIGHBORS(
	room::test1
)
ROOM_N_WALLS(6)
ROOM_WALLS(
	{Vec(50, 2.5), Vec(30, 7.5), 0, false},
	{Vec(30, 7.5), Vec(50, 2.5), 0, true},
	{Vec(50, 2.5), Vec(60, 2.5), 0, false},
	{Vec(60, 2.5), Vec(60, -2.5), 0, false},
	{Vec(60, -2.5), Vec(50, -2.5), 0, false},
	{Vec(50, -2.5), Vec(50, 2.5), 0, true},
)

ROOM_INIT(
	new Tilemap(HERE);
	new BGColor(HERE, pos, Vec(width, height), 0x7f7f7fff);
	new Flyer(HERE, pos+Vec(16.5, 4));
)

ROOM_END

