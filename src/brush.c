#include "brush.h"

static GtkWidget *color_picker = NULL;

GdkRGBA *get_brush_color(void)
{
    GdkRGBA *color = gtk_color_dialog_button_get_rgba(GTK_COLOR_DIALOG_BUTTON (color_picker));

    return color;
}

void activate_brush(GtkWidget *window, GtkWidget *color)
{
    GtkColorDialog *dialog = gtk_color_dialog_new();
    
    gtk_color_dialog_button_set_dialog(GTK_COLOR_DIALOG_BUTTON (color), dialog);

    color_picker = color;
}