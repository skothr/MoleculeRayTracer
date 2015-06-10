#ifndef APOLLO_EXPAND_ENUM_H
#define APOLLO_EXPAND_ENUM_H

#include <string>
#include <ostream>
#include <type_traits>
#include <vector>

//NOTE: For all these functions, the passed enum should have its
//	INVALID value < its EMPTY value (preferrably -1 and 0), and all other values greater than EMPTY.

//Casts value as the underlying type of EnumName
#define CAST_UNDERLYING_TYPE(EnumName, value) static_cast<std::underlying_type<EnumName>::type>(value)

//Expands the given enum class to have bitwise comparison operators overloaded (|, &, ^, |=, &=, ^=, ~)
//
//	Example:
//		enum class Test
//		{
//			INVALID = -1,
//			EMPTY	= 0x00,
//			VALUE1	= 0x01,
//			VALUE2	= 0x02
//		};
//		EXPAND_ENUM_CLASS_OPERATORS(Test);
//
/*
#define EXPAND_ENUM_CLASS_OPERATORS(EnumName)									\
																				\
	inline EnumName operator|(EnumName t1, EnumName t2)							\
	{																			\
		return static_cast<EnumName>( CAST_UNDERLYING_TYPE(EnumName, t1) |		\
									  CAST_UNDERLYING_TYPE(EnumName, t2));		\
	}																			\
	inline EnumName operator&(EnumName t1, EnumName t2)							\
	{																			\
		return static_cast<EnumName>( CAST_UNDERLYING_TYPE(EnumName, t1) &		\
									  CAST_UNDERLYING_TYPE(EnumName, t2));		\
	}																			\
	inline EnumName operator^(EnumName t1, EnumName t2)							\
	{																			\
		return static_cast<EnumName>( CAST_UNDERLYING_TYPE(EnumName, t1) ^		\
									  CAST_UNDERLYING_TYPE(EnumName, t2));		\
	}																			\
																				\
	inline EnumName operator|=(EnumName &t1, const EnumName &t2)				\
	{																			\
		return t1 = t1 | t2;													\
	}																			\
	inline EnumName operator&=(EnumName &t1, const EnumName &t2)				\
	{																			\
		return t1 = t1 & t2;													\
	}																			\
	inline EnumName operator^=(EnumName &t1, const EnumName &t2)				\
	{																			\
		return t1 = t1 ^ t2;													\
	}																			\
	inline EnumName operator~(EnumName t)										\
	{																			\
		return static_cast<EnumName>(~CAST_UNDERLYING_TYPE(EnumName, t));		\
	}																			\
//
*/

/*
//FLAG OPERATORS -- templated functions to define bitwise boolean operators between given types T1 and T2, and 
//						between T1 and unsigned int.
//		Returns type T1
//		These functions assume INVALID = -1, NONE = 0, and each flags is a unique power of 2 (> 0).
//		If either argument is INVALID, INVALID is returned

////OR////
template<typename T1, typename T2>
T1 operator|(T1 t1, T2 t2)
{
	return static_cast<T1>(
				static_cast<int>(t1) >= 0 && static_cast<int>(t2) >= 0
					? (static_cast<unsigned int>(t1) | static_cast<unsigned int>(t2))
					: -1
			);
}
template<typename T>
T operator|(T t, unsigned int f)
{
	return static_cast<T>(
				static_cast<int>(t) >= 0
					? (static_cast<unsigned int>(t) | f)
					: -1
			);
}
template<typename T>
T operator|(unsigned int f, T t)
{
	return static_cast<T>(
				static_cast<int>(t) >= 0
					? (f | static_cast<unsigned int>(t))
					: -1
			);
}
template<typename T1, typename T2>
T1& operator|=(T1 t1, T2 t2)
{
	//NOTE: Will only work if | is defined
	t1 = t1 | t2;
	return t1;
}
template<typename T>
T& operator|=(T t, unsigned int f)
{
	//NOTE: Will only work if | is defined
	t = t | f;
	return t;
}

////AND////
template<typename T1, typename T2>
T1 operator&(T1 t1, T2 t2)
{
	return static_cast<T1>(
				static_cast<int>(t1) >= 0 && static_cast<int>(t2) >= 0
					? (static_cast<unsigned int>(t1) & static_cast<unsigned int>(t2))
					: -1
			);
}
template<typename T>
T operator&(T t, unsigned int f)
{
	return static_cast<T>(
				static_cast<int>(t) >= 0
					? (static_cast<unsigned int>(t) & f)
					: -1
			);
}
template<typename T>
T operator&(unsigned int f, T t)
{
	return static_cast<T>(
				static_cast<int>(t) >= 0
					? (f & static_cast<unsigned int>(t))
					: -1
			);
}
template<typename T1, typename T2>
T1& operator&=(T1 t1, T2 t2)
{
	//NOTE: Will only work if & is defined
	t1 = t1 & t2;
	return t1;
}
template<typename T>
T& operator&=(T t, unsigned int f)
{
	//NOTE: Will only work if & is defined
	t = t & f;
	return t;
}

////XOR////
template<typename T1, typename T2>
T1 operator^(T1 t1, T2 t2)
{
	return static_cast<T1>(
				static_cast<int>(t1) >= 0 && static_cast<int>(t2) >= 0
					? (static_cast<unsigned int>(t1) ^ static_cast<unsigned int>(t2))
					: -1
			);
}
template<typename T>
T operator^(T t, unsigned int f)
{
	return static_cast<T>(
				static_cast<int>(t) >= 0
					? (static_cast<unsigned int>(t) ^ f)
					: -1
			);
}
template<typename T>
T operator^(unsigned int f, T t)
{
	return static_cast<T>(
				static_cast<int>(t) >= 0
					? (f ^ static_cast<unsigned int>(t))
					: -1
			);
}
template<typename T1, typename T2>
T1& operator^=(T1 t1, T2 t2)
{
	//NOTE: Will only work if ^ is defined
	t1 = t1 ^ t2;
	return t1;
}
template<typename T>
T& operator^=(T t, unsigned int f)
{
	//NOTE: Will only work if ^ is defined
	t = t ^ f;
	return t;
}

////NOT////
template<typename T>
T operator~(T t)
{
	return static_cast<T>(static_cast<int>(t) <= 0);
}
*/

//Expands the given enum class to have bitwise comparison operators overloaded (|, &, ^, |=, &=, ^=, ~)
//
//	Example:
//		enum class Test
//		{
//			INVALID = -1,
//			EMPTY	= 0x00,
//			VALUE1	= 0x01,
//			VALUE2	= 0x02
//		};
//		EXPAND_ENUM_CLASS_OPERATORS(Test);
//

#define EXPAND_ENUM_CLASS_OPERATORS(EnumName)									\
																				\
	inline EnumName operator|(EnumName t1, EnumName t2)							\
	{																			\
		return static_cast<EnumName>( CAST_UNDERLYING_TYPE(EnumName, t1) |		\
									  CAST_UNDERLYING_TYPE(EnumName, t2));		\
	}																			\
	inline EnumName operator&(EnumName t1, EnumName t2)							\
	{																			\
		return static_cast<EnumName>( CAST_UNDERLYING_TYPE(EnumName, t1) &		\
									  CAST_UNDERLYING_TYPE(EnumName, t2));		\
	}																			\
	inline EnumName operator^(EnumName t1, EnumName t2)							\
	{																			\
		return static_cast<EnumName>( CAST_UNDERLYING_TYPE(EnumName, t1) ^		\
									  CAST_UNDERLYING_TYPE(EnumName, t2));		\
	}																			\
																				\
	inline EnumName operator|=(EnumName &t1, const EnumName &t2)				\
	{																			\
		return t1 = t1 | t2;													\
	}																			\
	inline EnumName operator&=(EnumName &t1, const EnumName &t2)				\
	{																			\
		return t1 = t1 & t2;													\
	}																			\
	inline EnumName operator^=(EnumName &t1, const EnumName &t2)				\
	{																			\
		return t1 = t1 ^ t2;													\
	}																			\
	inline EnumName operator~(EnumName t)										\
	{																			\
		return static_cast<EnumName>(~CAST_UNDERLYING_TYPE(EnumName, t));		\
	}																			\
//



//Expands the given enum class to have a function valid() that returns whether an enum
//	value is valid (greater than empty_value --> so not empty)
//
//	Standard:	INVALID is <0, EMPTY is 0
//
//	Example:
//		enum class Test
//		{
//			INVALID = -1,
//			EMPTY	= 0x00,
//			VALUE1	= 0x01,
//			VALUE2	= 0x02
//		};
//		EXPAND_ENUM_CLASS_VALID(Test, Test::EMPTY);
//
#define EXPAND_ENUM_CLASS_VALID(EnumName, empty_value)													\
																										\
	inline bool valid(EnumName t)																		\
	{																									\
		return CAST_UNDERLYING_TYPE(EnumName, t) > CAST_UNDERLYING_TYPE(EnumName, empty_value);			\
	}																									\
//


//Expands the given enum class to have its << operator overloaded for printing.
//	Must have called EXPAND_ENUM_CLASS_VALID for this enum.
//	1st value in vectors must be INVALID and 2nd must be EMPTY.
//
//	Example:
//		enum class Test
//		{
//			INVALID = -1,
//			EMPTY	= 0x00,
//			VALUE1	= 0x01,
//			VALUE2	= 0x02
//		};
//		EXPAND_ENUM_CLASS_PRINT( Test,
//								 {Test::INVALID, Test::EMPTY, Test::VALUE1, Test::VALUE2},
//								 {"INVALID", "EMPTY", "VALUE1", "VALUE2"} );
//
#define EXPAND_ENUM_CLASS_PRINT(EnumName, values, strings)										\
																								\
	inline std::ostream& operator<<(std::ostream &os, const EnumName &t)						\
	{																							\
		static const std::vector<EnumName>		types		values;								\
		static const std::vector<std::string>	typeNames	strings;							\
																								\
		os << CAST_UNDERLYING_TYPE(EnumName, t) << " (";										\
																								\
		if(t == types[0])																	\
		{																						\
			os << typeNames[0];																	\
		}																						\
		else																					\
		{																						\
			bool before = false;	/*Whether a type has been listed already (to add comma)*/	\
			for(unsigned int i = 1; i < types.size(); i++)										\
			{																					\
				if(valid(t & types[i]))															\
				{																				\
					if(before)																	\
						os << ", ";																\
																								\
					os << typeNames[i];															\
					before = true;																\
				}																				\
			}																					\
		}																						\
																								\
		os << ")";																				\
		return os;																				\
	}																							\
//

		


#endif	//APOLLO_EXPAND_ENUM_H