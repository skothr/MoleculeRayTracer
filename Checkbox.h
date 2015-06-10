#ifndef APOLLO_CHECKBOX_H
#define APOLLO_CHECKBOX_H

#include "CompoundControl.h"
#include "Label.h"
#include "Callbacks.h"

class Checkbox : public CompoundControl
{
protected:
	Label			label;
	bool			value = false;		//Off or on

	voidCallback	onValueChanged = nullptr;

	virtual void onMouseUp(APoint m_pos, MouseButton b, bool direct) override;

public:
	Checkbox(ParentElement *parent_, APoint a_pos, GuiStateFlags s_flags, std::string label_text = "");
	virtual ~Checkbox();

	static const GuiPropFlags	PROP_FLAGS;
	static const AVec			OUTER_BOX_SIZE,
								INNER_BOX_SIZE,
								BOX_PADDING,
								BORDER_PADDING;
	static const int			TEXT_HEIGHT;

	void setValue(bool val);
	void toggleValue();
	bool getValue() const;

	void onPosChanged(AVec d_pos) override;

	void setCallback(voidCallback on_value_changed);


	virtual void draw(GlInterface &gl) override;
};

#endif	//APOLLO_CHECKBOX_H