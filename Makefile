# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -lSDL2 -lSDL2_image

# Source files
SOURCES = TextureManager.cpp Game.cpp  project.cpp

# Output executable
TARGET = project

# Build target
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(SOURCES) -o $(TARGET) $(CXXFLAGS)

# Clean target
clean:
	rm -f $(TARGET)