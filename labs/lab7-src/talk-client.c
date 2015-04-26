
#include <time.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib.h>
#include <glib/gi18n.h>

char * host;
char * user;
char * password;
char * sport;
char * currentRoom;
int port;
pthread_t thread;
GtkWidget *tree_view;
GtkWidget *text_view;
GtkWidget *text_entry;
GtkTextBuffer *messageBuffer;
GtkTextBuffer *sendBuffer;
GtkTreeStore *treeModel;
gboolean continueRefresh;
#define MAX_MESSAGES 100
#define MAX_MESSAGE_LEN 300
#define MAX_RESPONSE (20 * 1024)

int lastMessage = 0;

int open_client_socket(char * host, int port) {
	// Initialize socket address structure
	struct  sockaddr_in socketAddress;
	
	// Clear sockaddr structure
	memset((char *)&socketAddress,0,sizeof(socketAddress));
	
	// Set family to Internet 
	socketAddress.sin_family = AF_INET;
	
	// Set port
	socketAddress.sin_port = htons((u_short)port);
	
	// Get host table entry for this host
	struct  hostent  *ptrh = gethostbyname(host);
	if ( ptrh == NULL ) {
		perror("gethostbyname");
		exit(1);
	}
	
	// Copy the host ip address to socket address structure
	memcpy(&socketAddress.sin_addr, ptrh->h_addr, ptrh->h_length);
	
	// Get TCP transport protocol entry
	struct  protoent *ptrp = getprotobyname("tcp");
	if ( ptrp == NULL ) {
		perror("getprotobyname");
		exit(1);
	}
	
	// Create a tcp socket
	int sock = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
	if (sock < 0) {
		perror("socket");
		exit(1);
	}
	
	// Connect the socket to the specified server
	if (connect(sock, (struct sockaddr *)&socketAddress,
		    sizeof(socketAddress)) < 0) {
		perror("connect");
		exit(1);
	}
	
	return sock;
}

int sendCommand(char * host, int port, char * command, char * user,
		char * password, char * args, char * response) {
	int sock = open_client_socket( host, port);

	// Send command
	write(sock, command, strlen(command));
	write(sock, " ", 1);
	write(sock, user, strlen(user));
	write(sock, " ", 1);
	write(sock, password, strlen(password));
	write(sock, " ", 1);
	write(sock, args, strlen(args));
	write(sock, "\r\n",2);

	// Keep reading until connection is closed or MAX_REPONSE
	int n = 0;
	int len = 0;
	while ((n=read(sock, response+len, MAX_RESPONSE - len))>0) {
		len += n;
	}

	//printf("response:%s\n", response);

	close(sock);
}

void printUsage()
{
	printf("Usage: talk-client host port user password\n");
	exit(1);
}

void add_user() {
	// Try first to add user in case it does not exist.
	char response[ MAX_RESPONSE ];
	sendCommand(host, port, "ADD-USER", user, password, "", response);
	
	if (!strcmp(response,"OK\r\n")) {
		printf("User %s added\n", user);
	}
}
int login() {
	char response[ MAX_RESPONSE ];
	sendCommand(host, port, "LOGIN", user, password, "", response);
	
	if (!strcmp(response,"OK\r\n")) {
		printf("User %s exists\n", user);
		return 1;
	}
	return 0;
}

/*void nextInTree(GtkTreeIter *iter, GtkTreeIter *toplevel) {
	if (!gtk_tree_model_iter_next(GTK_TREE_MODEL(treeModel), iter)) {
			gtk_tree_store_append (treeModel, iter, toplevel);
			gtk_tree_store_set (treeModel, iter, 0, "", -1);
			//gtk_tree_store_set (treeModel, &toplevel, 0, roomArray[i], -1);
		}
}*/

void listRooms() {
	GtkTreeIter toplevel, child;
    GtkCellRenderer *cell;
    GtkTreeViewColumn *column;
    struct Room {
    	char * name;
    	char ** usersInRoom;
    	int nUsers;
    };
    typedef struct Room Room;
    Room * roomArray = (Room *) malloc(MAX_RESPONSE * sizeof(Room));
	char * response = (char *) malloc(MAX_RESPONSE * sizeof(char));
	char * responsePoint = response;
	char * msg = (char *) malloc(MAX_RESPONSE * sizeof(char));
	char * s = msg;
	int nRooms = 0;
	//int nUsers = 0;
	gboolean first = TRUE;
	gtk_tree_store_clear(treeModel);
	gboolean iterFirst = gtk_tree_model_get_iter_first(GTK_TREE_MODEL(treeModel), &toplevel);
	sendCommand(host, port, "LIST-ROOMS", user, password, "", response);

	while (responsePoint) {
		*s = *responsePoint;
		if (*s == '\r') {
			*s = 0;
			if(strlen(msg) <= 0) {
				//responsePoint++;
				//continue;
				break;
			}
			int newEntry = 0;
			roomArray[nRooms].name = strdup(msg);
			roomArray[nRooms].usersInRoom = (char **) malloc(MAX_RESPONSE * sizeof(char*));
			/*if (!iterFirst) {
				gtk_tree_store_append (treeModel, &toplevel, NULL);
			}
			if (!first) {
				
				if (!gtk_tree_model_iter_next(GTK_TREE_MODEL(treeModel), &toplevel)) {
					gtk_tree_store_append (treeModel, &toplevel, NULL);
					newEntry = 1;
				}
				while (!newEntry && gtk_tree_model_iter_parent(GTK_TREE_MODEL(treeModel), &child, &toplevel)) {
					if (!gtk_tree_model_iter_next(GTK_TREE_MODEL(treeModel), &toplevel)) {
						gtk_tree_store_append (treeModel, &toplevel, NULL);
						newEntry = 1;
					}
				}
			}
			first = FALSE;
        	gtk_tree_store_set (treeModel, &toplevel, 0, msg, -1);*/
        	s = msg;
        	responsePoint++;
        	responsePoint++;
        	char * response2 = (char *) malloc(MAX_RESPONSE * sizeof(char));
        	char *responsePoint2 = response2;
        	sendCommand(host, port, "GET-USERS-IN-ROOM", user, password, msg, response2);
        	roomArray[nRooms].nUsers = 0;
        	while (responsePoint2) {
        		*s = *responsePoint2;
				if (*s == '\r') {
					*s = 0;
					if(strlen(msg) <= 0) {
						break;
					}
					roomArray[nRooms].usersInRoom[roomArray[nRooms].nUsers++] = strdup(msg);
					/*if (!gtk_tree_model_iter_children(GTK_TREE_MODEL(treeModel), &child, &toplevel))
						gtk_tree_store_append (treeModel, &child, &toplevel);
		        	gtk_tree_store_set (treeModel, &child, 0, msg, -1);*/
		        	s = msg;
		        	responsePoint2++;
		        	responsePoint2++;
		        	continue;
		        }
		        s++;
		        responsePoint2++;
		    }
		    free(response2);
		    nRooms++;
		    continue;
		}
		s++;
		responsePoint++;
	}
	
	free(response);
	//int listExists = 1;

	//if (!gtk_tree_model_get_iter_first(GTK_TREE_MODEL(treeModel), &toplevel)) {
		for (int i = 0; i < nRooms; i++) {
			gtk_tree_store_append (treeModel, &toplevel, NULL);
			gtk_tree_store_set (treeModel, &toplevel, 0, roomArray[i].name, -1);
			for (int j = 0; j < roomArray[i].nUsers; j++) {
				gtk_tree_store_append (treeModel, &child, &toplevel);
				gtk_tree_store_set (treeModel, &child, 0, roomArray[i].usersInRoom[j], -1);
				free(roomArray[i].usersInRoom[j]);
			}
			free(roomArray[i].name);
			//free(roomArray[i]);
		}
		return;
	//}
	/*
	for (int i = 0; i < nRooms; i++, nextInTree(&toplevel, NULL)) {
		
		gtk_tree_model_get(GTK_TREE_MODEL(treeModel), &toplevel, 0, (gchar **) &msg,  -1);
		if (strcmp(roomArray[i], msg)) {
			gtk_tree_store_set (treeModel, &toplevel, 0, roomArray[i], -1);
		}
			if (!gtk_tree_model_iter_children(GTK_TREE_MODEL(treeModel), &child, &toplevel)) {
				for (int j = 0; j < nUsers; j++) {
					gtk_tree_store_append (treeModel, &child, &toplevel);
					gtk_tree_store_set (treeModel, &child, 0, userArray[j], -1);
					free(userArray[j]);
				}
				continue;
			}
			for (int j = 0; j < nUsers; j++, nextInTree(&child, &toplevel)) {
				gtk_tree_model_get(GTK_TREE_MODEL(treeModel), &child, 0, (gchar **) &msg,  -1);
				if (strcmp(userArray[j], msg)) {
					gtk_tree_store_set (treeModel, &child, 0, userArray[j], -1);
				}
				
			}
		free(roomArray[i]);
	}*/
	free(msg);
	free(roomArray);
	//free(userArray);

}

void enterRoom(char * roomName) {
	char response[ MAX_RESPONSE ];
	sendCommand(host, port, "ENTER-ROOM", user, password, roomName, response);
	
	if (!strcmp(response,"OK\r\n")) {
		printf("User %s added to %s\n", user, roomName);
	}
}

void leaveRoom(GtkWidget * widget) {
	continueRefresh = FALSE;

	GtkTextIter start;
	GtkTextIter end;
    messageBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
    gtk_text_buffer_get_start_iter (messageBuffer, &start);
    gtk_text_buffer_get_end_iter (messageBuffer, &end);
    gtk_text_buffer_delete (messageBuffer, &start, &end);

	char response[ MAX_RESPONSE ];
	sendCommand(host, port, "LEAVE-ROOM", user, password, currentRoom, response);
	
	if (!strcmp(response,"OK\r\n")) {
		printf("User %s left room %s\n", user, currentRoom);
	}
}

void getMessages() {
	char response[ MAX_RESPONSE ];
	char args[ MAX_RESPONSE ];
	if (lastMessage == -1)
		sprintf(args, "%d %s", 0, currentRoom);
	else
		sprintf(args, "%d %s", lastMessage + 1, currentRoom);
	sendCommand(host, port, "GET-MESSAGES", user, password, args, response);

	if (!strcmp(response,"NO NEW MESSAGES\r\n")) {
		return;
	}
	char * final = (char *) malloc(MAX_RESPONSE*sizeof(char));
	char * responsePoint = response;
	char * userSent = (char *) malloc(MAX_RESPONSE*sizeof(char));
	char * line;
	char * tempMessage = (char *) malloc(MAX_RESPONSE*sizeof(char));
	char * timestr = (char *) malloc(10*sizeof(char));
	int charCount;
	int messageNum;
	line = strtok(responsePoint, "\r\n");
	/*if (sscanf(line, "%d %s%n", lastMessage, userSent, charCount) < 2)
		return;
	line += charCount;
	strcat(final, line);*/
	//responsePoint += charCount;
	strcpy(final, "");
	while (line != NULL) {
		if (sscanf(line, "%d %s %s%n", &lastMessage, userSent, timestr, &charCount) < 2) {
			break;
		}

		line += charCount;
		sprintf(tempMessage, "[%s] <%s> %s\r\n", timestr, userSent, line);
		strcat(final, tempMessage);
		line = strtok(NULL, "\r\n");
	}



	GtkTextIter start;
	GtkTextIter end;
    messageBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
    gtk_text_buffer_get_start_iter (messageBuffer, &start);
    gtk_text_buffer_get_end_iter (messageBuffer, &end);

    gtk_text_buffer_insert (messageBuffer, &end, (gchar*) final, -1);
    free(final);
    free(userSent);
    free(tempMessage);
}
char * timestr() {
	char * buffer = (char*) malloc(20*sizeof(char));
  	//if (widget->window == NULL) return FALSE;

  	time_t curtime;
  	struct tm *loctime;

  	curtime = time(NULL);
  	loctime = localtime(&curtime);
  	strftime(buffer, 256, "%T", loctime);

  	//GtkTextIter iter2;
    //messageBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
    //gtk_text_buffer_get_iter_at_offset (messageBuffer, &iter2, 0);

    //gtk_text_buffer_insert (messageBuffer,&iter2, (gchar*) buffer, -1);
    //listRooms();
  	//printf("%s\n", buffer);
  	//gtk_widget_queue_draw(widget);
  	return buffer;
}

void sendMessage(GtkWidget * widget) {
	GtkTextIter start;
	GtkTextIter end;
	sendBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW(text_entry));
	gtk_text_buffer_get_start_iter(sendBuffer, &start);
	gtk_text_buffer_get_end_iter(sendBuffer, &end);

	char * getText = (char *) gtk_text_buffer_get_text(sendBuffer, &start, &end, FALSE);
	char * message = (char *) malloc(MAX_RESPONSE*sizeof(char));
	char * timestamp = timestr();
	sprintf(message, "%s %s %s", currentRoom, timestamp, getText);
	char response[ MAX_RESPONSE ];
	sendCommand(host, port, "SEND-MESSAGE", user, password, message, response);
	
	if (!strcmp(response,"OK\r\n")) {
		printf("Message sent\n", user);
	}
	gtk_text_buffer_delete(sendBuffer, &start, &end);
	getMessages();
	g_free(getText);
	free(message);
	free(timestamp);
}
/*gboolean on_key_press (GtkWidget * widget, GdkEventKey* pKey) {
	if (pKey->type == GDK_KEY_PRESS) {
		if (pKey->keyval == GDK_KEY_Return)
			sendMessage(widget);
	}
}*/

void createRoom(GtkWidget * widget, GtkWidget *mainWindow) {
	GtkWidget * dialog = gtk_dialog_new_with_buttons ("Create Room",
		GTK_WINDOW(mainWindow), 
		(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
		_("_OK"), GTK_RESPONSE_ACCEPT, _("_Cancel"),
		GTK_RESPONSE_REJECT, NULL);

	GtkWidget * contentArea = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	GtkWidget * roomLabel = gtk_label_new ("Enter Room Name");
	GtkWidget * roomName = gtk_entry_new();
	//GtkEntryBuffer * buffer = gtk_entry_get_buffer(roomName);

	gtk_container_add(GTK_CONTAINER(contentArea), roomLabel);
	gtk_container_add(GTK_CONTAINER(contentArea), roomName);

	g_signal_connect_swapped(dialog, "response",
		G_CALLBACK(gtk_widget_destroy), dialog);

	gtk_widget_show_all(dialog);

}

void printPrompt() {
	printf("talk> ");
	fflush(stdout);
}

void printHelp() {
	printf("Commands:\n");
	printf(" -who   - Gets users in room\n");
	printf(" -users - Prints all registered users\n");
	printf(" -help  - Prints this help\n");
	printf(" -quit  - Leaves the room\n");
	printf("Anything that does not start with \"-\" will be a message to the chat room\n");
}



void * getMessagesThread(void * arg) {
	// This code will be executed simultaneously with main()
	// Get messages to get last message number. Discard the initial Messages
	
	//while (1) {
		// Get messages after last message number received.
		
		// Print messages

		// Sleep for ten seconds
		//usleep(2*1000*1000);
	//}
}

gboolean refreshFunc(GtkWidget *widget) {
	listRooms();
	getMessages();
	return continueRefresh;
}

void startGetMessageThread()
{
	//pthread_create(&thread, NULL, getMessagesThread, NULL);
	g_timeout_add_seconds(5, (GSourceFunc) refreshFunc, (gpointer) text_view);
}

static GtkWidget *create_text1()
{
   GtkWidget *scrolled_window;
   //GtkWidget *text_view;

   text_view = gtk_text_view_new();
   messageBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
   gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_view),GTK_WRAP_WORD);
   gtk_text_view_set_indent (GTK_TEXT_VIEW (text_view), -15);
   gtk_text_view_set_left_margin (GTK_TEXT_VIEW (text_view), 10);
   scrolled_window = gtk_scrolled_window_new (NULL, NULL);
   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                   GTK_POLICY_AUTOMATIC,
           GTK_POLICY_AUTOMATIC);

   gtk_container_add (GTK_CONTAINER (scrolled_window), text_view);
   //insert_text (buffer);

   gtk_widget_show_all (scrolled_window);

   return scrolled_window;
}

static GtkWidget *create_text2()
{
   GtkWidget *scrolled_window;
   //GtkWidget *text_view;

   text_entry = gtk_text_view_new();
   sendBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_entry));
   gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (text_entry),GTK_WRAP_WORD);
   gtk_text_view_set_indent (GTK_TEXT_VIEW (text_entry), -15);
   gtk_text_view_set_left_margin (GTK_TEXT_VIEW (text_entry), 10);
   scrolled_window = gtk_scrolled_window_new (NULL, NULL);
   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                   GTK_POLICY_AUTOMATIC,
           GTK_POLICY_AUTOMATIC);

   gtk_container_add (GTK_CONTAINER (scrolled_window), text_entry);
   //insert_text (buffer);

   gtk_widget_show_all (scrolled_window);

   return scrolled_window;
}

void roomSelected(GtkWidget *widget, gpointer textView) 
{
	//GtkTextBuffer *buffer;
  	GtkTreeIter iter;
  	GtkTreeIter parent;
	GtkTreeModel *model;
  	char *value;
  	gchar *text;

  	if (gtk_tree_selection_get_selected(
      	GTK_TREE_SELECTION(widget), &model, &iter)) {
    	if (gtk_tree_model_iter_parent(model, &parent, &iter))
        	return;
    	gtk_tree_model_get(model, &iter, 0, &value,  -1);
    	GtkTextIter start;
		GtkTextIter end;
    	messageBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
    	gtk_text_buffer_get_start_iter (messageBuffer, &start);
    	gtk_text_buffer_get_end_iter (messageBuffer, &end);
    	enterRoom(value);
    	gtk_text_buffer_delete (messageBuffer, &start, &end);
    	//gtk_text_buffer_set_text (messageBuffer,(gchar*) value, -1);
    	currentRoom = strdup(value);
    	lastMessage = -1;
    	continueRefresh = TRUE;
    	g_free(value);   
  	}
  	getMessages();
  	startGetMessageThread();
}

static GtkWidget * create_list()
{

    GtkWidget *scrolled_window;
    //GtkWidget *tree_view;
    //GtkTreeStore *treeModel;
    GtkTreeIter toplevel, child;
    GtkCellRenderer *cell;
    GtkTreeViewColumn *column;

    int i;
   	printf("Create List\n");
    /* Create a new scrolled window, with scrollbars only if needed */
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
            GTK_POLICY_AUTOMATIC, 
            GTK_POLICY_AUTOMATIC);
   
    treeModel = gtk_tree_store_new (1, G_TYPE_STRING);
    tree_view = gtk_tree_view_new ();
    gtk_container_add (GTK_CONTAINER (scrolled_window), tree_view);
    gtk_tree_view_set_model (GTK_TREE_VIEW (tree_view), GTK_TREE_MODEL (treeModel));
    gtk_widget_show (tree_view);
   
    /* Add some messages to the window */
    
   
    cell = gtk_cell_renderer_text_new ();

    column = gtk_tree_view_column_new_with_attributes ("Chat Rooms",
                                                       cell,
                                                       "text", 0,
                                                       NULL);
  
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view),
                 GTK_TREE_VIEW_COLUMN (column));
    printf("Create List Finished\n");
    return scrolled_window;
}

int main(int argc, char **argv) {

	char line[MAX_MESSAGE_LEN+1];
	
	if (argc < 5) {
		printUsage();
	}

	host = argv[1];
	sport = argv[2];
	user = argv[3];
	password = argv[4];

	printf("\nStarting talk-client %s %s %s %s\n", host, sport, user, password);

	// Convert port to number
	sscanf(sport, "%d", &port);
	if (login()) {
	}
	else {
		add_user();
	}

	// Enter room
	//enterRoom();

	// Start message thread
	//startGetMessageThread();

	GtkWidget *window;
  	GtkWidget *table;
  	GtkWidget *bigPane;
  	GtkWidget *hpaned;
  	GtkWidget *vpaned;

  	GtkWidget *toolbar;
  	GtkToolItem *refresh;
  	GtkToolItem *createRoomButton;
  	GtkToolItem *leaveRoomButton;
  	GtkToolItem *logout;

  	GtkWidget *textEntry;
  	GtkWidget *textView;
  	GtkWidget *button;
  	GtkWidget *roomList;
  	GtkWidget *text;
  	GtkTreeSelection *roomSelection;

  	gtk_init(&argc, &argv);

  	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_widget_set_size_request (window, 600, 400);
	gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
	gtk_window_set_title(GTK_WINDOW(window), "IRC Client");
	gtk_container_set_border_width(GTK_CONTAINER(window), 15);

	bigPane = gtk_vpaned_new();
	gtk_container_add(GTK_CONTAINER(window), bigPane);

	toolbar = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
	gtk_container_set_border_width(GTK_CONTAINER(toolbar), 2);
	gtk_container_add(GTK_CONTAINER(bigPane), toolbar);

	refresh = gtk_tool_button_new_from_stock(GTK_STOCK_REFRESH);
	createRoomButton = gtk_tool_button_new_from_stock(GTK_STOCK_ADD);
	leaveRoomButton = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
	logout = gtk_tool_button_new_from_stock(GTK_STOCK_DISCONNECT);

	/*gtk_tool_button_set_label(GTK_TOOL_BUTTON(refresh), "Refresh");
	gtk_tool_button_set_label(GTK_TOOL_BUTTON(createRoom), "Create Room");
	gtk_tool_button_set_label(GTK_TOOL_BUTTON(leaveRoomButton), "Leave Room");
	gtk_tool_button_set_label(GTK_TOOL_BUTTON(logout), "Logout");
	*/

	gtk_widget_set_tooltip_text(GTK_WIDGET(refresh), "Refresh");
	gtk_widget_set_tooltip_text(GTK_WIDGET(createRoomButton), "Create Room");
	gtk_widget_set_tooltip_text(GTK_WIDGET(leaveRoomButton), "Leave Room");
	gtk_widget_set_tooltip_text(GTK_WIDGET(logout), "Logout");

	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), createRoomButton, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), refresh, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), leaveRoomButton, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), logout, -1);

	vpaned = gtk_vpaned_new();
	gtk_container_add(GTK_CONTAINER(bigPane), vpaned);

	hpaned = gtk_hpaned_new ();
  	gtk_container_add (GTK_CONTAINER (vpaned), hpaned);

  	roomList = create_list ();
  	gtk_widget_set_size_request(roomList, 130, 225);
  	gtk_paned_add1 (GTK_PANED (hpaned), roomList);

  	roomSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
  	textView = create_text1();
  	gtk_paned_add2 (GTK_PANED (hpaned), textView);

	table = gtk_table_new(4, 2, FALSE);
	gtk_container_add (GTK_CONTAINER(vpaned), table);
	textEntry = create_text2();

	button = gtk_button_new_with_label ("Send");
	gtk_table_attach(GTK_TABLE(table), button, 3, 4, 0, 1, 0, 0, 1, 1);
	gtk_table_attach(GTK_TABLE(table), textEntry, 0, 3, 0, 1,
      GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 1, 1);

	//gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 5);
	//gtk_box_pack_start(GTK_BOX(vbox), vpaned, FALSE, FALSE, 5);

	g_signal_connect_swapped(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), G_OBJECT(window));
  	g_signal_connect(roomSelection, "changed", 
    	G_CALLBACK(roomSelected), text);

  	g_signal_connect(refresh, "clicked", 
    	G_CALLBACK(refreshFunc), (gpointer) NULL);

  	g_signal_connect(leaveRoomButton, "clicked", 
    	G_CALLBACK(leaveRoom), (gpointer) NULL);

  	g_signal_connect(createRoomButton, "clicked", 
    	G_CALLBACK(createRoom), (gpointer) NULL);

	g_signal_connect(button, "clicked", 
    	G_CALLBACK(sendMessage), (gpointer) NULL);

	//gtk_widget_set_can_default(button, TRUE);
	//gtk_window_set_default(GTK_WINDOW(window), button);
	//g_signal_connect(text_entry, "key_press_event",
    	//G_CALLBACK(on_key_press), (gpointer) NULL);

  	gtk_widget_show_all(window);


  	listRooms();


  	gtk_main();

	/*  COMMOAND LINE CODE
	while (1) {
		printPrompt();

		char * s = fgets(line,MAX_MESSAGE_LEN, stdin);
		if (s==NULL) {
			leave_room();
			printf("talk-client exiting...\n");
			exit(1);
		}

		if (!isatty(0)) {
			// If it is not a terminal, echo command as well
			printf("%s\n", line);
		}

		if (line[0]=='-') {
			// This is a command process it
			if (!strcmp(line,"-help")) {
				printHelp();
			}
			else if (!strcmp(line,"-quit")) {
				printf("talk-client exiting...\n");
				exit(1);
			}
			// Put other commands here
		}
		else if (line[0]==0) {
			// Empty line. Print help
			printf("Type -help to print the available commands\n");
		}
		else {
			// Send message
		}
	}*/
	
	printf("TEST ENDS\n");
	return 0;
}
