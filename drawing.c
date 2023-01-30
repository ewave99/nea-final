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

static void doDrawing(cairo_t *canvas);

static const VisualModule items[2] = {
    {
        { 400, 100, 200, 100 },
        { 0, 1, 1 },
        { 0, 0, 0.7 },
        "OSCILLATOR",
        0, { },
        1, { "out" }
    },
    {
        { 400, 500, 300, 200 },
        { 1, 0.5, 0 },
        { 0.4, 0, 0 },
        "MIXER",
        4, { "in0", "in1", "in2", "in3" },
        1, { "out" }
    }
};

static const Connection connections[1] = {
    { 0, 0, 1, 1 }
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
drawInputsOfModule(cairo_t *canvas, Rect module_rect, double line_colour[3],
        double font_size, int num_inputs, char input_names[8][16])
{
    cairo_set_source_rgb(canvas,
            line_colour[0],
            line_colour[1],
            line_colour[2]);
    cairo_select_font_face(canvas, "Times New Roman",
            CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(canvas, font_size);

    for (int input = 0; input < num_inputs; input ++)
    {
        cairo_move_to(canvas,
                module_rect.x + module_rect.w / num_inputs * input,
                module_rect.y + font_size);
        cairo_show_text(canvas, input_names[input]);
    }
}

static void
drawOutputsOfModule(cairo_t *canvas, Rect module_rect, double line_colour[3],
        double font_size, int num_outputs, char output_names[8][16])
{
    cairo_set_source_rgb(canvas,
            line_colour[0],
            line_colour[1],
            line_colour[2]);
    cairo_select_font_face(canvas, "Times New Roman",
            CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(canvas, font_size);

    for (int output = 0; output < num_outputs; output ++)
    {
        cairo_move_to(canvas,
                module_rect.x + module_rect.w / num_outputs * output,
                module_rect.y + module_rect.h);
        cairo_show_text(canvas, output_names[output]);
    }
}

static void
doDrawing(cairo_t *canvas)
{
    if (DRAWING) {
        int limit = 2;
        double font_size = 20;

        int i = 0;
        while (i < limit)
        {
            drawModuleRectangle(canvas, items[i].rect, items[i].fill_colour,
                    items[i].line_colour);

            displayModuleName(canvas, items[i].rect, items[i].line_colour,
                    font_size, items[i].module_name);
            
            drawInputsOfModule(canvas, items[i].rect, items[i].line_colour,
                    font_size, items[i].num_inputs, items[i].inputs);

            drawOutputsOfModule(canvas, items[i].rect, items[i].line_colour,
                    font_size, items[i].num_outputs, items[i].outputs);

            i ++;
        }

        //draw connection(s)
        int connection_index;
        double start_x, start_y;
        double end_x, end_y;
        VisualModule src_module;
        VisualModule dst_module;

        // just for now
        connection_index = 0;

        src_module = items[connections[connection_index].src_module_index];
        start_x = src_module.rect.x + src_module.rect.w
            / src_module.num_outputs
            * connections[connection_index].src_module_output_index;
        start_y = src_module.rect.y + src_module.rect.h;

        dst_module = items[connections[connection_index].dst_module_index];
        end_x = dst_module.rect.x + dst_module.rect.w
            / dst_module.num_inputs
            * connections[connection_index].dst_module_input_index;
        end_y = dst_module.rect.y;

        //DRAW A LINE USING THESE VALUES
        cairo_set_source_rgb(canvas, 0, 0, 0);
        cairo_set_line_width(canvas, 1);
        cairo_move_to(canvas, start_x, start_y);
        cairo_line_to(canvas, end_x, end_y);
        cairo_stroke(canvas);

        DRAWING = 0;
    }
    else
    {
        DRAWING = 1;
    }
}

