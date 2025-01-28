#include <iostream>
#include <string>
#include <vector>

#include "book.h"

using namespace std;

class Library {
private:
	// This creates a vector that stores pointers to 'Book' objects. 
	// Pointers are used to avoid copying the entire 'Book' object which is more effecient for larger objects.
	// This enables effecient memory usage by using pointers.
	vector<Book*> books;

public:
	// Destructor
	// Cleans up memory by iterating over each pointer and deallocating the memory.
	~Library() {
		for (Book* book : books) {
			delete book;
		}
	}

	// Adds the book pointer (which points to the book object) to the end of the books vector.
	void addBook(Book* book) {
		books.push_back(book);
	}

	// Loops through each book pointer and calls the display function which the book points to.
	void showAllBooks() const {
		for (const Book* book : books) {
			book->display();
		}
	}

	// Loops through each book pointer but only displays books from the class specified.
	void showBookByType(const string& type) const {
		for (const Book* book : books) {
			if (type == "PhysicalBook" && dynamic_cast<const PhysicalBook*>(book)) {
				book->display();
			}
			else if (type == "OnlineBook" && dynamic_cast<const OnlineBook*>(book)) {
				book->display();
			}
		}
	}

	// Loops through each book pointer and checks if the book title is the same as the title passed in as a parameter (Book requested by user).
	// If the book is found, it will increment the counter, stopping the for loop, and then displaying that book.
	bool showBookByTitle(const string& title) const {
		int counter = 0;
		for (const Book* book : books) {
			if (book->getTitle() == title) {
				counter++;
				book->display();
				break;
			}
		}
		if (counter >= 1) {
			return true;
		}
		else {
			return false;
		}
	}

	// Loops through each book pointer and checks if the book author is the same as the author passed in as a parameter (Book requested by user).
	// If the book is found, it will increment the counter, stopping the for loop, and then displaying that book.
	bool showBookByAuthor(const string& author) const {
		int counter = 0;
		for (const Book* book : books) {
			if (book->getAuthor() == author) {
				counter++;
				book->display();
				break;
			}
		}
		if (counter >= 1) {
			return true;
		}
		else {
			return false;
		}
	}

	// Loops through each book pointer and checks if the book title is the same as title passed in as a parameter (Book requested by user).
	// It then displays that book and returns it.
	const Book* getBookByTitle(const string& title) const {
		for (const Book* book : books) {
			if (book->getTitle() == title) {
				book->display();
				return book;
			}
		}
		return nullptr;
	}

	// Loops through each book pointer and checks if the book author is the same as author passed in as a parameter (Book requested by user).
	// It then displays that book and returns it.
	const Book* getBookByAuthor(const string& author) const {
		for (const Book* book : books) {
			if (book->getAuthor() == author) {
				book->display();
				return book;
			}
		}
		return nullptr;
	}

	// This method searches for the book pointer that is passed in, if it is found it is removed from the book pointer vector, otherwise the book was not in the library and an appropriate message is outputted.
	void deleteBook(const Book* book) {
		// Searches through the range of books from beginning to end. If the book is found, it returns the iterator pointing to that book, otherwise it returns 'books.end()'.
		auto it = find(books.begin(), books.end(), book);
		if (it != books.end()) {
			// The book pointer is a constant so it cannot be deleted, therefore the const qualifier needs to be removed.
			delete const_cast<Book*>(*it); // Deallocate memory
			books.erase(it); // Remove pointer from vector
			cout << "\nBook deleted...\n";
		}
		else {
			cout << "\nBook not in library...";
		}
	}

	// This method displays the last book in the vector which would be the most recent book added.
	void showNewestBook() const {
		if (!books.empty()) { // check if the books vector is empty
			books.back()->display(); // displays the last element in the vector (newest book added)
		}
		else {
			cout << "Library is empty!";
		}
	}

};
