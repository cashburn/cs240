#include <gtk/gtk.h>
#include <stdio.h>

GtkWidget *tree_view;
GtkWidget *text_view;
GtkWidget *text_entry;
GtkTextBuffer *messageBuffer;
GtkTextBuffer *sendBuffer;

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
  GtkTreeIter iter;
  GtkTreeModel *model;
  char *value;
  gchar *text;


  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {
    if (!gtk_tree_model_iter_has_child(model, &iter))
        return;
    gtk_tree_model_get(model, &iter, 0, &value,  -1);
    /*gtk_statusbar_push(GTK_STATUSBAR(statusbar),
        gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
            value), value);*/
    GtkTextIter iter2;
    messageBuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (text_view));
    gtk_text_buffer_get_iter_at_offset (messageBuffer, &iter2, 0);

    gtk_text_buffer_set_text (messageBuffer,(gchar*) value, -1);
    g_free(value);
    //printf("%s",buffer);

    
        
  }
}

static GtkWidget * create_list(GtkWidget *tree_view)
{

    GtkWidget *scrolled_window;
    //GtkWidget *tree_view;
    GtkTreeStore *model;
    GtkTreeIter toplevel, child;
    GtkCellRenderer *cell;
    GtkTreeViewColumn *column;

    int i;
   
    /* Create a new scrolled window, with scrollbars only if needed */
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
            GTK_POLICY_AUTOMATIC, 
            GTK_POLICY_AUTOMATIC);
   
    model = gtk_tree_store_new (1, G_TYPE_STRING);
    gtk_container_add (GTK_CONTAINER (scrolled_window), tree_view);
    gtk_tree_view_set_model (GTK_TREE_VIEW (tree_view), GTK_TREE_MODEL (model));
    gtk_widget_show (tree_view);
   
    /* Add some messages to the window */
    for (i = 0; i < 10; i++) {
        gchar *msg = g_strdup_printf ("Room %d", i);
        gtk_tree_store_append (model, &toplevel, NULL);
        gtk_tree_store_set (model, &toplevel, 0, msg, -1);
        g_free (msg);
        for (int j = 0; j < 10; j++) {
            gchar *msg = g_strdup_printf ("User %d", j);
            gtk_tree_store_append (model, &child, &toplevel);
            gtk_tree_store_set (model, &child, 0, msg, -1);
            g_free (msg);
        }
    }
   
    cell = gtk_cell_renderer_text_new ();

    column = gtk_tree_view_column_new_with_attributes ("Chat Rooms",
                                                       cell,
                                                       "text", 0,
                                                       NULL);
  
    gtk_tree_view_append_column (GTK_TREE_VIEW (tree_view),
                 GTK_TREE_VIEW_COLUMN (column));

    return scrolled_window;
}
   
/* Add some text to our text widget - this is a callback that is invoked
when our window is realized. We could also force our window to be
realized with gtk_widget_realize, but it would have to be part of
a hierarchy first */

   
/* Create a scrolled text area that displays a "message" */


void edit_text( GtkTextBuffer *buffer) {

}

int main(int argc, char **argv) {
    GtkWidget *window;
    GtkWidget *table;

    GtkWidget *hpaned;
    GtkWidget *vpaned;

    GtkWidget *textEntry;
    GtkWidget *textView;
    GtkWidget *button;
    GtkWidget *roomList;
    GtkWidget *text;
    //GtkWidget *tree_view;
    GtkTreeSelection *roomSelection;
    //GtkTextBuffer *buffer;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_widget_set_size_request (window, 500, 300);
  gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
  gtk_window_set_title(GTK_WINDOW(window), "IRC Client");
  gtk_container_set_border_width(GTK_CONTAINER(window), 15);

  vpaned = gtk_vpaned_new();
  gtk_container_add(GTK_CONTAINER(window), vpaned);

  hpaned = gtk_hpaned_new ();
    gtk_container_add (GTK_CONTAINER (vpaned), hpaned);

    tree_view = gtk_tree_view_new ();
    roomList = create_list (tree_view);
    gtk_widget_set_size_request(roomList, 130, 225);
    gtk_paned_add1 (GTK_PANED (hpaned), roomList);

    roomSelection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
    textEntry = create_text2();
    textView = create_text1();
    gtk_paned_add2 (GTK_PANED (hpaned), textView);

  table = gtk_table_new(4, 1, FALSE);
  gtk_container_add (GTK_CONTAINER(vpaned), table);
  

  button = gtk_button_new_with_label ("Send");
  gtk_table_attach(GTK_TABLE(table), button, 3, 4, 0, 1, 0, 0, 1, 1);
  gtk_table_attach(GTK_TABLE(table), textEntry, 0, 3, 0, 1,
      GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 1, 1);

  g_signal_connect_swapped(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), G_OBJECT(window));
    g_signal_connect(roomSelection, "changed", 
      G_CALLBACK(roomSelected), text);

    gtk_widget_show_all(window);
    gtk_main();

  printf("TEST ENDS\n");
  return 0;
}
