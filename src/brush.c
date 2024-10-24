#include "spritedust.h"

typedef struct Brush {
	GdkRGBA *color;
	PixelSize size;
} Brush;

static Brush brush = { NULL, NORMAL };
static GtkColorDialog *color_picker = NULL;

GdkRGBA *get_brush_color(void)
{
    brush.color = gtk_color_dialog_button_get_rgba(GTK_COLOR_DIALOG_BUTTON (color_picker));

    return brush.color;
}

int get_brush_size(void) 
{
    return brush.size;
}

PixelSize set_brush_size(PixelSize size)
{
    return brush.size = size;
}

void activate_brush(GtkWidget *window, GtkWidget *color)
{
    GtkColorDialog *dialog = gtk_color_dialog_new();
    
    gtk_color_dialog_button_set_dialog(GTK_COLOR_DIALOG_BUTTON (color), dialog);

    color_picker  = color;
}
