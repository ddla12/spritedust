#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include "spritedust.h"

typedef struct
{
    unsigned char *pos;
    unsigned char *end;
} closure_t;

static unsigned int total_length = 0;

// Custom writer
cairo_status_t write_callback (
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

	int stride = cairo_image_surface_get_stride(current_surface);

	data = (unsigned char *) malloc(stride);

	if (data == NULL)
		return;

	closure_t cl;

	cl.pos = data;
	cl.end = data + stride;

	cairo_surface_write_to_png_stream (
		current_surface,
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

