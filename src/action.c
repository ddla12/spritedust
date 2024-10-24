#include <gtk/gtk.h>
#include "spritedust.h"

static GtkWindow *parent = NULL;

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

void activate_action(GtkWidget *window, GtkBuilder *builder) {
    GObject *action_bar = gtk_builder_get_object(builder, "action_bar");

    gtk_widget_set_size_request (GTK_WIDGET (action_bar), NORMAL, NORMAL);

    // Save
    GObject *save_button = gtk_builder_get_object(builder, "save_button");

    g_signal_connect(GTK_BUTTON (save_button), "clicked", G_CALLBACK (save_callback), NULL);

    parent = GTK_WINDOW (window);
}
