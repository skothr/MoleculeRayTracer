#ifndef APOLLO_STATIC_ELEMENT_H
#define APOLLO_STATIC_ELEMENT_H

#include "GuiElement.h"

class Container;

class StaticElement : public virtual GuiElement
{
private:

protected:
	StaticElement();

public:
	virtual ~StaticElement();
};

#endif	//APOLLO_STATIC_ELEMENT_H