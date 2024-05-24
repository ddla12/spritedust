#include <gtk/gtk.h>
#include "canvas.h"
#include "brush.h"
#include <math.h>

/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;
static GtkWidget *canvas = NULL;
static int grid_size = 32;

static void clear_surface (void)
{
  cairo_surface_t *pattern_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, grid_size, grid_size);
  cairo_t *cr;
  cairo_pattern_t *pattern;

  cr = cairo_create(pattern_surface);

  cairo_set_line_width (cr, 0.25);
  cairo_rectangle (cr, 0, 0, grid_size, grid_size);
  cairo_stroke (cr);
  cairo_destroy(cr);

  pattern = cairo_pattern_create_for_surface(pattern_surface);
  cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);

  cr = cairo_create (surface);

  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);
  cairo_set_source(cr, pattern);
  cairo_paint (cr);

  cairo_destroy (cr);
  cairo_surface_destroy(pattern_surface);
  cairo_pattern_destroy(pattern);
}

/* Create a new surface of the appropriate size to store our scribbles */
static void resize_cb (GtkWidget *widget,
           int        width,
           int        height,
           gpointer   data)
{
  if (surface)
    {
      cairo_surface_destroy (surface);
      surface = NULL;
    }

  if (gtk_native_get_surface (gtk_widget_get_native (widget)))
    {
      surface = gdk_surface_create_similar_surface (gtk_native_get_surface (gtk_widget_get_native (widget)),
                                                    CAIRO_CONTENT_COLOR,
                                                    gtk_widget_get_width (widget),
                                                    gtk_widget_get_height (widget));

      /* Initialize the surface to white */
      clear_surface ();
    }
}

/* Redraw the screen from the surface. Note that the draw
 * callback receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static void draw_cb (GtkDrawingArea *drawing_area,
         cairo_t        *cr,
         int             width,
         int             height,
         gpointer        data)
{
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);
}

/* Draw a rectangle on the surface at the given position */
static void draw_brush (GtkWidget *widget,
            double     x,
            double     y)
{
  cairo_t *cr;

  /* Paint to the surface, where we store our state */
  cr = cairo_create (surface);

  const int SIZE = get_brush_size();
  GdkRGBA *color = get_brush_color();

  cairo_set_source_rgb (cr, color->red, color->green, color->blue);
  cairo_rectangle (cr, floor(x / grid_size) * grid_size, floor(y / grid_size) * grid_size, SIZE, SIZE);
  cairo_fill (cr);

  cairo_destroy (cr);

  /* Now invalidate the drawing area. */
  gtk_widget_queue_draw (widget);
}

static double start_x;
static double start_y;

static void drag_begin (GtkGestureDrag *gesture,
            double          x,
            double          y,
            GtkWidget      *area)
{
  start_x = x;
  start_y = y;

  draw_brush (area, x, y);
}

static void drag_update (GtkGestureDrag *gesture,
             double          x,
             double          y,
             GtkWidget      *area)
{
  draw_brush (area, start_x + x, start_y + y);
}

static void drag_end (GtkGestureDrag *gesture,
          double          x,
          double          y,
          GtkWidget      *area)
{
  draw_brush (area, start_x + x, start_y + y);
}

static void pressed (GtkGestureClick *gesture,
         int              n_press,
         double           x,
         double           y,
         GtkWidget       *area)
{
  clear_surface ();
  gtk_widget_queue_draw (area);
}

static void close_window (void)
{
  if (surface)
    cairo_surface_destroy (surface);
}

void activate_canvas(GtkWidget *window, GtkWidget *drawing_area)
{
  g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

  const int CANVAS_SIZE = 320;

  /* set a minimum size */
  gtk_widget_set_size_request (drawing_area, CANVAS_SIZE, CANVAS_SIZE);

  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (drawing_area), draw_cb, NULL, NULL);

  g_signal_connect_after (drawing_area, "resize", G_CALLBACK (resize_cb), NULL);

  GtkGesture *drag = gtk_gesture_drag_new ();

  gtk_gesture_single_set_button (GTK_GESTURE_SINGLE (drag), GDK_BUTTON_PRIMARY);
  gtk_widget_add_controller (drawing_area, GTK_EVENT_CONTROLLER (drag));
  g_signal_connect (drag, "drag-begin", G_CALLBACK (drag_begin), drawing_area);
  g_signal_connect (drag, "drag-update", G_CALLBACK (drag_update), drawing_area);
  g_signal_connect (drag, "drag-end", G_CALLBACK (drag_end), drawing_area);

  GtkGesture *press = gtk_gesture_click_new ();

  gtk_gesture_single_set_button (GTK_GESTURE_SINGLE (press), GDK_BUTTON_SECONDARY);
  gtk_widget_add_controller (drawing_area, GTK_EVENT_CONTROLLER (press));

  g_signal_connect (press, "pressed", G_CALLBACK (pressed), drawing_area);

  canvas = drawing_area;
}