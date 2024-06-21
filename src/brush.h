#include <gtk/gtk.h>

#ifndef BRUSH_H
#define BRUSH_H

GdkRGBA *get_brush_color(void);
int get_brush_size();
int set_brush_size(int size);
void activate_brush(GtkWidget *widget, GtkWidget *color_button);

#endif /* BRUSH_H */