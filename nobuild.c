#define NOBUILD_IMPLEMENTATION
#include "src/nobuild.h"

#define CC "g++"
#define SOURCE "src/main.cpp"
#define OLC "src/olc/olcPixelGameEngine.cpp"
#define OUTPUT "-o", "build/app.exe"
#define FLAGS "-Wall", "-Wextra", "-g"
#define LIB_DIRS "-L", "src/lua546"
#define INCLUDE_DIRS "-I", "src/sol2", "-I", "src/lua546"

#ifdef _WIN32
    #define LIBS "-luser32", "-lgdi32", "-lopengl32", "-lgdiplus", "-lShlwapi", "-ldwmapi", "-lm", "-llua", "-lstdc++fs", "-std=c++17"
#else
    #define LIBS "-lX11", "-lGL", "-lpthread", "-lpng", "-lstdc++fs", "-llua", "-lm", "-std=c++17"
#endif

// LUA section
#define LUA_CFLAGS "-Wall", "-Wextra", "-O2"
#define LUA_PATH PATH("src", "lua546", "")

static const char* luaFiles[] = {
    "lapi", "lcode", "lctype", "ldebug", "ldo", "ldump", "lfunc", "lgc", "llex", "lmem", "lobject",
    "lopcodes", "lparser", "lstate", "lstring", "ltable", "ltm", "lundump", "lvm", "lzio",
    "lauxlib", "lbaselib", "lcorolib", "ldblib", "liolib", "lmathlib", "loslib", "lstrlib", "ltablib",
    "lutf8lib", "loadlib", "linit", 0
};

int compileLua() {
    const char** ptr = luaFiles;
    while (*ptr != 0) {
        Cstr libfile = CONCAT(LUA_PATH, *ptr, ".o");
        CMD("gcc", LUA_CFLAGS, "-c", "-o", libfile, CONCAT(LUA_PATH, *ptr, ".c"));
        ++ ptr;
    }
    return 0;
}

int cleanLua() {
    const char** ptr = luaFiles;
    while (*ptr != 0) {
        Cstr fileToDel = CONCAT(LUA_PATH, *ptr, ".o");
        INFO("Removing file: %s\n", fileToDel);
        path_rm(fileToDel);
        ptr++;
    }
    return 0;
}

int makeLua() {
    if (PATH_EXISTS(PATH(LUA_PATH, "linit.o")) != 1) {
        INFO("Compiling Lua");
        return compileLua();
    } else {
        INFO("Lua already compiled, skipping");
        return 0;
    }
}

// End of LUA section

// OLC:PixelGameEngine section

int makeOlcPixelGameEngine() {
    if (PATH_EXISTS(PATH("obj", "olcPixelGameEngine.o")) != 1) {
        INFO("Compiling olc:PixelGameEngine");
        CMD(CC, "-c", "-g", "-o", "obj/olcPixelGameEngine.o", OLC);
        return 0;
    } else {
        INFO("olc:PixelGameEngine already compiled, skipping");
        return 0;
    }
}

// end of OLC:PixelGameEngine section

int main(int argc, char* argv[]) {
GO_REBUILD_URSELF(argc, argv);
    
    MKDIRS("build");
    MKDIRS("obj");
    makeOlcPixelGameEngine();
    makeLua();
    CMD(CC, "-c", FLAGS, "-o", "obj/objects.o", "src/objects.cpp");
    CMD(CC, "-c", FLAGS, "-o", "obj/animation.o", "src/animation.cpp");
    CMD(CC, "-c", INCLUDE_DIRS, FLAGS, "-o", "obj/map.o", "src/map.cpp");
    CMD(CC, "-c", INCLUDE_DIRS, FLAGS, "-o", "obj/main.o", SOURCE);


    const char** ptr = luaFiles;
    Cstr allLibs = "";
    while (*ptr != 0) {
        Cstr libfile = CONCAT(LUA_PATH, *ptr, ".o");
        allLibs = CONCAT(allLibs, " ", libfile);
        ptr++;
    }

    CMD(CC, "-g", OUTPUT, LIB_DIRS, "obj/main.o", "obj/olcPixelGameEngine.o", "obj/objects.o", "obj/animation.o", "obj/map.o", allLibs, LIBS);
    return 0;
}