#ifndef APOLLO_UTILITY_H
#define APOLLO_UTILITY_H

#include "AStatus.h"

//UTILITY NAMESPACE -- Contains common utilities.
namespace AU
{
	//SAFE DELETE function --> deletes pointer only if not null, then sets to null.
	//TODO: Replace pointers with smart pointers.
	template<typename T>
	AStatus safeDelete(T*& obj)
	{
		AStatus status;

		if(obj)
			delete obj;
		else
			status.setWarning(AS::WType::GENERAL, "Object was already deleted (nullptr).");

		obj = nullptr;
		return status;
	}

	//Exploits bitwise structure of a double for super fast 
	//TODO: Doesnt work :/
	inline double fastAbs(double num)
	{
		static const unsigned char mask = ~0x08;

		unsigned char *bytes = (unsigned char*)&num;
		bytes[sizeof(double) - 1] &= mask;

		return *((double*)bytes);
	}

};



#endif	//APOLLO_UTILITY_H