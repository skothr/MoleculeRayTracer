#include "ActiveElement.h"

#include "GuiGroup.h"
#include "VirtualTransform.h"
#include "Container.h"
#include "TransformContainer.h"

/////ACTIVE ELEMENT/////

ActiveElement::ActiveElement(GuiProps flags)
	: enabled(flags.isEnabled()), inFocus(false),
		draggable(flags.isDraggable()), droppable(flags.isDroppable()),
		hardBack(flags.hasHardBack()), absorbScroll(flags.absorbsScroll()),
		uncontained(flags.isUncontained())
{
	setFocus(flags.isFocused());
}

ActiveElement::~ActiveElement()
{ }

bool ActiveElement::mouseMove(APoint m_pos, AVec d_pos, bool blocked)
{
	last_m_pos = m_pos;

	if(enabled && visible && respondToMouse(m_pos))
	{
		bool	overlap = pointInside(m_pos),
				blocking = overlap && hardBack,
				absorbed = blocked,
				direct = false;

		APoint	a_m_pos = m_pos - pos;
		
		//Check if this element is a container (has children)
		Container *ce = dynamic_cast<Container*>(this);
		
		if(ce)
		{
			const GuiGroup			*floating = ce->getFloatingChildren(),
									*body = ce->getBodyChildren();

			if(floating && body)
			{
				TransformContainer *tce = dynamic_cast<TransformContainer*>(this);
				const VirtualTransform *trans = tce ? tce->getTransform() : nullptr;

				APoint v_m_pos = trans ? trans->absoluteToVirtualPoint(a_m_pos) : a_m_pos;
				AVec v_d_pos = trans ? trans->absoluteToVirtualVec(d_pos) : d_pos;

				absorbed |= floating->mouseMove(a_m_pos, d_pos, absorbed);	//STATIC CHILDREN
				absorbed |= body->mouseMove(v_m_pos, v_d_pos, absorbed);	//VIRTUAL CHILDREN
			}
		}

		if(!blocked || cState == CS::DRAGGING)
		{
			direct = !blocked && overlap && !absorbed;
			//blocked |= (direct || absorbed) && hardBack;

			if(direct && droppable && Mouse::isDragging() && Mouse::getDragElement() != this)
			{
				//Mouse is dragging something else over this element
				cState = CS::DROP_HOVERING;
			}
			else
			{
				bool drag = clickSelect;

				switch(cState)
				{
				case CS::NONE:
				case CS::HOVERING:
					//Now hovering
					cState = direct ? CS::HOVERING : CS::NONE;
					drag |= (direct && Mouse::buttonDown(MB::LEFT));	//TEMP
					break;
					
				case CS::DROP_HOVERING:
					cState = direct ? CS::DROP_HOVERING : CS::NONE;
					break;

				case CS::CLICKING:
					//Start dragging
					if(draggable)
						Mouse::startDragging(this);
					cState = CS::DRAGGING;

				case CS::DRAGGING:
					//Keep dragging
					drag = true;
					//direct &= clickSelect;
					break;

				default:
					//Invalid control state??
					break;
				}

				if(drag)
					onDrag(a_m_pos, d_pos, direct);
			}
			
			onMouseMove(a_m_pos, d_pos, direct);
			//TODO: This might be called multiple times per mouse move for some reason...
		}
		else
			cState = CS::NONE;

		return (blocking || (direct && cState != CS::DRAGGING) || absorbed) && hardBack;
	}
	else
		cState = CS::NONE;

	return blocked;
}

bool ActiveElement::mouseDown(APoint m_pos, MouseButton b, bool blocked)
{
	if(enabled && visible && respondToClick(m_pos, b))
	{
		bool	overlap = pointInside(m_pos),
				blocking = hardBack && overlap,
				absorbed = blocked,
				direct = false;

		if(!blocked && (overlap || uncontained))
		{
			APoint a_m_pos = m_pos - pos;

			//Check if this element is a container (has children)
			Container *ce = dynamic_cast<Container*>(this);

			if(ce)
			{
				const GuiGroup			*floating = ce->getFloatingChildren(),
										*body = ce->getBodyChildren();

				if(floating && body)
				{
					TransformContainer *tce = dynamic_cast<TransformContainer*>(this);
					const VirtualTransform *trans = tce ? tce->getTransform() : nullptr;
					APoint v_m_pos = trans ? trans->absoluteToVirtualPoint(a_m_pos) : a_m_pos;

					absorbed |= floating->mouseDown(a_m_pos, b, absorbed);	//STATIC CHILDREN
					absorbed |= body->mouseDown(v_m_pos, b, absorbed);		//VIRTUAL CHILDREN
				}
			}

			//bool	click_this = (absorbed && childEventEcho) || (!absorbed && overlap);

			direct = (!absorbed && overlap);

			clickSelect = direct;
			onMouseDown(a_m_pos, b, direct);

			cState = direct ? CS::CLICKING : CS::NONE;
			setFocus(direct || (uncontained && absorbed));
		}
		else
		{
			setFocus(false);
			clickSelect = false;
		}

		return blocking || direct || absorbed;
	}
	else
	{
		setFocus(false);
		clickSelect = false;
		return blocked;
	}
}

bool ActiveElement::mouseUp(APoint m_pos, MouseButton b, bool blocked)
{
	if(enabled && visible)
	{
		bool	overlap = pointInside(m_pos),
				blocking = hardBack && overlap,
				absorbed = blocked,
				direct = false;
		
		APoint	a_m_pos = m_pos - pos;
		
		//Check if this element is a container (has children)
		Container *ce = dynamic_cast<Container*>(this);

		if(ce)
		{
			const GuiGroup			*floating = ce->getFloatingChildren(),
									*body = ce->getBodyChildren();

			if(floating && body)
			{
				TransformContainer *tce = dynamic_cast<TransformContainer*>(this);
				const VirtualTransform *trans = tce ? tce->getTransform() : nullptr;
				APoint v_m_pos = trans ? trans->absoluteToVirtualPoint(a_m_pos) : a_m_pos;

				absorbed |= floating->mouseUp(a_m_pos, b, absorbed);	//STATIC CHILDREN
				absorbed |= body->mouseUp(v_m_pos, b, absorbed);		//VIRTUAL CHILDREN
			}
		}

		//bool	call_mouse_up = !blocked && (overlap || (uncontained && absorbed)),
		//		defocus = blocked || (!absorbed && !overlap);

		direct = /*!blocked && */overlap && !absorbed && respondToClick(m_pos, b);
				
		if(direct && Mouse::isDragging())
		{
			//Stop dragging
			cState = (overlap || uncontained) ? CS::HOVERING : CS::NONE;

			if(droppable && Mouse::getDragElement() != this && clickSelect)
				onDrop(m_pos, Mouse::getDragElement());

			Mouse::stopDragging();
		}
		else
		{
			onMouseUp(m_pos, b, direct);
			cState = direct ? CS::HOVERING : CS::NONE;
		}

		//setFocus(direct);
		clickSelect = false;

		return blocking || absorbed || (direct && hardBack);
	}
	else
		return blocked;
}

bool ActiveElement::mouseScroll(APoint m_pos, AVec d_scroll, bool blocked)
{
	APoint a_m_pos = m_pos - pos;

	if(enabled && visible && respondToMouse(m_pos))
	{
		bool	overlap = pointInside(m_pos),
				blocking = absorbScroll && overlap,
				absorbed = blocked;

		if(!blocked && (overlap || uncontained))
		{
			//Check if this element is a container (has children)
			Container *ce = dynamic_cast<Container*>(this);

			if(ce)
			{
				const GuiGroup			*floating = ce->getFloatingChildren(),
										*body = ce->getBodyChildren();
				
				if(floating && body)
				{
					TransformContainer *tce = dynamic_cast<TransformContainer*>(this);
					const VirtualTransform *trans = tce ? tce->getTransform() : nullptr;
					APoint v_m_pos = trans ? trans->absoluteToVirtualPoint(a_m_pos) : a_m_pos;

					absorbed |= floating->mouseScroll(a_m_pos, d_scroll, absorbed);	//STATIC CHILDREN
					absorbed |= body->mouseScroll(v_m_pos, d_scroll, absorbed);		//VIRTUAL CHILDREN
				}
			}

			bool direct = overlap && !absorbed;

			onScroll(a_m_pos, d_scroll, direct);
			//setFocus(direct);
		}
		else
		{
			//setFocus(false);
		}

		return blocking || absorbed;
	}
	else
		return blocked;
}

void ActiveElement::onPosChanged(AVec d_pos)
{
	//mouseMove(last_m_pos, -d_pos, false);
}

void ActiveElement::onSizeChanged(AVec d_size)
{
	//mouseMove(last_m_pos, AVec(), false);
}

void ActiveElement::drawBackground(GlInterface &gl)
{
	setBgColor(bgStateColors[cState]);
	GuiElement::drawBackground(gl);
	/*
	if(inFocus)
	{
		gl.setColor(Color(1.0f, 0.0f, 0.0f, 1.0f));
		gl.drawLine(APoint(0.5f, 0.5f),						APoint(0.5f, size.y - 0.5f));
		gl.drawLine(APoint(0.5f, size.y - 0.5f),			APoint(size.x - 0.5f, size.y - 0.5f));
		gl.drawLine(APoint(size.x - 0.5f, size.y - 0.5f),	APoint(size.x - 0.5f, 0.5f));
		gl.drawLine(APoint(size.x - 0.5f, 0.5f),			APoint(0.5f, 0.5f));
	}
	*/
}

void ActiveElement::setDefaultBg()
{
	setAllBgStateColors(Color(0.15f, 0.15f, 0.15f, 1.0f));
	setBgStateColor(Color(1.0f, 0.0f, 1.0f, 1.0f), CS::INVALID);
}

void ActiveElement::setFocus(bool focused)
{
	if(inFocus != focused)
	{
		inFocus = focused;
		
		if(inFocus)
			onGainedFocus();
		else
			onLostFocus();
	}
	//Else do nothing -- already correct focus
}

bool ActiveElement::isInFocus() const
{
	return inFocus;
}

void ActiveElement::setBgStateColor(Color col, ControlState state)
{
	bgStateColors[state] = col;
}

void ActiveElement::setAllBgStateColors(Color col)
{
	for(unsigned int i = 0; i < CS::COUNT; i++)
		bgStateColors[i] = col;
}

void ActiveElement::setFgStateColor(Color col, ControlState state)
{
	fgStateColors[state] = col;
}

void ActiveElement::setAllFgStateColors(Color col)
{
	for(unsigned int i = 0; i < CS::COUNT; i++)
		fgStateColors[i] = col;
}

void ActiveElement::update(const Time &dt)
{
	bgColor = bgStateColors[cState];
}