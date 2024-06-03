#define NOBUILD_IMPLEMENTATION
#include "src/nobuild.h"

#define CC "g++"
#define SOURCE "src/main.cpp"
#define OLC "src/olc/olcPixelGameEngine.cpp"
#define OUTPUT "-o", "build/app.exe"
#define FLAGS "-Wall", "-Wextra", "-g"

#ifdef _WIN32
    #define LIBS "-luser32", "-lgdi32", "-lopengl32", "-lgdiplus", "-lShlwapi", "-ldwmapi", "-lm", "-lstdc++fs", "-std=c++17"
#else
    #define LIBS "-lX11", "-lGL", "-lpthread", "-lpng", "-lstdc++fs", "-lm", "-std=c++17"
#endif

int main(int argc, char* argv[]) {
    GO_REBUILD_URSELF(argc, argv);
    MKDIRS("build");
    MKDIRS("obj");
    CMD(CC, "-c", "-g", "-o", "obj/olcPixelGameEngine.o", OLC);
    CMD(CC, "-c", FLAGS, "-o", "obj/objects.o", "src/objects.cpp");
    CMD(CC, "-c", FLAGS, "-o", "obj/animation.o", "src/animation.cpp");
    CMD(CC, "-c", FLAGS, "-o", "obj/main.o", SOURCE);

    CMD(CC, "-g", OUTPUT, "obj/main.o", "obj/olcPixelGameEngine.o", "obj/objects.o", "obj/animation.o", LIBS);
    return 0;
}