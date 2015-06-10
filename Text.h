#ifndef APOLLO_TEXT_H
#define APOLLO_TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H//<freetype2/freetype/freetype.h>
#include FT_GLYPH_H
#include <string>

#include "Vector.h"
#include "Texture.h"

class GlInterface;

class Font
{
private:
	static FT_Library ft;

	FT_Face			m_fontface;
	FT_GlyphSlot	ascii_glyphs[256];
	float			charHeight;

	void update();

public:
	Font();
	~Font();
	
	static void loadResources();

	void load(std::string fileName);
	void setCharacterSize(float height);

	int getAscent();
	int getDescent();

	Vec3f getStringSize(std::string str);
	FT_GlyphSlot getCharacter(char c);
};

class Text
{
private:
	//static ShaderProgram *textSP;
	//static GLuint vbID;

	//GlInterface		*gl = nullptr;

	Font			*font = nullptr;
	std::string		text = "";
	Texture			tex;

	float	charHeight = 0.0f,
			origin_y = 0.0f;


	void update();

public:
	Text();
	Text(std::string str, Font *f, float height = 30.0f);

	virtual ~Text();
	
	//Text resources (per glContext)
	static ShaderProgram *textSP;
	static GLuint vbID;

	static void loadResources();

	void setFont(Font *f);
	void setCharacterSize(float height);
	void setString(std::string str);

	Vec2f getSize();

	void draw(Point2f pos, Vec2f size, Vec2f view_size);

};


#endif	//APOLLO_TEXT_H