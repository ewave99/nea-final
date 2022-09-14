#include <gtk/gtk.h>
#include <cairo/cairo.h>

static void doDrawing(cairo_t *canvas);

gboolean
onDrawEvent(GtkWidget *widget, cairo_t *canvas,
        gpointer user_data)
{
    doDrawing(canvas);

    return FALSE;
}

static void
doDrawing(cairo_t *canvas)
{
    cairo_set_source_rgb(canvas, 0, 0, 0);
    cairo_select_font_face(canvas, "Sans", CAIRO_FONT_SLANT_NORMAL, 
            CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(canvas, 40.0);

    cairo_move_to(canvas, 10.0, 50.0);
    cairo_show_text(canvas, "NEA Final.");

    cairo_rectangle(canvas, 20, 50, 90, 500);
    cairo_set_source_rgb(canvas, 255, 0, 0);
    cairo_fill(canvas);
}

