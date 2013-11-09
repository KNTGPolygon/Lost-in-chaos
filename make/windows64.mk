RES=windres
CFLAGS+= -m64 -D_WIN32 -D_WIN64 -D_WINDOWS
LDFLAGS+= -static
RC=make/$(TARGET)/RC.o
