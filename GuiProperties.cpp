#include "GuiProperties.h"

/////GUI PROPERTIES/////

GuiProperties::GuiProperties()
	: stateFlags(SFlags::NONE), propFlags(PFlags::NONE)
{ }

GuiProperties::GuiProperties(GuiStateFlags s_flags, GuiPropFlags p_flags)
	: stateFlags(s_flags), propFlags(p_flags)
{ }


//STATIC CHECKING
bool GuiProperties::checkFlag(GuiStateFlags s_flags, GuiStateFlags check)
{
	return static_cast<bool>(s_flags & check);
}

bool GuiProperties::checkFlag(GuiPropFlags p_flags, GuiPropFlags check)
{
	return static_cast<bool>(p_flags & check);
}

bool GuiProperties::isVisible(GuiStateFlags s_flags)
{
	return checkFlag(s_flags, SFlags::VISIBLE);
}

bool GuiProperties::isEnabled(GuiStateFlags s_flags)
{
	return checkFlag(s_flags, SFlags::ENABLED);
}

bool GuiProperties::isFocused(GuiStateFlags s_flags)
{
	return checkFlag(s_flags, SFlags::FOCUSED);
}

bool GuiProperties::isFloating(GuiStateFlags s_flags)
{
	return checkFlag(s_flags, SFlags::FLOATING);
}


bool GuiProperties::isDraggable(GuiPropFlags p_flags)
{
	return checkFlag(p_flags, PFlags::DRAGGABLE);
}

bool GuiProperties::isDroppable(GuiPropFlags p_flags)
{
	return checkFlag(p_flags, PFlags::DROPPABLE);
}

bool GuiProperties::hasHardBack(GuiPropFlags p_flags)
{
	return checkFlag(p_flags, PFlags::HARD_BACK);
}

bool GuiProperties::absorbsScroll(GuiPropFlags p_flags)
{
	return checkFlag(p_flags, PFlags::ABSORB_SCROLL);
}

bool GuiProperties::echosChildEvents(GuiPropFlags p_flags)
{
	return checkFlag(p_flags, PFlags::CHILD_EVENT_ECHO);
}

bool GuiProperties::isUncontained(GuiPropFlags p_flags)
{
	return checkFlag(p_flags, PFlags::UNCONTAINED);
}


//INDIVIDUAL CHECKING
void GuiProperties::setStateFlags(GuiStateFlags s_flags)
{
	stateFlags = s_flags;
}

GuiStateFlags GuiProperties::getStateFlags() const
{
	return stateFlags;
}

void GuiProperties::setPropFlags(GuiPropFlags p_flags)
{
	propFlags = p_flags;
}

GuiPropFlags GuiProperties::getPropFlags() const
{
	return propFlags;
}

bool GuiProperties::checkFlag(GuiStateFlags s_flags) const
{
	return checkFlag(stateFlags, s_flags);
}

bool GuiProperties::checkFlag(GuiPropFlags p_flags) const
{
	return checkFlag(propFlags, p_flags);
}


bool GuiProperties::isVisible() const
{
	return checkFlag(SFlags::VISIBLE);
}

bool GuiProperties::isEnabled() const
{
	return checkFlag(SFlags::ENABLED);
}

bool GuiProperties::isFocused() const
{
	return checkFlag(SFlags::FOCUSED);
}

bool GuiProperties::isFloating() const
{
	return checkFlag(SFlags::FLOATING);
}


bool GuiProperties::isDraggable() const
{
	return checkFlag(PFlags::DRAGGABLE);
}

bool GuiProperties::isDroppable() const
{
	return checkFlag(PFlags::DROPPABLE);
}

bool GuiProperties::hasHardBack() const
{
	return checkFlag(PFlags::HARD_BACK);
}

bool GuiProperties::absorbsScroll() const
{
	return checkFlag(PFlags::ABSORB_SCROLL);
}

bool GuiProperties::echosChildEvents() const
{
	return checkFlag(PFlags::CHILD_EVENT_ECHO);
}

bool GuiProperties::isUncontained() const
{
	return checkFlag(PFlags::UNCONTAINED);
}