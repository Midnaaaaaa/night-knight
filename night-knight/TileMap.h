#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <vector>

#define TILE_NOT_SOLID 0
#define TILE_SOLID 1
#define TILE_SPIKE 2
#define TILE_PLATFORM 3
#define TILE_PLATFORM_ACTIVATED 4

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:

	int LEFT_WALL, RIGHT_WALL;

	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, bool bJumping) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, bool bJumping) const;
	int collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, bool ignorePlatform) const;

	void modifyTileMap(int i, int j, int newTile);

	bool tevacae(const glm::ivec2& pos, const glm::ivec2& size, bool rightSight) const;

	void reduceNumberOfPlatforms();
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;

	glm::vec2 minCoords;

	glm::ivec2 position;
	//Map size equals number of tiles of the map 16 horizontal 11 vertical
	glm::ivec2 mapSize;

	//Numero de tiles en tileSet
	glm::ivec2 tilesheetSize;

	//Numero de pixeles del tile
	int tileSize, blockSize;
	Texture tilesheet;

	//
	glm::vec2 tileTexSize;
	int *map;
	

	//[i,j] -> nTile
	int *numTile;


	int numPlatforms;

	static const vector<int> tileType;

};


#endif // _TILE_MAP_INCLUDE


