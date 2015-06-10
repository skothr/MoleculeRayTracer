#include "GuiGroup.h"

#include "GuiElement.h"
#include "ActiveElement.h"
#include "Container.h"

#include <algorithm>


/////GUI GROUP/////
GuiGroup::GuiGroup()
{ }

GuiGroup::GuiGroup(const std::vector<GuiElement*> &group)
	: members(group)
{ }

GuiGroup::GuiGroup(const GuiGroup &other)
	: members(other.members.begin(), other.members.end())
{ }

GuiGroup::~GuiGroup()
{
	members.clear();
}

void GuiGroup::copy(const GuiGroup &other)
{
	members.clear();
	members.reserve(other.members.size());
	members.insert(members.begin(), other.members.begin(), other.members.end());
}

void GuiGroup::addFront(GuiElement *e)
{
	if(e)	members.push_back(e);
}

void GuiGroup::addFront(const std::vector<GuiElement*> &e_list)
{
	//Append to the end of the vector (front of the gui)
	members.reserve(members.size() + e_list.size());
	for(auto e : e_list)
	{
		if(e)
			members.push_back(e);
	}
}

void GuiGroup::addBack(GuiElement *e)
{
	if(e)
		members.insert(members.begin(), e);
}

void GuiGroup::addBack(const std::vector<GuiElement*> &e_list)
{
	//Append to the beginning of the vector (back of the gui)
	std::vector<GuiElement*> valid_e;
	valid_e.reserve(e_list.size());
	for(unsigned int i = 0; i < e_list.size(); i++)
	{
		GuiElement *e = e_list[i];
		if(e)
			valid_e.push_back(e);
	}

	members.reserve(members.size() + valid_e.size());
	members.insert(members.begin(), valid_e.begin(), valid_e.end());
}

void GuiGroup::add(GuiElement *e, unsigned int index)
{
	if(e && index < members.size())
		members.insert(members.begin() + index, e);
}

void GuiGroup::add(const std::vector<GuiElement*> &e_list, unsigned int index)
{
	//Append to the middle of the vector, specified by index
	std::vector<GuiElement*> valid_e;
	valid_e.reserve(e_list.size());
	for(unsigned int i = 0; i < e_list.size(); i++)
	{
		GuiElement *e = e_list[i];
		if(e)
			valid_e.push_back(e);
	}

	members.reserve(members.size() + valid_e.size());
	members.insert(members.begin() + index, valid_e.begin(), valid_e.end());
}


bool GuiGroup::remove(GuiElement *e)
{
	auto iter = std::find(members.begin(), members.end(), e);

	if(iter != members.end())
	{
		members.erase(iter);
		return true;
	}
	else
		return false;
}

bool GuiGroup::remove(unsigned int index)
{
	if(index < members.size())
	{
		members.erase(members.begin() + index);
		return true;
	}
	else
		return false;
}

void GuiGroup::clear()
{
	members.clear();
}

bool GuiGroup::contains(const GuiElement *e) const
{
	return (std::find(members.begin(), members.end(), e) != members.end());
}

GuiElement* GuiGroup::front() const
{
	return members[members.size() - 1];
}

GuiElement* GuiGroup::back() const
{
	return members[0];
}

const std::vector<GuiElement*>* GuiGroup::getMembers() const
{
	return &members;
}


bool GuiGroup::mouseMove(APoint m_pos, AVec d_pos, bool blocked) const
{
	//Iterate over members in reverse order (front to back)
	for(int i = (int)members.size() - 1; i >= 0; i--)
	{
		ActiveElement *ae = dynamic_cast<ActiveElement*>(members[i]);

		if(ae)
			blocked |= ae->mouseMove(m_pos, d_pos, blocked);
	}

	return blocked;
}

bool GuiGroup::mouseDown(APoint m_pos, MouseButton b, bool blocked) const
{
	//Iterate over members in reverse order (front to back)
	for(int i = (int)members.size() - 1; i >= 0; i--)
	{
		ActiveElement *ae = dynamic_cast<ActiveElement*>(members[i]);

		if(ae)
			blocked |= ae->mouseDown(m_pos, b, blocked);
	}
	
	return blocked;
}

bool GuiGroup::mouseUp(APoint m_pos, MouseButton b, bool blocked) const
{
	//Iterate over members in reverse order (front to back)
	for(int i = (int)members.size() - 1; i >= 0; i--)
	{
		ActiveElement *ae = dynamic_cast<ActiveElement*>(members[i]);

		if(ae)
			blocked |= ae->mouseUp(m_pos, b, blocked);
	}

	return blocked;
}

bool GuiGroup::mouseScroll(APoint m_pos, AVec d_scroll, bool blocked) const
{
	//Iterate over members in reverse order (front to back)
	for(int i = (int)members.size() - 1; i >= 0; i--)
	{
		ActiveElement *ae = dynamic_cast<ActiveElement*>(members[i]);

		if(ae)
			blocked |= ae->mouseScroll(m_pos, d_scroll, blocked);
	}
	
	return blocked;
}

void GuiGroup::keyDown(KeyCode key)
{
	//Iterate over members in reverse order (front to back)
	for(int i = (int)members.size() - 1; i >= 0; i--)
	{
		ActiveElement *ae = dynamic_cast<ActiveElement*>(members[i]);

		if(ae)
			ae->onKeyDown(key);
	}
}

void GuiGroup::keyUp(KeyCode key)
{
	//Iterate over members in reverse order (front to back)
	for(int i = (int)members.size() - 1; i >= 0; i--)
	{
		ActiveElement *ae = dynamic_cast<ActiveElement*>(members[i]);

		if(ae)
			ae->onKeyUp(key);
	}
}

void GuiGroup::updateResources() const
{
	for(auto e : members)
		e->updateResources();
}

void GuiGroup::update(const Time &dt) const
{
	//Update back to front
	for(auto e : members)
		e->update(dt);
}

void GuiGroup::draw(GlInterface &gl) const
{
	//Draw back to front
	for(auto e : members)
		e->draw(gl);
}