#include "ScrollBar.h"

#include "TransformContainer.h"

/////SCROLL BAR/////

const float ScrollBar::DEFAULT_WIDTH = 18.0f,
			ScrollBar::ARROW_LENGTH = 18.0f,
			ScrollBar::HANDLE_PADDING = 4.0f,
			ScrollBar::OFFSET_SINGLESTEP = 10.0f;

const GuiPropFlags ScrollBar::PROP_FLAGS = PFlags::HARD_BACK | PFlags::ABSORB_SCROLL;

ScrollBar::ScrollBar(ParentElement *parent_, GuiStateFlags s_flags, float bar_width, bool horiz)
	: GuiElement(parent_, APoint(), AVec(), GuiProps(s_flags, PROP_FLAGS)),
		CompoundControl(GuiProps(s_flags, PROP_FLAGS)),
		width(bar_width <= 0 ? DEFAULT_WIDTH : bar_width), horizontal(horiz)
{
	AVec	parent_size = parent->getSize(),
			a_size = horizontal ? AVec(parent_size.x, width) : AVec(width, parent_size.y);
	APoint	a_pos = horizontal ? APoint(0.0f, parent_size.y - width) : APoint(parent_size.x - width, 0.0f);
	
	pageSize = horizontal ? parent_size.x : parent_size.y;

	setPos(a_pos);
	setSize(a_size);

	float	handle_length = 0,
			handle_pos = ARROW_LENGTH + HANDLE_PADDING;

	GuiStateFlags child_state = DEFAULT_STATE_FLOAT;

	if(horiz)
	{
		up = new BaseButton(this, APoint(0.0f, 0.0f), AVec(ARROW_LENGTH, width), child_state, "");
		down = new BaseButton(this, APoint(a_size.x - ARROW_LENGTH, 0), AVec(ARROW_LENGTH, width), child_state, "");
		handle = new ScrollHandle(this, APoint(handle_pos, HANDLE_PADDING), child_state, width - 2.0f*HANDLE_PADDING, handle_length, horizontal);
		
		//Attach to the bottom side of parent
		attachTo(parent, Side::BOTTOM, 0.0f);
		attachTo(parent, Side::LEFT, 0.0f);
		attachTo(parent, Side::RIGHT, 0.0f);
	}
	else //Vertical
	{
		up = new BaseButton(this, APoint(0.0f, 0.0f), AVec(width, ARROW_LENGTH), child_state, "");
		down = new BaseButton(this, APoint(0.0f, a_size.y - ARROW_LENGTH), AVec(width, ARROW_LENGTH), child_state, "");
		handle = new ScrollHandle(this, APoint(HANDLE_PADDING, handle_pos), child_state, width - 2.0f*HANDLE_PADDING, handle_length, horizontal);
		
		//Attach to the right side of parent
		attachTo(parent, Side::TOP, 0.0f);
		attachTo(parent, Side::RIGHT, 0.0f);
		attachTo(parent, Side::BOTTOM, 0.0f);
	}

	up->setClickFunction(std::bind(&ScrollBar::decrementOffset, this));
	down->setClickFunction(std::bind(&ScrollBar::incrementOffset, this));

	//Initialize Compound Control
	std::vector<GuiElement*> children {down, up, handle};
	CompoundControl::init(&children, nullptr);
	
	setAllBgStateColors(Color(0.2f, 0.2f, 0.2f, 1.0f));
}

ScrollBar::ScrollBar(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags, bool horiz)
	: GuiElement(parent_, a_pos, a_size, GuiProps(s_flags, PROP_FLAGS)),
		CompoundControl(GuiProps(s_flags, PROP_FLAGS)),
		width(horiz ? a_size.y : a_size.x), horizontal(horiz)
{
	AVec parent_size = parent->getSize();
	pageSize = horizontal ? parent_size.x : parent_size.y;
	
	float	handle_length = 0,
			handle_pos = ARROW_LENGTH + HANDLE_PADDING;
	
	GuiStateFlags child_state = DEFAULT_STATE_FLOAT;

	if(horiz)
	{
		up = new BaseButton(this, APoint(0, 0), AVec(ARROW_LENGTH, width), child_state, "");
		down = new BaseButton(this, APoint(a_size.x - ARROW_LENGTH, 0), AVec(ARROW_LENGTH, width), child_state, "");
		handle = new ScrollHandle(this, APoint(handle_pos, HANDLE_PADDING), child_state, width - 2.0f*HANDLE_PADDING, handle_length, horizontal);
	}
	else //Vertical
	{
		up = new BaseButton(this, APoint(0, 0), AVec(width, ARROW_LENGTH), child_state, "");
		down = new BaseButton(this, APoint(0, a_size.y - ARROW_LENGTH), AVec(width, ARROW_LENGTH), child_state, "");
		handle = new ScrollHandle(this, APoint(HANDLE_PADDING, handle_pos), child_state, width - 2.0f*HANDLE_PADDING, handle_length, horizontal);
	}

	up->setClickFunction(std::bind(&ScrollBar::decrementOffset, this));
	down->setClickFunction(std::bind(&ScrollBar::incrementOffset, this));
	
	//Initialize Compound Control
	std::vector<GuiElement*> children {down, up, handle};
	CompoundControl::init(&children, nullptr);
	
	setAllBgStateColors(Color(0.2f, 0.2f, 0.2f, 1.0f));
}

ScrollBar::~ScrollBar()
{
	if(up) delete up;
	up = nullptr;

	if(down) delete down;
	down = nullptr;

	if(handle) delete handle;
	handle = nullptr;
}

void ScrollBar::updateHandleLength()
{
	if(handle)
	{
		float	handle_area_length = (horizontal ? size.x : size.y) - 2*ARROW_LENGTH - 2*HANDLE_PADDING,
				handle_length = handle_area_length*pageSize/(maxOffset + pageSize);
	
		if(horizontal)
			handle->setWidth(handle_length);
		else
			handle->setHeight(handle_length);
	}
}

void ScrollBar::updateHandlePos()
{
	if(handle)
	{
		float	handle_area_length = (horizontal ? size.x : size.y) - 2*ARROW_LENGTH - 2*HANDLE_PADDING,
				handle_offset = handle_area_length*(offset/(maxOffset + pageSize));

		if(horizontal)
			handle->setX(ARROW_LENGTH + HANDLE_PADDING + handle_offset);
		else
			handle->setY(ARROW_LENGTH + HANDLE_PADDING + handle_offset);
	}
}

void ScrollBar::onSizeChanged(AVec d_size)
{
	updateHandleLength();
	updateHandlePos();

	if(down)
	{
		if(horizontal)
			down->setPos(APoint(size.x - ARROW_LENGTH, 0.0f));
		else
			down->setPos(APoint(0.0f, size.y - ARROW_LENGTH));
	}
}

void ScrollBar::setPageSize(float new_page_size)
{
	pageSize = new_page_size;
	updateHandleLength();
}

float ScrollBar::getPageSize() const
{
	return pageSize;
}

void ScrollBar::setOffset(float new_offset)
{
	if(new_offset < 0)
		new_offset = 0;
	else if(new_offset > maxOffset)
		new_offset = maxOffset;

	offset = new_offset;
	if(offsetCallback) offsetCallback();

	updateHandlePos();
}

void ScrollBar::moveOffset(float d_offset)
{
	setOffset(offset + d_offset);
}

float ScrollBar::getOffset() const
{
	return offset;
}


void ScrollBar::setOffsetCallback(voidCallback callback)
{
	offsetCallback = callback;
}


void ScrollBar::setMaxOffset(float new_max_offset)
{
	maxOffset = new_max_offset >= 0.0f ? new_max_offset : 0.0f;
	
	if(offset > maxOffset)
		setOffset(maxOffset);
	
	updateHandleLength();
}

float ScrollBar::getMaxOffset() const
{
	return maxOffset;
}


void ScrollBar::incrementOffset()
{
	moveOffset(OFFSET_SINGLESTEP);
}

void ScrollBar::decrementOffset()
{
	moveOffset(-OFFSET_SINGLESTEP);
}

void ScrollBar::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		drawBackground(gl);
		drawChildren(gl);

		restoreViewport(gl);
	}

	//std::cout << offset << "\n";
}


/////SCROLL HANDLE/////
const GuiPropFlags ScrollHandle::PROP_FLAGS = PFlags::HARD_BACK;

ScrollHandle::ScrollHandle(ScrollBar *parent_, APoint a_pos, GuiStateFlags s_flags, float width, float length, bool horiz)
	: GuiElement(parent_, a_pos, (horiz ? AVec(length, width) : AVec(width, length)), GuiProps(s_flags, PROP_FLAGS)),
		Control(GuiProps(s_flags, PROP_FLAGS)),
		sb_parent(parent_), horizontal(horiz)
{
	setAllBgStateColors(Color(0.5f, 0.5f, 0.5f, 1.0f));
	setBgStateColor(Color(0.7f, 0.7f, 0.7f, 1.0f), CS::HOVERING);
	setBgStateColor(Color(0.9f, 0.9f, 0.9f, 1.0f), CS::CLICKING);
	setBgStateColor(Color(1.0f, 0.7f, 0.7f, 1.0f), CS::DRAGGING);
}

ScrollHandle::~ScrollHandle()
{ }

void ScrollHandle::onMouseDown(APoint m_pos, MouseButton b, bool direct)
{
	if(direct && valid(b & MB::LEFT))
		guidePoint = horizontal ? m_pos.x : m_pos.y;
}

void ScrollHandle::onMouseUp(APoint m_pos, MouseButton b, bool direct)
{
	if(direct && valid(b & MB::LEFT))// && pointInside(m_pos))
		guidePoint = -1.0f;
}

void ScrollHandle::onDrag(APoint m_pos, AVec d_pos, bool direct)
{
	if(guidePoint >= 0)
	{
		float	page_size = sb_parent->getPageSize(),
				guide_vec = (horizontal ? m_pos.x : m_pos.y) - guidePoint;
		
		sb_parent->moveOffset(page_size*guide_vec/(horizontal ? size.x : size.y));
	}
}

void ScrollHandle::onMouseMove(APoint m_pos, AVec d_pos, bool direct)
{
	//std::cout << "SCROLL HANDLE MOUSE MOVE!! " << cState << "\n";
}

void ScrollHandle::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		drawBackground(gl);
		restoreViewport(gl);
	}
}