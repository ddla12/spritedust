#include <gtk/gtk.h>

#ifndef CANVAS_H
#define CANVAS_H

void activate_canvas(GtkWidget *window, GtkWidget *drawing_area);
void set_canvas_size(int size);

#endif /* CANVAS_H */