#include <gtk/gtk.h>
#include "types.h"

static GtkWindow *parent = NULL;

static GtkWindow *create_modal_dialog(void) {
  GtkWindow *window = gtk_window_new();

  gtk_window_set_modal(window, TRUE);
  gtk_window_set_transient_for(window, parent);

  GtkFrame *frame = gtk_frame_new("New pixel size");
  GtkAdjustment* adjustement = gtk_adjustment_new(
    0.0,
    LOWEST_PIXEL_SIZE,
    HIGHEST_PIXEL_SIZE,
    1.0,
    LOWEST_PIXEL_SIZE,
    0.0
  );

  GtkSpinButton *button = gtk_spin_button_new(adjustement, 1.0, 0);

  gtk_frame_set_child(frame, GTK_WIDGET (button));
  gtk_window_set_child(window, GTK_WIDGET (frame));

  return window;
}

static void clicked_callback (GtkButton* self, gpointer user_data) {
  GtkWindow *window = create_modal_dialog();

  gtk_window_present(window);
}

void activate_action(GtkBuilder *builder, GtkWidget *window, gpointer user_data) {
    GObject *action_bar = gtk_builder_get_object(builder, "action_bar");

    gtk_widget_set_size_request (GTK_WIDGET (action_bar), DEFAULT_PIXEL_SIZE, DEFAULT_PIXEL_SIZE);

    GObject *size_button = gtk_builder_get_object(builder, "pixel_button");

    g_signal_connect(GTK_BUTTON (size_button), "clicked", G_CALLBACK (clicked_callback), user_data);

    parent = GTK_WINDOW (window);
}