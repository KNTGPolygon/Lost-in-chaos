TARGET=$(MAKECMDGOALS)

CC=g++
CFLAGS=-c -std=c++11 -Iinclude -DGLEW_STATIC
LFLAGS=-std=c++11 -Llib -static
FLIBS=-lglfw3-$(TARGET) -lglew-$(TARGET)s -lopengl32 -lgdi32 -lDevIL -lILU -lILUT

RM=rm -f

SOURCES=source/Main.cpp source/Window.cpp source/Game.cpp source/Props.cpp source/EntPlayer.cpp source/Math.cpp source/Random.cpp source/Resources.cpp
OBJDIR=make/$(TARGET)/

OBJECTS=$(SOURCES:source/%.cpp=$(OBJDIR)%.o)
RC=

OUTPUT=bin/lost-in-chaos-$(TARGET).exe

include make/$(TARGET).mk

$(OBJDIR)%.o: source/%.cpp
	@echo $<...
	$(CC) $(CFLAGS) $< -o $@

.PHONY: resources printc compile link clean windows32 windows64 linux32 linux64

resources:
	@echo Resources...
	$(RES) source/Resources.rc $(RC)

printc:
	@echo Compiling...

compile: printc $(OBJECTS)

link:
	@echo Linking...
	$(CC) $(LFLAGS) $(OBJECTS) $(RC) $(FLIBS) -o $(OUTPUT)

clean:
	@echo Cleaning...
	$(RM) make/windows32/*.o
	$(RM) make/windows64/*.o
	$(RM) make/linux32/*.o
	$(RM) make/linux64/*.o
	$(RM) $(OBJECTS) $(RC)

windows32: compile resources link

windows64: compile resources link

linux32: compile resources link

linux64: compile resources link
