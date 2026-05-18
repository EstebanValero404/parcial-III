CXX      = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall
SRCS     = src/main.cpp src/mapa.cpp src/juego.cpp src/utilidades.cpp
TARGET   = dungeon

ifeq ($(OS), Windows_NT)
    RM       = del /f
    EXE      = $(TARGET).exe
    CXXFLAGS += -ISDL2/include/SDL2
    LDFLAGS   = -lmingw32 -LSDL2/lib -lSDL2main -lSDL2
else
    UNAME := $(shell uname -s)
    ifeq ($(UNAME), Darwin)
        RM      = rm -f
        EXE     = $(TARGET)
        CXXFLAGS += $(shell sdl2-config --cflags)
        LDFLAGS  = $(shell sdl2-config --libs)
    else
        RM      = rm -f
        EXE     = $(TARGET)
        CXXFLAGS += $(shell sdl2-config --cflags)
        LDFLAGS  = $(shell sdl2-config --libs)
    endif
endif

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)

clean:
	$(RM) $(EXE)
