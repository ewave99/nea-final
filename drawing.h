#include <gtk/gtk.h>
#include <cairo/cairo.h>

gboolean onDrawEvent(GtkWidget *widget, cairo_t *canvas,
        gpointer user_data);

typedef struct
{
    int src_module_index;
    int src_module_output_index;
    int dst_module_index;
    int dst_module_input_index;
} Connection;
