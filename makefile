CXX      = g++
SRCS = calabozo_oscuro/src/main.cpp calabozo_oscuro/src/mapa.cpp calabozo_oscuro/src/juego.cpp calabozo_oscuro/src/utilidades.cpp
CXXFLAGS = -std=c++17 -Icalabozo_oscuro/include -Wall
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
