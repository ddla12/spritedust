#include "brush.h"

#define MINUS_SYMBOL '-'
#define PLUS_SYMBOL '+'

static const int MAX_BRUSH_SIZE = 64;
static int brush_size = 16;
static GtkWidget *color_picker = NULL;

static int increment_brush_size(void)
{
    return (brush_size + 1 <= MAX_BRUSH_SIZE)
        ? brush_size++
        : brush_size;
}

static int decrement_brush_size(void)
{
    return (brush_size - 1 >= 1)
        ? brush_size--
        : brush_size;
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
    GdkRGBA *color = gtk_color_dialog_button_get_rgba(GTK_COLOR_DIALOG_BUTTON (color_picker));

    return color;
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

    color_picker = color;
}