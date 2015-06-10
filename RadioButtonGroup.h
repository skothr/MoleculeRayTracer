#ifndef APOLLO_RADIO_BUTTON_H
#define APOLLO_RADIO_BUTTON_H

#include "Control.h"
#include "CompoundControl.h"
#include "Label.h"

#include <vector>

class RadioButtonGroup;

class RadioButton : public Control
{
protected:
	Label label;
	bool value;					//Selected or not

	RadioButtonGroup *group;	//Must be part of a group -- only one selected at a time
	unsigned int groupIndex;

	virtual void onMouseUp(APoint m_pos, MouseButton b, bool direct) override;

public:
	RadioButton(ParentElement *parent_, APoint a_pos, GuiStateFlags s_flags, unsigned int g_index, const std::string &label_text = "");
	virtual ~RadioButton();
	
	static const GuiPropFlags	PROP_FLAGS;
	static const AVec			OUTER_CIRCLE_SIZE,
								INNER_CIRCLE_SIZE,
								CIRCLE_PADDING,
								BORDER_PADDING,
								PADDED_CIRCLE_SIZE;
	static const int			TEXT_HEIGHT;

	void setValue(bool val);
	bool getValue() const;
	
	unsigned int getGroupIndex() const;
	
	void setLabel(const std::string &new_text);
	std::string getLabel() const;

	virtual void draw(GlInterface &gl) override;
};


class RadioButtonGroup : public CompoundControl
{
protected:
	unsigned int selectedIndex = 0;
	std::vector<RadioButton*> buttons;

public:
	RadioButtonGroup(ParentElement *parent_, APoint a_pos, GuiStateFlags s_flags, const std::vector<std::string> &labels);
	virtual ~RadioButtonGroup();
	
	static const GuiPropFlags	PROP_FLAGS;
	static const float			PADDING;		//Padding between radio buttons

	void selectValue(unsigned int rb_index);
	void selectValue(const RadioButton &selected);
	void selectValue(const std::string &label_text);
	unsigned int getValue() const;
	
	virtual void draw(GlInterface &gl) override;

};



#endif	//APOLLO_RADIO_BUTTON_H