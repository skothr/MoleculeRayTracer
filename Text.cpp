#include "Text.h"
#include <iostream>
#include <iomanip>
#include <GL/glew.h>
#include "Screen.h"

#include "GlContext.h"

#include FT_GLYPH_H

//////FONT CLASS
FT_Library Font::ft;

Font::Font()
	: charHeight(0), ascii_glyphs()
{

}

Font::~Font()
{
	if(m_fontface) FT_Done_Face(m_fontface);
}

void Font::loadResources()
{
	if(FT_Init_FreeType(&ft))
	{
		std::cout << "Could not initialize FreeType!\n";
		return;
	}
}

void Font::update()
{
	if(charHeight)
	{
		if(FT_Set_Pixel_Sizes(m_fontface, 0, charHeight))
			std::cout << "Failed to set pixel size!\n";
		/*
		//Load glyphs into array
		for(int i = 0; i < 256; i++)
		{
			FT_ULong ch = (FT_ULong)i;
			//std::cout << "Char " << ch << ", " << FT_Get_Char_Index(m_fontface, ch) << "\n";
			if(FT_Load_Glyph(m_fontface, FT_Get_Char_Index(m_fontface, ch), FT_LOAD_RENDER))
			{
				std::cout << "Skipping char " << ch << ".\n";
				continue;
			}

			
			//if(i == 'A')
			//{
			//	FT_GlyphSlot g(m_fontface->glyph);

			//	int j = 0;
			//	for(int y = 0; y < g->bitmap.rows; y++)
			//	{
			//		for(int x = 0; x < g->bitmap.width; x++)
			//		{
			//			std::cout << std::hex << (int)g->bitmap.buffer[i] << " ";
			//			j++;
			//		}

			//		std::cout << "\n";
			//	}
			//	std::cout << "\n";
			//}
			
			FT_Glyph g;
			FT_Get_Glyph(m_fontface->glyph, &g);
			
			FT_Vector origin;
			origin.x = 0;
			origin.y = 0;

			FT_Glyph_To_Bitmap(&g, FT_RENDER_MODE_NORMAL, &origin, 1);

			ascii_glyphs[i] = (FT_BitmapGlyph)g;
		}
		*/
	}
}

void Font::load(std::string fileName)
{
	if(FT_New_Face(ft, fileName.c_str(), 0, &m_fontface))
	{
		std::cout << "Could not load font \"" << fileName << "\".\n";
		return;
	}

	update();
}

void Font::setCharacterSize(float height)
{
	if(charHeight != height)
	{
		charHeight = height;
		update();
	}
}

Vec3f Font::getStringSize(std::string str)
{
	Vec3f size;
	int max_y = 0;
	int min_y = 0;
	for(int i = 0; i < str.size(); i++)
	{
		FT_Load_Char(m_fontface, str[i], FT_LOAD_RENDER);
		size.x += m_fontface->glyph->advance.x >> 6;

		int ascent = m_fontface->glyph->bitmap_top;//getAscent() - m_fontface->glyph->bitmap_top;
		int descent = m_fontface->glyph->bitmap.rows - ascent;

		if(ascent > max_y)
			max_y = ascent;

		if(descent > min_y)
			min_y = descent;
	}

	size.y = max_y + min_y;//(m_fontface->ascender - m_fontface->descender) >> 6;//(m_fontface->bbox.yMax - m_fontface->bbox.yMin) >> 6;//(m_fontface->ascender - m_fontface->descender) >> 6;
	size.z = max_y;

	return size;
}

int Font::getAscent()
{
	return m_fontface->ascender >> 6;
}

int Font::getDescent()
{
	return m_fontface->descender >> 6;
}

FT_GlyphSlot Font::getCharacter(char c)
{
	FT_Load_Char(m_fontface, c, FT_LOAD_RENDER);

	return m_fontface->glyph;
}




/////TEXT CLASS/////
ShaderProgram	*Text::textSP = nullptr;
GLuint			Text::vbID = 0;

/*
Text::Text()
{
	update();
}
*/
Text::Text(std::string str, Font *f, float height)
	: font(f), charHeight(height), text(str), origin_y(0.0f)
{
	update();
}

Text::~Text()
{ }

void Text::loadResources()
{
	textSP = new ShaderProgram();
	textSP->loadShaders("shaders/fontRender.vsh", "shaders/fontRender.fsh");

	textSP->setUniform4f("text_color", 1.0f, 1.0f, 1.0f, 1.0f);
		
	//Create vertex buffer object
	float data[4][4] =
	{
		{0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 0.0f}
	};

	glGenBuffers(1, &vbID);

	//Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*4, data, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
}


void Text::update()
{
	if(charHeight && font && text != "")
	{
		font->setCharacterSize(charHeight);
		
		//Create texture for rendering characters
		GLuint charTex = 0;
		glGenTextures(1, &charTex);
		glBindTexture(GL_TEXTURE_2D, charTex);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
		textSP->setUniform1i("tex", 0);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbID);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		
		//Draw text to texture
		Vec3f strDims = font->getStringSize(text);//Vec2i(text.length()*30, 30);//font->getStringSize(text);

		AVec strSize(strDims.x, strDims.y);
		origin_y = strDims.z;

		tex.create(strSize);
		glClearColor(0.0, 0.0, 0.0, 0);
		tex.setActive(true);
		textSP->setActive();
		
		glBindTexture(GL_TEXTURE_2D, charTex);
		
		glDisable(GL_BLEND);
		Point2i cursor(0, origin_y);
		for(int i = 0; i < text.size(); i++)
		{
			FT_GlyphSlot g = font->getCharacter(text[i]);

			Point2f glCursor = toGlPoint(cursor + Vec2i(g->bitmap_left, -g->bitmap_top), strSize);

			Vec2i glyphSize = Vec2i(g->bitmap.width, g->bitmap.rows);
			Vec2f glSize = toGlVec(glyphSize, strSize);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, g->bitmap.width, g->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			float data[4][4] =
			{
				{glCursor.x, glCursor.y, 0.0f, 0.0f},
				{glCursor.x + glSize.x, glCursor.y, 1.0f, 0.0f},
				{glCursor.x, glCursor.y + glSize.y, 0.0f, 1.0f},
				{glCursor.x + glSize.x, glCursor.y + glSize.y, 1.0f, 1.0f},
			};
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*4, data, GL_DYNAMIC_DRAW);
			
			glColor3f(1.0f, 1.0f, 1.0f);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			cursor.x += g->advance.x >> 6;
			cursor.y += g->advance.y >> 6;
		}
		glEnable(GL_BLEND);

		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		
		glDeleteTextures(1, &charTex);
		glBindTexture(GL_TEXTURE_2D, NULL);
		glDisable(GL_TEXTURE_2D);

		glUseProgram(0);
		tex.setActive(false);
		
		glClearColor(0.15, 0.15, 0.15, 1.0);

	}

	return;
}

void Text::setFont(Font *f)
{
	font = f;
	update();
}

void Text::setCharacterSize(float height)
{
	if(font)
	{
		charHeight = height;
		font->setCharacterSize(charHeight);
		update();
	}
}

void Text::setString(std::string str)
{
	text = str;
	update();
}


Vec2f Text::getSize()
{
	return tex.getSize();
}


void Text::draw(Point2f pos, Vec2f size, Vec2f view_size)
{
	//setCharacterSize(100);
	tex.draw(pos + Vec2f(0.0f, (tex.getSize().y - origin_y)*(1.0f/2.0f)), size, view_size);
}