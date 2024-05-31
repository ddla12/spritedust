#include <gtk/gtk.h>
#include "canvas.h"
#include "brush.h"
#include "action.h"
#include "src/types.h"

static void activate_objects(GtkBuilder *builder, GtkWidget *window, gpointer user_data) {
    // Action
    activate_action(builder, window, user_data);
    
    // Canvas
    GObject *drawing_area = gtk_builder_get_object(builder, "drawing_area");

    activate_canvas(window, GTK_WIDGET (drawing_area), user_data);

    // Brush
    GObject *color_button = gtk_builder_get_object(builder, "color_button");

    activate_brush(window, GTK_WIDGET (color_button));
}

static void activate (GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder = gtk_builder_new();

    gtk_builder_add_from_file (builder, "ui/window.ui", NULL);

    // Window object
    GObject *window = gtk_builder_get_object (builder, "window");

    gtk_window_set_application(GTK_WINDOW(window), app);

    // Other objects
    activate_objects(builder, GTK_WIDGET (window), user_data);

    gtk_window_present (GTK_WINDOW (window));

    g_object_unref (builder);
}

int main(int argc, char *argv[]) {
    GtkApplication *app;
    app_data data;
    int status;
    
    app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);

    data.pixel_size = DEFAULT_PIXEL_SIZE;

    g_signal_connect (app, "activate", G_CALLBACK (activate), &data);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
