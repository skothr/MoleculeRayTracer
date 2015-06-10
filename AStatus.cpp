#include "AStatus.h"



/////ASTATUS/////


AStatus::AStatus()
{ }

AStatus::AStatus(Type status, std::string message)
	: status_code(status), msg(message)
{ }


AStatus& AStatus::operator=(Type status)
{
	status_code = status;
	error_code = ErrorType::UNSPECIFIED;
	return *this;
}

bool AStatus::operator==(Type t) const
{
	return status_code == t;
}
bool AStatus::operator==(const AStatus &s) const
{
	return status_code == s.status_code && error_code == s.error_code;
}

bool AStatus::operator!=(Type t) const
{
	return !(*this == t);
}
bool AStatus::operator!=(const AStatus &s) const
{
	return !(*this == s);
}

bool const AStatus::operator!() const
{
	return !statusGood(*this);
}


void AStatus::setMessage(std::string new_msg)
{
	msg = new_msg;
}
std::string AStatus::getMessage() const
{
	return msg;
}

void AStatus::setError(ErrorType error_type, std::string new_msg)
{
	status_code = A_ERROR;
	error_code = error_type;
	setMessage(new_msg);
}

void AStatus::setWarning(WarningType warning_type, std::string new_msg)
{
	status_code = A_WARNING;
	warning_code = warning_type;
	setMessage(new_msg);
}

void AStatus::setSuccess()
{
	status_code = A_SUCCESS;
	error_code = EType::UNSPECIFIED;
	setMessage("");
}

AS::ErrorType AStatus::getErrorCode() const
{
	return (status_code == A_ERROR ? error_code : EType::UNSPECIFIED);
}

AS::WarningType AStatus::getWarningCode() const
{
	return (status_code == A_WARNING ? warning_code : WType::UNSPECIFIED);
}