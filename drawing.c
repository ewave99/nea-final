#include <gtk/gtk.h>
#include <cairo/cairo.h>

typedef struct {
    int x;
    int y;
    int w;
    int h;
} Rect;

typedef struct
{
    Rect rect;
    double fill_colour[3];
    double line_colour[3];
    char module_name[16];
}
VisualModule;

static void doDrawing(cairo_t *canvas);

static const VisualModule items[1] = {
    { { 100, 100, 200, 200 }, { 0, 1, 1 }, { 0, 0, 0.7 }, "OSCILLATOR" }
};

static int DRAWING = 0;

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
    if (DRAWING) {
        int limit = 1;
        int font_size = 20;

        int i = 0;
        while (i < limit)
        {
            cairo_rectangle(canvas,
                    items[i].rect.x,
                    items[i].rect.y,
                    items[i].rect.w,
                    items[i].rect.h);
            cairo_set_source_rgb(canvas,
                    items[i].line_colour[0],
                    items[i].line_colour[1],
                    items[i].line_colour[2]);
            cairo_stroke_preserve(canvas);

            cairo_set_source_rgb(canvas,
                    items[i].fill_colour[0],
                    items[i].fill_colour[1],
                    items[i].fill_colour[2]);
            cairo_fill(canvas);

            cairo_set_source_rgb(canvas,
                    items[i].line_colour[0],
                    items[i].line_colour[1],
                    items[i].line_colour[2]);
            cairo_select_font_face(canvas, "Times New Roman",
                    CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
            cairo_set_font_size(canvas, font_size);
            cairo_move_to(canvas,
                    items[i].rect.x + font_size,
                    items[i].rect.y + font_size * 2);
            cairo_show_text(canvas, items[i].module_name);

            i ++;
        }
        DRAWING = 0;
    }
    else
    {
        DRAWING = 1;
    }
}

