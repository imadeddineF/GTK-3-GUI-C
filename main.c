#include <gtk/gtk.h>

static void activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *label;

  // Create a new window
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Hello GTK");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

  // Create a new label with the text "Hello, World!"
  label = gtk_label_new("Hello World!");

  // Add the label to the window
  gtk_container_add(GTK_CONTAINER(window), label);

  // Show all widgets in the window
  gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
  GtkApplication *app;
  int status;

  // Create a new GTK application
  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);

  // Connect the "activate" signal to the activate function
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  // Run the application
  status = g_application_run(G_APPLICATION(app), argc, argv);

  // Release resources
  g_object_unref(app);

  return status;
}
