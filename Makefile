# Compiler
CXX = g++

# Assembler
ASM = nasm

# Compiler flags
CXXFLAGS = -lSDL2 -lSDL2_image

# Source files
SOURCES = TextureManager.cpp vector2D.cpp ECS/KeyboardController.cpp Game.cpp project.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o) calculator.o

# Output executable
TARGET = project

# Build target
all: $(TARGET)

# Rule to create the target executable
$(TARGET): calculator.o $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(CXXFLAGS)

# Rule to assemble the calculator.asm file
calculator.o: calculator.asm
	$(ASM) -f elf64 calculator.asm -o calculator.o

# Rule to compile each source file into an object file
%.o: %.cpp
	$(CXX) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJECTS) $(TARGET)  # Remove all object files and the executable