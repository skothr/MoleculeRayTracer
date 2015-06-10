#ifndef APOLLO_SIMPLE_CONTAINERS_H
#define APOLLO_SIMPLE_CONTAINERS_H

#include "Container.h"

//A designated area on the screen, useful for grouping things together.
//	- Only effect is child elements being grouped together.
//	- 
class Area : public Container
{
private:

public:
	Area(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags);
	virtual ~Area();

	static const GuiPropFlags PROP_FLAGS;

	virtual void update(const Time &dt) override;
	virtual void draw(GlInterface &gl) override;
};


#endif	//APOLLO_SIMPLE_CONTAINERS_H