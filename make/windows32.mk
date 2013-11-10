RES=windres
CFLAGS+= -m32 -D_WIN32 -D_WINDOWS
LDFLAGS+= -static
RC=make/$(TARGET)/RC.o
