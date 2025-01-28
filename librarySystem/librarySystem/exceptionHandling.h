#include <exception>
#include <string>

using namespace std;

// Exception Handling using Exception Classes (Inheriting from Exception) 
// The class LibraryException inherits from the class exception.
// Explicit is used prevent implicit conversions and ensure the constructor is only called when explicitly provded with the correct argument type.
// The constructor takes a 'const char*' parameter which is used to pass a string message which describes the error or exception. 
// 'exception(message)' is calling the constructor of the base class 'exception' and passing the message to it. This ensures the message passed to the LibraryException constructor is then forwarded to the base class which initialises the base class's error message.
// The '{}' in the constructor indicates no additional actions are preformed within the constructor as all initialisation is handled by the initialiser list which is the 'exception' class.
class LibraryException : public exception {
public:
	explicit LibraryException(const char* message) : exception(message) {} // Constructor
};

class WinsockException : public exception {
private:
	int errorCode;	// Error code recieved from winsock
	string errorMessage;  // Error message outputted

public:
	// Constructor
	// Initialise error code and error message.
	WinsockException(int errorCode, const string& errorMessage) : errorCode(errorCode), errorMessage(errorMessage) {}

	// Override base class exception 'what()' method to get a more descriptive message
	const char* what() const noexcept override {
		return errorMessage.c_str();
	}

	int getErrorCode() const {
		return errorCode;
	}

};