#include "brush.h"
#include "types.h"

static GtkWidget *color_picker = NULL;
static int brush_size = DEFAULT_PIXEL_SIZE;

GdkRGBA *get_brush_color(void)
{
    GdkRGBA *color = gtk_color_dialog_button_get_rgba(GTK_COLOR_DIALOG_BUTTON (color_picker));

    return color;
}

int get_brush_size() 
{
    return brush_size;
}

int set_brush_size(int size)
{
    return brush_size = size;
}

void activate_brush(GtkWidget *window, GtkWidget *color)
{
    GtkColorDialog *dialog = gtk_color_dialog_new();
    
    gtk_color_dialog_button_set_dialog(GTK_COLOR_DIALOG_BUTTON (color), dialog);

    color_picker = color;
}
