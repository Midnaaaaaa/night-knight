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
	TileMap(const string &levelInfoStr, const glm::vec2 &minCoords, ShaderProgram &program);

public:

	int LEFT_WALL, RIGHT_WALL;

	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelInfoStr, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }
	glm::ivec2 getMapSize() const { return mapSize; }

	int collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, bool bJumping = false) const;
	int collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, bool bJumping = false) const;
	int collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY = nullptr) const;
	int collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, bool ignorePlatform = false) const;

	int collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &colliderOffset, const glm::ivec2 &colliderSize, bool ignorePlatform = false) const;
	int collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &colliderOffset, const glm::ivec2 &colliderSize, bool ignorePlatform = false) const;
	int collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &colliderOffset, const glm::ivec2 &colliderSize, int *posY = nullptr) const;
	int collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &colliderOffset, const glm::ivec2 &colliderSize, bool ignorePlatform = false) const;

	int modifyTileMap(int i, int j, int newTile);

	bool tevacae(const glm::ivec2& pos, const glm::ivec2& size, bool rightSight) const;

	void reduceNumberOfPlatforms();
	int getNumOfTilesRemaining();
	glm::ivec2 getRandomPlatform();

	bool dentroDePlataforma(const glm::ivec2& pos, const glm::ivec2& colliderOffset, const glm::ivec2& colliderSize);
	
private:
	bool loadLevel(const string &levelInfoStr);
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

	vector<glm::ivec2> platformPositions;

	static const vector<int> tileType;

	int numPlatforms;


};


#endif // _TILE_MAP_INCLUDE


