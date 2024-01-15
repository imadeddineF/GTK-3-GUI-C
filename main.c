#include <gtk/gtk.h>
#include <stdbool.h>

typedef struct Node {
  int value;
  struct Node *next;
  double opacity;  // Opacity for search animation
} Node;
Node *head;
// check if the list has been created
bool created = false;

// CSS for modern styling
const gchar *css_style =
    "button {"
    "  background-color: #3498db;"
    "  color: #ffffff;"
    "  border: 1px solid #2980b9;"
    "  border-radius: 10px;"
    "  padding: 5px 10px;"
    "  font-size: 14px;"
    "}"

    "entry {"
    "  margin: 5px;"
    "  padding: 5px;"
    "  border-radius: 10px;"
    "  color: #000000;"
    "  font-size: 16px;"
    "  background-color: #ffffff;"
    "}"

    "entry:focus {"
    "  border-color: #2980b9;"
    "}";

// The animation function declaration
gboolean animate(GtkWidget *widget);

// Logic function declarations
Node *create_list();
void add_node_at_head(int value);
void add_node_at_tail(int value);
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

// UI function declarations
void draw_capsule(GtkWidget *widget, cairo_t *cr, gint x, gint y, gint width, gint height, gchar *text, gboolean is_head);
void draw_linked_list(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void clear_entry(GtkEntry *entry, gpointer user_data);
void on_create_list_button_clicked(GtkButton *button, gpointer user_data);
void on_insert_head_button_clicked(GtkButton *button, gpointer user_data);
void on_insert_tail_button_clicked(GtkButton *button, gpointer user_data);
void on_delete_button_clicked(GtkButton *button, gpointer user_data);
void on_delete_head_button_clicked(GtkButton *button, gpointer user_data);
void on_delete_tail_button_clicked(GtkButton *button, gpointer user_data);
void on_search_button_clicked(GtkButton *button, gpointer user_data);
void on_sort_button_clicked(GtkButton *button, gpointer user_data);
void on_clear_button_clicked(GtkButton *button, gpointer user_data);
void show_message(const gchar *message);

// The main function of the GTK application
int main(int argc, char *argv[]) {
  // Initialize GTK
  gtk_init(&argc, &argv);

  // Create a new top-level window
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  // Connect the 'destroy' event to the main_quit function to close the application
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  // Create a drawing area widget for custom drawings
  GtkWidget *drawing_area = gtk_drawing_area_new();
  // Connect the 'draw' event for the drawing area to the draw_linked_list callback function
  g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_linked_list), NULL);

  // Create buttons for various actions in the application
  GtkWidget *create_list = gtk_button_new_with_label("Create");
  GtkWidget *insert_head_button = gtk_button_new_with_label("Insert h");
  GtkWidget *insert_tail_button = gtk_button_new_with_label("Insert t");
  GtkWidget *delete_button = gtk_button_new_with_label("Delete");
  GtkWidget *delete_head_button = gtk_button_new_with_label("Delete h");
  GtkWidget *delete_tail_button = gtk_button_new_with_label("Delete t");
  GtkWidget *search_button = gtk_button_new_with_label("Search");
  GtkWidget *sort_button = gtk_button_new_with_label("Sort");
  GtkWidget *clear_button = gtk_button_new_with_label("Clear");

  // Create an entry widget for user input
  GtkWidget *entry = gtk_entry_new();
  // Create a label for the entry widget
  GtkWidget *entry_label = gtk_label_new("Enter value:");

  // Create a vertical box to organize widgets vertically
  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  // Add the vertical box to the main window
  gtk_container_add(GTK_CONTAINER(window), vbox);
  // Add the drawing area to the vertical box
  gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 5);

  // Create a horizontal box to organize some widgets horizontally
  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  // Add the horizontal box to the vertical box
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);

  // Add the entry label and entry widget to the horizontal box
  gtk_box_pack_start(GTK_BOX(hbox), entry_label, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 5);

  // Add all buttons to the horizontal box
  gtk_box_pack_start(GTK_BOX(hbox), create_list, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), insert_head_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), insert_tail_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), delete_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), delete_head_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), delete_tail_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), search_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), sort_button, FALSE, FALSE, 5);
  gtk_box_pack_start(GTK_BOX(hbox), clear_button, FALSE, FALSE, 5);

  // Store references to the entry and drawing_area widgets for later use
  g_object_set_data(G_OBJECT(drawing_area), "entry", entry);
  g_object_set_data(G_OBJECT(drawing_area), "drawing_area", drawing_area);

  // Connect 'clicked' signals of buttons to their respective callback functions
  g_signal_connect(create_list, "clicked", G_CALLBACK(on_create_list_button_clicked), drawing_area);
  g_signal_connect(insert_head_button, "clicked", G_CALLBACK(on_insert_head_button_clicked), drawing_area);
  g_signal_connect(insert_tail_button, "clicked", G_CALLBACK(on_insert_tail_button_clicked), drawing_area);
  g_signal_connect(delete_button, "clicked", G_CALLBACK(on_delete_button_clicked), drawing_area);
  g_signal_connect(delete_head_button, "clicked", G_CALLBACK(on_delete_head_button_clicked), drawing_area);
  g_signal_connect(delete_tail_button, "clicked", G_CALLBACK(on_delete_tail_button_clicked), drawing_area);
  g_signal_connect(search_button, "clicked", G_CALLBACK(on_search_button_clicked), drawing_area);
  g_signal_connect(sort_button, "clicked", G_CALLBACK(on_sort_button_clicked), drawing_area);
  g_signal_connect(clear_button, "clicked", G_CALLBACK(on_clear_button_clicked), drawing_area);

  // Connect the 'activate' signal of the entry to the clear_entry callback function
  g_signal_connect(entry, "activate", G_CALLBACK(clear_entry), NULL);

  // Set the default size and title of the window
  gtk_window_set_default_size(GTK_WINDOW(window), 1000, 600);
  gtk_window_set_title(GTK_WINDOW(window), "Linked List");

  // Create and configure a CSS provider for custom styling
  GtkCssProvider *provider = gtk_css_provider_new();
  const gchar *css = "window { background: url('./assets/background.png') repeat; }";
  GError *error = NULL;
  gtk_css_provider_load_from_data(provider, css, -1, &error);

  // Set up a periodic animation function to animate the opacity of the nodes
  g_timeout_add(100, (GSourceFunc)animate, drawing_area);

  // Create another CSS provider for additional styling
  GtkCssProvider *cssProvider = gtk_css_provider_new();
  // Load CSS styles defined in a global css_style variable
  gtk_css_provider_load_from_data(cssProvider, css_style, -1, NULL);
  // Apply CSS styling to the screen
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  // Show all widgets within the window
  gtk_widget_show_all(window);

  // Run the GTK main event loop
  gtk_main();

  return 0;
}

// Function to create the head of the linked list without initializing the first node
Node *create_list() {
  Node *head = NULL;
  created = true;
  // Show a text message to indicate that the list has been created
  show_message("The list has been created successfully!");
  return head;
}

// Function to add a new node with the given value at the head of the linked list
void add_node_at_head(int value) {
  // Create a new node and allocate memory for it
  if (created == true) {
    Node *new_node = (Node *)g_malloc(sizeof(Node));
    new_node->value = value;
    initialize_opacity(new_node);  // Initialize opacity for the new node
    // Make the new node the head and update the next pointer
    new_node->next = head;
    head = new_node;
  } else {
    show_message("Create a list first!");
  }
}

// Function to add a new node with the given value at the tail of the linked list
void add_node_at_tail(int value) {
  // Create a new node and allocate memory for it
  Node *new_node = (Node *)g_malloc(sizeof(Node));
  new_node->value = value;
  new_node->next = NULL;
  initialize_opacity(new_node);  // Initialize opacity for the new node

  if (created == true) {
    if (head == NULL) {
      // If the list is empty, make the new node the head
      head = new_node;
    } else {
      // Traverse to the end of the list and add the new node
      Node *last = head;
      while (last->next != NULL) {
        last = last->next;
      }
      last->next = new_node;
    }
  } else {
    show_message("Create a list first!");
    g_free(new_node);  // Free the memory allocated for the new node
  }
}

// Function to delete a node with the given value from the linked list
void delete_node(int value) {
  Node *current = head;
  Node *prev = NULL;

  if (created == true) {
    if (head != NULL) {
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
        show_message("The number has been deleted successfully!");
      } else {
        show_message("The number does not exist in the list!");
      }
    } else {
      show_message("This list is empty!");
    }
  } else {
    show_message("There is no list to delete from!");
  }
}

// Function to delete the head node from the linked list
void delete_node_head() {
  if (created == true) {
    if (head != NULL) {
      Node *temp = head;
      head = head->next;
      g_free(temp);
      show_message("The head node has been deleted successfully!");
    } else {
      show_message("This list is empty!");
    }
  } else {
    show_message("There is no list to delete from!");
  }
}

// Function to delete the tail node from the linked list
void delete_node_tail() {
  Node *current = head;
  Node *prev = NULL;
  if (created == true) {
    if (head != NULL) {
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
      show_message("The tail node has been deleted successfully!");
    } else {
      show_message("This list is empty!");
    }
  } else {
    show_message("There is no list to delete from!");
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

// Function to clear the linked list
void clear_list() {
  while (head != NULL) {
    Node *temp = head;
    head = head->next;
    g_free(temp);
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

// We add these two logic functions without UI implementation
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
  show_message("The list has been sorted successfully!");
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

// Function to display a message dialog with the given message
void show_message(const gchar *message) {
  GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", message);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
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

// Event handler for the creation of button click event
void on_create_list_button_clicked(GtkButton *button, gpointer user_data) {
  // Check if the list has already been created
  if (!created) {
    head = create_list();
  }
  gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
}

// Event handler for the insert at the head button click event
void on_insert_head_button_clicked(GtkButton *button, gpointer user_data) {
  GtkWidget *entry = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "entry"));
  const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
  int value = atoi(text);

  add_node_at_head(value);  // Corrected function call

  gtk_entry_set_text(GTK_ENTRY(entry), "");  // Clear the entry
  gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
}

// Event handler for the insert at the head button click event
void on_insert_tail_button_clicked(GtkButton *button, gpointer user_data) {
  GtkWidget *entry = GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "entry"));
  const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
  int value = atoi(text);
  add_node_at_tail(value);
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
    g_usleep(100);  // Sleep for 100,000 microseconds (0.1 seconds)

    current = current->next;
  }

  // Display the result based on the search outcome
  if (found) {
    // Element found
    current->opacity = 1.0;  // Highlight the found node
    gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
    g_usleep(200);  // Sleep for 2,000,000 microseconds (2 seconds)

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
  insertion_sort();
  // selection_sort();
  // bubble_sort();

  gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
}

// Event handler for the clear button click event
void on_clear_button_clicked(GtkButton *button, gpointer user_data) {
  clear_list();
  gtk_widget_queue_draw(GTK_WIDGET(g_object_get_data(G_OBJECT(user_data), "drawing_area")));
}