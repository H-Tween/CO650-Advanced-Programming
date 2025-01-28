#include "stdafx.h"
#include <winsock2.h>
#include <WS2tcpip.h>
#include <algorithm> 
#include <cctype>  
#include <iostream>
#include <string>

using namespace std;

class ClientSocket {
private:
	SOCKET clientSocket;
	sockaddr_in serverAddress;

public:
	// Constructor that initializes the socket and the server address
	ClientSocket(int port, const char* ipAddress) : clientSocket(INVALID_SOCKET) {
		// Initialize the sockaddr_in structure with default values
		memset(&serverAddress, 0, sizeof(serverAddress));

		serverAddress.sin_family = AF_INET; // Sets the address family to IPv4 structure
		if (InetPtonA(AF_INET, ipAddress, &serverAddress.sin_addr.s_addr) != 1) { // Checks if the IP conversion from text to binary format failed.
			cout << "\nInvalid IP address provided..." << endl;
			WSACleanup(); // Releases memory.
			exit(1); // Exit
		}
		serverAddress.sin_port = htons(port); // Set port
	}

	// Destructor
	~ClientSocket() {
		cleanUp();
	}

	// Initalises the winsock dll
	bool initaliseWinsock() {
		try {
			WSADATA wsaData; // Holds configuration data for initialising the windows sockets communication.
			WORD wVersionRequested = MAKEWORD(2, 2); // Version 2.2 of winsock.
			int wsaerr = WSAStartup(wVersionRequested, &wsaData); // Initalises the winsock library for communication.
			if (wsaerr != 0) {
				throw WinsockException(wsaerr, "Failed to initalise Winsock: Winsock dll not found!"); // Throws exception with error code and message.

			}
			cout << "The Winsock dll found!" << endl;
			cout << "The status: " << wsaData.szSystemStatus << endl;
			return true;
		}
		catch (const WinsockException& e) {
			cerr << "Errorcode: " << e.getErrorCode() << " ErrorMessage: " << e.what() << endl;
			return false;
		}
	}

	// Create network socket for communication.
	bool createSocket() {
		try {
			clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Creates a TCP socket using IPv4.
			if (clientSocket == INVALID_SOCKET) { // Checks if socket is invalid.
				throw WinsockException(WSAGetLastError(), "Failed to create Winsock socket"); // Throws exception with error code and message.
				WSACleanup(); // Releases memory.

			}
			cout << "socket() is OK!" << endl;
			return true;
		}
		catch (const WinsockException& e) {
			cerr << "Errorcode: " << e.getErrorCode() << " ErrorMessage: " << e.what() << endl;
			return false;
		}
	}

	// Connects to a server using the client socket.
	bool connectToServer() {
		try {
			if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) { // Attempts to connect to the specified server address and server port.
				throw WinsockException(WSAGetLastError(), "Failed to connet to Winsock server"); // Throws exception with error code and message.
				WSACleanup(); // Releases memory.
			}
			cout << "Client: connect() is OK." << endl;
			cout << "Client: Can start sending and receiving data..." << endl;
			return true;
		}
		catch (const WinsockException& e) {
			cerr << "Errorcode: " << e.getErrorCode() << " ErrorMessage: " << e.what() << endl;
			return false;
		}
	}

	// Send a message to the server.
	bool sendMessage(const string& message) {
		try {
			int byteCount = send(clientSocket, message.c_str(), message.length(), 0); // Sends message using the connected client socket.

			// If bytecount is the same as the socket error code, the message failed to send.
			if (byteCount == SOCKET_ERROR) {
				throw WinsockException(WSAGetLastError(), "Message failed to send"); // Throws exception with error code and message.
			}
			cout << "\nMessage sent: " << message << endl;
			return true;
		}
		catch (const WinsockException& e) {
			cerr << "Errorcode: " << e.getErrorCode() << " ErrorMessage: " << e.what() << endl;
			return false;
		}
	}

	// Recieves message from the server.
	bool receiveMessage(string& result) {
		try {
			// Creates a character array of 512 bytes, with each value being set to 0.
			// This is to ensure the buffer is cleared before receiving data so it prevents any possible left over data from affecing the recieved message. from the server.
			char buffer[512] = { 0 };
			int byteCount = recv(clientSocket, buffer, sizeof(buffer), 0); // Receive message from server.

			// If bytecount > 0, then a message was recieved.
			if (byteCount > 0) {
				result = string(buffer, byteCount);
				return true;
			}
			else {
				throw WinsockException(WSAGetLastError(), "Receiving message failed"); // Throws exception with error code and message.
			}
		}
		catch (const WinsockException& e) {
			cerr << "Errorcode: " << e.getErrorCode() << " ErrorMessage: " << e.what() << endl;
			return false;
		}
	}

	// Close the socket and deallocate memory.
	void cleanUp() {
		if (clientSocket != INVALID_SOCKET) {
			closesocket(clientSocket);
		}
		WSACleanup(); // Releases memory.
	}


};
