#include <gtk/gtk.h>

#ifndef BRUSH_H
#define BRUSH_H

GdkRGBA *get_brush_color(void);
void activate_brush(GtkWidget *widget, GtkWidget *color_button);

#endif /* BRUSH_H */