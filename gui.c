#include <gtk/gtk.h>
#include <cairo/cairo.h>

/*
    GUI HIERARCHY:
    ==============
    window |-> grid |-> grid |-> scrolled window -> text view
                    |        |-> horizontal box -> buttons
                    |        |-> scrolled window -> text view (non-editable)
                    |-> drawing area

    IN OTHER WORDS:
    ===============
    window |-> left pane |-> text editing area
           |             |-> area for control buttons
           |             |-> area for error reporting (non-editable)
           |-> right pane (drawing area)
*/

static void createWindow();
static GdkRectangle getScreenDimensions();
static GtkWidget* createLeftPane();
static GtkWidget* createTextEditingArea();
static GtkWidget* createControlButtonArea();
static GtkWidget* createErrorReportingArea();
static GtkWidget* createRightPane();

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

static gboolean
onDrawEvent(GtkWidget *widget, cairo_t *canvas,
        gpointer user_data)
{
    doDrawing(canvas);

    return FALSE;
}

void
activate(GtkApplication *app, gpointer user_data)
{
    createWindow();

    gtk_main();
}

static void
createWindow()
{
    GdkRectangle screen_dimensions;
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *left_pane;
    GtkWidget *right_pane;

    screen_dimensions = getScreenDimensions();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), screen_dimensions.width,
            screen_dimensions.height);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_title(GTK_WINDOW(window), "NEA: Music Interface");

    grid = gtk_grid_new();

    left_pane = createLeftPane();
    right_pane = createRightPane();

    gtk_grid_attach(GTK_GRID(grid), left_pane, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), right_pane, 1, 1, 1, 1);

    gtk_container_add(GTK_CONTAINER(window), grid);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),
            NULL);

    gtk_widget_show_all(window);
}

static GdkRectangle
getScreenDimensions()
{
    GdkRectangle workarea = {0};
    gdk_monitor_get_workarea(
            gdk_display_get_primary_monitor(gdk_display_get_default()),
            &workarea);
    return workarea;
}

static GtkWidget*
createLeftPane()
{
    GtkWidget* grid;
    GtkWidget* text_editing_area;

    grid = gtk_grid_new();

    text_editing_area = createTextEditingArea();

    gtk_grid_attach(GTK_GRID(grid), text_editing_area, 0, 1, 1, 1);

    return grid;
}

static GtkWidget*
createTextEditingArea()
{
    GtkWidget* scrolled_window;
    GtkWidget* text_view;
    GdkRectangle screen_dimensions;

    screen_dimensions = getScreenDimensions();

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
            GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window),
            screen_dimensions.width / 2, screen_dimensions.height);

    text_view = gtk_text_view_new();
    gtk_widget_set_size_request(GTK_WIDGET(text_view),
            screen_dimensions.width / 2, screen_dimensions.height);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(text_view), TRUE);

    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    return scrolled_window;
}

static GtkWidget*
createControlButtonArea()
{
    GtkWidget* hbox;
    return hbox;
}

static GtkWidget*
createErrorReportingArea()
{
    GtkWidget* text_view;
    return text_view;
}

static GtkWidget*
createRightPane()
{
    GtkWidget* drawing_area;
    GdkRectangle screen_dimensions;

    screen_dimensions = getScreenDimensions();

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, screen_dimensions.width / 2,
            screen_dimensions.height);

    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(onDrawEvent),
            NULL);
    gtk_widget_queue_draw(drawing_area);

    return drawing_area;
}
