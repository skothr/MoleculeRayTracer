#include "CollapseList.h"

#include "Label.h"

/////ELEMENT TREE/////

ElementTree::ElementTree(GuiElement *this_element, bool is_collapsed)
	: element(this_element), collapsed(is_collapsed), needElementDelete(false)
{ }

ElementTree::ElementTree(const std::string &text, bool is_collapsed)
	: collapsed(is_collapsed), needElementDelete(true),
		element(new Label(dynamic_cast<ParentElement*>(this), APoint(0.0f, 0.0f), (SFlags::ENABLED | SFlags::FLOATING | (is_collapsed ? SFlags::NONE : SFlags::VISIBLE)), text, 14))
{ }

ElementTree::ElementTree(const ElementTree &other)
	: element(other.element), collapsed(other.collapsed), needElementDelete(other.needElementDelete)
{
	children.reserve(other.children.size());
	children.insert(children.end(), other.children.begin(), other.children.end());

	childList = other.childList;

	if(needElementDelete)
	{
		//Make new label
		Label *other_l = dynamic_cast<Label*>(other.element);
		if(other_l)
			element = new Label(dynamic_cast<ParentElement*>(this), other_l->getPos(), SFlags::ENABLED | (other_l->isVisible() ? SFlags::VISIBLE : SFlags::NONE), other_l->getText(), 14);
		else
			needElementDelete = false;
	}

}

ElementTree::~ElementTree()
{
	if(element && needElementDelete)
		delete element;
	element = nullptr;
}

ElementTree& ElementTree::operator=(const ElementTree &other)
{
	element = other.element;
	collapsed = other.collapsed;
	needElementDelete = other.needElementDelete;

	children.reserve(other.children.size());
	children.insert(children.end(), other.children.begin(), other.children.end());
	
	childList = other.childList;
	
	if(needElementDelete)
	{
		//Make new label
		Label *other_l = dynamic_cast<Label*>(other.element);
		if(other_l)
			element = new Label(dynamic_cast<ParentElement*>(this), other_l->getPos(), SFlags::ENABLED | SFlags::FLOATING | (other_l->isVisible() ? SFlags::VISIBLE : SFlags::NONE), other_l->getText(), 14);
		else
			needElementDelete = false;
	}

	return *this;
}

void ElementTree::addChild(const ElementTree &child)
{
	children.push_back(child);
}

void ElementTree::setCollapsed(bool is_collapsed)
{
	if(collapsed != is_collapsed)
	{
		collapsed = is_collapsed;

		//if(element)
		//	element->setVisible(!collapsed);

		if(childList)
		{
			childList->setVisible(!collapsed);
			childList->updateSize(true);
		}
	}
}

void ElementTree::toggleCollapse()
{
	setCollapsed(!collapsed);
}

void ElementTree::collapse()
{
	setCollapsed(true);
}

void ElementTree::expand()
{
	setCollapsed(false);
}


/////COLLAPSE LIST/////
const GuiPropFlags	CollapseList::PROP_FLAGS	= PFlags::HARD_BACK;
const AVec			CollapseList::PADDING		= AVec(2.0f, 2.0f),//AVec(5.0f, 5.0f),
					CollapseList::CHILD_OFFSET	= AVec(2.0f, 2.0f);//AVec(10.0f, 3.0f);

CollapseList::CollapseList(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags, const std::vector<ElementTree> &collapse_trees)
	: GuiElement(parent_, a_pos, a_size, GuiProps(s_flags, PROP_FLAGS)),
		Container(GuiProps(s_flags, PROP_FLAGS)),
		trees(collapse_trees)
{
	setAllBgStateColors(Color(0.25f, 1.0f, 0.25f, 1.0f));

	CollapseElement *last_ce = nullptr;

	for(auto t : trees)
	{
		CollapseElement *ce = new CollapseElement(this, PADDING, AVec(size.x - 2.0f*PADDING.x, 0.0f), DEFAULT_STATE_FLOAT, t);
		ce->setOffset(child_offset - ce->getPos().x);
		innerTrees.push_back(ce);

		if(last_ce)
			ce->attachTo(last_ce, Side::TOP, CHILD_OFFSET.y);
		else
			ce->attachTo(this, Side::TOP, PADDING.y);
		
		ce->updateSize(false);

		last_ce = ce;
	}

	updateSize(false);
}

CollapseList::~CollapseList()
{
	for(auto it : innerTrees)
		if(it) delete it;
	innerTrees.clear();
}

void CollapseList::onSizeChanged(AVec d_size)
{
	CollapseElement *ce_parent = dynamic_cast<CollapseElement*>(parent);

	if(ce_parent)
		ce_parent->updateSize(true);
}

void CollapseList::updateSize(bool propogate_parent)
{
	AVec new_size(0.0f, 0.0f);
	
	for(auto it : innerTrees)
	{
		it->updateSize(false);

		APoint max_p = it->getPos() + it->getSize();
		new_size.x = max(new_size.x, max_p.x);
		new_size.y = max(new_size.y, max_p.y);
	}
	
	new_size += PADDING;

	if(propogate_parent)
		setSize(new_size);
	else
	{
		size = new_size;
		resolveAttachments();
	}
}


void CollapseList::setOffset(float new_offset)
{
	child_offset = new_offset;

	for(auto it : innerTrees)
		it->setOffset(child_offset - it->getPos().x);
}


void CollapseList::setTrees(const std::vector<ElementTree> &collapse_trees)
{
	for(auto it : innerTrees)
		if(it) delete it;
	innerTrees.clear();

	trees.clear();
	trees.reserve(collapse_trees.size());
	trees.insert(trees.end(), collapse_trees.begin(), collapse_trees.end());
	
	CollapseElement *last_ce = nullptr;
	for(auto t : trees)
	{
		CollapseElement *ce = new CollapseElement(this, PADDING, AVec(size.x - 2.0f*PADDING.x, 0.0f), DEFAULT_STATE_FLOAT, t);
		ce->setOffset(child_offset - ce->getPos().x);
		innerTrees.push_back(ce);

		if(last_ce)
			ce->attachTo(last_ce, Side::TOP, CHILD_OFFSET.y);
		else
			ce->attachTo(this, Side::TOP, PADDING.y);
		
		ce->updateSize(false);

		last_ce = ce;
	}

	updateSize(true);
}

void CollapseList::addTree(ElementTree new_tree)
{
	trees.push_back(new_tree);

	CollapseElement *ce = new CollapseElement(this, PADDING, AVec(size.x - 2.0f*PADDING.x, 0.0f), DEFAULT_STATE_FLOAT, new_tree);
	ce->setOffset(child_offset - ce->getPos().x);
	innerTrees.push_back(ce);

	if(innerTrees.size() > 1)
		ce->attachTo(innerTrees[innerTrees.size() - 2], Side::TOP, CHILD_OFFSET.y);
	else
		ce->attachTo(this, Side::TOP, PADDING.y);
	
	ce->updateSize(true);
}

void CollapseList::removeTree(unsigned int index)
{
	trees.erase(trees.begin() + index);

	if(innerTrees[index])
	{
		removeChild(innerTrees[index], true);
		delete innerTrees[index];
	}
	innerTrees.erase(innerTrees.begin() + index);

	updateSize(true);
}

std::vector<float> CollapseList::getElementHeights() const
{
	std::vector<float> heights;

	heights.reserve(innerTrees.size());

	for(auto it : innerTrees)
		heights.push_back(it->getPos().y);

	return heights;
}


void CollapseList::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		drawBackground(gl);
		drawChildren(gl);

		restoreViewport(gl);
	}
}



/////COLLAPSE ELEMENT/////
const GuiPropFlags	CollapseElement::PROP_FLAGS		= PFlags::HARD_BACK;
const AVec			CollapseElement::PADDING		= AVec(5.0f, 5.0f),
					CollapseElement::BUTTON_SIZE	= AVec(10.0f, 10.0f),
					CollapseElement::CHILD_OFFSET	= AVec(5.0f, 5.0f);
const float			CollapseElement::LINE_OFFSET	= 2.0f;

CollapseElement::CollapseElement(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags, const ElementTree &expand_tree)
	: GuiElement(parent_, a_pos, a_size, GuiProps(s_flags, PROP_FLAGS)),
		Container(GuiProps(s_flags, PROP_FLAGS)),
		tree(expand_tree)
{
	setAllBgStateColors(Color(1.0f, 0.1f, 0.1f, 1.0f));

	bool has_inner_list = tree.children.size() > 0;
	
	APoint offset = APoint(PADDING) + AVec(BUTTON_SIZE.x + CHILD_OFFSET.x, 0.0f);//PADDING.x + (has_inner_list ? BUTTON_SIZE.x : 0.0f), PADDING.y);

	if(tree.element)
	{
		//offset.x += CHILD_OFFSET.x;

		tree.element->setParent(this, true, true);
		tree.element->setPos(offset);

		tree.element->setVisible(true);//!tree.collapsed);

		offset.y += tree.element->getSize().y + CHILD_OFFSET.y;
		offset.x += CHILD_OFFSET.x;
	}
	else
		offset.y += BUTTON_SIZE.y + CHILD_OFFSET.y;

	offset.x += LINE_OFFSET;

	if(has_inner_list)
	{
		GuiStateFlags inner_flags = SFlags::ENABLED | SFlags::FLOATING;
		inner_flags |= (tree.collapsed ? SFlags::NONE : SFlags::VISIBLE);

		innerList = new CollapseList(this, offset, AVec(size.x - 2.0f*PADDING.x, 0.0f), inner_flags, tree.children);
		innerList->setOffset(child_offset - innerList->getPos().x);
		
		tree.childList = innerList;

		if(tree.element)
			innerList->attachTo(tree.element, AttachSide::TOP, CHILD_OFFSET.y);
		else
			innerList->attachTo(this, AttachSide::TOP, offset.y);

		innerList->updateSize(false);
		
		button = new CollapseButton(this, APoint(PADDING), BUTTON_SIZE, DEFAULT_STATE_FLOAT, &tree);
	}

	updateSize(false);
}

CollapseElement::~CollapseElement()
{
	if(button) delete button;
	button = nullptr;

	if(innerList) delete innerList;
	innerList = nullptr;
}

void CollapseElement::onSizeChanged(AVec d_size)
{
	CollapseList *cl_parent = dynamic_cast<CollapseList*>(parent);

	if(cl_parent)
		cl_parent->updateSize(true);
}

void CollapseElement::updateSize(bool propogate_parent)
{
	AVec	new_size(0.0f, 0.0f);
	
	if(tree.element)
	{
		APoint max_p = tree.element->getPos() + tree.element->getSize();
		new_size.x = max_p.x;
		new_size.y = max_p.y;
	}

	if(innerList && !tree.collapsed)
	{
		APoint max_p = innerList->getPos() + innerList->getSize();
		new_size.x = max(new_size.x, max_p.x);
		new_size.y = max(new_size.y, max_p.y);
	}

	new_size += PADDING;
	
	if(propogate_parent)
		setSize(new_size);
	else
	{
		size = new_size;
		resolveAttachments();
	}
}

void CollapseElement::setOffset(float new_offset)
{
	child_offset = new_offset;

	if(tree.element)
		tree.element->setX(child_offset);

	if(innerList)
		innerList->setOffset(child_offset - innerList->getPos().x);

}

void CollapseElement::setTree(const ElementTree &expand_tree)
{
	tree = expand_tree;

	//TODO: Update innerList and size
}


void CollapseElement::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		drawBackground(gl);
		drawChildren(gl);

		//Draw child lines
		gl.setColor(Color(0.6f, 0.6f, 0.6f, 1.0f));
		
		float	main_x = PADDING.x + BUTTON_SIZE.x*(1.0f/2.0f),
				hor_x = PADDING.x + BUTTON_SIZE.x,
				inner_x = PADDING.x + BUTTON_SIZE.x + CHILD_OFFSET.x - LINE_OFFSET,
				inner_child_x = inner_x + PADDING.x + CHILD_OFFSET.x;

		if(innerList)
		{
			if(!tree.collapsed)
			{
				std::vector<float> element_ys = innerList->getElementHeights();

				float	max_y = 0.0f,
						y_offset = PADDING.y*2.0f + (tree.element ? tree.element->getSize().y + CHILD_OFFSET.y : BUTTON_SIZE.y)
									+ BUTTON_SIZE.y*(1.0f/2.0f);

				for(auto y : element_ys)
				{
					float hor_y = y_offset + y;
					max_y = max(max_y, hor_y);
					gl.drawLine(APoint(main_x, hor_y), APoint(inner_child_x, hor_y));
				}

				gl.drawLine(APoint(main_x, PADDING.y + BUTTON_SIZE.y + 2.0f), APoint(main_x, max_y));
			}
			
			
			if(tree.element)
			{
				float top_y = PADDING.y + BUTTON_SIZE.y*(1.0f/2.0f);
				gl.drawLine(APoint(hor_x, top_y), APoint(inner_x, top_y));
			}
						
		}


		restoreViewport(gl);
	}
}


/////EXPAND BUTTON/////
const GuiPropFlags	CollapseButton::PROP_FLAGS = PFlags::NONE;

CollapseButton::CollapseButton(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags, ElementTree *collapse_tree)
	: GuiElement(parent_, a_pos, a_size, GuiProps(s_flags, PROP_FLAGS)),
		Control(GuiProps(s_flags, PROP_FLAGS)),
		collapseTree(collapse_tree), ce_parent(dynamic_cast<CollapseElement*>(parent_))
{

}

CollapseButton::~CollapseButton()
{ }

void CollapseButton::onMouseDown(APoint m_pos, MouseButton b, bool direct)
{
	if(direct && collapseTree)
	{
		collapseTree->toggleCollapse();
		//if(ce_parent) ce_parent->updateSize();
	}
}

void CollapseButton::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		gl.setColor(Color(0.6f, 0.6f, 0.6f, 1.0f));

		//Draw outline
		gl.drawLine(APoint(0.0f, 0.0f), APoint(size.x, 0.0f));
		gl.drawLine(APoint(size.x, 0.0f), APoint(size.x, size.y));
		gl.drawLine(APoint(size.x, size.y), APoint(0.0f, size.y));
		gl.drawLine(APoint(0.0f, size.y), APoint(0.0f, 0.0f));

		//Draw plus/minus//

		//Horizontal line
		gl.drawLine(APoint(2.0f, size.y*(1.0f/2.0f)), APoint(size.x - 2.0f, size.y*(1.0f/2.0f)));
		
		//Vertical line
		if(collapseTree && collapseTree->collapsed)
			gl.drawLine(APoint(size.x*(1.0f/2.0f), 2.0f), APoint(size.x*(1.0f/2.0f), size.y - 2.0f));

		restoreViewport(gl);
	}
}