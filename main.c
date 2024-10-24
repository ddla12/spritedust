#include <gtk/gtk.h>
#include "spritedust.h"

static void activate_objects(GtkBuilder *builder, GtkWidget *window, gpointer user_data) {
    // Canvas
    GObject *drawing_area = gtk_builder_get_object(builder, "drawing_area");

    activate_canvas(window, GTK_WIDGET (drawing_area));

    // Action
    activate_action(window, builder);

    // Brush
    GObject *color_button = gtk_builder_get_object(builder, "color_button");

    activate_brush(window, GTK_WIDGET (color_button));
}

static void activate (GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();

    gtk_builder_add_from_file (builder, "./ui/window.ui", NULL);

    // Window object
    GObject *window = gtk_builder_get_object (builder, "window");

    gtk_window_set_application(GTK_WINDOW (window), app);

    // Other objects
    activate_objects(builder, GTK_WIDGET (window), user_data);

    gtk_window_present (GTK_WINDOW (window));

    g_object_unref (builder);
}

int main(int argc, char *argv[]) {
    GtkApplication *app;
    int status;
    
    app = gtk_application_new ("org.spritedust", G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
