#ifndef APOLLO_LABEL_H
#define APOLLO_LABEL_H

#include "Display.h"
#include "Text.h"
#include <string>

class Label : public Display
{
protected:
	std::string	text;
	APoint		center_point;
	bool		centered = false;

	Text		textObj;

	void updateText();

public:
	static Font font;

	Label(ParentElement *parent_, APoint a_pos, GuiStateFlags s_flags, const std::string &text_, int font_height);
	virtual ~Label();

	static const GuiPropFlags PROP_FLAGS;

	static void initResources();

	void centerAround(APoint point);
	void setText(const std::string &new_text);
	std::string getText() const;

	virtual void draw(GlInterface &gl) override;
};


#endif	//APOLLO_LABEL_H