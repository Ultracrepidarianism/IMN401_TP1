CC = gcc
CXX = g++ -std=c++11

BUILD_DIR = ./build
COMPILE_FLAGS = -Wall -ggdb -O3
LINK_FLAGS = -lglfw -lGL -ldl

glfw = /usr/include/GLFW
glad = ./third_party/glad/include
glm = ./third_party/glm

INCLUDES = -I$(glad) -I$(glfw) -I$(glm) -I./include
LIBRARIES = -lGL -lglfw

all: main.o
	$(CXX) $(LIBRARIES) -o ./main ./build/glad.o ./build/main.o $(LINK_FLAGS)

main.o: glad.o
	mkdir -p $(BUILD_DIR)
	$(CXX) $(COMPILE_FLAGS) $(INCLUDES) $(LIBRARIES) -c -o ./build/main.o ./src/main.cpp

glad.o:
	mkdir -p $(BUILD_DIR)
	$(CC) $(COMPILE_FLAGS) $(INCLUDES) -c -o ./build/glad.o ./third_party/glad/src/glad.c
