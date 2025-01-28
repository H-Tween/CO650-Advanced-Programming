#include <iostream>
#include <string>
#include <vector>

#include "logger.h"
#include "book.h"
#include "exceptionHandling.h"
#include "onlineBook.h"
#include "physicalBook.h"
#include "librarian.h"
#include "library.h"
#include "clientSocket.h"

using namespace std;

int Book::totalBooks = 0; // Initialize static member

// This method is used to show Functional pointers.
// It displays a simple starting message when the application is started.
void startMessage() {
	cout << "----------Library Management System----------" << endl; 
}

// This method is used to send messages to the winsock server. 
// It checks if the message is sent, if so it gets the response and displays it.
// 'sendMessage' and 'receiveMessage' handle errors.
void sendServerMessage(string message, ClientSocket& client) {
	if (client.sendMessage(message)) {

		string result;
		if (client.receiveMessage(result)) {
			cout << "Server response: " << result << endl;
		}
	}
}
// This function is for displaying the admin menu to add, remove and alter information about the books.
void displayAdminMenu(Library& library, Librarian& librarian, ClientSocket& client) { // pass by reference not value so it can be altered and not cause memory allocation that can't be accessed.
	char adminChoice, bookType;
	string title, author, url, userInput;
	int shelfNum;

	const Book* book = nullptr;

	do {
		cout << "\n---Admin Menu---\n";
		cout << "1: Add New Book\n";
		cout << "2: Delete Book\n";
		cout << "3: Modify Book Title\n";
		cout << "4: Modify Book Author\n";
		cout << "5: Return to Main Menu\n";
		cout << "Enter the number of your choice: ";
		cin >> adminChoice;
		cin.ignore();


		switch (adminChoice) {
		
			// Add New Book
		case '1':
			// First the user enters what type of book they want to add to the library.
			cout << "\nEnter book type, p for Physical or o for Online: ";
			cin >> bookType;
			cin.ignore();
			if (bookType == 'p' || bookType == 'P' || bookType == 'o' || bookType == 'O') {

				// Then the user is asked to enter the title and author of the book.
				cout << "\nEnter book title: ";
				getline(cin, title); // Using getline instead of 'cin >>' to include spaces for multiple words
				cout << "\nEnter book author: ";
				getline(cin, author);

				// Depending on what type of book the user wants to add to the library, they are required to enter either a shelf number or URL.
				if (bookType == 'p' || bookType == 'P') {
					cout << "\nEnter book shelf number: ";
					cin >> shelfNum;
					// The book is then added to the library.
					library.addBook(new PhysicalBook(title, author, shelfNum));
					// A message is sent to the server to simulate updating its database with the books title and author.
					sendServerMessage("New Physical Book added to library titled: " + title + ", author: " + author, client);
					// The latest book is then displayed (This will be the book just added as it is appended to the end of the library)
					library.showNewestBook();
				}
				else {
					cout << "\nEnter book url: ";
					cin >> url;
					// The book is then added to the library.
					library.addBook(new OnlineBook(title, author, url));
					// A message is sent to the server to simulate updating its database with the books title and author.
					sendServerMessage("New Online Book added to library titled: " + title + ", author: " + author, client);
					// The latest book is then displayed (This will be the book just added as it is appended to the end of the library)
					library.showNewestBook();
				}
			}
			else {
				cout << "\nInvalid Input...";
			}
			break;

			// Delete Book
		case '2':
			cout << "\nEnter book title: ";
			getline(cin, title);
			// Get book pointer from user input
			book = library.getBookByTitle(title);
			if (book) {
				// Set the book title and author as they are going to be deleted so they cannot be called when sending a message to the server to tell it what book is deleted.
				const string bookTitle = book->getTitle();
				const string bookAuthor = book->getAuthor();
				cout << "\n Book Found...";
				cout << "\n Please type confirm to delete book, otherwise any other input to cancel: ";
				cin >> userInput;
				if (userInput == "confirm" || userInput == "Confirm") {
					// Delete book if found
					library.deleteBook(book);
					// A message is sent to the server to simulate updating its database with the books title and author of the book deleted.
					sendServerMessage("Deleted book from library titled: " + bookTitle + ", author: " + bookAuthor, client);
				}
				else {
					cout << "\nBook deletion canceled..." << endl;
				}
			}
			else {
				
				cout << "\n No books with that title were found...\n";
			}

			break;

			// Modify Book Title
		case '3':
			cout << "\nEnter book title: ";
			getline(cin, title);
			// Get book pointer from user input
			book = library.getBookByTitle(title);
			if (book) {
				cout << "\n Book Found...";
				cout << "\n Enter new title: ";
				getline(cin, title);
				// Set the currentTitle of the book so it can be used to simulate updating the server database.
				const string currentTitle = book->getTitle();
				// The book title is then changed with this method.
				librarian.modifiyBookTitle(*book, title);
				cout << "\n Book updated to title: " << title << endl;
				// A message is sent to the server to simulate updating its database with the old book title and the new book title.
				sendServerMessage("Book title updated in library from: " + currentTitle + ", to: " + title, client);
			} 
			else {
				cout << "\nBook with title " << title << " not found" << endl;
			}
			break;

			// Modify Book Author
		case '4':
			cout << "\nEnter book author: ";
			getline(cin, author);
			// Get book pointer from user input
			book = library.getBookByAuthor(author);
			if (book) {
				cout << "\n Book Found...";
				cout << "\n Enter new author: ";
				getline(cin, author);
				// Set the currentAuthor of the book so it can be used to simulate updating the server database.
				const string currentAuthor = book->getAuthor();
				// The book author is then changed with this method.
				librarian.modifiyBookAuthor(*book, author);
				cout << "\n Book updated to author: " << author << endl;
				// A message is sent to the server to simulate updating its database with the old book author and the new book author.
				sendServerMessage("Book author updated in library from: " + currentAuthor + ", to: " + author, client);
			}
			else {
				cout << "\nBook with author " << author << " not found" << endl;
			}

			break;

			// Exit Admin Menu
		case 'q':
		case '5':
			cout << "\nExiting Admin Menu..." << endl;
			break;
		default:
			cout << "\nInvalid choice. Please try again.\n";

		}
	} while (adminChoice != 'q' && adminChoice != '5'); // Loops through the admin menu until the user quits using 'q' or '5' as an input.
}

// Main Program
int main(int argc, char* argv[]) {
	try {
		// Functional Pointer
		void (*startFunction)() = startMessage;
		startFunction();
		
		// Instantiate class object
		Library library;

		// Adding books manually on startup
		library.addBook(new PhysicalBook("The Silent Echo", "Emma Blackwood", 82));
		library.addBook(new PhysicalBook("Whispers in the Dark", "Liam Hunter", 150));
		library.addBook(new PhysicalBook("The Last Embrace", "Dylan Cooper", 199));
		library.addBook(new PhysicalBook("The Forgotten Path", "James Whitmore", 177));
		library.addBook(new PhysicalBook("Shadows of the Lost", "Grace Bennett", 43));
		library.addBook(new PhysicalBook("The Hidden Garden", "Ava Montgomery", 37));
		library.addBook(new PhysicalBook("Fragments of the Past", "Nora Stevens", 163));
		library.addBook(new PhysicalBook("The Burning Sky", "Oliver Gray", 185));
		library.addBook(new PhysicalBook("Dance of the Stars", "Harper Wilson", 135));
		library.addBook(new PhysicalBook("Shattered Glass", "Sebastian Cole", 180));
		library.addBook(new PhysicalBook("Between Worlds", "Jasper Ford", 211));
		library.addBook(new PhysicalBook("The Heart of the Storm", "Mason White", 129));
		library.addBook(new PhysicalBook("A Symphony of Souls", "Leo Knight", 53));

		library.addBook(new OnlineBook("The Shadow's Edge", "Clara Mills", "www.myOnlineBook/index/ApbD7fI2"));
		library.addBook(new OnlineBook("The Forgotten Kingdom", "Henry Wright", "www.myOnlineBook/index/W7kRm9Dg"));
		library.addBook(new OnlineBook("The Depths of Desire", "Lena Murphy", "www.myOnlineBook/index/X4nB2qJk"));
		library.addBook(new OnlineBook("The Silent Witness", "David Reed", "www.myOnlineBook/index/F3nGp0Ls"));
		library.addBook(new OnlineBook("Requiem for the Lost", "Natalie Stone", "www.myOnlineBook/index/P1cQxR8I"));
		library.addBook(new OnlineBook("Journey into the Unknown", "Isobel Price", "www.myOnlineBook/index/Uj4Cz1T9"));
		library.addBook(new OnlineBook("A World of Dreams", "Mason Hart", "www.myOnlineBook/index/L1zX9fT6"));
		library.addBook(new OnlineBook("The Edge of Tomorrow", "Ethan Matthews", "www.myOnlineBook/index/M0yJq2F7"));
		library.addBook(new OnlineBook("Whispers in the Wind", "Benjamin Miles", "www.myOnlineBook/index/Z0mV9p4J"));

		// Instantiate class object
		Librarian librarian;

		// LOGGING (Testing)
		Logger logger;
		logger.logMessage(Book::getTotalBooks()); // Log the total number of books currently in the library

		const char* serverIp = "127.0.0.1"; // Set IP (local)
		int port = 55555;					// Set port (local)

		// Instantiate class object
		ClientSocket client(port, serverIp); 

		// These methods start a client connection to the server.
		// This method finds the Winsock dll.
		if (!client.initaliseWinsock()) {
			logger.logMessage("\nFailure to initialise winsock client...");
			return 0;
		}
		// This method creates a client socket.
		if (!client.createSocket()) {
			logger.logMessage("\nFailure to create socket...");
			return 0;
		}

		// This method connects to the server.
		if (!client.connectToServer()) {
			logger.logMessage("\nFailure to connect to server");
			return 0;
		}

		char choice;
		string userInput;
		bool result;

		// This is the console applications main menu.
		do {
			cout << "\n---Main Menu---\n";
			cout << "1: View All Books\n";
			cout << "2: View Physical Books\n";
			cout << "3: View Online Books\n";
			cout << "4: Total Number of Books\n";
			cout << "5: Search for book by title\n";
			cout << "6: Search for book by author\n";
			cout << "7: Admin Menu\n";
			cout << "8: Quit\n";
			cout << "Enter the number of your choice: ";
			cin >> choice;

			// Clear the newline character left in the buffer
			cin.ignore();  // This will clear the '\n' character

			cout << "\n";

			switch (choice) {

				// View all books
			case '1':
				library.showAllBooks(); // Calls the 'showAllBooks()' method which displays all books.
				break;
				// View all Physical books
			case '2': 
				library.showBookByType("PhysicalBook"); // Displays all books with type 'PhysicalBook'
				break;
				// View all Online books
			case '3':
				library.showBookByType("OnlineBook"); // Displays all books with type 'OnlineBook'
				break;
				// Display total number of books
			case '4':
				cout << "\nTotal number of books: " << Book::getTotalBooks() << endl; // Displays total number of books using 'getTotalBooks()' method.
				break;
				// Search for a book by title
			case '5':
				cout << "\nEnter the title of the book: ";
				getline(cin, userInput);
				cout << "" << endl;
				// Displays book that the user inputted (by title), returning false if an error occured.
				result = library.showBookByTitle(userInput);
				// If no books are found with that title, appropriate error is displayed
				if (!result) {
					cout << "\n No books with that title were found...\n";
				}
				break;
				// Search for a book by author
			case '6':
				cout << "\nEnter the author of the book: ";
				getline(cin, userInput);
				cout << "" << endl;
				// Displays book that the user inputted (by author), returning false if an error occured.
				result = library.showBookByAuthor(userInput);
				// If no books are found with that title, appropriate error is displayed
				if (!result) {
					cout << "\n No books with that author were found...\n";
				}
				break;
				// Display the admin menu
			case '7':
				displayAdminMenu(library, librarian, client);
				break;
				// Exit application
			case 'q':
			case '8':
				cout << "Exiting Program" << endl;
				break;
			default:
				cout << "Invalid choice. Please try again.\n";
			}

		} while (choice != 'q' && choice != '8'); // Loops through the main menu until the user quits using 'q' or '8' as an input.

		client.cleanUp(); // Deallocating memory 

	}
	// Catch custom library exception
	catch (const LibraryException& e) {
		cerr << "Library Error: " << e.what() << endl;
	}
	// Catch normal exception
	catch (const exception& e) {
		cerr << "Unexpected Error: " << e.what() << endl;
	}

	return 0;

}