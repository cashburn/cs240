#include <gtk/gtk.h>

GtkWidget *tree_view;
GtkTextBuffer *buffer;

void on_changed(GtkWidget *widget, gpointer statusbar) 
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  char *value;


  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {
    if (!gtk_tree_model_iter_has_child(model, &iter))
        return;
    gtk_tree_model_get(model, &iter, 0, &value,  -1);
    gtk_statusbar_push(GTK_STATUSBAR(statusbar),
        gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
            value), value);
    g_free(value);
  }
}

static GtkWidget * create_list( void )
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
    tree_view = gtk_tree_view_new ();
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

static void insert_text( GtkTextBuffer *buffer )
{
   GtkTextIter iter;
 
   gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);

   gtk_text_buffer_insert (buffer, &iter,   
    "From: pathfinder@nasa.gov\n"
    "To: mom@nasa.gov\n"
    "Subject: Made it!\n"
    "\n"
    "We just got in this morning. The weather has been\n"
    "great - clear but cold, and there are lots of fun sights.\n"
    "Sojourner says hi. See you soon.\n"
    " -Path\n", -1);
}
   
/* Create a scrolled text area that displays a "message" */
static GtkWidget *create_text(GtkTextBuffer *buffer)
{
   GtkWidget *scrolled_window;
   GtkWidget *view;

   view = gtk_text_view_new ();
   buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

   scrolled_window = gtk_scrolled_window_new (NULL, NULL);
   gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                   GTK_POLICY_AUTOMATIC,
           GTK_POLICY_AUTOMATIC);

   gtk_container_add (GTK_CONTAINER (scrolled_window), view);
   insert_text (buffer);

   gtk_widget_show_all (scrolled_window);

   return scrolled_window;
}

int main( int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *table;

  GtkWidget *hpaned;
  GtkWidget *statusbar;
  GtkWidget *vbox;

  GtkWidget *title;
  GtkWidget *activate;
  GtkWidget *halign;
  GtkWidget *halign2;

  GtkWidget *valign;
  GtkWidget *roomList;
  GtkTreeSelection *selection;
  GtkWidget *text;

  GtkWidget *help;
  GtkWidget *ok;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_widget_set_size_request (window, 500, 300);
  gtk_window_set_resizable(GTK_WINDOW(window), TRUE);

  gtk_window_set_title(GTK_WINDOW(window), "IRC Client");

  gtk_container_set_border_width(GTK_CONTAINER(window), 15);

  //table = gtk_table_new(8, 4, FALSE);
  //gtk_table_set_col_spacings(GTK_TABLE(table), 3);
  
  vbox = gtk_vbox_new(FALSE, 2);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  hpaned = gtk_hpaned_new ();
  gtk_container_add (GTK_CONTAINER (vbox), hpaned);

  statusbar = gtk_statusbar_new();
  gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 1);

  roomList = create_list ();
  gtk_widget_set_size_request(roomList, 130, 100);

  gtk_paned_add1 (GTK_PANED (hpaned), roomList);

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));

  text = create_text(buffer);
  //gtk_text_view_set_editable(GTK_TEXT_VIEW(text), TRUE);
  //gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text), FALSE);
  gtk_paned_add2 (GTK_PANED (hpaned), text);
  //gtk_table_attach(GTK_TABLE(table), text, 3, 4, 1, 3,
      //GTK_FILL | GTK_EXPAND, GTK_FILL | GTK_EXPAND, 1, 1);

  
 
  //gtk_table_set_row_spacing(GTK_TABLE(table), 1, 3);
  //gtk_table_attach(GTK_TABLE(table), roomList, 0, 2, 1, 2, 
      //GTK_FILL, GTK_FILL | GTK_EXPAND, 1, 1);
/*
  halign2 = gtk_alignment_new(0, 1, 0, 0);
  help = gtk_button_new_with_label("Help");
  gtk_container_add(GTK_CONTAINER(halign2), help);
  gtk_widget_set_size_request(help, 70, 30);
  gtk_table_set_row_spacing(GTK_TABLE(table), 3, 6);
  gtk_table_attach(GTK_TABLE(table), halign2, 0, 1, 4, 5, 
      GTK_FILL, GTK_FILL, 0, 0);
*/
  
  //gtk_container_add(GTK_CONTAINER(window), table);

  g_signal_connect_swapped(G_OBJECT(window), "destroy",
        G_CALLBACK(gtk_main_quit), G_OBJECT(window));
  g_signal_connect(selection, "changed", 
      G_CALLBACK(on_changed), statusbar);

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}