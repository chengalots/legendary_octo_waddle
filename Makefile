CXX=g++
CXXFLAGS= -std=c++14 -Wall -Wextra -pedantic
LDLIBS=-lsdl2 -lsdl2_image -lsdl2_ttf

all: gameExecutable

mVector.o: mVector.hpp mVector.cpp
    $(CXX) $(CXXFLAGS) -c mVector.cpp -o mVector.o

Timer.o: Timer.hpp Timer.cpp
    $(CXX) $(CXXFLAGS) -c Timer.cpp -o Timer.o

PhysicsBody.o: PhysicsBody.hpp PhysicsBody.cpp Timer.hpp mVector.hpp StatusEffect.hpp
    $(CXX) $(CXXFLAGS) -c PhysicsBody.cpp -o PhysicsBody.o

Tile.o: Tile.hpp Tile.cpp
    $(CXX) $(CXXFLAGS) -c Tile.cpp -o Tile.o

Chunk.o: Chunk.hpp Chunk.cpp Tile.hpp Character.hpp
    $(CXX) $(CXXFLAGS) -c Chunk.cpp -o Chunk.o

Texture.o: Texture.hpp Texture.cpp
    $(CXX) $(CXXFLAGS) -c Texture.cpp -o Texture.o

Animation.o: Animation.hpp Animation.cpp Texture.hpp
    $(CXX) $(CXXFLAGS) -c Animation.cpp -o Animation.o

Loader.o: Loader.hpp Loader.cpp Animation.hpp Attack.hpp StatusEffect.hpp
    $(CXX) $(CXXFLAGS) -c Loader.cpp -o Loader.o

Character.o: Character.hpp Character.cpp Animation.hpp Attack.hpp Loader.hpp PhysicsBody.hpp
    $(CXX) $(CXXFLAGS) -c Character.cpp -o Character.o

Enemy.o: Enemy.hpp Enemy.cpp PhysicsBody.hpp Character.hpp
    $(CXX) $(CXXFLAGS) -c Enemy.cpp -o Enemy.o

Player.o: Player.hpp Player.cpp Character.hpp
    $(CXX) $(CXXFLAGS) -c Player.cpp -o Player.o

Game.o: Game.hpp Game.cpp Player.hpp Enemy.hpp Chunk.hpp
    $(CXX) $(CXXFLAGS) -c Game.cpp -o Game.o

main.o: main.cpp Game.hpp
    $(CXX) $(CXXFLAGS) -c main.cpp -o main.o

gameExecutable: mVector.o Timer.o PhysicsBody.o Tile.o Chunk.o Texture.o Animation.o Loader.o Enemy.o Character.o Player.o Game.o main.o
    $(CXX) $(LDLIBS) mVector.o Timer.o PhysicsBody.o Tile.o Chunk.o Texture.o Animation.o Loader.o Enemy.o Character.o Player.o Game.o main.o -o gameExecutable

clean:
    rm -f *.o
    rm -f gameExecutable
