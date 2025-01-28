#include <iostream>
#include <string>

using namespace std;

// This is a logger class
// It is helpful for organising and managing logging across the application, providing a consistant logging format and simplifiying error tracking and debugging.
class Logger {
public:
	// Overloading
	// Each method in the class 'Logger' has the same name but takes in different parameters. This is called overloading. 
	// It enables the same function to handle different datatypes or arguments.
	void logMessage(const string& message) {
		cout << "[LOG MESSAGE] " << message << endl;
	}

	void logMessage(string& message) {
		cout << "[LOG MESSAGE] " << message << endl;
	}

	void logMessage(int message) {
		cout << "[LOG MESSAGE] " << message << endl;
	}
};