#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "serverSocket.h"

using namespace std;


// Main Program
int main(int argc, char* argv[]) {

	const char* ipAddress = "127.0.0.1"; // Set IP (local)
	int port = 55555;					 // Set port (local)

	// Instantiate class object
	ServerSocket server(port, ipAddress);

	// These methods start a server connection, waiting for a client connection.
	// This method finds the Winsock dll.
	if (!server.initaliseWinsock())
		return 0;
	// This method creates a server socket.
	if (!server.createSocket())
		return 0;
	// This method binds the server socket to the specified address and port.
	if (!server.bindSocket())
		return 0;
	// This method sets up the socket to listen for incoming clients.
	if (!server.listener())
		return 0;
	// Wait for connection from the client.
	if (!server.acceptConnection())
		return 0;

	string message;

	// Constantly listen for incoming messages.
	while (true) {
		if (!server.receiveMessage(message)) {
			cout << "\nLost connection..." << endl;
			break;
		}

		server.handleMessage(message); // Handle incoming messages.

	}

	return 0;
}
