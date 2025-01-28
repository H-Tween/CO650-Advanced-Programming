#pragma once

#include <iostream>
#include <string>

using namespace std;

// Blueprint class for creating book objects.
class Book {

protected:
	mutable string title;	// Mutable so it can be changed by the Librarian class
	mutable string author;  // Mutable so it can be changed by the Librarian class
	static int totalBooks;

public:
	// Constructor 
	// Used to initialise objects of the book class. It initialises the 'title' and 'author' attributes, then increments the 'totalBooks' counter so the library has the correct number of total books.
	Book(const string& title, const string& author) : title(title), author(author) {
		++totalBooks; // Increase the static book count by 1
	}

	// Destructor 
	// Used to decrement the 'totalBooks' counter and ensure proper exeuction of derived class destructors during cleanup
	// The reason this destructor is declared as virtual is to maintain polymorphic behaviour during the object destruction. 
	virtual ~Book() {
		--totalBooks; // Decrease the static book count by 1
	}

	// Return book title
	virtual string getTitle() const {
		return title;
	}

	// Return book author
	virtual string getAuthor() const {
		return author;
	}

	// Display function using polymorphism
	virtual void display() const {
		cout << "Title: " << title << ", Author: " << author << endl;
	}

	// Static member function for returning 'totalBooks'
	static int getTotalBooks() {
		return totalBooks;
	}

	friend class Librarian; // Allows the 'Librarian' class to access protected attributes in 'Book' class.

};