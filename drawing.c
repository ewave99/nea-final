#include <gtk/gtk.h>
#include <cairo/cairo.h>

static void doDrawing(cairo_t *canvas);

const int RECTS[][4] = {
    { 0, 0, 40, 40 },
    { 40, 40, 500, 20 },
    { 20, 400, 400, 400 },
    { 0, 0, 0, 0 }
};

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
    cairo_set_source_rgb(canvas, 255, 0, 0);

    gint i = 0;
    while (RECTS[i][2] != 0 && RECTS[i][3] != 0)
    {
        cairo_rectangle(canvas, RECTS[i][0], RECTS[i][1], RECTS[i][2],
                RECTS[i][3]);
        cairo_fill(canvas);

        i ++;
    }
}

