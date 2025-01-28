#include <string>

#include "book.h"

using namespace std;

// Friendship
// Admin class 
class Librarian {
public:
	// These methods are for changing the title and author of the book passed in as a parameter to the new title/author also passed in as a parameter.
	void modifiyBookTitle(const Book& book, const string& newTitle) {
		book.title = newTitle; // Accessing private member (friendship)
	}

	void modifiyBookAuthor(const Book& book, const string& newAuthor) {
		book.author = newAuthor; // Accessing private member (friendship)
	}

};
