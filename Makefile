#OBJS specifies which files to compile as part of the project
OBJS = main.cpp Game.cpp GameObject.cpp TextureManager.cpp Map.cpp Vector2D.cpp Collision.cpp ECS/ECS.cpp

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IE:\Development\SDL2\include\SDL2 -IE:\Development\SDL2_image\include\SDL2 -IE:\Development\SDL2_ttf\include\SDL2 -IE:\Development\SDL2_mixer\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LE:\Development\SDL2\lib -LE:\Development\SDL2_image\lib -LE:\Development\SDL2_ttf\lib -LE:\Development\SDL2_mixer\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = main

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	main.exe