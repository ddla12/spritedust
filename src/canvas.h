#include <gtk/gtk.h>

#ifndef CANVAS_H
#define CANVAS_H

void activate_canvas(GtkWidget *window, GtkWidget *drawing_area, gpointer user_data);
void save_canvas (GFile *file);
#endif /* CANVAS_H */