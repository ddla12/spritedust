#include <gtk/gtk.h>

#ifndef BRUSH_H
#define BRUSH_H

GdkRGBA get_brush_color();
int get_brush_size();
void activate_brush(GtkWidget *widget);

#endif /* BRUSH_H */