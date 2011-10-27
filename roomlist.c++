

enum ID {
	roompicker,
	test1,
	test2,
	test3,
	test4,
	edit1,
	empty,
	lotsodanger,
	n_rooms
};

namespace file {
	namespace roompicker { extern Room room; }
	namespace test1 { extern Room room; }
	namespace test2 { extern Room room; }
	namespace test3 { extern Room room; }
	namespace test4 { extern Room room; }
	namespace edit1 { extern Room room; }
	namespace empty { extern Room room; }
	namespace lotsodanger { extern Room room; }
	namespace roompicker {
		#include "rooms/0-roompicker.room.c++"
	}
	namespace test1 {
		#include "rooms/1-test1.room.c++"
	}
	namespace test2 {
		#include "rooms/2-test2.room.c++"
	}
	namespace test3 {
		#include "rooms/3-test3.room.c++"
	}
	namespace test4 {
		#include "rooms/4-test4.room.c++"
	}
	namespace edit1 {
		#include "rooms/5-edit1.room.c++"
	}
	namespace empty {
		#include "rooms/6-empty.room.c++"
	}
	namespace lotsodanger {
		#include "rooms/7-lotsodanger.room.c++"
	}
}

Room* list [] = {
	&file::roompicker::room,
	&file::test1::room,
	&file::test2::room,
	&file::test3::room,
	&file::test4::room,
	&file::edit1::room,
	&file::empty::room,
	&file::lotsodanger::room
};

#ifdef MAPEDITOR
char* filename [] = {
	"rooms/0-roompicker.room.c++",
	"rooms/1-test1.room.c++",
	"rooms/2-test2.room.c++",
	"rooms/3-test3.room.c++",
	"rooms/4-test4.room.c++",
	"rooms/5-edit1.room.c++",
	"rooms/6-empty.room.c++",
	"rooms/7-lotsodanger.room.c++"
};
#endif

