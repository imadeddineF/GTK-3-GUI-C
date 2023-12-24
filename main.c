#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#define M_PI 3.14159265358979323846

typedef struct Node
{
  int value;
  struct Node *next;
} Node;

Node *head = NULL;

void add_node(int value)
{
  Node *new_node = g_malloc(sizeof(Node));
  new_node->value = value;
  new_node->next = NULL;

  if (head == NULL)
  {
    head = new_node;
    return;
  }

  Node *last = head;
  while (last->next != NULL)
  {
    last = last->next;
  }

  last->next = new_node;
}

void delete_node(int value)
{
  Node *current = head;
  Node *prev = NULL;

  while (current != NULL && current->value != value)
  {
    prev = current;
    current = current->next;
  }

  if (current != NULL)
  {
    if (prev != NULL)
    {
      prev->next = current->next;
    }
    else
    {
      head = current->next;
    }

    g_free(current);
  }
}

Node *search_node(int value)
{
  Node *current = head;

  while (current != NULL && current->value != value)
  {
    current = current->next;
  }

  return current;
}

void selection_sort()
{
  Node *i, *j;
  int temp;

  for (i = head; i != NULL; i = i->next)
  {
    for (j = i->next; j != NULL; j = j->next)
    {
      if (i->value > j->value)
      {
        temp = i->value;
        i->value = j->value;
        j->value = temp;
      }
    }
  }
}

void show_message(const gchar *message)
{
  GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", message);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void draw_linked_list(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
  Node *current = head;
  int x = 50, y = 50;

  while (current != NULL)
  {
    // Draw circle filled with baby blue
    cairo_set_source_rgb(cr, 0.69, 0.88, 0.9); // Baby Blue
    cairo_arc(cr, x + 20, y + 20, 20, 0, 2 * M_PI);
    cairo_fill_preserve(cr);

    // Draw border with french blue
    cairo_set_source_rgb(cr, 0, 0.45, 0.73); // French Blue
    cairo_set_line_width(cr, 2.0);
    cairo_stroke(cr);

    // Draw text
    cairo_set_source_rgb(cr, 0, 0, 0);
    char value_str[10];
    snprintf(value_str, sizeof(value_str), "%d", current->value);
    cairo_move_to(cr, x + 15, y + 25);
    cairo_show_text(cr, value_str);

    x += 60;

    if (current->next != NULL)
    {
      // Draw arrow with french blue
      cairo_set_source_rgb(cr, 0, 0.45, 0.73); // French Blue
      cairo_move_to(cr, x - 20, y + 20);
      cairo_line_to(cr, x, y + 20);
      cairo_stroke(cr);

      // Draw arrowhead
      cairo_move_to(cr, x, y + 20);
      cairo_line_to(cr, x - 10, y + 15);
      cairo_line_to(cr, x - 10, y + 25);
      cairo_close_path(cr);
      cairo_fill(cr);
    }

    current = current->next;
  }
}

void clear_entry(GtkEntry *entry, gpointer user_data)
{
  gtk_entry_set_text(entry, "");
}

void on_insert_button_clicked(GtkButton *button, gpointer user_data)
{
  GtkWidget *entry = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "entry"));
  const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));

  int value = atoi(text);
  add_node(value);

  gtk_entry_set_text(GTK_ENTRY(entry), ""); // Clear the entry
  gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
}

void on_delete_button_clicked(GtkButton *button, gpointer user_data)
{
  GtkWidget *entry = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "entry"));
  const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));

  int value = atoi(text);
  delete_node(value);

  gtk_entry_set_text(GTK_ENTRY(entry), ""); // Clear the entry
  gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
}

void on_search_button_clicked(GtkButton *button, gpointer user_data)
{
  GtkWidget *entry = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "entry"));
  const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));

  int value = atoi(text);
  Node *result = search_node(value);

  if (result != NULL)
  {
    gchar message[100];
    g_snprintf(message, sizeof(message), "THE NUMBER '%d' EXISTS IN THE LIST.", result->value);
    show_message(message);
  }
  else
  {
    show_message("THE NUMBER DOES NOT EXIST IN THE LIST");
  }
}

void on_sort_button_clicked(GtkButton *button, gpointer user_data)
{
  selection_sort();
  gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
}

int main(int argc, char *argv[])
{
  gtk_init(&argc, &argv);

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget *drawing_area = gtk_drawing_area_new();
  g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_linked_list), NULL);

  GtkWidget *insert_button = gtk_button_new_with_label("Insert");
  GtkWidget *delete_button = gtk_button_new_with_label("Delete");
  GtkWidget *search_button = gtk_button_new_with_label("Search");
  GtkWidget *sort_button = gtk_button_new_with_label("Sort");

  GtkWidget *entry = gtk_entry_new();
  GtkWidget *entry_label = gtk_label_new("Enter value:");

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  // Drawing area at the top
  gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 5);

  // Input area below
  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);

  gtk_box_pack_start(GTK_BOX(hbox), entry_label, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), entry, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), insert_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), delete_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), search_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), sort_button, FALSE, FALSE, 5);

  // Set data for entry and drawing area to be accessed in the callback
  g_object_set_data(G_OBJECT(drawing_area), "entry", entry);
  g_object_set_data(G_OBJECT(drawing_area), "drawing_area", drawing_area);

  g_signal_connect(insert_button, "clicked", G_CALLBACK(on_insert_button_clicked), drawing_area);
  g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_button_clicked), drawing_area);
  g_signal_connect(search_button, "clicked", G_CALLBACK(on_search_button_clicked), drawing_area);
  g_signal_connect(sort_button, "clicked", G_CALLBACK(on_sort_button_clicked), drawing_area);

  g_signal_connect(entry, "activate", G_CALLBACK(clear_entry), NULL);

  // Set window size
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

  // Set window title
  gtk_window_set_title(GTK_WINDOW(window), "Linked List");

  // Set light blue background using CSS
  GtkCssProvider *provider = gtk_css_provider_new();
  const gchar *css = "window { background-color: #ADD8E6; }";
  GError *error = NULL;
  gtk_css_provider_load_from_data(provider, css, -1, &error);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}