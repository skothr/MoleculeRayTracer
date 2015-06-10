#include "Label.h"
#include "ParentElement.h"

/////LABEL/////
Font Label::font;
const GuiPropFlags Label::PROP_FLAGS = PFlags::NONE;

Label::Label(ParentElement *parent_, APoint a_pos, GuiStateFlags s_flags, const std::string &text_, int font_height)
	: GuiElement(parent_, a_pos, AVec(0.0f, font_height), GuiProps(s_flags, PROP_FLAGS)),
		text(text_), textObj(text, &font, font_height)
{
	updateText();
}

Label::~Label()
{

}

void Label::initResources()
{
	font.load("fonts/Sansation_Regular.ttf");
	font.setCharacterSize(50);
}

void Label::updateText()
{
	textObj.setString(text);
	setSize(textObj.getSize());
}

void Label::centerAround(APoint point)
{
	centered = true;
	center_point = point;
	setPos(center_point - size*(1.0f/2.0f));
}

void Label::setText(const std::string &new_text)
{
	text = new_text;
	updateText();
}

std::string Label::getText() const
{
	return text;
}

void Label::draw(GlInterface &gl)
{
	//Only draw label if there's text to display (TODO: change this if you can do background colors and stuff)
	if(text != "" && isolateViewport(gl))
	{
		//drawBackground(gl);
		//updateText();
		//font.setCharacterSize(gl.absoluteToViewVec(size).y);
		//textObj.setCharacterSize(gl.absoluteToViewVec(size).y);
		//textObj.

		//if(centered)
		//	setPos(center_point - size*(1.0f/2.0f));

		gl.drawText(textObj, size, APoint(0.0f, 0.0f));

		restoreViewport(gl);
	}
}