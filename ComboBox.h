#ifndef APOLLO_COMBO_BOX_H
#define APOLLO_COMBO_BOX_H

#include "CompoundControl.h"

#include <vector>
#include <string>

class ComboBox : public CompoundControl
{
protected:



public:
	ComboBox(GuiElement *parent_, APoint a_pos, const std::vector<std::string> &items, GuiStateFlags s_flags);
	virtual ~ComboBox();

	static const GuiPropFlags PROP_FLAGS;



};


#endif	//APOLLO_COMBO_BOX_H