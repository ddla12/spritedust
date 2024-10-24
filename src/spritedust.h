#include <gtk/gtk.h>

#ifndef SPRITEDUST_H
#define SPRITEDUST_H

typedef enum PixelSize {
	SMALL = 4,
	NORMAL = 8,
	MEDIUM = 16,
	BIG = 32
} PixelSize;

// Brush
GdkRGBA *get_brush_color(void);
int get_brush_size(void);
PixelSize set_brush_size(PixelSize size);

// Writer
void save_canvas (GFile *file, gpointer user_data);

// Canvas
extern cairo_surface_t *current_surface; 

// Initializers
void activate_action(GtkWidget *window, GtkBuilder *builder);
void activate_brush(GtkWidget *window, GtkWidget *color_button);
void activate_canvas(GtkWidget *window, GtkWidget *drawing_area);
#endif
