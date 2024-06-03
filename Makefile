CC = g++
CFLAGS = -Wall -Wextra
SRCPATH = src
OBJPATH = obj
EXEPATH = build
ifeq ($(OS), Windows_NT)
	LIBS = -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lm -lstdc++fs -std=c++17
else
	LIBS = -lX11 -lGL -lpthread -lpng -lstdc++fs -lm -std=c++17
endif

all: olc main objects animation map $(EXEPATH)
	$(CC) -g -o build/app.exe obj/main.o obj/olcPixelGameEngine.o obj/objects.o obj/animation.o obj/map.o $(LIBS)


olc: $(OBJPATH)
	$(CC) -c -g -o obj/olcPixelGameEngine.o src/olc/olcPixelGameEngine.cpp

objects: $(OBJPATH)
	$(CC) -c -g -o obj/objects.o $(CFLAGS) src/objects.cpp

animation: $(OBJPATH)
	$(CC) -c -g -o obj/animation.o $(CFLAGS) src/animation.cpp

main: $(OBJPATH)
	$(CC) -c -g -o obj/main.o $(CFLAGS) src/main.cpp

map: $(OBJPATH)
	$(CC) -c -g -o obj/map.o $(CFLAGS) src/map.cpp

$(OBJPATH):
	mkdir -p obj

$(EXEPATH):
	mkdir -p build

clean:
	rm -rf build
	rm -rf obj