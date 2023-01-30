#include <gtk/gtk.h>
#include <cairo/cairo.h>

gboolean onDrawEvent(GtkWidget *widget, cairo_t *canvas,
        gpointer user_data);

typedef struct {
    int x;
    int y;
    int w;
    int h;
} Rect;

typedef struct
{
    char module_name[16];
    Rect rect;
    double fill_colour[3];
    double line_colour[3];
    int num_inputs;
    char inputs[8][16];
    int num_outputs;
    char outputs[8][16];
}
VisualModule;

typedef struct
{
    int src_module_index;
    int src_module_output_index;
    int dst_module_index;
    int dst_module_input_index;
} Connection;
