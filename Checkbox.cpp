#include "Checkbox.h"

/////CHECKBOX/////
const GuiPropFlags Checkbox::PROP_FLAGS = PFlags::HARD_BACK;
const AVec			Checkbox::OUTER_BOX_SIZE = AVec(10.0f, 10.0f),
					Checkbox::INNER_BOX_SIZE = AVec(8.0f, 8.0f),
					Checkbox::BOX_PADDING = AVec(3.0f, 3.0f),
					Checkbox::BORDER_PADDING = AVec(3.0f, 3.0f);
const int			Checkbox::TEXT_HEIGHT = 14.0f;

Checkbox::Checkbox(ParentElement *parent_, APoint a_pos, GuiStateFlags s_flags, std::string label_text)
	: GuiElement(parent_, a_pos, AVec(), GuiProps(s_flags, PROP_FLAGS)),
		CompoundControl(GuiProps(s_flags, PROP_FLAGS)),
		value(false),
		label(this, APoint(BORDER_PADDING.x + 2.0f*BOX_PADDING.x + OUTER_BOX_SIZE.x, 0.0f), SFlags::VISIBLE | SFlags::ENABLED, label_text, TEXT_HEIGHT)
{
	AVec label_size = label.getSize();
	
	AVec padded_box_size = (BORDER_PADDING + BOX_PADDING)*2.0f + OUTER_BOX_SIZE;
	setSize(padded_box_size + AVec(label_size.x, 0.0f));

	label.setY((size.y - label_size.y)*(1.0f/2.0f));

}

Checkbox::~Checkbox()
{ }

void Checkbox::onMouseUp(APoint m_pos, MouseButton b, bool direct)
{
	if(direct)
		toggleValue();
}

void Checkbox::setValue(bool val)
{
	if(value != val)
	{
		value = val;
		if(onValueChanged)
			onValueChanged();
	}
}

void Checkbox::toggleValue()
{
	value = !value;
	if(onValueChanged)
		onValueChanged();
}

bool Checkbox::getValue() const
{
	return value;
}

void Checkbox::onPosChanged(AVec d_pos)
{
	label.move(d_pos);
}


void Checkbox::setCallback(voidCallback on_value_changed)
{
	onValueChanged = on_value_changed;
}


void Checkbox::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		//Draw checkbox base
		gl.setColor(Color(0.75f, 0.75f, 0.75f, 1.0f));
		gl.drawRect(Rect(APoint(BOX_PADDING + BORDER_PADDING), OUTER_BOX_SIZE));

		if(value)
		{
			//Draw filled checkbox
			gl.setColor(Color(0.15f, 0.25f, 0.6f, 1.0f));
			gl.drawRect(Rect(APoint(BOX_PADDING + BORDER_PADDING) + (OUTER_BOX_SIZE - INNER_BOX_SIZE)*(1.0f/2.0f), INNER_BOX_SIZE));
		}
		
		label.draw(gl);

		restoreViewport(gl);
	}
}