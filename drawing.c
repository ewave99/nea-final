#include <stdlib.h>
#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include <application.h>

static void doDrawing(cairo_t *canvas, ApplicationContext *context);

gboolean
onDrawEvent(GtkWidget *widget, cairo_t *canvas, ApplicationContext* context)
{
    arrangeModulesOnCanvas(context->canvas_width, context->canvas_height,
            context->num_modules, context->visual_modules,
            context->num_connections, context->connections);

    doDrawing(canvas, context);

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
drawModule(cairo_t *canvas, VisualModule module, double font_size)
{
    drawModuleRectangle(canvas, module.rect, module.fill_colour,
            module.line_colour);

    displayModuleName(canvas, module.rect, module.line_colour,
            font_size, module.module_name);
    
    drawInputsOfModule(canvas, module.rect, module.line_colour,
            font_size, module.num_inputs, module.inputs);

    drawOutputsOfModule(canvas, module.rect, module.line_colour,
            font_size, module.num_outputs, module.outputs);
}

static void
drawModules(cairo_t *canvas, int num_modules, VisualModule *visual_modules,
        double font_size)
{
    for (int i = 0; i < num_modules; i ++)
        drawModule(canvas, visual_modules[i], font_size);
}

static void
drawConnection(cairo_t *canvas, VisualModule *visual_modules,
        Connection connection)
{
    double start_x, start_y;
    double end_x, end_y;
    VisualModule src_module;
    VisualModule dst_module;
    
    src_module = visual_modules[connection.src_module_index];
    if (src_module.num_outputs == 0) return;
    start_x = src_module.rect.x + src_module.rect.w
        / src_module.num_outputs
        * connection.src_module_output_index;
    start_y = src_module.rect.y + src_module.rect.h;

    dst_module = visual_modules[connection.dst_module_index];
    if (dst_module.num_inputs == 0) return;
    end_x = dst_module.rect.x + dst_module.rect.w
        / dst_module.num_inputs
        * connection.dst_module_input_index;
    end_y = dst_module.rect.y;

    cairo_set_source_rgb(canvas, 0, 0, 0);
    cairo_set_line_width(canvas, 1);
    cairo_move_to(canvas, start_x, start_y);
    cairo_line_to(canvas, end_x, end_y);
    cairo_stroke(canvas);
}

static void
drawConnections(cairo_t *canvas, VisualModule *visual_modules,
        int num_connections, Connection *connections)
{
    for (int i = 0; i < num_connections; i ++)
        drawConnection(canvas, visual_modules, connections[i]);
}

static void
doDrawing(cairo_t *canvas, ApplicationContext *context)
{
    if (context->num_modules == 0)
        return;
    if (context->visual_modules == NULL)
        return;

    double font_size = 20;

    drawModules(canvas, context->num_modules, context->visual_modules,
            font_size);

    drawConnections(canvas, context->visual_modules,
            context->num_connections, context->connections);

    context->drawing = 0;
}

