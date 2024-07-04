#include <math.h>
#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include "canvas.h"
#include "brush.h"
#include "types.h"

#define NEAREST_INTEGER(x, y) floor(x / y) * y
#define SQUARE(x) pow(x, 2)

static int canvas_width;
static int canvas_height;
static int grid_size;

typedef struct Point {
  double x;
  double y;
} Point;

typedef struct PointList {
  Point *value;
  struct PointList *next;
} PointList;

static Point *create_point(double x, double y)
{
  Point *p = (Point *)malloc(sizeof(Point));

  p->x = x;
  p->y = y;

  return p;
}

static Point *add_point(Point *a, double x, double y)
{
  return create_point(a->x + x, a->y + y);
}

static Point *point_to_unit(Point *p)
{
  return create_point(floor(p->x / grid_size), floor(p->y / grid_size));
}

static Point *point_to_pixels_in_place(Point *p)
{
  p->x *= grid_size;
  p->y *= grid_size;

  return p;
}

// Took it from https://gist.github.com/bert/1085538
static PointList *bresenham_algorithm(Point *a, Point *b)
{
  int x0 = a->x, x1 = b->x;
  int y0 = a->y, y1 = b->y;
  int dx =  fabs (b->x - a->x), sx = a->x < b->x ? 1 : -1;
  int dy = -fabs (b->y - a->y), sy = a->y < b->y ? 1 : -1; 
  int err = dx + dy, e2;
 
  PointList *head = (PointList *)malloc(sizeof(PointList));
  PointList *tmp = head;

  while(1) {
    Point *p = create_point(x0, y0);

    tmp->value = p;
    tmp->next = NULL;

    if(x0 == x1 && y0 == y1) 
      break;

    PointList *next = (PointList *)malloc(sizeof(PointList));

    tmp->next = next;
    tmp = next;

    e2 = 2 * err;

    if (e2 >= dy) { err += dy; x0 += sx; }
    if (e2 <= dx) { err += dx; y0 += sy; }
  }

  return head;
}

static Point *brush_p = NULL;
static Point *eraser_p = NULL;
static cairo_surface_t *surface = NULL;
static cairo_surface_t *temp = NULL;
static GtkWidget *canvas = NULL;
static gboolean straigt_lines = FALSE;

typedef struct
{
    unsigned char *pos;
    unsigned char *end;
} closure_t;

// Set surface to white
static void clear_surface (void)
{
  cairo_t *cr;

  cr = cairo_create (surface);

  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);

  cairo_destroy (cr);
}

static void resize_cb (GtkWidget *widget,
           int        width,
           int        height,
           gpointer   user_data)
{
	grid_size = get_brush_size();
	canvas_width = width;
	canvas_height = height;

	if (surface)
	{
	    cairo_surface_destroy (surface);
	    surface = NULL;
	}
	
	// Create surface when resize
	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, canvas_width, canvas_height);
	clear_surface ();
}

static void draw_cb (GtkDrawingArea *drawing_area,
         cairo_t        *cr,
         int             width,
         int             height,
         gpointer        data)
{
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);
}

static void draw_brush (GtkWidget *widget, Point *point)
{
  cairo_t *cr;

  cr = cairo_create (surface);

  GdkRGBA *color = get_brush_color();

  cairo_set_source_rgb (cr, color->red, color->green, color->blue);

  int size = get_brush_size ();

  cairo_rectangle (cr, NEAREST_INTEGER (point->x, size), NEAREST_INTEGER(point->y, size), size, size);
  cairo_fill (cr);

  cairo_destroy (cr);

  gtk_widget_queue_draw (widget);
}

static void erase_pixel(GtkWidget *widget, Point *point)
{
  cairo_t *cr;

  cr = cairo_create (surface);

  int size = get_brush_size ();

  // Just set pipxel to white
  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_rectangle (cr, NEAREST_INTEGER (point->x, size), NEAREST_INTEGER(point->y, size), size, size);
  cairo_fill(cr);

  cairo_destroy (cr);

  gtk_widget_queue_draw (widget);
}

// Temp surface for draw straight line onto the main surface
static void create_temp_surface (void) {
	temp = cairo_surface_create_similar_image (surface, CAIRO_FORMAT_ARGB32, canvas_width, canvas_height);

	cairo_t *cr;

	cr = cairo_create (temp);

	cairo_set_source_surface (cr, surface, 0, 0);
	cairo_paint (cr);
	cairo_destroy (cr);
}

static void draw_straight_line(GtkWidget *widget, Point *point)
{
  if (temp == NULL)
	  create_temp_surface ();

  Point *a = point_to_unit(brush_p);
  Point *b = point_to_unit(point);

  PointList *current = bresenham_algorithm(a, b);

  cairo_t *cr;

  cr = cairo_create (surface);
  
  // We draw onto temp so we keep the original picture
  cairo_set_source_surface (cr, temp, 0, 0);
  cairo_paint (cr);

  GdkRGBA *color = get_brush_color();

  cairo_set_operator (cr, CAIRO_OPERATOR_OVER);
  cairo_set_source_rgb (cr, color->red, color->green, color->blue);
  
  int size = get_brush_size();

  while(current != NULL) {
    Point *p = current->value;

    point_to_pixels_in_place(p);

    cairo_rectangle (cr, NEAREST_INTEGER (p->x, size), NEAREST_INTEGER(p->y, size), size, size);
    cairo_fill (cr);

    PointList* tmp = current->next;

    free(p);
    free(current);

    current = tmp;
  }

  cairo_destroy(cr);

  free(a);
  free(b);

  gtk_widget_queue_draw (widget);
}

static void drag_begin (GtkGestureDrag *gesture,
            double          x,
            double          y,
            GtkWidget      *area)
{
  if(brush_p != NULL)
    free(brush_p);

  brush_p = create_point(x, y);

  draw_brush (area, brush_p);
}

static void drag_update (GtkGestureDrag *gesture,
             double          x,
             double          y,
             GtkWidget      *area)
{
  Point *p = add_point(brush_p, x, y);

  if (!straigt_lines)
    draw_brush (area, p);
  else
    draw_straight_line(area, p);

  free(p);
}

static void drag_end (GtkGestureDrag *gesture,
          double          x,
          double          y,
          GtkWidget      *area)
{
	if (temp) {
		cairo_surface_destroy (temp);
		temp = NULL;
	}

	Point *p = add_point(brush_p, x, y);

	draw_brush (area, p);

	free(p);
}

static void erase_begin (GtkGestureDrag *gesture,
            double          x,
            double          y,
            GtkWidget      *area)
{
  if(eraser_p != NULL) {
    free(eraser_p);
  }

  eraser_p = create_point(x, y);

  erase_pixel (area, eraser_p);
}

static void erase_update (GtkGestureDrag *gesture,
             double          x,
             double          y,
             GtkWidget      *area)
{
  Point *p = add_point(eraser_p, x, y);

  erase_pixel (area, p);

  free(p);
}

static void erase_end (GtkGestureDrag *gesture,
          double          x,
          double          y,
          GtkWidget      *area)
{
  Point *p = add_point(eraser_p, x, y);

  erase_pixel (area, p);

  free(p);
}

static void close_window (void)
{
  if (surface)
    cairo_surface_destroy (surface);
}

static gboolean key_pressed (
  GtkEventControllerKey* self,
  guint keyval,
  guint keycode,
  GdkModifierType state,
  gpointer user_data
) {
	// If key is left shift
	straigt_lines = keyval == GDK_KEY_Shift_L;

	return TRUE;
}

static void key_released (
  GtkEventControllerKey* self,
  guint keyval,
  guint keycode,
  GdkModifierType state,
  gpointer user_data
) {
  straigt_lines = FALSE;
}

static void set_brush(GtkWidget *drawing_area) {
  GtkGesture *drag = gtk_gesture_drag_new ();

  gtk_gesture_single_set_button (GTK_GESTURE_SINGLE (drag), GDK_BUTTON_PRIMARY);
  gtk_widget_add_controller (drawing_area, GTK_EVENT_CONTROLLER (drag));
  g_signal_connect (drag, "drag-begin", G_CALLBACK (drag_begin), drawing_area);
  g_signal_connect (drag, "drag-update", G_CALLBACK (drag_update), drawing_area);
  g_signal_connect (drag, "drag-end", G_CALLBACK (drag_end), drawing_area);
}

static void set_eraser(GtkWidget *drawing_area) {
  GtkGesture *drag = gtk_gesture_drag_new ();

  gtk_gesture_single_set_button (GTK_GESTURE_SINGLE (drag), GDK_BUTTON_SECONDARY);
  gtk_widget_add_controller (drawing_area, GTK_EVENT_CONTROLLER (drag));
  g_signal_connect (drag, "drag-begin", G_CALLBACK (erase_begin), drawing_area);
  g_signal_connect (drag, "drag-update", G_CALLBACK (erase_update), drawing_area);
  g_signal_connect (drag, "drag-end", G_CALLBACK (erase_end), drawing_area);
}

static unsigned int total_length = 0;

// Custom writer
static cairo_status_t write_callback (
		void *closure,
		const unsigned char *data,
	       	unsigned int length) {
    closure_t *cl = (closure_t *) closure;

    if ((cl->pos + length) > (cl->end))
        return CAIRO_STATUS_WRITE_ERROR;

    memcpy (cl->pos, data, length);
    cl->pos += length;
    total_length += length;

    return CAIRO_STATUS_SUCCESS;
}

static unsigned char *data = NULL;

static void save_file_complete (GObject      *source_object,
                    GAsyncResult *result,
                    gpointer      user_data)
{
  GFile *file = G_FILE (source_object);

  g_autoptr (GError) error =  NULL;
  g_file_replace_contents_finish (file, result, NULL, &error);

  if (error != NULL)
    {
      g_printerr ("Unable to save: %s\n", error->message);
    }

  if (data != NULL)
	  free (data);
}

void save_canvas (GFile *file, gpointer user_data) {
	total_length = 0;

	int stride = cairo_image_surface_get_stride(surface);

	data = (unsigned char *) malloc(stride);
	closure_t cl;

	cl.pos = data;
	cl.end = data + stride;

	cairo_surface_write_to_png_stream (
		surface,
		write_callback,
		&cl
	);

	g_file_replace_contents_async (
		file,
		(char *) data,	
		total_length,
		NULL,
		FALSE,
		G_FILE_CREATE_NONE,
		NULL,
		save_file_complete,
		user_data
	);
}

void activate_canvas(GtkWidget *window, GtkWidget *drawing_area, gpointer user_data)
{
  g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

  const int CANVAS_SIZE = get_brush_size() * 10;

  /* set a minimum size */
  gtk_widget_set_size_request (drawing_area, CANVAS_SIZE, CANVAS_SIZE);

  gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (drawing_area), draw_cb, NULL, NULL);

  g_signal_connect_after (drawing_area, "resize", G_CALLBACK (resize_cb), user_data);

  //  Connect tools
  set_brush(drawing_area);
  set_eraser(drawing_area);

  // Straight line controller
  GtkEventController *key = gtk_event_controller_key_new();

  gtk_widget_add_controller(window, key);

  g_signal_connect (GTK_EVENT_CONTROLLER_KEY (key), "key-pressed", G_CALLBACK (key_pressed), NULL);
  g_signal_connect (GTK_EVENT_CONTROLLER_KEY (key), "key-released", G_CALLBACK (key_released), NULL);

  canvas = drawing_area;
}
