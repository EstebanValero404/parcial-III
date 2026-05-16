CXX      = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall
SRCS     = src/main.cpp src/mapa.cpp src/juego.cpp src/utilidades.cpp
TARGET   = dungeon

ifeq ($(OS), Windows_NT)
    RM = del /f
    EXE = $(TARGET).exe
else
    RM = rm -f
    EXE = $(TARGET)
endif

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	$(RM) $(EXE)
