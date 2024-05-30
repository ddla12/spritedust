#include "brush.h"

static GtkWidget *color_picker = NULL;
static int grid_size = 32;

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
    GtkColorDialog *dialog = gtk_color_dialog_new();
    
    gtk_color_dialog_button_set_dialog(GTK_COLOR_DIALOG_BUTTON (color), dialog);

    color_picker = color;
}