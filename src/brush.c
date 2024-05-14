#include "brush.h"

#define MINUS_SYMBOL '-'
#define PLUS_SYMBOL '+'

static int brush_size = 2;

static int increment_brush_size(void)
{
    return brush_size++;
}

static int decrement_brush_size(void)
{
    return brush_size--;
}

static gboolean key_pressed (
    GtkEventControllerKey* self,
    guint keyval,
    guint keycode,
    GdkModifierType state,
    gpointer user_data
) {
    char key = (char)keyval;

    if(key == MINUS_SYMBOL)
        decrement_brush_size();
    else if(key == PLUS_SYMBOL)
        increment_brush_size();

    return TRUE;
}

GdkRGBA *get_brush_color(void)
{
    return NULL;
}

int get_brush_size(void)
{
    return brush_size;
}


void activate_brush(GtkWidget *window, GtkWidget *color)
{
    GtkEventController *event_controller;
    GtkColorDialog *dialog = gtk_color_dialog_new();
    
    gtk_color_dialog_button_set_dialog(GTK_COLOR_DIALOG_BUTTON (color), dialog);

    event_controller = gtk_event_controller_key_new ();

    g_signal_connect (event_controller, "key-pressed", G_CALLBACK (key_pressed), NULL);

    gtk_widget_add_controller(window, event_controller);
}