#include <gtk/gtk.h>
#include "canvas.h"
#include "brush.h"

static void activate_objects(GtkBuilder *builder, GtkWidget *window) {
    GObject *action_bar = gtk_builder_get_object(builder, "action_bar");

    gtk_widget_set_size_request (GTK_WIDGET (action_bar), 16, 16);

    // Canvas
    GObject *drawing_area = gtk_builder_get_object(builder, "drawing_area");

    activate_canvas(window, GTK_WIDGET (drawing_area));

    // Brush
    GObject *color_button = gtk_builder_get_object(builder, "color_button");

    activate_brush(window, GTK_WIDGET (color_button));
}

static void test(GtkWindow *window, gpointer user_data) {
    int width, height;

    gtk_window_get_default_size(window, &width, &height);

    g_print("%i - %i\n", width, height);
}

static void activate (GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();

    gtk_builder_add_from_file (builder, "ui/window.ui", NULL);

    // Window object
    GObject *window = gtk_builder_get_object (builder, "window");

    g_object_notify(window, "default-width");

    g_signal_connect(window, "notify", G_CALLBACK (test), NULL);

    gtk_window_set_application(GTK_WINDOW(window), app);

    // Other objects
    activate_objects(builder, GTK_WIDGET (window));

    gtk_window_present (GTK_WINDOW (window));

    g_object_unref (builder);
}

int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;
    
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
