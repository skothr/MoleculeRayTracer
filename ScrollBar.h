#ifndef APOLLO_SCROLLBAR_H
#define APOLLO_SCROLLBAR_H

#include "GuiElement.h"
#include "ParentElement.h"
#include "CompoundControl.h"
#include "Control.h"

#include "Buttons.h"

class ScrollHandle;

//Functions as a scrollbar that can control various 
class ScrollBar : public CompoundControl
{
protected:
	BaseButton		*up = nullptr,
					*down = nullptr;
	ScrollHandle	*handle = nullptr;

	voidCallback	offsetCallback = nullptr;

	float			width,
					maxOffset = 0,
					offset = 0,
					pageSize = 0;

	bool			horizontal = false;
	
	void updateHandleLength();
	void updateHandlePos();

	virtual void onSizeChanged(AVec d_size) override;

public:
	//This constructor aligns the scrollbar on the right or bottom of the parent depending on whether it's horizontal
	ScrollBar(ParentElement *parent_, GuiStateFlags s_flags, float bar_width, bool horiz);
	//This contructor allows it to be placed freely
	ScrollBar(ParentElement *parent_, APoint a_pos, AVec a_size, GuiStateFlags s_flags, bool horiz);
	virtual ~ScrollBar();

	static const GuiPropFlags PROP_FLAGS;

	static const float	DEFAULT_WIDTH,
						ARROW_LENGTH,
						HANDLE_PADDING,
						OFFSET_SINGLESTEP;

	void setPageSize(float new_page_size);
	float getPageSize() const;

	//Offset -- How far away from the top or left the scroll handle is
	void setOffset(float new_offset);
	void moveOffset(float d_offset);
	float getOffset() const;

	void setOffsetCallback(voidCallback callback);

	void setMaxOffset(float new_max_offset);
	float getMaxOffset() const;

	void incrementOffset();
	void decrementOffset();

	virtual void draw(GlInterface &gl) override;

};


class ScrollHandle : public Control
{
private:
	ScrollBar	*sb_parent = nullptr;
	bool		horizontal = false;
	float		guidePoint = -1.0f;		// >= 0.0 when dragging handle --> prevents handle from going past edges

protected:
	virtual void onMouseDown(APoint m_pos, MouseButton b, bool direct) override;
	virtual void onMouseUp(APoint m_pos, MouseButton b, bool direct) override;
	virtual void onDrag(APoint m_pos, AVec d_pos, bool direct) override;

	virtual void onMouseMove(APoint m_pos, AVec d_pos, bool direct) override;

public:
	ScrollHandle(ScrollBar *parent_, APoint a_pos, GuiStateFlags s_flags, float width, float length, bool horiz);
	virtual ~ScrollHandle();

	static const GuiPropFlags PROP_FLAGS;

	virtual void draw(GlInterface &gl) override;
};

#endif	//APOLLO_SCROLLBAR_H