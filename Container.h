#ifndef APOLLO_CONTAINER_H
#define APOLLO_CONTAINER_H

#include "GuiElement.h"
#include "ParentElement.h"
#include "ActiveElement.h"
#include "StaticElement.h"

class Container : public ParentElement, public ActiveElement
{
protected:
	Container(GuiProps flags);

public:
	virtual ~Container();

	virtual void update(const Time &dt) override;
};


class StaticContainer : public ParentElement, public StaticElement
{
protected:
	StaticContainer();

public:
	virtual ~StaticContainer();
};


#endif	//APOLLO_CONTAINER_H