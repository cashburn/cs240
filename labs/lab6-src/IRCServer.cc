
const char * usage =
"                                                               \n"
"IRCServer:                                                   \n"
"                                                               \n"
"Simple server program used to communicate multiple users       \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   IRCServer <port>                                            \n"
"                                                               \n"
"Where 1024 < port < 65536.                                     \n"
"                                                               \n"
"In another window type:                                        \n"
"                                                               \n"
"   telnet <host> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where talk-server      \n"
"is running. <port> is the port number you used when you run    \n"
"daytime-server.                                                \n"
"                                                               \n";

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "IRCServer.h"
#include "HashTableVoid.cc"

int QueueLength = 5;
HashTableVoid passwords;

int
IRCServer::open_server_socket(int port) {

	// Set the IP address and port for this server
	struct sockaddr_in serverIPAddress; 
	memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
	serverIPAddress.sin_port = htons((u_short) port);
  
	// Allocate a socket
	int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
	if ( masterSocket < 0) {
		perror("socket");
		exit( -1 );
	}

	// Set socket options to reuse port. Otherwise we will
	// have to wait about 2 minutes before reusing the sae port number
	int optval = 1; 
	int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, 
			     (char *) &optval, sizeof( int ) );
	
	// Bind the socket to the IP address and port
	int error = bind( masterSocket,
			  (struct sockaddr *)&serverIPAddress,
			  sizeof(serverIPAddress) );
	if ( error ) {
		perror("bind");
		exit( -1 );
	}
	
	// Put socket in listening mode and set the 
	// size of the queue of unprocessed connections
	error = listen( masterSocket, QueueLength);
	if ( error ) {
		perror("listen");
		exit( -1 );
	}

	return masterSocket;
}

void
IRCServer::runServer(int port)
{
	int masterSocket = open_server_socket(port);

	initialize();
	
	while ( 1 ) {
		
		// Accept incoming connections
		struct sockaddr_in clientIPAddress;
		int alen = sizeof( clientIPAddress );
		int slaveSocket = accept( masterSocket,
					  (struct sockaddr *)&clientIPAddress,
					  (socklen_t*)&alen);
		
		if ( slaveSocket < 0 ) {
			perror( "accept" );
			exit( -1 );
		}
		
		// Process request.
		processRequest( slaveSocket );		
	}
}

int
main( int argc, char ** argv )
{
	// Print usage if not enough arguments
	if ( argc < 2 ) {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}
	
	// Get the port from the arguments
	int port = atoi( argv[1] );

	IRCServer ircServer;
	//passwords = new HashTableVoid;
	// It will never return
	ircServer.runServer(port);
	
}

//
// Commands:
//   Commands are started by the client.
//
//   Request: ADD-USER <USER> <PASSWD>\r\n
//   Answer: OK\r\n or DENIED\r\n
//
//   REQUEST: GET-ALL-USERS <USER> <PASSWD>\r\n
//   Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//
//   REQUEST: CREATE-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LIST-ROOMS <USER> <PASSWD>\r\n
//   Answer: room1\r\n
//           room2\r\n
//           ...
//           \r\n
//
//   Request: ENTER-ROOM <USER> <PASSWD> <ROOM>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: LEAVE-ROOM <USER> <PASSWD>\r\n
//   Answer: OK\n or DENIED\r\n
//
//   Request: SEND-MESSAGE <USER> <PASSWD> <MESSAGE> <ROOM>\n
//   Answer: OK\n or DENIED\n
//
//   Request: GET-MESSAGES <USER> <PASSWD> <LAST-MESSAGE-NUM> <ROOM>\r\n
//   Answer: MSGNUM1 USER1 MESSAGE1\r\n
//           MSGNUM2 USER2 MESSAGE2\r\n
//           MSGNUM3 USER2 MESSAGE2\r\n
//           ...\r\n
//           \r\n
//
//    REQUEST: GET-USERS-IN-ROOM <USER> <PASSWD> <ROOM>\r\n
//    Answer: USER1\r\n
//            USER2\r\n
//            ...
//            \r\n
//

void
IRCServer::processRequest( int fd )
{
	// Buffer used to store the comand received from the client
	const int MaxCommandLine = 1024;
	char commandLine[ MaxCommandLine + 1 ];
	int commandLineLength = 0;
	int n;
	
	// Currently character read
	unsigned char prevChar = 0;
	unsigned char newChar = 0;
	
	//
	// The client should send COMMAND-LINE\n
	// Read the name of the client character by character until a
	// \n is found.
	//

	// Read character by character until a \n is found or the command string is full.
	while ( commandLineLength < MaxCommandLine &&
		read( fd, &newChar, 1) > 0 ) {

		if (newChar == '\n' && prevChar == '\r') {
			break;
		}
		
		commandLine[ commandLineLength ] = newChar;
		commandLineLength++;

		prevChar = newChar;
	}
	
	// Add null character at the end of the string
	// Eliminate last \r
	commandLineLength--;
        commandLine[ commandLineLength ] = 0;

	printf("RECEIVED: %s\n", commandLine);
	/*
	printf("The commandLine has the following format:\n");
	printf("COMMAND <user> <password> <arguments>. See below.\n");
	printf("You need to separate the commandLine into those components\n");
	printf("For now, command, user, and password are hardwired.\n");

	const char * command = "ADD-USER";
	const char * user = "peter";
	const char * password = "spider";
	const char * args = "";
	*/
	const char * command;
	const char * user;
	const char * password;
	const char * args;
	int lastSpace = 0;
	char * temp = (char *) malloc(1024 * sizeof(char));
	char * t = temp;
	for (int i = 0; i < commandLineLength; i++) {
		if (commandLine[i] == ' ') {
			*t = 0;
			command = strdup(temp);
			lastSpace = i;
			break;
		}
		*t = commandLine[i];
		t++;
	}
	t = temp;
	for (int i = lastSpace + 1; i < commandLineLength; i++) {
		if (commandLine[i] == ' ') {
			*t = 0;
			user = strdup(temp);
			lastSpace = i;
			break;
		}
		*t = commandLine[i];
		t++;
	}
	t = temp;
	for (int i = lastSpace + 1; i <= commandLineLength; i++) {
		if ((commandLine[i] == ' ') || (commandLine[i] == '\0')) {
			*t = 0;
			password = strdup(temp);
			lastSpace = i;
			break;
		}
		*t = commandLine[i];
		t++;
	}
	t = temp;
	for (int i = lastSpace + 1; i < commandLineLength; i++) {
		*t = commandLine[i];
		t++;
	}
	*t = 0;
	args = strdup(temp);
	free(temp);
	printf("command=%s\n", command);
	printf("user=%s\n", user);
	printf( "password=%s\n", password );
	printf("args=%s\n", args);

	if (!strcmp(command, "ADD-USER")) {
		addUser(fd, user, password, args);
	}
	else if (!strcmp(command, "ENTER-ROOM")) {
		enterRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LEAVE-ROOM")) {
		leaveRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "SEND-MESSAGE")) {
		sendMessage(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-MESSAGES")) {
		getMessages(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-USERS-IN-ROOM")) {
		getUsersInRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-ALL-USERS")) {
		getAllUsers(fd, user, password, args);
	}
	else {
		const char * msg =  "UNKNOWN COMMAND\r\n";
		write(fd, msg, strlen(msg));
	}

	// Send OK answer
	//const char * msg =  "OK\n";
	//write(fd, msg, strlen(msg));

	close(fd);	
}

void
IRCServer::initialize()
{
	// Open password file
	FILE * passFile = fopen("passwords.txt","a+");

	// Initialize users in room
	char * temp = (char *) malloc(1024 * sizeof(char));
	char * user;
	char * password;
	char * t = temp;
	int last;
	int c;
	while ((c = fgetc(passFile)) != EOF) {
		if (c == ' ') {
			*t = 0;
			user = strdup(temp);
			t = temp;
			continue;
		}
		else if ((c == '\n') && (last == '\r')) {
			t--;
			*t = 0;
			password = strdup(temp);
			t = temp;
			if ((user != NULL) && (password != NULL))
				addUser(1, user, password, "initialize");
			continue;
		}
		*t = c;
		last = c;
		t++;
	}
	/**t = 0;
	password = strdup(temp);
	t = temp;
	if ((user != NULL) && (password != NULL))
		addUser(1, user, password, "initialize");
		*/
	free(temp);
	fclose(passFile);
	// Initalize message list
	
}

bool
IRCServer::checkPassword(int fd, const char * user, const char * password) {
	char * stored = (char *) malloc(sizeof(password));
	if(passwords.find(user, (void**) &stored)) {
		if (!strcmp(password, stored))
			return true;
	}
	return false;
}

void
IRCServer::addUser(int fd, const char * user, const char * password, const char * args) {
	// Here add a new user. For now always return OK.
	const char * msg;
	char ** temp;
	if(!passwords.find(user, (void**) temp)) {
		if(!passwords.insertItem(user, (void*) password))
			msg =  "OK\r\n";
		else
			msg = "DENIED\r\n";
	}
	else
		msg = "DENIED\r\n";
	write(fd, msg, strlen(msg));
	if (strcmp(args, "initialize")) {
		FILE * passFile = fopen("passwords.txt","a+");
		fprintf(passFile, "%s %s\r\n", user, password);
		fclose(passFile);
	}
	//write(fd, user, strlen(user));
	//write(fd, password, strlen(password));

	return;		
}
void createRoom(int fd, const char * user, const char * password, const char * args) {

}
void listRoom(int fd, const char * user, const char * password, const char * args) {

}
void
IRCServer::enterRoom(int fd, const char * user, const char * password, const char * args) {
}

void
IRCServer::leaveRoom(int fd, const char * user, const char * password, const char * args)
{
}

void
IRCServer::sendMessage(int fd, const char * user, const char * password, const char * args)
{
}

void
IRCServer::getMessages(int fd, const char * user, const char * password, const char * args)
{
}

void
IRCServer::getUsersInRoom(int fd, const char * user, const char * password, const char * args)
{
}

void
IRCServer::getAllUsers(int fd, const char * user, const char * password, const  char * args) {
	HashTableVoidIterator iterator(&passwords);
	FILE * fssock = fdopen(fd,"r+");
	if (!checkPassword(fd, user, password)) {
		fprintf(fssock,"DENIED\r\n");
		fclose(fssock);
		return;
	}
	const char * key;
	void * data;
	while (iterator.next(key, data)) {
		fprintf(fssock,"%s\r\n",key);
	}
	fprintf(fssock,"\r\n");
	fclose(fssock);
}

