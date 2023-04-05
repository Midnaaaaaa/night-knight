#ifndef _TEXT_INCLUDE
#define _TEXT_INCLUDE


#include <ft2build.h>
#include FT_FREETYPE_H
#include "Texture.h"
#include "ShaderProgram.h"
//#include "TexturedQuad.h"
#include "Sprite.h"

#define NUM_CHARACTERS (128 - 32)




struct CharMetrics
{
	int tx, ty; 		// Pos x, y inside texture in pixels
	int sx, sy; 		// Size of char in pixels
	int ax, ay; 		// Advance for each character in a string sequence
	int bl, bt; 		// Bitmap left and top adjusting
};


// Using Freetype the Text class is capable of building a texture atlas
// for a given font and size. Combining with TexturedQuad allows it 
// to render text strings


class Text
{

public:
	Text();
	~Text();
	
	bool init(const char *filename, const glm::vec2& tileMapPos, const glm::mat4& projection);
	void destroy();
	
	ShaderProgram &getProgram();
	int getSize() const;
	void render(char c, const glm::vec2 &pixel, int size, const glm::vec4 &color);
	void render(const string &str, const glm::vec2 &pixel, int size, const glm::vec4 &color, int alignment = 0);

	int width(const string& str, int size);
	
	static const int CENTERED = 1;
	static const int LEFT_ALIGNED = 0;
	static const int RIGHT_ALIGNED = 2;

	Texture* getAlias();

private:
	void initShaders();
	bool extractCharSizes(int *maxCharWidth, int *maxCharHeight);
	void createTextureAtlas();
	
private:
	int fontSize, textureSize, maxCharWidth, maxCharHeight;
	FT_Face face;
	CharMetrics chars[NUM_CHARACTERS];
	Texture textureAtlas;
	ShaderProgram program;
	Sprite *quad;

	static bool bLibInit;
	static FT_Library library;

	glm::mat4 projection;


	
};


#endif // _TEXT_INCLUDE


