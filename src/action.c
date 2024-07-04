#include <gtk/gtk.h>
#include "types.h"
#include "brush.h"
#include "canvas.h"

static GtkWindow *parent = NULL;

// Save new pixel value
static void destroy (GtkWidget* self, gpointer user_data) {
  set_brush_size (gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (self)));
}

// Create a modal for pixel size
static GtkWindow *create_modal_dialog(gpointer user_data) {
  GtkWindow *window = GTK_WINDOW (gtk_window_new());

  gtk_window_set_modal(window, TRUE);
  gtk_window_set_transient_for(window, parent);

  GtkWidget *frame = gtk_frame_new("New pixel size");
  GtkWidget *spin = gtk_spin_button_new_with_range(LOWEST_PIXEL_SIZE, HIGHEST_PIXEL_SIZE, 1);

  gtk_frame_set_child(GTK_FRAME (frame), spin);
  gtk_window_set_child(window, GTK_WIDGET (frame));

  g_signal_connect_after(spin, "destroy", G_CALLBACK (destroy), user_data);

  return window;
}

static void clicked_callback (GtkButton* self, gpointer user_data) {
  GtkWindow *window = create_modal_dialog(user_data);

  gtk_window_present(window);
}

// Init save process
static void on_save_response (GObject *source, GAsyncResult *result, gpointer user_data)
{
  GtkFileDialog *dialog = GTK_FILE_DIALOG (source);

  g_autoptr (GFile) file = gtk_file_dialog_save_finish (dialog, result, NULL);

  if (file != NULL)
    save_canvas (file, user_data);
}

static void save_callback (GtkButton* self, gpointer user_data) {
  GtkFileDialog *file_dialog = gtk_file_dialog_new ();

  gtk_file_dialog_save (file_dialog, parent, NULL, on_save_response, user_data);
}

void activate_action(GtkBuilder *builder, GtkWidget *window, gpointer user_data) {
    GObject *action_bar = gtk_builder_get_object(builder, "action_bar");

    gtk_widget_set_size_request (GTK_WIDGET (action_bar), DEFAULT_PIXEL_SIZE, DEFAULT_PIXEL_SIZE);

    // Size
    GObject *size_button = gtk_builder_get_object(builder, "pixel_button");

    g_signal_connect(GTK_BUTTON (size_button), "clicked", G_CALLBACK (clicked_callback), user_data);

    // Save
    GObject *save_button = gtk_builder_get_object(builder, "save_button");

    g_signal_connect(GTK_BUTTON (save_button), "clicked", G_CALLBACK (save_callback), user_data);

    parent = GTK_WINDOW (window);
}
