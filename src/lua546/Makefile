OBJS0=lapi.o lcode.o lctype.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o lmem.o lobject.o lopcodes.o lparser.o lstate.o lstring.o ltable.o ltm.o lundump.o lvm.o lzio.o
OBJS= $(OBJS0) lua.o lauxlib.o lbaselib.o lcorolib.o ldblib.o liolib.o lmathlib.o loslib.o lstrlib.o ltablib.o lutf8lib.o loadlib.o linit.o
OBJS2= $(OBJS0) luac.o lauxlib.o

CFLAGS= -Wall -Wextra -O2

T= lua

all:	$T luac
	./$T test.lua
	./luac -l test.lua
	ar rvs liblua.a $(OBJS) $(OBJS2)

$T:	$(OBJS)
	$(CC) -o $@ $(OBJS) -lm

luac:	$(OBJS2)
	$(CC) -o $@ $(OBJS2) -lm


clean:
	rm -f $T $(OBJS) $(OBJS2) core core.* luac.out luac liblua.a

diff:
	diff ORIG . | grep -v ^Only > DIFFS
