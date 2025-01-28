#pragma once

#include <iostream>
#include <string>

#include "book.h"

using namespace std;

// Derived class inheriting from the 'Book' class.
class OnlineBook : public Book {
private:
	string url;

public:
	// Constructor
	// Used to initialise objects of the OnlineBook class. 
	// This involves calling the base class 'Book' constructor to initialise the base class members 'title' and 'author'. It then initialises the 'url' member specific to 'OnlineBook'
	OnlineBook(const string& title, const string& author, const string& url) : Book(title, author), url(url) {}

	// Override 
	// This display function overrides the the virtual display function of the base class 'Book'
	void display() const override {
		Book::display();
		cout << "Url: " << url << endl;
	}

};
