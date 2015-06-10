#ifndef APOLLO_GUI_GROUP_H
#define APOLLO_GUI_GROUP_H

#include "GuiElement.h"
#include "Mouse.h"
#include "Keyboard.h"
#include <vector>

class GuiElement;

//Represents/controls a group of GUI elements
//	- Order of elements:
//			--> members[0] is the very back, members[members.size()] is the very front.
//	- This order is assumed for all vectors handled by this class.
class GuiGroup
{
private:
	std::vector<GuiElement*> members;

public:
	GuiGroup();
	GuiGroup(const std::vector<GuiElement*> &group);
	GuiGroup(const GuiGroup &other);
	virtual ~GuiGroup();

	void copy(const GuiGroup &other);

	//Adds an element in front of all current group elements
	void addFront(GuiElement *e);
	void addFront(const std::vector<GuiElement*> &e_list);
	//Adds an element behind all current group elements
	void addBack(GuiElement *e);
	void addBack(const std::vector<GuiElement*> &e_list);
	//Adds an element(s) at the specified index
	void add(GuiElement *e, unsigned int index);
	void add(const std::vector<GuiElement*> &e_list, unsigned int index);

	bool remove(GuiElement *e);			//Remove functions return whether the element was in 
	bool remove(unsigned int index);	//	the group (so if it was removed)

	void clear();						//Removes all members

	bool contains(const GuiElement *e) const;


	GuiElement* front() const;	//Returns the GUI element farthest forward
	GuiElement* back() const;	//Returns the GUI element farthest back

	const std::vector<GuiElement*>* getMembers() const;
	
	//These return whether the event was absorbed
	bool mouseMove(APoint m_pos, AVec d_pos, bool blocked) const;
	bool mouseDown(APoint m_pos, MouseButton b, bool blocked) const;
	bool mouseUp(APoint m_pos, MouseButton b, bool blocked) const;
	bool mouseScroll(APoint m_pos, AVec d_scroll, bool blocked) const;

	void keyDown(KeyCode key);
	void keyUp(KeyCode key);

	void updateResources() const;

	void update(const Time &dt) const;
	void draw(GlInterface &gl) const;

};

#endif	//APOLLO_GUI_GROUP_H