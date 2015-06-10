#include "ScrollArea.h"

#include "ScrollBar.h"
#include "Keyboard.h"

/////SCROLL AREA/////
float		ScrollArea::SCROLLBAR_WIDTH = 17.0f,
			ScrollArea::FRAME_THICKNESS = 2.0f,
			ScrollArea::SCROLL_MULT = 10.0f;
const AVec	ScrollArea::LOWER_PADDING = AVec(5, 5);

const GuiPropFlags ScrollArea::PROP_FLAGS = PFlags::HARD_BACK | PFlags::ABSORB_SCROLL;

ScrollArea::ScrollArea(GuiStateFlags s_flags)
	: GuiElement(),
		TransformContainer(GuiProps(s_flags, PROP_FLAGS))
{
	horizontal = new ScrollBar(this, DEFAULT_STATE_FLOAT, SCROLLBAR_WIDTH, true);
	horizontal->hide();
	horizontal->setOffsetCallback(std::bind(&ScrollArea::onScrollBarChanged, this));

	vertical = new ScrollBar(this, DEFAULT_STATE_FLOAT, SCROLLBAR_WIDTH, false);
	vertical->hide();
	vertical->setOffsetCallback(std::bind(&ScrollArea::onScrollBarChanged, this));

	setMinViewOffset(APoint(0.0f, 0.0f));
	setMinZoom(RVec(1.0f, 1.0f));
	setMaxZoom(RVec(1.0f, 1.0f));
	//setMinZoom(RVec(0.1f, 0.1f));
	//setMaxZoom(RVec(10.0f, 10.0f));

	updateScrollBars();

	setAllBgStateColors(Color(0.1f, 0.1f, 0.1f, 1.0f));
}

ScrollArea::ScrollArea(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags)
	: GuiElement(parent_, a_pos, a_size, GuiProps(s_flags, PROP_FLAGS)),
		TransformContainer(GuiProps(s_flags, PROP_FLAGS))
{
	horizontal = new ScrollBar(this, DEFAULT_STATE_FLOAT, SCROLLBAR_WIDTH, true);
	horizontal->hide();
	horizontal->setOffsetCallback(std::bind(&ScrollArea::onScrollBarChanged, this));

	vertical = new ScrollBar(this, DEFAULT_STATE_FLOAT, SCROLLBAR_WIDTH, false);
	vertical->hide();
	vertical->setOffsetCallback(std::bind(&ScrollArea::onScrollBarChanged, this));

	setMinViewOffset(APoint(0.0f, 0.0f));
	setMinZoom(RVec(1.0f, 1.0f));
	setMaxZoom(RVec(1.0f, 1.0f));
	//setMinZoom(RVec(0.1f, 0.1f));
	//setMaxZoom(RVec(10.0f, 10.0f));

	updateScrollBars();

	setAllBgStateColors(Color(0.1f, 0.1f, 0.1f, 1.0f));
}

ScrollArea::~ScrollArea()
{
	if(horizontal) delete horizontal;
	horizontal = nullptr;

	if(vertical) delete vertical;
	vertical = nullptr;
}

void ScrollArea::updateScrollBars()
{
	if(!horizontal || !vertical)
		return;

	AVec v_max_size(0.0f, 0.0f);
	for(auto e : *bodyChildren.getMembers())
	{
		APoint p = e->getPos();
		AVec s = e->getSize();
		if(p.x + s.x > v_max_size.x)
			v_max_size.x = p.x + s.x;
		if(p.y + s.y > v_max_size.y)
			v_max_size.y = p.y + s.y;
	}
	//AVec	v_size = transform.absoluteToVirtualVec(size),
	//		va_size = transform.absoluteToVirtualVec(size - AVec(SCROLLBAR_WIDTH, SCROLLBAR_WIDTH));

	AVec	a_size = size;

	v_max_size += LOWER_PADDING;

	AVec a_max_size = transform.virtualToAbsoluteVec(v_max_size);

	//Add scrollbars if necessary
	bool	h = a_max_size.x > a_size.x,
			v = a_max_size.y > a_size.y;

	v |= (h && a_max_size.y > a_size.y - SCROLLBAR_WIDTH);
	h |= (v && a_max_size.x > a_size.x - SCROLLBAR_WIDTH);
	
	AVec aa_size = size - AVec(v, h)*SCROLLBAR_WIDTH;


	AVec page_size = transform.absoluteToVirtualVec(aa_size);
	APoint max_offset = APoint(h*(v_max_size.x - page_size.x), v*(v_max_size.y - page_size.y));

	horizontal->setVisible(h);
	vertical->setVisible(v);
	
	//Adjust attachments in case both are visible or not (so they dont overlap)
	horizontal->attachTo(this, AttachSide::RIGHT, (h && v) ? SCROLLBAR_WIDTH : 0.0f);
	vertical->attachTo(this, AttachSide::BOTTOM, (h && v) ? SCROLLBAR_WIDTH : 0.0f);

	horizontal->setMaxOffset(max_offset.x);
	vertical->setMaxOffset(max_offset.y);

	horizontal->setPageSize(page_size.x);
	vertical->setPageSize(page_size.y);

	setMaxViewOffset(max_offset);

	horizontal->updateResources();
	vertical->updateResources();

	//updateResources();
}

void ScrollArea::onMouseMove(APoint m_pos, AVec d_pos, bool direct)
{

}


void ScrollArea::onScroll(APoint m_pos, AVec d_scroll, bool direct)
{
	if(direct)
	{
		bool	h = horizontal->isVisible(),
				v = vertical->isVisible();

		if(Keyboard::keyDown(Keys::K_LALT))
		{
			//Adjust zoom
			AVec a_size = size - AVec(v*SCROLLBAR_WIDTH, h*SCROLLBAR_WIDTH);
			APoint old_center = transform.absoluteToVirtualPoint(a_size*(1.0f/2.0f));
			scaleZoom(RVec(1.0f, 1.0f) - RVec(0.05f, 0.0f)*d_scroll.y);
			updateScrollBars();

			//updateScrollBars();
			APoint new_center = transform.absoluteToVirtualPoint(a_size*(1.0f/2.0f));
		
			//Fix offset (So zooming is centered around view)
			AVec	center_diff = old_center - new_center;
		
			horizontal->setOffset(horizontal->getOffset() + center_diff.x);
			vertical->setOffset(vertical->getOffset() + center_diff.y);
		}
		else
		{
			//SCROLL/OFFSET

			updateScrollBars();

			if(h && !v)
			{
				horizontal->setOffset(horizontal->getOffset() + (d_scroll.x + d_scroll.y)*SCROLL_MULT);
			}
			else if(v && !h)
			{
				vertical->setOffset(vertical->getOffset() + (d_scroll.x + d_scroll.y)*SCROLL_MULT);
			}
			else if(v && h)
			{
				horizontal->setOffset(horizontal->getOffset() + d_scroll.x*SCROLL_MULT);
				vertical->setOffset(vertical->getOffset() + d_scroll.y*SCROLL_MULT);
			}

			//setZoomPivot(APoint(horizontal->getPageSize()*(1.0f/2.0f),
			//					vertical->getPageSize()*(1.0f/2.0f)));
		}
	//std::cout << horizontal->getOffset() << ", " << vertical->getOffset() << "\n";
	}
}

void ScrollArea::onDrag(APoint m_pos, AVec d_pos, bool direct)
{
	if(direct)
		setViewOffset(AVec(horizontal->getOffset(), vertical->getOffset()));
}


void ScrollArea::onSizeChanged(AVec d_size)
{
	updateScrollBars();
}

void ScrollArea::onAddElement()
{
	updateScrollBars();
}

void ScrollArea::onAttach(GuiAttachment &attachment)
{
	//Add some space for any scrollbars that are visible
	if(attachment.child &&
		((vertical && vertical->isVisible() && attachment.side == AttachSide::RIGHT) ||
		(horizontal && horizontal->isVisible() && attachment.side == AttachSide::BOTTOM)))
		attachment.offset += SCROLLBAR_WIDTH;
}

void ScrollArea::onScrollBarChanged()
{
	setViewOffset(AVec(horizontal->getOffset(), vertical->getOffset()));
}

void ScrollArea::draw(GlInterface &gl)
{
	if(isolateViewport(gl))
	{
		drawBackground(gl);
		drawChildren(gl);

		if(vertical->isVisible() && horizontal->isVisible())
		{
			//Draw square in space between scroll bars
			AVec rect_size(SCROLLBAR_WIDTH, SCROLLBAR_WIDTH);

			gl.setColor(bgStateColors[cState]);
			gl.drawRect(APoint(size) - rect_size, rect_size);
		}

		restoreViewport(gl);
	}
}