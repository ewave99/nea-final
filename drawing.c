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
    char inputs[8][16];
    char outputs[8][16];
}
VisualModule;

static void doDrawing(cairo_t *canvas);

static const VisualModule items[2] = {
    {
        { 100, 100, 200, 200 },
        { 0, 1, 1 },
        { 0, 0, 0.7 },
        "OSCILLATOR",
        { },
        { "out" }
    },
    {
        { 500, 500, 100, 50 },
        { 1, 0.5, 0 },
        { 0.4, 0, 0 },
        "CLOCK",
        { },
        { "out" }
    }
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
drawModuleRectangle(cairo_t *canvas, Rect rect, double fill_colour[3],
        double line_colour[3])
{
    cairo_rectangle(canvas,
            rect.x,
            rect.y,
            rect.w,
            rect.h);
    cairo_set_source_rgb(canvas,
            line_colour[0],
            line_colour[1],
            line_colour[2]);
    cairo_stroke_preserve(canvas);

    cairo_set_source_rgb(canvas,
            fill_colour[0],
            fill_colour[1],
            fill_colour[2]);
    cairo_fill(canvas);
}

static void
displayModuleName(cairo_t *canvas, Rect module_rect, double line_colour[3],
        double font_size, char module_name[16])
{
    cairo_set_source_rgb(canvas,
            line_colour[0],
            line_colour[1],
            line_colour[2]);
    cairo_select_font_face(canvas, "Times New Roman",
            CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(canvas, font_size);
    cairo_move_to(canvas,
            module_rect.x + font_size,
            module_rect.y + font_size * 2);
    cairo_show_text(canvas, module_name);
}

static void
doDrawing(cairo_t *canvas)
{
    if (DRAWING) {
        int limit = 2;
        int font_size = 20;

        int i = 0;
        while (i < limit)
        {
            drawModuleRectangle(canvas, items[i].rect, items[i].fill_colour,
                    items[i].line_colour);

            displayModuleName(canvas, items[i].rect, items[i].line_colour,
                    font_size, items[i].module_name);

            i ++;
        }
        DRAWING = 0;
    }
    else
    {
        DRAWING = 1;
    }
}

