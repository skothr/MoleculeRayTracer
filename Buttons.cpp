#include "Buttons.h"

#include "ParentElement.h"

#include <SFML/Graphics.hpp>
#include <GL/glew.h>


/////BASE BUTTON/////

AVec BaseButton::LABEL_PADDING = AVec(75, 25);
const GuiPropFlags BaseButton::PROP_FLAGS = PFlags::HARD_BACK;

BaseButton::BaseButton(GuiProps flags, std::string text)
	: GuiElement(),
		Control(flags),
		label(dynamic_cast<ParentElement*>(this), APoint(), DEFAULT_STATE_FLOAT, text, 14)
{
	//label.setParent(parent, true, true);
	if(text != "")
		setSize(AVec(label.getSize().x, 0.0f) + LABEL_PADDING);
}

BaseButton::BaseButton(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags, std::string text)
	: GuiElement(parent_, a_pos, a_size, GuiProps(s_flags, PROP_FLAGS)),
		Control(GuiProps(s_flags, PROP_FLAGS)),
		label(dynamic_cast<ParentElement*>(this), a_pos, DEFAULT_STATE_FLOAT, text, 14)
{
	//label.setParent(parent, true, true);
	if(text != "")
		setSize(AVec(label.getSize().x, 0.0f) + LABEL_PADDING);

	//Default background colors
	setAllBgStateColors(Color(1.0f, 0.0f, 1.0f, 1.0f));
	setBgStateColor(Color(0.3f, 0.3f, 0.3f, 1.0f), CS::NONE);
	setBgStateColor(Color(0.3f, 0.4f, 0.5f, 1.0f), CS::HOVERING);
	setBgStateColor(Color(0.1f, 0.1f, 0.1f, 1.0f), CS::CLICKING);
	setBgStateColor(Color(0.1f, 0.1f, 0.1f, 1.0f), CS::DRAGGING);

	updateResources();
}

BaseButton::BaseButton(ParentElement *parent_, APoint a_pos, GuiStateFlags s_flags, std::string text)
	: GuiElement(parent_, a_pos, AVec(), GuiProps(s_flags, PROP_FLAGS)),
		Control(GuiProps(s_flags, PROP_FLAGS)),
		label(dynamic_cast<ParentElement*>(this), a_pos, DEFAULT_STATE_FLOAT, text, 14)
{
	if(text != "")
		setSize(AVec(label.getSize().x, 0.0f) + LABEL_PADDING);
	updateResources();
}

BaseButton::~BaseButton()
{ }

void BaseButton::onSizeChanged(AVec d_size)
{
	Control::onSizeChanged(d_size);

	//Set new label position
	label.centerAround(size/2.0f);
}

void BaseButton::onMouseUp(APoint m_pos, MouseButton b, bool direct)
{
	if(direct && clickAction)
		clickAction();
}

void BaseButton::setText(const std::string &new_text)
{
	label.setText(new_text);
}

void BaseButton::setClickFunction(voidCallback click_action)
{
	clickAction = click_action;
}


void BaseButton::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		drawBackground(gl);
		label.draw(gl);

		restoreViewport(gl);
	}
}


/////IMAGE BUTTON/////
Texture
	ImageButton::texBtnCorner_tl,
	ImageButton::texBtnCorner_tr,
	ImageButton::texBtnCorner_bl,
	ImageButton::texBtnCorner_br,
	ImageButton::texBtnSide_l,
	ImageButton::texBtnSide_t,
	ImageButton::texBtnSide_r,
	ImageButton::texBtnSide_b,
	ImageButton::texBtnFill,
	
	ImageButton::texBtnHoverCorner_tl,
	ImageButton::texBtnHoverCorner_tr,
	ImageButton::texBtnHoverCorner_bl,
	ImageButton::texBtnHoverCorner_br,
	ImageButton::texBtnHoverSide_l,
	ImageButton::texBtnHoverSide_t,
	ImageButton::texBtnHoverSide_r,
	ImageButton::texBtnHoverSide_b,
	ImageButton::texBtnHoverFill,
	
	ImageButton::texBtnClickCorner_tl,
	ImageButton::texBtnClickCorner_tr,
	ImageButton::texBtnClickCorner_bl,
	ImageButton::texBtnClickCorner_br,
	ImageButton::texBtnClickSide_l,
	ImageButton::texBtnClickSide_t,
	ImageButton::texBtnClickSide_r,
	ImageButton::texBtnClickSide_b,
	ImageButton::texBtnClickFill;


ImageButton::ImageButton(GuiProps flags, std::string text)
	: GuiElement(),
		BaseButton(flags, text)
{ }


ImageButton::ImageButton(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags, std::string text)
	: GuiElement(parent_, a_pos, a_size, GuiProps(s_flags, PROP_FLAGS)),
		BaseButton(GuiProps(s_flags, PROP_FLAGS), text)
{
	updateResources();
}

ImageButton::ImageButton(ParentElement *parent_, APoint a_pos, GuiStateFlags s_flags, std::string text)
	: GuiElement(parent_, a_pos, AVec(), GuiProps(s_flags, PROP_FLAGS)),
		BaseButton(GuiProps(s_flags, PROP_FLAGS), text)
{
	//setSize(AVec(100.0f, 30.0f));
	//setSize(AVec(label.getSize().x, 0.0f) + LABEL_PADDING);
	updateResources();
}

ImageButton::~ImageButton()
{ }

void ImageButton::loadResources()
{
	sf::Image im;
	//Base textures for button components
	im.loadFromFile("button/btn-corner-tl.png");
	texBtnCorner_tl.create(AVec(AVec(im.getSize().x, im.getSize().y)), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-corner-tr.png");
	texBtnCorner_tr.create(AVec(AVec(im.getSize().x, im.getSize().y)), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-corner-bl.png");
	texBtnCorner_bl.create(AVec(AVec(im.getSize().x, im.getSize().y)), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-corner-br.png");
	texBtnCorner_br.create(AVec(AVec(im.getSize().x, im.getSize().y)), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-side-l.png");
	texBtnSide_l.create(AVec(AVec(im.getSize().x, im.getSize().y)), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-side-t.png");
	texBtnSide_t.create(AVec(AVec(im.getSize().x, im.getSize().y)), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-side-r.png");
	texBtnSide_r.create(AVec(AVec(im.getSize().x, im.getSize().y)), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-side-b.png");
	texBtnSide_b.create(AVec(AVec(im.getSize().x, im.getSize().y)), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-fill.png");
	texBtnFill.create(AVec(AVec(im.getSize().x, im.getSize().y)), (void*)im.getPixelsPtr());

	
	//Hover textures for button components
	im.loadFromFile("button/btn-hover-corner-tl.png");
	texBtnHoverCorner_tl.create(AVec(AVec(im.getSize().x, im.getSize().y)), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-hover-corner-tr.png");
	texBtnHoverCorner_tr.create(AVec(AVec(im.getSize().x, im.getSize().y)), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-hover-corner-bl.png");
	texBtnHoverCorner_bl.create(AVec(AVec(im.getSize().x, im.getSize().y)), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-hover-corner-br.png");
	texBtnHoverCorner_br.create(AVec(AVec(im.getSize().x, im.getSize().y)), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-hover-side-l.png");
	texBtnHoverSide_l.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-hover-side-t.png");
	texBtnHoverSide_t.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-hover-side-r.png");
	texBtnHoverSide_r.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-hover-side-b.png");
	texBtnHoverSide_b.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-hover-fill.png");
	texBtnHoverFill.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
	
	//Click textures for button components
	im.loadFromFile("button/btn-click-corner-tl.png");
	texBtnClickCorner_tl.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-click-corner-tr.png");
	texBtnClickCorner_tr.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-click-corner-bl.png");
	texBtnClickCorner_bl.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-click-corner-br.png");
	texBtnClickCorner_br.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-click-side-l.png");
	texBtnClickSide_l.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-click-side-t.png");
	texBtnClickSide_t.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-click-side-r.png");
	texBtnClickSide_r.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-click-side-b.png");
	texBtnClickSide_b.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
	im.loadFromFile("button/btn-click-fill.png");
	texBtnClickFill.create(AVec(im.getSize().x, im.getSize().y), (void*)im.getPixelsPtr());
}

void ImageButton::renderBase()
{
	//Create newly-sized textures
	baseTexture.create(size);
	hoverTexture.create(size);
	clickTexture.create(size);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	AVec	texSize = texBtnCorner_tl.getSize(),	//All textures same size for now
			inside_dim = size - texSize*2;			//Inside width/height

	////Draw base button
	baseTexture.setActive(true);
	//Corners
	texBtnCorner_tl.draw(APoint(0, 0), texSize, size);
	texBtnCorner_tr.draw(APoint(size.x - texSize.x, 0), texSize, size);
	texBtnCorner_bl.draw(APoint(0, size.y - texSize.y), texSize, size);
	texBtnCorner_br.draw(APoint(size.x - texSize.x, size.y - texSize.y), texSize, size);
	
	//Sides
	texBtnSide_l.draw(APoint(0, texSize.y), AVec(texSize.x, inside_dim.y), size);
	texBtnSide_r.draw(APoint(size.x - texSize.x, texSize.y), AVec(texSize.x, inside_dim.y), size);
	texBtnSide_t.draw(APoint(texSize.x, 0), AVec(inside_dim.x, texSize.y), size);
	texBtnSide_b.draw(APoint(texSize.x, size.y - texSize.y), AVec(inside_dim.x, texSize.y), size);

	//Center
	texBtnFill.draw(APoint(texSize.x, texSize.y), inside_dim, size);

	baseTexture.setActive(false);
	
	////Draw hover button
	hoverTexture.setActive(true);
	//Corners
	texBtnHoverCorner_tl.draw(APoint(0, 0), texSize, size);
	texBtnHoverCorner_tr.draw(APoint(size.x - texSize.x, 0), texSize, size);
	texBtnHoverCorner_bl.draw(APoint(0, size.y - texSize.y), texSize, size);
	texBtnHoverCorner_br.draw(APoint(size.x - texSize.x, size.y - texSize.y), texSize, size);
	
	//Sides
	texBtnHoverSide_l.draw(APoint(0, texSize.y), AVec(texSize.x, inside_dim.y), size);
	texBtnHoverSide_r.draw(APoint(size.x - texSize.x, texSize.y), AVec(texSize.x, inside_dim.y), size);
	texBtnHoverSide_t.draw(APoint(texSize.x, 0), AVec(inside_dim.x, texSize.y), size);
	texBtnHoverSide_b.draw(APoint(texSize.x, size.y - texSize.y), AVec(inside_dim.x, texSize.y), size);

	//Center
	texBtnHoverFill.draw(APoint(texSize.x, texSize.y), inside_dim, size);

	hoverTexture.setActive(false);
	
	////Draw click button
	clickTexture.setActive(true);
	//Corners
	texBtnClickCorner_tl.draw(APoint(0, 0), texSize, size);
	texBtnClickCorner_tr.draw(APoint(size.x - texSize.x, 0), texSize, size);
	texBtnClickCorner_bl.draw(APoint(0, size.y - texSize.y), texSize, size);
	texBtnClickCorner_br.draw(APoint(size.x - texSize.x, size.y - texSize.y), texSize, size);
	
	//Sides
	texBtnClickSide_l.draw(APoint(0, texSize.y), AVec(texSize.x, inside_dim.y), size);
	texBtnClickSide_r.draw(APoint(size.x - texSize.x, texSize.y), AVec(texSize.x, inside_dim.y), size);
	texBtnClickSide_t.draw(APoint(texSize.x, 0), AVec(inside_dim.x, texSize.y), size);
	texBtnClickSide_b.draw(APoint(texSize.x, size.y - texSize.y), AVec(inside_dim.x, texSize.y), size);

	//Center
	texBtnClickFill.draw(APoint(texSize.x, texSize.y), inside_dim, size);

	clickTexture.setActive(false);

}

void ImageButton::onSizeChanged(AVec d_size)
{
	BaseButton::onSizeChanged(d_size);
	renderBase();
}

void ImageButton::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		//Draw button background (texture)
		switch(cState)
		{
		case CS::NONE:
			gl.drawTexture(baseTexture, APoint(0.0f, 0.0f), size);
			break;

		case CS::HOVERING:
			gl.drawTexture(hoverTexture, APoint(0.0f, 0.0f), size);
			break;

		case CS::CLICKING:
		case CS::DRAGGING:
			gl.drawTexture(clickTexture, APoint(0.0f, 0.0f), size);
			break;

		default:
			break;
		}

		label.draw(gl);

		restoreViewport(gl);
	}
}