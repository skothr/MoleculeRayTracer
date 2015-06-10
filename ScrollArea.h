#ifndef APOLLO_SCROLL_AREA_H
#define APOLLO_SCROLL_AREA_H

#include "GuiElement.h"
#include "TransformContainer.h"

class ScrollBar;

class ScrollArea : public TransformContainer
{
protected:
	ScrollArea(GuiStateFlags s_flags);

	ScrollBar			*horizontal = nullptr,
						*vertical = nullptr;

	APoint last_center;

	void updateScrollBars();

	virtual void onMouseMove(APoint m_pos, AVec d_pos, bool direct) override;
	virtual void onScroll(APoint m_pos, AVec d_scroll, bool direct) override;
	virtual void onDrag(APoint m_pos, AVec d_pos, bool direct) override;

	virtual void onSizeChanged(AVec d_size) override;
	virtual void onAddElement() override;
	virtual void onAttach(GuiAttachment &attachment) override;

public:
	static float		SCROLLBAR_WIDTH,
						FRAME_THICKNESS,
						SCROLL_MULT;
	static const AVec	LOWER_PADDING;

	ScrollArea(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags);
	virtual ~ScrollArea();

	static const GuiPropFlags PROP_FLAGS;

	void onScrollBarChanged();

	virtual void draw(GlInterface &gl) override;
};

#endif	//APOLLO_SCROLL_AREA_H