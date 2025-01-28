#pragma once

#include <iostream>
#include <string>

#include "book.h"

using namespace std;

// Derived class inheriting from the 'Book' class.
class PhysicalBook : public Book {
private:
	int shelfNum;

public:
	// Constructor
	// Used to initialise objects of the PhysicalBook class. 
	// This involves calling the base class 'Book' constructor to initialise the base class members 'title' and 'author'. It then initialises the 'shelfNum' member specific to 'PhysicalBook'
	// 'shelfNum' is passed in by value instead of by reference because it is a small data type and copying it is easier.
	PhysicalBook(const string& title, const string& author, int shelfNum) : Book(title, author), shelfNum(shelfNum) {}

	// Override 
	// This display function overrides the the virtual display function of the base class 'Book'
	void display() const override {
		Book::display();
		cout << "Shelf Number: " << shelfNum << endl;
	}

};
