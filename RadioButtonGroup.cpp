#include "RadioButtonGroup.h"

#include <GL/glew.h>

/////RADIO BUTTON/////
const GuiPropFlags RadioButton::PROP_FLAGS = PFlags::HARD_BACK;
const AVec			RadioButton::OUTER_CIRCLE_SIZE = AVec(10.0f, 10.0f),
					RadioButton::INNER_CIRCLE_SIZE = AVec(8.0f, 8.0f),
					RadioButton::CIRCLE_PADDING = AVec(3.0f, 3.0f),
					RadioButton::BORDER_PADDING = AVec(3.0f, 3.0f),
					RadioButton::PADDED_CIRCLE_SIZE = (BORDER_PADDING + CIRCLE_PADDING)*2.0f + OUTER_CIRCLE_SIZE;
const int			RadioButton::TEXT_HEIGHT = 14.0f;

RadioButton::RadioButton(ParentElement *parent_, APoint a_pos, GuiStateFlags s_flags, unsigned int g_index, const std::string &label_text)
	: GuiElement(parent_, a_pos, AVec(), GuiProps(s_flags, PROP_FLAGS)),
		Control(GuiProps(s_flags, PROP_FLAGS)),
		label(dynamic_cast<ParentElement*>(this), APoint(BORDER_PADDING.x + 2.0f*CIRCLE_PADDING.x + OUTER_CIRCLE_SIZE.x, 0.0f), DEFAULT_STATE_FLOAT, label_text, TEXT_HEIGHT),
		group(dynamic_cast<RadioButtonGroup*>(parent_)), groupIndex(g_index)
{
	AVec label_size = label.getSize();

	//label.setParent(parent, false);
	
	setSize(PADDED_CIRCLE_SIZE + AVec(label_size.x, 0.0f));

	label.setY((size.y - label_size.y)*(1.0f/2.0f));

}

RadioButton::~RadioButton()
{ }

void RadioButton::onMouseUp(APoint m_pos, MouseButton b, bool direct)
{
	if(direct)
		group->selectValue(groupIndex);
}

void RadioButton::setValue(bool val)
{
	value = val;
}

bool RadioButton::getValue() const
{
	return value;
}

unsigned int RadioButton::getGroupIndex() const
{
	return groupIndex;
}

void RadioButton::setLabel(const std::string &new_text)
{
	label.setText(new_text);
}
std::string RadioButton::getLabel() const
{
	return label.getText();
}


void RadioButton::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		glPointSize(OUTER_CIRCLE_SIZE.x);
		//Draw checkbox base
		gl.setColor(Color(0.75f, 0.75f, 0.75f, 1.0f));
		//gl.drawRect(Rect(APoint(CIRCLE_PADDING + BORDER_PADDING), OUTER_CIRCLE_SIZE));
		gl.drawPoint(APoint(CIRCLE_PADDING + BORDER_PADDING) + OUTER_CIRCLE_SIZE*(1.0f/2.0f));

		if(value)
		{
			//Draw filled checkbox
			glPointSize(INNER_CIRCLE_SIZE.x);
			gl.setColor(Color(0.15f, 0.25f, 0.6f, 1.0f));
			//gl.drawRect(Rect(APoint(CIRCLE_PADDING + BORDER_PADDING) + (OUTER_CIRCLE_SIZE - INNER_CIRCLE_SIZE)*1.0f/2.0f, INNER_CIRCLE_SIZE));
			gl.drawPoint(APoint(CIRCLE_PADDING + BORDER_PADDING) + OUTER_CIRCLE_SIZE*(1.0f/2.0f));
		
		}

		glPointSize(10.0f);	//(default)

		label.draw(gl);

		restoreViewport(gl);
	}
}




/////RADIO BUTTON GROUP/////
const GuiPropFlags	RadioButtonGroup::PROP_FLAGS = PFlags::HARD_BACK | PFlags::ABSORB_SCROLL;
const float			RadioButtonGroup::PADDING = 2.0f;

RadioButtonGroup::RadioButtonGroup(ParentElement *parent_, APoint a_pos, GuiStateFlags s_flags, const std::vector<std::string> &labels)
	: GuiElement(parent_, a_pos, AVec(), GuiProps(s_flags, PROP_FLAGS)),
		CompoundControl(GuiProps(s_flags, PROP_FLAGS))
{
	GuiStateFlags button_states = SFlags::VISIBLE | SFlags::ENABLED | SFlags::FLOATING;

	std::vector<GuiElement*> children;

	float	y_offset = 0.0f,
			max_width = 0.0f;
	buttons.reserve(labels.size());
	for(unsigned int i = 0; i < labels.size(); i++)
	{
		RadioButton *rb = new RadioButton(this, APoint(0.0f, y_offset), button_states, i, labels[i]);
		buttons.push_back(rb);
		children.push_back(rb);

		y_offset += rb->getSize().y + PADDING;
		max_width = max(max_width, rb->getSize().x);
	}

	setSize(AVec(max_width, y_offset - PADDING));

	CompoundControl::init(nullptr, &children);

	//Initially selected
	selectValue(0u);
}

RadioButtonGroup::~RadioButtonGroup()
{
	for(auto rb : buttons)
		if(rb) delete rb;
	buttons.clear();
}

void RadioButtonGroup::selectValue(unsigned int rb_index)
{
	buttons[selectedIndex]->setValue(false);
	selectedIndex = rb_index;
	buttons[selectedIndex]->setValue(true);
}

void RadioButtonGroup::selectValue(const RadioButton &selected)
{
	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		if(buttons[i]->getGroupIndex() == selected.getGroupIndex())
		{
			selectValue(i);
			return;
		}
	}
	/*
	auto iter = std::find(buttons.begin(), buttons.end(), selected);

	if(iter != buttons.end())
		selectValue(iter - buttons.begin());
		*/
}

void RadioButtonGroup::selectValue(const std::string &label_text)
{
	for(unsigned int i = 0; i < buttons.size(); i++)
	{
		if(buttons[i]->getLabel() == label_text)
		{
			selectValue(i);
			return;
		}
	}
}

unsigned int RadioButtonGroup::getValue() const
{
	return selectedIndex;
}
	
void RadioButtonGroup::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		drawChildren(gl);

		restoreViewport(gl);
	}
}