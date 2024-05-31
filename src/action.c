#include <gtk/gtk.h>
#include "types.h"

static GtkWindow *parent = NULL;
static GtkWidget *canvas = NULL;

static void destroy (GtkWidget* self, gpointer user_data) {
  app_data *data = APP_DATA (user_data);

  data->pixel_size = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON (self));

  g_signal_emit_by_name(canvas, "resize", 0, 0, user_data);
}

static GtkWindow *create_modal_dialog(gpointer user_data) {
  GtkWidget *window = GTK_WINDOW (gtk_window_new());

  gtk_window_set_modal(window, TRUE);
  gtk_window_set_transient_for(window, parent);

  GtkFrame *frame = gtk_frame_new("New pixel size");
  GtkWidget *spin = gtk_spin_button_new_with_range(LOWEST_PIXEL_SIZE, HIGHEST_PIXEL_SIZE, 1);

  gtk_frame_set_child(frame, spin);
  gtk_window_set_child(window, GTK_WIDGET (frame));

  g_signal_connect_after(spin, "destroy", G_CALLBACK (destroy), user_data);

  return window;
}

static void clicked_callback (GtkButton* self, gpointer user_data) {
  GtkWindow *window = create_modal_dialog(user_data);

  gtk_window_present(window);
}

void activate_action(GtkBuilder *builder, GtkWidget *window, gpointer user_data, GtkWidget *drawing_area) {
    GObject *action_bar = gtk_builder_get_object(builder, "action_bar");

    gtk_widget_set_size_request (GTK_WIDGET (action_bar), DEFAULT_PIXEL_SIZE, DEFAULT_PIXEL_SIZE);

    GObject *size_button = gtk_builder_get_object(builder, "pixel_button");

    g_signal_connect(GTK_BUTTON (size_button), "clicked", G_CALLBACK (clicked_callback), user_data);

    parent = GTK_WINDOW (window);
    canvas = drawing_area;
}