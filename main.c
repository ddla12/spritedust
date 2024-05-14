#include <gtk/gtk.h>
#include "canvas.h"
#include "brush.h"

static void activate (GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();

    gtk_builder_add_from_file (builder, "ui/window.ui", NULL);

    // Window object
    GObject *window = gtk_builder_get_object (builder, "window");

    gtk_window_set_application(GTK_WINDOW(window), app);

    // Canvas
    GObject *drawing_area = gtk_builder_get_object(builder, "drawing_area");

    activate_canvas(GTK_WIDGET (window), GTK_WIDGET (drawing_area));

    // Brush
    GObject *color_button = gtk_builder_get_object(builder, "color_button");

    activate_brush(GTK_WIDGET (window), GTK_WIDGET (color_button));

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
