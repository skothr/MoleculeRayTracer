#ifndef APOLLO_BUTTON_H
#define APOLLO_BUTTON_H

#include "Control.h"
#include "Texture.h"
#include "Label.h"

#include "Callbacks.h"

#include <functional>

//Button base class -- represents a clickable button that executes a function.
//	- Draws a basic rectangle, and executes a function on click (mouse up)
//	- Has optional text (label)
class BaseButton : public Control
{
protected:
	//For child class initialization
	BaseButton(GuiProps flags, std::string text);
	
	Label label;
	voidCallback clickAction = nullptr;

	virtual void onMouseUp(APoint m_pos, MouseButton b, bool direct) override;
	virtual void onSizeChanged(AVec d_size) override;

public:
	static AVec LABEL_PADDING;

	BaseButton(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags, std::string text = "");
	BaseButton(ParentElement *parent_, APoint a_pos, GuiStateFlags s_flags, std::string text = "");
	virtual ~BaseButton();

	static const GuiPropFlags PROP_FLAGS;
	
	void setText(const std::string &new_text);
	void setClickFunction(voidCallback click_action);

	virtual void draw(GlInterface &gl) override;
};

//Button with a background constructed with images rather than just a plain rectangle.
class ImageButton : public BaseButton
{
protected:
	ImageButton(GuiProps flags, std::string text = "");

	static Texture
		texBtnCorner_tl,
		texBtnCorner_tr,
		texBtnCorner_bl,
		texBtnCorner_br,
		texBtnSide_l,
		texBtnSide_t,
		texBtnSide_r,
		texBtnSide_b,
		texBtnFill,
		
		texBtnHoverCorner_tl,
		texBtnHoverCorner_tr,
		texBtnHoverCorner_bl,
		texBtnHoverCorner_br,
		texBtnHoverSide_l,
		texBtnHoverSide_t,
		texBtnHoverSide_r,
		texBtnHoverSide_b,
		texBtnHoverFill,
		
		texBtnClickCorner_tl,
		texBtnClickCorner_tr,
		texBtnClickCorner_bl,
		texBtnClickCorner_br,
		texBtnClickSide_l,
		texBtnClickSide_t,
		texBtnClickSide_r,
		texBtnClickSide_b,
		texBtnClickFill;

	Texture	baseTexture,
			hoverTexture,
			clickTexture;

	void renderBase();
	
	virtual void onSizeChanged(AVec d_size) override;

public:
	ImageButton(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags, std::string text = "");
	ImageButton(ParentElement *parent_, APoint a_pos, GuiStateFlags s_flags, std::string text = "");
	virtual ~ImageButton();

	static void loadResources();

	virtual void draw(GlInterface &gl) override;
};


#endif	//APOLLO_BUTTON_H