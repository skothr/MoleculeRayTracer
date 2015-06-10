#ifndef APOLLO_STATUS_H
#define APOLLO_STATUS_H

#include <iostream>
#include <string>

//Determines what debug level the program will print in.
//	0 -- No debug info is printed.
//	1 -- Print error info, omit success and warning info.
//	2 -- Print error and warning info, omit success info.
//	3 -- Print all debug info.
// TODO: More debug levels?
#define APOLLO_DEBUG_LEVEL 2

struct AStatus;
bool statusGood(const AStatus &status);


struct AStatus
{
public:
	static enum Type
	{
		A_ERROR = -1,
		A_SUCCESS = 0,
		A_WARNING
	};

	static enum class ErrorType
	{
		UNSPECIFIED = -1,

		GENERAL = 0,
		MISSING_INPUT,
		MISSING_OUTPUT,
		INDEX_OUT_OF_BOUNDS,

	};
	typedef ErrorType EType;

	static enum class WarningType
	{
		UNSPECIFIED			= -1,

		GENERAL				= 0,
		NOT_REGISTERED,
		ALREADY_REGISTERED,
		ALREADY_INITIALIZED,

		NO_ACTION_TAKEN,
		PARAMETERS_CHANGED
	};
	typedef WarningType WType;
	
	//Message to accompany an error or a warning
	std::string msg = "";

private:
	Type status_code = A_SUCCESS;
	union
	{
		ErrorType error_code = EType::UNSPECIFIED;
		WarningType warning_code;
	};
	
	//struct BoolConversion { int dummy; };

public:
	/*
	inline operator int BoolConversion::*()
	{
		return (status_code == AStatus::A_SUCCESS) ? &BoolConversion::dummy : 0;
	}
	*/
public:
	AStatus();
	AStatus(Type status, std::string message = "");

	AStatus& operator=(Type status);
	
	bool operator==(Type t) const;
	bool operator==(const AStatus &s) const;

	bool operator!=(Type t) const;
	bool operator!=(const AStatus &s) const;
	
	bool const operator!() const;
	
	void setMessage(std::string new_msg);
	std::string getMessage() const;

	void setError(ErrorType error_type, std::string new_msg = "General error.");
	void setWarning(WarningType warning_type, std::string new_msg = "General Warning.");
	void setSuccess();

	ErrorType getErrorCode() const;
	WarningType getWarningCode() const;

	friend bool operator==(AStatus::Type t, const AStatus &s);
	friend bool operator!=(AStatus::Type t, const AStatus &s);
	friend std::ostream& operator<<(std::ostream &os, const AStatus &s);
};
typedef AStatus AS;

//AStatus operator overloads (cont.)
inline bool operator==(AS::Type t, const AStatus &s)
{
	return t == s.status_code;
}

inline bool operator!=(AS::Type t, const AStatus &s)
{
	return !(t == s);
}

inline std::ostream& operator<<(std::ostream &os, const AStatus &s)
{
	os << "-------------------------\n";
	os << "STATUS:\t";

	if(s.status_code == AS::A_SUCCESS)
	{
		os << "SUCCESS\n\n";
	}
	else if(s.status_code == AS::A_WARNING)
	{
		os << "WARNING\n\n";
	}
	else
	{
		os << "ERROR\n\tCODE:  " << static_cast<int>(s.error_code);
		switch(s.error_code)
		{
		case AS::ErrorType::UNSPECIFIED:
			os << "(UNSPECIFIED)";
			break;
		case AS::ErrorType::GENERAL:
			os << "(GENERAL)";
			break;
		case AS::ErrorType::MISSING_INPUT:
			os << "(MISSING INPUT)";
			break;
		case AS::ErrorType::MISSING_OUTPUT:
			os << "(MISSING OUTPUT)";
			break;
		case AS::ErrorType::INDEX_OUT_OF_BOUNDS:
			os << "(INDEX OUT OF BOUNDS)";
			break;
		default:
			os << "(UNKNOWN CODE)";
			break;
		}
		os << "\n\n";
	}
	os << s.msg << "\n\n";
	os << "-------------------------\n";

	return os;
}


//Functions to handle AStatuses


//Checks whether a status succeeded without any warning
inline bool statusSucceeded(const AStatus &status)
{
	return status == AS::A_SUCCESS;
}

//Checks whether a status succeeded, with or without warning
inline bool statusGood(const AStatus &status)
{
	return statusSucceeded(status) || status == AS::A_WARNING;
}

/*
//Checks whether a status failed fatally.
inline bool statusFailed(const AStatus &status)
{
	return !statusGood(status);
}
*/

//Prints out status debug info (depending on debug level)
inline void debugPrint(const AStatus &status, std::string signifier = "")
{
	//Add new line to signifier if its not empty (aesthetics)
	if(signifier != "")
		signifier.append("\n");

	//Decide what to print out
	switch(APOLLO_DEBUG_LEVEL)
	{
	case 0:
		//Don't print out anything
		break;
	case 1:
		//Print error info
		if(!statusGood(status))
			std::cout << signifier << status;
		break;
	case 2:
		//Print error and warning
		if(!statusSucceeded(status))
			std::cout << signifier << status;
		break;
	case 3:
		//Print everything
		std::cout << signifier << status;
		break;
	default:
		//Default to printing everything
		std::cout << signifier << status;
		break;
	}
}


#endif	//APOLLO_STATUS_H