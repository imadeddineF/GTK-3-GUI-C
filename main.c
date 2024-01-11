#include <gtk/gtk.h>
#include <stdbool.h>

typedef struct Node {
  int value;
  struct Node *next;
  double opacity;  // Opacity for animation
} Node;
Node *head = NULL;

// CSS for modern styling
const gchar *css_style =
    "button {"
    "  background-color: #3498db;"
    "  color: #ffffff;"
    "  border: 1px solid #2980b9;"
    "  border-radius: 10px;"
    "  padding: 5px 10px;"
    "  font-size: 14px;"
    "  transition: background-color 0.3s, color 0.3s;"
    "}"

    "button:hover {"
    "  background-color: #2980b9;"
    "  color: #ffffff;"
    "}"

    "label {"
    "  color: #ffffff;"
    "}"

    "entry {"
    "  margin: 5px;"
    "  padding: 5px;"
    "  border-radius: 10px;"
    "  color: #000000;"
    "  font-size: 16px;"
    "  background-color: #ffffff;"
    "  transition: border-color 0.3s;"
    "}"

    "entry:focus {"
    "  border-color: #2980b9;"
    "}"

    // Additional styles for animation during search
    "entry.searching {"
    "  border-color: #f39c12 !important;"
    "  transition: border-color 0.5s;"
    "}"

    "node.found {"
    "  fill: #27ae60 !important;"  // Green color for found node
    "  transition: fill 0.5s;"
    "}";

gboolean animate(GtkWidget *widget);

// Function declarations
void add_node(int value);
void add_node_at_head(int value);
void add_node_before_value(int new_value, int target_value);
void delete_node(int value);
bool delete_node_after_value(int target_value);
Node *search_node(int value);
void selection_sort();
void insertion_sort();
void bubble_sort();
void delete_node_head();
void delete_node_tail();
void clear_list();
void initialize_opacity(Node *node);

// Other function declarations
void draw_capsule(GtkWidget *widget, cairo_t *cr, gint x, gint y, gint width, gint height, gchar *text, gboolean is_head);
void draw_linked_list(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void clear_entry(GtkEntry *entry, gpointer user_data);
void on_insert_button_clicked(GtkButton *button, gpointer user_data);
void on_delete_button_clicked(GtkButton *button, gpointer user_data);
void on_delete_head_button_clicked(GtkButton *button, gpointer user_data);
void on_delete_tail_button_clicked(GtkButton *button, gpointer user_data);
void on_search_button_clicked(GtkButton *button, gpointer user_data);
void on_sort_button_clicked(GtkButton *button, gpointer user_data);
void on_clear_button_clicked(GtkButton *button, gpointer user_data);
void show_message(const gchar *message);

// The main function
int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  GtkWidget *drawing_area = gtk_drawing_area_new();
  g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_linked_list), NULL);
  GtkWidget *insert_button = gtk_button_new_with_label("Insert");
  GtkWidget *delete_button = gtk_button_new_with_label("Delete");
  GtkWidget *delete_head_button = gtk_button_new_with_label("Delete Head");
  GtkWidget *delete_tail_button = gtk_button_new_with_label("Delete Tail");
  GtkWidget *search_button = gtk_button_new_with_label("Search");
  GtkWidget *sort_button = gtk_button_new_with_label("Sort");
  GtkWidget *clear_button = gtk_button_new_with_label("Clear");
  GtkWidget *entry = gtk_entry_new();
  GtkWidget *entry_label = gtk_label_new("Enter value:");
  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(window), vbox);
  gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 5);
  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), entry_label, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), insert_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), delete_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), delete_head_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), delete_tail_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), search_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), sort_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), clear_button, FALSE, FALSE, 5);
  g_object_set_data(G_OBJECT(drawing_area), "entry", entry);
  g_object_set_data(G_OBJECT(drawing_area), "drawing_area", drawing_area);
  g_signal_connect(insert_button, "clicked", G_CALLBACK(on_insert_button_clicked), drawing_area);
  g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_button_clicked), drawing_area);
  g_signal_connect(delete_head_button, "clicked", G_CALLBACK(on_delete_head_button_clicked), drawing_area);
  g_signal_connect(delete_tail_button, "clicked", G_CALLBACK(on_delete_tail_button_clicked), drawing_area);
  g_signal_connect(search_button, "clicked", G_CALLBACK(on_search_button_clicked), drawing_area);
  g_signal_connect(sort_button, "clicked", G_CALLBACK(on_sort_button_clicked), drawing_area);
  g_signal_connect(clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), drawing_area);
  g_signal_connect(entry, "activate", G_CALLBACK(clear_entry), NULL);
  gtk_window_set_default_size(GTK_WINDOW(window), 1000, 600);
  gtk_window_set_title(GTK_WINDOW(window), "Linked List");
  GtkCssProvider *provider = gtk_css_provider_new();
  const gchar *css = "window { background: url('./assets/background.png') repeat; }";
  GError *error = NULL;
  gtk_css_provider_load_from_data(provider, css, -1, &error);
  // Set up animation
  g_timeout_add(100, (GSourceFunc)animate, drawing_area);
  // Apply CSS styling
  GtkCssProvider *cssProvider = gtk_css_provider_new();
  gtk_css_provider_load_from_data(cssProvider, css_style, -1, NULL);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  gtk_widget_show_all(window);
  gtk_main();
  return 0;
}
// Function to add a new node with the given value to the end of the linked list
void add_node(int value) {
  // Create a new node and allocate memory for it
  Node *new_node = g_malloc(sizeof(Node));
  new_node->value = value;
  new_node->next = NULL;
  initialize_opacity(new_node);  // Initialize opacity for the new node

  // If the list is empty, make the new node the head
  if (head == NULL) {
    head = new_node;
    return;
  }

  // Traverse to the end of the list and add the new node
  Node *last = head;
  while (last->next != NULL) {
    last = last->next;
  }
  last->next = new_node;
}

// Function to add a new node with the given value at the head of the linked list
void add_node_at_head(int value) {
  // Create a new node and allocate memory for it
  Node *new_node = (Node *)g_malloc(sizeof(Node));
  new_node->value = value;

  // If the list is empty, make the new node the head
  if (head == NULL) {
    new_node->next = NULL;
    head = new_node;
  } else {
    // Make the new node the head and update the next pointer
    new_node->next = head;
    head = new_node;
  }
}

// Function to add a new node with the given value before a node with the target value
void add_node_before_value(int new_value, int target_value) {
  // Create a new node and allocate memory for it
  Node *new_node = (Node *)g_malloc(sizeof(Node));
  new_node->value = new_value;

  Node *current = head;
  Node *prev = NULL;

  // Traverse the list to find the node with the target value
  while (current != NULL && current->value != target_value) {
    prev = current;
    current = current->next;
  }

  // If the target value is found, insert the new node before it
  if (current != NULL) {
    if (prev != NULL) {
      new_node->next = current;
      prev->next = new_node;
    } else {
      new_node->next = head;
      head = new_node;
    }
  }
}

// Function to delete a node with the given value from the linked list
void delete_node(int value) {
  Node *current = head;
  Node *prev = NULL;

  // Traverse the list to find the node with the target value
  while (current != NULL && current->value != value) {
    prev = current;
    current = current->next;
  }

  // If the target value is found, delete the node
  if (current != NULL) {
    if (prev != NULL) {
      prev->next = current->next;
    } else {
      head = current->next;
    }
    g_free(current);
  }
}

// Function to search for a node with the given value in the linked list
Node *search_node(int value) {
  Node *current = head;

  // Traverse the list to find the node with the target value
  while (current != NULL && current->value != value) {
    current = current->next;
  }

  return current;
}

// Function to sort the linked list using the selection sort algorithm
void selection_sort() {
  Node *i, *j;
  int temp;

  // Iterate over the list and compare each pair of nodes
  for (i = head; i != NULL; i = i->next) {
    for (j = i->next; j != NULL; j = j->next) {
      // Swap the values if the current node is greater than the next node
      if (i->value > j->value) {
        temp = i->value;
        i->value = j->value;
        j->value = temp;
      }
    }
  }
}

// Function to sort the linked list using the insertion sort algorithm
void insertion_sort() {
  // If the list is empty or has only one node, no sorting is required
  if (head == NULL || head->next == NULL)
    return;

  Node *sorted = NULL;
  Node *current = head;

  // Iterate over the list and insert each node into the sorted portion of the list
  while (current != NULL) {
    Node *next = current->next;
    if (sorted == NULL || sorted->value >= current->value) {
      // Insert the current node at the beginning of the sorted list
      current->next = sorted;
      sorted = current;
    } else {
      // Find the correct position to insert the current node in the sorted list
      Node *temp = sorted;
      while (temp->next != NULL && temp->next->value < current->value) {
        temp = temp->next;
      }
      current->next = temp->next;
      temp->next = current;
    }
    current = next;
  }
  head = sorted;  // Update the head of the list
}

// Function to sort the linked list using the bubble sort algorithm
void bubble_sort() {
  int swapped;
  Node *ptr1;
  Node *lptr = NULL;

  // Iterate over the list and compare each pair of adjacent nodes
  if (head == NULL)
    return;
  do {
    swapped = 0;
    ptr1 = head;
    while (ptr1->next != lptr) {
      // Swap the values if the current node is greater than the next node
      if (ptr1->value > ptr1->next->value) {
        int temp = ptr1->value;
        ptr1->value = ptr1->next->value;
        ptr1->next->value = temp;
        swapped = 1;
      }
      ptr1 = ptr1->next;
    }
    lptr = ptr1;
  } while (swapped);
}

// Function to delete the head node from the linked list
void delete_node_head() {
  if (head != NULL) {
    Node *temp = head;
    head = head->next;
    g_free(temp);
  }
}

// Function to delete the tail node from the linked list
void delete_node_tail() {
  if (head != NULL) {
    Node *current = head;
    Node *prev = NULL;

    // Traverse the list to find the tail node
    while (current->next != NULL) {
      prev = current;
      current = current->next;
    }

    // Delete the tail node
    if (prev != NULL) {
      prev->next = NULL;
      g_free(current);
    } else {
      g_free(current);
      head = NULL;
    }
  }
}

// Function to delete the node after a node with the target value from the linked list
bool delete_node_after_value(int target_value) {
  Node *current = head;
  Node *prev = NULL;

  // Traverse the list to find the node with the target value
  while (current != NULL && current->value != target_value) {
    prev = current;
    current = current->next;
  }

  // If the target value is found and there is a node after it, delete the next node
  if (current != NULL && current->next != NULL) {
    Node *node_to_delete = current->next;
    current->next = node_to_delete->next;
    if (node_to_delete == head) {
      head = node_to_delete->next;
    }
    g_free(node_to_delete);
    return true;
  }
  return false;
}

// Function to display a message dialog with the given message
void show_message(const gchar *message) {
  GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", message);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

// Function to draw a capsule shape with the given properties
void draw_capsule(GtkWidget *widget, cairo_t *cr, gint x, gint y, gint width, gint height, gchar *text, gboolean is_head) {
  // Draw the capsule frame
  gint radius = height / 2;
  cairo_set_source_rgb(cr, 0, 0.45, 0.73);  // French Blue for frame
  cairo_move_to(cr, x + radius, y);
  cairo_line_to(cr, x + width - radius, y);
  cairo_arc(cr, x + width - radius, y + radius, radius, -G_PI / 2, 0);
  cairo_line_to(cr, x + width, y + height - radius);
  cairo_arc(cr, x + width - radius, y + height - radius, radius, 0, G_PI / 2);
  cairo_line_to(cr, x + radius, y + height);
  cairo_arc(cr, x + radius, y + height - radius, radius, G_PI / 2, G_PI);
  cairo_line_to(cr, x, y + radius);
  cairo_arc(cr, x + radius, y + radius, radius, G_PI, 3 * G_PI / 2);
  cairo_stroke_preserve(cr);

  // Set a background color
  cairo_set_source_rgb(cr, 0.69, 0.88, 0.9);  // Baby Blue for fill
  cairo_fill(cr);

  // Center the text inside the capsule
  PangoLayout *layout = gtk_widget_create_pango_layout(widget, text);
  PangoRectangle rect;
  pango_layout_get_pixel_extents(layout, NULL, &rect);

  gint text_x = x + (width - rect.width) / 2;
  gint text_y = y + (height - rect.height) / 2;

  // Set the text color
  if (is_head) {
    cairo_set_source_rgb(cr, 0, 0.45, 0.73);  // French Blue for head text
  } else {
    cairo_set_source_rgb(cr, 0, 0, 0);  // Black color for the text
  }

  cairo_move_to(cr, text_x, text_y);
  pango_cairo_show_layout(cr, layout);
  g_object_unref(layout);
}

// Function to draw the linked list on a drawing area widget
void draw_linked_list(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  Node *current = head;
  gint x = 50, y = 50;
  gint capsule_width = 80;
  gint capsule_height = 40;
  gint arrow_distance = 20;

  // Draw the head node with an arrow pointing to the first node
  if (current != NULL) {
    draw_capsule(widget, cr, x, y, capsule_width, capsule_height, "head", TRUE);
    // Draw an arrow from head to the first node
    cairo_set_source_rgb(cr, 0, 0.45, 0.73);  // French Blue
    cairo_move_to(cr, x + capsule_width, y + capsule_height / 2);
    cairo_line_to(cr, x + capsule_width + arrow_distance, y + capsule_height / 2);
    cairo_stroke(cr);
    cairo_move_to(cr, x + capsule_width + arrow_distance, y + capsule_height / 2);
    cairo_line_to(cr, x + capsule_width + arrow_distance - 10, y + capsule_height / 2 - 5);
    cairo_line_to(cr, x + capsule_width + arrow_distance - 10, y + capsule_height / 2 + 5);
    cairo_close_path(cr);
    cairo_fill(cr);
    x += capsule_width + arrow_distance;
  }

  // Draw the remaining nodes in the linked list
  while (current != NULL) {
    gchar value_str[10];
    if (current->value >= 0 && current->value < 10) {
      // Add a leading zero for numbers 0 to 9
      g_snprintf(value_str, sizeof(value_str), "0%d", current->value);
    } else {
      g_snprintf(value_str, sizeof(value_str), "%d", current->value);
    }

    // Set the opacity based on the animation progress
    cairo_set_source_rgba(cr, 0, 0, 0, current->opacity);
    draw_capsule(widget, cr, x, y, strlen(value_str) * 10 + 20, capsule_height, value_str, FALSE);
    x += strlen(value_str) * 10 + 20 + arrow_distance;

    if (current->next != NULL) {
      cairo_set_source_rgb(cr, 0, 0.45, 0.73);  // French Blue
      cairo_move_to(cr, x - arrow_distance, y + capsule_height / 2);
      cairo_line_to(cr, x, y + capsule_height / 2);
      cairo_stroke(cr);
      cairo_move_to(cr, x, y + capsule_height / 2);
      cairo_line_to(cr, x - 10, y + capsule_height / 2 - 5);
      cairo_line_to(cr, x - 10, y + capsule_height / 2 + 5);
      cairo_close_path(cr);
      cairo_fill(cr);
    }
    current = current->next;
  }

  // Add animation for capsule movement
  static gint animation_step = 0;
  if (animation_step < x) {
    x -= 5;  // Adjust the step size as needed
    animation_step += 1;
    gtk_widget_queue_draw(widget);
  }
}

// Function to animate the opacity of the nodes in the linked list
gboolean animate(GtkWidget *widget) {
  Node *current = head;
  while (current != NULL) {
    if (current->opacity < 1.0) {
      current->opacity += 0.3;
      gtk_widget_queue_draw(widget);
    }
    current = current->next;
  }
  return G_SOURCE_CONTINUE;
}

// Function to initialize the opacity of a node to 0.0
void initialize_opacity(Node *node) {
  node->opacity = 0.0;
}

// Function to clear the text entry widget
void clear_entry(GtkEntry *entry, gpointer user_data) {
  gtk_entry_set_text(entry, "");
}

// Event handler for the insert button click event
void on_insert_button_clicked(GtkButton *button, gpointer user_data) {
  GtkWidget *entry = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "entry"));
  const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
  int value = atoi(text);
  add_node(value);
  gtk_entry_set_text(GTK_ENTRY(entry), "");  // Clear the entry
  gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
}

// Event handler for the delete button click event
void on_delete_button_clicked(GtkButton *button, gpointer user_data) {
  GtkWidget *entry = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "entry"));
  const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
  int value = atoi(text);
  delete_node(value);
  gtk_entry_set_text(GTK_ENTRY(entry), "");  // Clear the entry
  gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
}

// Event handler for the delete head button click event
void on_delete_head_button_clicked(GtkButton *button, gpointer user_data) {
  delete_node_head();  // Call the function to delete the head
  gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
}

// Event handler for the delete tail button click event
void on_delete_tail_button_clicked(GtkButton *button, gpointer user_data) {
  delete_node_tail();  // Call the function to delete the tail
  gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
}

// Event handler for the search button click event
void on_search_button_clicked(GtkButton *button, gpointer user_data) {
  GtkWidget *entry = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "entry"));
  const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
  int value = atoi(text);

  // Set a flag to indicate the searching state
  gboolean searching = TRUE;

  Node *current = head;
  gboolean found = FALSE;

  // Traverse the list to find the node with the target value
  while (current != NULL) {
    if (current->value == value) {
      found = TRUE;

      // Remove the searching style from the entry
      GtkStyleContext *context = gtk_widget_get_style_context(entry);
      gtk_style_context_remove_class(context, "searching");

      // Apply found style to the found node
      gchar node_name[20];
      g_snprintf(node_name, sizeof(node_name), "node-%d", current->value);
      GtkWidget *drawing_area = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area"));
      gtk_widget_set_name(gtk_bin_get_child(GTK_BIN(drawing_area)), "");  // Remove existing node style
      gtk_widget_set_name(gtk_bin_get_child(GTK_BIN(drawing_area)), node_name);
      break;
    }

    // Perform animation for the current node
    current->opacity = 0.0;
    gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
    g_usleep(100000);  // Sleep for 100,000 microseconds (0.1 seconds)

    current = current->next;
  }

  // Display the result based on the search outcome
  if (found) {
    // Element found
    current->opacity = 1.0;  // Highlight the found node
    gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
    g_usleep(2000000);  // Sleep for 2,000,000 microseconds (2 seconds)

    gchar message[100];
    g_snprintf(message, sizeof(message), "THE NUMBER %d EXISTS IN THE LIST", current->value);
    show_message(message);
  } else {
    // Element not found
    gchar message[100];
    g_snprintf(message, sizeof(message), "THE NUMBER %d DOES NOT EXIST IN THE LIST", value);
    show_message(message);
  }

  // Reset the searching state flag
  searching = FALSE;
}

// Event handler for the sort button click event
void on_sort_button_clicked(GtkButton *button, gpointer user_data) {
  selection_sort();
  gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
}

// Function to clear the linked list
void clear_list() {
  while (head != NULL) {
    Node *temp = head;
    head = head->next;
    g_free(temp);
  }
}

// Event handler for the clear button click event
void on_clear_button_clicked(GtkButton *button, gpointer user_data) {
  clear_list();
  gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
}
