#ifndef APOLLO_COLLAPSE_ELEMENT_H
#define APOLLO_COLLAPSE_ELEMENT_H

#include "Container.h"
#include "Control.h"

#include <string>

class CollapseList;
class CollapseElement;
class CollapseButton;

struct ElementTree
{
	GuiElement						*element = nullptr;
	CollapseList					*childList = nullptr;

	std::vector<ElementTree>		children;
	bool	collapsed,
			needElementDelete;

	//Make any GuiElement act as the title of this tree
	ElementTree(GuiElement *this_element = nullptr, bool is_collapsed = true);
	//Create a label to be the title of this tree
	ElementTree(const std::string &text, bool is_collapsed = true);

	ElementTree(const ElementTree &other);
	virtual ~ElementTree();

	ElementTree& operator=(const ElementTree &other);

	void addChild(const ElementTree &child);

	void setCollapsed(bool is_collapsed);
	void toggleCollapse();
	void collapse();
	void expand();
};


class CollapseList : public Container
{
protected:
	std::vector<ElementTree> trees;
	std::vector<CollapseElement*> innerTrees;

	virtual void onSizeChanged(AVec d_size) override;

	float child_offset;

public:
	CollapseList(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags, const std::vector<ElementTree> &collapse_trees);
	virtual ~CollapseList();

	static const GuiPropFlags	PROP_FLAGS;
	static const AVec			PADDING,
								CHILD_OFFSET;

	void updateSize(bool propogate_parent);

	void setTrees(const std::vector<ElementTree> &collapse_trees);
	void addTree(ElementTree new_tree);
	void removeTree(unsigned int idex);

	void setOffset(float new_offset);

	std::vector<float> getElementHeights() const;

	virtual void draw(GlInterface &gl) override;

	friend class CollapseElement;
};


class CollapseElement : public Container
{
protected:
	ElementTree		tree;
	CollapseButton	*button = nullptr;
	CollapseList	*innerList = nullptr;
	
	float			child_offset;

	virtual void onSizeChanged(AVec d_size) override;
	
	CollapseList* getTopList();

public:
	CollapseElement(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags, const ElementTree &expand_tree);
	virtual ~CollapseElement();

	static const GuiPropFlags	PROP_FLAGS;
	static const AVec			PADDING,
								BUTTON_SIZE,
								CHILD_OFFSET;	//Offset between children
	static const float			LINE_OFFSET;	//Offset between children and connecting lines

	void updateSize(bool propogate_parent);

	void setOffset(float new_offset);

	void setTree(const ElementTree &expand_tree);

	virtual void draw(GlInterface &gl) override;
	
	friend class CollapseList;
};


class CollapseButton : public Control
{
protected:
	ElementTree		*collapseTree;
	CollapseElement *ce_parent;

	virtual void onMouseDown(APoint m_pos, MouseButton b, bool direct) override;

public:
	CollapseButton(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags, ElementTree *collapse_tree);
	virtual ~CollapseButton();

	static const GuiPropFlags PROP_FLAGS;

	virtual void draw(GlInterface &gl) override;
};


#endif	//APOLLO_COLLAPSE_ELEMENT_H