TARGET=$(MAKECMDGOALS)

CC=g++
CFLAGS=-c -std=c++11
LFLAGS=-std=c++11

SOURCES=source/Main.cpp
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
	$(CC) $(LDFLAGS) $(OBJECTS) $(RC) $(FLIBS) -o $(OUTPUT)

clean:
	@echo Cleaning...
	$(RM) $(OUTPUT)
	$(RM) $(OBJECTS) $(RC)

windows32: compile resources link

windows64: compile resources link

linux32: compile resources link

linux64: compile resources link
