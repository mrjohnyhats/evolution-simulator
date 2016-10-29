OBJS = evolution.cpp globals.cpp Entity.cpp Disease.cpp Pop_counter.cpp

OBJ_NAME = evolution

w : $(OBJS)
	g++ $(OBJS) -std=gnu++11 -D __WIN32__ -g -lmingw32 -lSDL2_ttf -lSDL2main -lSDL2 -o $(OBJ_NAME)
linux : $(OBJS)
	g++ $(OBJS) -std=gnu++11 -ggdb -w -lSDL2 -lDSL2_ttf -o $(OBJ_NAME)
osx : $(OBJS)
	clang++ $(OBJS) -std=gnu++11 -ggdb -I/Library/Frameworks/SDL2.framework/Headers -framework SDL2 -framework SDL2_ttf -o $(OBJ_NAME)