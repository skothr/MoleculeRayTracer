#ifndef APOLLO_CONTROL_H
#define APOLLO_CONTROL_H

#include "ActiveElement.h"

class Container;

class Control : public ActiveElement
{
protected:
	Control(GuiProps flags);

public:
	virtual ~Control();


};

#endif	//APOLLO_CONTROL_H