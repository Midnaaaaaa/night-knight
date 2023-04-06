#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;

/*
const vector<int> TileMap::tileType = {	0,1,3,0,0,0,0,0,
										0,0,0,1,0,0,0,0,
										0,0,2,2,2,0,0,0,
										1,1,2,1,1,1,3,3,
										1,1,1,1,1,4,0,0,
										0,0,0,0,1,0,0,0,
										0,0,0,0,1,0,0,0,
										0,0,0,0,0,0,0,0};

const vector<int> TileMap::tileType = { 0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
										3,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
										4,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
										1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
										1,1,1,1,1,1,1,0,0,0,0,3,0,0,0,0,
										1,1,1,0,0,1,1,0,0,0,0,4,0,0,0,0,
										1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,
										0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,0};
										*/

const vector<int> TileMap::tileType = { 0,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,
										2,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,
										2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
										2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
										0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
										0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
										0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	// -----------------------------------------------------------------
										0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 
										0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
										0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
										0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,
										0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,
										0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
										0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

TileMap *TileMap::createTileMap(const string &levelInfoStr, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelInfoStr, minCoords, program);
	return map;
}


TileMap::TileMap(const string &levelInfoStr, const glm::vec2 &minCoords, ShaderProgram &program)
{
	numPlatforms = 0;
	mapSize = glm::ivec2(32, 22);
	tileSize = 16;
	blockSize = 16;
	loadLevel(levelInfoStr);
	this->minCoords = minCoords;
	prepareArrays(minCoords, program);
	LEFT_WALL = 2 * tileSize;
	RIGHT_WALL = (mapSize.x - 2) * tileSize;
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete[] map; // seguro que es delete[] ??
	if (numTile != nullptr)
		delete[] numTile;
	free();
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelInfoStr)
{
	stringstream fin(levelInfoStr);
	string line, tilesheetFile;
	stringstream sstream;
	short tile;
	
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);

	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.clear();
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	
	map = new int[mapSize.x * mapSize.y];
	numTile = new int[mapSize.x * mapSize.y];


	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			tile = 0;
			char c;
			fin.get(c);
			//Pasar char a int
			while (c != ',' && c != '\n')
			{
				tile = tile * 10 + c - int('0');
				fin.get(c);
			}
			if (tileType[tile] == TILE_PLATFORM){
				numPlatforms++;
			}
			if (tile == 1) platformPositions.push_back(glm::ivec2(i, j));
			map[j * mapSize.x + i] = tile;
		}
	//#ifndef _WIN32
	//	fin.get(tile);
	//#endif
	}	
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel, quarterTexel;
	vector<float> vertices;
	
	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	quarterTexel = glm::vec2(0.25f / tilesheet.width(), 0.25f / tilesheet.height());

	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				numTile[j * mapSize.x + i] = nTiles;
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile)%tilesheetSize.x) / tilesheetSize.x, float((tile)/tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= quarterTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

int TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, bool bJumping) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		int tile = map[y * mapSize.x + x];
		int type = tileType[tile];
		switch (type)
		{
		case TILE_NOT_SOLID:
			break;
		case TILE_PLATFORM:
		case TILE_PLATFORM_ACTIVATED:
			if (bJumping) break;
		case TILE_SOLID:
		case TILE_SPIKE:
			return type;
		}
	}
	
	return 0;
}

int TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, bool bJumping) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		int tile = map[y * mapSize.x + x];
		int type = tileType[tile];
		switch (type)
		{
		case TILE_NOT_SOLID:
			break;
		case TILE_PLATFORM:
		case TILE_PLATFORM_ACTIVATED:
			if (bJumping) break;
		case TILE_SOLID:
		case TILE_SPIKE:
			return type;
		}
	}
	
	return 0;
}

int TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	int offsetY = (posY != nullptr) ? pos.y - *posY : 0;
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		int tile = map[y * mapSize.x + x];
		int type = tileType[tile];
		switch (type)
		{
		case TILE_NOT_SOLID:
			break;
		case TILE_SOLID:
		case TILE_PLATFORM:
		case TILE_PLATFORM_ACTIVATED:
		case TILE_SPIKE:
			//if (posY == nullptr) {
			//	return type;
			//}
			if (pos.y - tileSize * y + size.y <= 4)
			{
				if (posY != nullptr)
					*posY = tileSize * y - size.y - offsetY;
				return type;
			}
		}
	}
	
	return 0;
}

int TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, bool ignorePlatform) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		int tile = map[y * mapSize.x + x];
		int type = tileType[tile];
		switch (type)
		{
		case TILE_NOT_SOLID:
			break;
		case TILE_PLATFORM:
		case TILE_PLATFORM_ACTIVATED:
			if (ignorePlatform)
				break;
		case TILE_SOLID:
		case TILE_SPIKE:
			return type;
		}
	}

	return 0;
}


// Collision tests for axis aligned bounding boxes with collider offsets

int TileMap::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& colliderOffset, const glm::ivec2& colliderSize, bool bJumping) const
{
	int x, y0, y1;

	x = (pos.x + colliderOffset.x) / tileSize;
	y0 = (pos.y + colliderOffset.y) / tileSize;
	y1 = (pos.y + colliderOffset.y + colliderSize.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		int tile = map[y * mapSize.x + x];
		int type = tileType[tile];
		switch (type)
		{
		case TILE_NOT_SOLID:
			break;
		case TILE_PLATFORM:
		case TILE_PLATFORM_ACTIVATED:
			if (bJumping) break;
		case TILE_SOLID:
		case TILE_SPIKE:
			return type;
		}
	}

	return 0;
}

int TileMap::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& colliderOffset, const glm::ivec2& colliderSize, bool bJumping) const
{
	int x, y0, y1;

	x = (pos.x + colliderOffset.x + colliderSize.x - 1) / tileSize;
	y0 = (pos.y + colliderOffset.y) / tileSize;
	y1 = (pos.y + colliderOffset.y + colliderSize.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		int tile = map[y * mapSize.x + x];
		int type = tileType[tile];
		switch (type)
		{
		case TILE_NOT_SOLID:
			break;
		case TILE_PLATFORM:
		case TILE_PLATFORM_ACTIVATED:
			if (bJumping) break;
		case TILE_SOLID:
		case TILE_SPIKE:
			return type;
		}
	}

	return 0;
}

int TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& colliderOffset, const glm::ivec2& colliderSize, int* posY) const
{
	int x0, x1, y;
	int offsetY = (posY != nullptr) ? (pos.y + colliderOffset.y) - *posY : 0;
	x0 = (pos.x + colliderOffset.x) / tileSize;
	x1 = (pos.x + colliderOffset.x + colliderSize.x - 1) / tileSize;
	y = (pos.y + colliderOffset.y + colliderSize.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		int tile = map[y * mapSize.x + x];
		int type = tileType[tile];
		switch (type)
		{
		case TILE_NOT_SOLID:
			break;
		case TILE_SOLID:
		case TILE_PLATFORM:
		case TILE_PLATFORM_ACTIVATED:
		case TILE_SPIKE:
			//if (posY == nullptr) {
			//	return type;
			//}
			if ((pos.y + colliderOffset.y) - tileSize * y + colliderSize.y <= 4)
			{
				if (posY != nullptr)
					*posY = tileSize * y - colliderSize.y - offsetY;
				return type;
			}
		}
	}

	return 0;
}

int TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& colliderOffset, const glm::ivec2& colliderSize, bool ignorePlatform) const
{
	int x0, x1, y;

	x0 = (pos.x + colliderOffset.x) / tileSize;
	x1 = (pos.x + colliderOffset.x + colliderSize.x - 1) / tileSize;
	y = (pos.y + colliderOffset.y) / tileSize;
	if (y < 0) return TILE_SOLID;
	for (int x = x0; x <= x1; x++)
	{
		int tile = map[y * mapSize.x + x];
		int type = tileType[tile];
		switch (type)
		{
		case TILE_NOT_SOLID:
			break;
		case TILE_PLATFORM:
		case TILE_PLATFORM_ACTIVATED:
			if (ignorePlatform)
				break;
		case TILE_SOLID:
		case TILE_SPIKE:
			return type;
		}
	}

	return 0;
}

bool TileMap::dentroDePlataforma(const glm::ivec2& pos, const glm::ivec2& colliderOffset, const glm::ivec2& colliderSize) {
	
	int pos0, pos1, coordFija;

	//topLeft ----> topRight
	//coordFija -> y
	pos0 = (pos.x + colliderOffset.x) / tileSize;
	pos1 = (pos.x + colliderOffset.x + colliderSize.x - 1) / tileSize;
	coordFija = (pos.y + colliderOffset.y) / tileSize;
	for (int pos = pos0; pos <= pos1; pos++) {
		int tile = map[coordFija * mapSize.x + pos];
		if (tileType[tile] == TILE_PLATFORM || tileType[tile] == TILE_PLATFORM_ACTIVATED) 
			return true;
	}

	//right
	//invariante -> x
	coordFija = (pos.x + colliderOffset.x + colliderSize.x - 1) / tileSize;
	pos0 = (pos.y + colliderOffset.y) / tileSize;
	pos1 = (pos.y + colliderOffset.y + colliderSize.y - 1) / tileSize;
	for (int pos = pos0; pos <= pos1; pos++) {
		int tile = map[pos * mapSize.x + coordFija];
		if (tileType[tile] == TILE_PLATFORM || tileType[tile] == TILE_PLATFORM_ACTIVATED)
			return true;
	}

	//left
	//invariante -> x
	coordFija = (pos.x + colliderOffset.x) / tileSize;
	pos0 = (pos.y + colliderOffset.y) / tileSize;
	pos1 = (pos.y + colliderOffset.y + colliderSize.y - 1) / tileSize;
	for (int pos = pos0; pos <= pos1; pos++) {
		int tile = map[pos * mapSize.x + coordFija];
		if (tileType[tile] == TILE_PLATFORM || tileType[tile] == TILE_PLATFORM_ACTIVATED)
			return true;
	}

	//down
	//coordFija -> y
	pos0 = (pos.x + colliderOffset.x) / tileSize;
	pos1 = (pos.x + colliderOffset.x + colliderSize.x - 1) / tileSize;
	coordFija = (pos.y + colliderOffset.y + colliderSize.y - 1) / tileSize;
	for (int pos = pos0; pos <= pos1; pos++) {
		int tile = map[coordFija * mapSize.x + pos];
		if (tileType[tile] == TILE_PLATFORM || tileType[tile] == TILE_PLATFORM_ACTIVATED)
			return true;
	}

	return false;
}


int TileMap::modifyTileMap(int i, int j, int newTile) {
	int oldTile = map[mapSize.x * i + j];
	if (oldTile != 1 && oldTile != 2) return -1;
	if (newTile < 0) newTile = map[mapSize.x * i + j] + (-newTile); //XD confia
	map[mapSize.x * i + j] = newTile;

	glm::vec2 posTile, oldPosTile, texCoordTile[2], halfTexel;
	posTile = glm::vec2(minCoords.x + j * tileSize, minCoords.y + i * tileSize);
	texCoordTile[0] = glm::vec2(float((newTile) % tilesheetSize.x) / tilesheetSize.x, float((newTile) / tilesheetSize.x) / tilesheetSize.y);
	texCoordTile[1] = texCoordTile[0] + tileTexSize;
	//texCoordTile[0] += halfTexel;
	//texCoordTile[1] -= halfTexel;
	

	vector<float> vertices;
	// First triangle
	vertices.push_back(posTile.x); vertices.push_back(posTile.y);
	vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
	vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
	vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
	vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
	vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
	// Second triangle
	vertices.push_back(posTile.x); vertices.push_back(posTile.y);
	vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
	vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
	vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
	vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
	vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 24 * numTile[i*mapSize.x + j], sizeof(float) * 24, &vertices[0]);

	return oldTile;
}

bool TileMap::tevacae(const glm::ivec2& pos, const glm::ivec2& size, bool rightSight) const {
	int pixelX, pixelY;
	if (rightSight) {
		pixelX = pos.x + size.x;
		pixelY = pos.y + size.y;
	}
	else {
		pixelX = pos.x-1;
		pixelY = pos.y + size.y;
	}
	return map[(pixelX / tileSize) + (pixelY / tileSize) * mapSize.x] == TILE_NOT_SOLID;
}


int TileMap::getNumOfTilesRemaining() { return numPlatforms; }

void TileMap::reduceNumberOfPlatforms() {
	numPlatforms--;
}

glm::ivec2 TileMap::getRandomPlatform() {
	glm::ivec2 randomPlatform(platformPositions[rand() % platformPositions.size()]);
	return randomPlatform;
}

























