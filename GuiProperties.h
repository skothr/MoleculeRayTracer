#ifndef APOLLO_GUI_PROPERTY_FLAGS
#define APOLLO_GUI_PROPERTY_FLAGS

#include "ExpandEnum.h"

//Represents the state a GUI element is in, including whether it's visible, enabled, in focus, (etc?)
//namespace SFlags
//{
	enum class GuiStateFlags
	{
		NONE			= 0x00,

		VISIBLE			= 0x01,
		ENABLED			= 0x02,
		FOCUSED			= 0x04,

		//COLLAPSED		= 0x08,	//Only applies to collapsible elements
		
		FLOATING		= 0x08,	//No virtual parent transform
	};
//};

//typedef SFlags::GuiStateFlags GuiStateFlags;
typedef GuiStateFlags SFlags;
EXPAND_ENUM_CLASS_OPERATORS(GuiStateFlags)


//Represents (usually) static properties that an element has.
//namespace PFlags
//{
	enum class GuiPropFlags
	{
		NONE				= 0x00,

		//Actions
		DRAGGABLE			= 0x01,
		DROPPABLE			= 0x02,
		HARD_BACK			= 0x04,
		ABSORB_SCROLL		= 0x08,

		CHILD_EVENT_ECHO	= 0x10,

		UNCONTAINED			= 0x20,
	};
//};

//typedef PFlags::GuiPropFlags GuiPropFlags;
typedef GuiPropFlags PFlags;
EXPAND_ENUM_CLASS_OPERATORS(GuiPropFlags)

//Default values
static const GuiStateFlags	DEFAULT_STATE = SFlags::VISIBLE | SFlags::ENABLED,
							DEFAULT_STATE_FLOAT = DEFAULT_STATE | SFlags::FLOATING;

/*
//Represents properties about an element's placement.
enum class GuiPlacementFlags
{
	NONE		= 0x00,

};
typedef GuiPlacementFlags PFlags;
EXPAND_ENUM_CLASS_OPERATORS(GuiPlacementFlags)
*/

class GuiProperties
{
private:
	GuiStateFlags stateFlags;
	GuiPropFlags propFlags;
	//GuiPlacementFlags placementFlags;

public:
	GuiProperties();
	GuiProperties(GuiStateFlags s_flags, GuiPropFlags p_flags);

	//Static checking
	static bool checkFlag(GuiStateFlags s_flags, GuiStateFlags check);
	static bool checkFlag(GuiPropFlags p_flags, GuiPropFlags check);

	static bool isVisible(GuiStateFlags s_flags);
	static bool isEnabled(GuiStateFlags s_flags);
	static bool isFocused(GuiStateFlags s_flags);
	static bool isFloating(GuiStateFlags p_flags);

	static bool isDraggable(GuiPropFlags p_flags);
	static bool isDroppable(GuiPropFlags p_flags);
	static bool hasHardBack(GuiPropFlags p_flags);
	static bool absorbsScroll(GuiPropFlags p_flags);
	static bool echosChildEvents(GuiPropFlags p_flags);

	static bool isUncontained(GuiPropFlags p_flags);

	//Individual checking
	void setStateFlags(GuiStateFlags s_flags);
	GuiStateFlags getStateFlags() const;
	void setPropFlags(GuiPropFlags p_flags);
	GuiPropFlags getPropFlags() const;

	bool checkFlag(GuiStateFlags sf) const;
	bool checkFlag(GuiPropFlags pf) const;

	bool isVisible() const;
	bool isEnabled() const;
	bool isFocused() const;
	bool isFloating() const;

	bool isDraggable() const;
	bool isDroppable() const;
	bool hasHardBack() const;
	bool absorbsScroll() const;
	bool echosChildEvents() const;

	bool isUncontained() const;

};

typedef GuiProperties GuiProps;



#endif	//APOLLO_GUI_PROPERTY_FLAGS