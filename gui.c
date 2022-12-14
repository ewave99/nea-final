#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include "drawing.h"
#include "text.h"

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

typedef struct {
    GtkWidget *text_edit_text_view;
    GtkWidget *error_reporting_text_view;
} ApplicationContext;

void activate(GtkApplication *app, gpointer user_data);
static void createWindow();
static GdkRectangle getScreenDimensions();
static GtkWidget* createLeftPane();
static GtkWidget* createTextEditingArea(GtkWidget **text_view);
static GtkWidget* createControlButtonArea(GtkWidget **button_run,
        GtkWidget **button_stop, GtkWidget **button_update,
        GtkWidget **button_save);
static GtkWidget* createErrorReportingArea(GtkWidget **text_view);
static GtkWidget* createRightPane();

static void doRunButtonCallback(GtkWidget *widget, ApplicationContext* context);
static void doStopButtonCallback(GtkWidget *widget, ApplicationContext* context);
static void doUpdateButtonCallback(GtkWidget *widget, ApplicationContext* context);
static void doSaveButtonCallback(GtkWidget *widget, ApplicationContext* context);

void
activate(GtkApplication *app, gpointer user_data)
{
    createWindow();

    gtk_main();
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

static GtkWidget*
createLeftPane()
{
    GtkWidget* grid;

    GtkWidget* text_editing_area;
    GtkWidget* control_button_area;
    GtkWidget* error_reporting_area;

    GtkWidget* button_run;
    GtkWidget* button_stop;
    GtkWidget* button_update;
    GtkWidget* button_save;

    GtkWidget* text_edit_text_view;
    GtkWidget* error_reporting_text_view;

    ApplicationContext* context;

    grid = gtk_grid_new();
    text_editing_area = createTextEditingArea(&text_edit_text_view);
    control_button_area = createControlButtonArea(&button_run, &button_stop,
            &button_update, &button_save);
    error_reporting_area = createErrorReportingArea(
            &error_reporting_text_view);

    gtk_grid_attach(GTK_GRID(grid), text_editing_area, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), control_button_area, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), error_reporting_area, 0, 3, 1, 1);

    context = (ApplicationContext*) malloc(sizeof(ApplicationContext));
    context->text_edit_text_view = text_edit_text_view;
    context->error_reporting_text_view = error_reporting_text_view;

    g_signal_connect(button_run, "clicked",
            G_CALLBACK(doRunButtonCallback), context);
    g_signal_connect(button_stop, "clicked",
            G_CALLBACK(doStopButtonCallback), context);
    g_signal_connect(button_update, "clicked",
            G_CALLBACK(doUpdateButtonCallback), context);
    g_signal_connect(button_stop, "clicked",
            G_CALLBACK(doSaveButtonCallback), context);

    return grid;
}

static GtkWidget*
createTextEditingArea(GtkWidget **text_view)
{
    GtkWidget* scrolled_window;
    GdkRectangle screen_dimensions;

    screen_dimensions = getScreenDimensions();

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
            GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window),
            screen_dimensions.width / 2, screen_dimensions.height * 2 / 3);

    *text_view = gtk_text_view_new();
    gtk_widget_set_size_request(GTK_WIDGET(*text_view),
            screen_dimensions.width / 2, screen_dimensions.height * 2 / 3);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(*text_view), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(*text_view), TRUE);

    gtk_container_add(GTK_CONTAINER(scrolled_window), *text_view);

    return scrolled_window;
}

static GtkWidget*
createControlButtonArea(GtkWidget **button_run, GtkWidget **button_stop,
        GtkWidget **button_update, GtkWidget **button_save)
{
    GtkWidget* hbox;

    hbox = gtk_hbox_new(TRUE, 10);

    *button_run    = gtk_button_new_with_label("Run");
    *button_stop   = gtk_button_new_with_label("Stop");
    *button_update = gtk_button_new_with_label("Update");
    *button_save   = gtk_button_new_with_label("Save");

    gtk_box_pack_start(GTK_BOX(hbox),    *button_run, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),   *button_stop, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), *button_update, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),   *button_save, TRUE, TRUE, 0);

    return hbox;
}

static GtkWidget*
createErrorReportingArea(GtkWidget **text_view)
{
    GtkWidget* scrolled_window;
    GdkRectangle screen_dimensions;

    screen_dimensions = getScreenDimensions();

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
            GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window),
            screen_dimensions.width / 2, screen_dimensions.height / 6);

    *text_view = gtk_text_view_new();
    gtk_widget_set_size_request(GTK_WIDGET(*text_view),
            screen_dimensions.width / 2, screen_dimensions.height / 6);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(*text_view), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(*text_view), TRUE);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(*text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(*text_view), FALSE);

    gtk_container_add(GTK_CONTAINER(scrolled_window), GTK_WIDGET(*text_view));

    return scrolled_window;
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

static void
doRunButtonCallback(GtkWidget *widget, ApplicationContext* context)
{
    GtkTextBuffer* text_edit_text_buffer;
    GtkTextIter text_edit_buffer_start;
    GtkTextIter text_edit_buffer_end;
    gchar* text_edit_text_buffer_text;

    GtkTextBuffer* error_reporting_text_buffer;

    text_edit_text_buffer = gtk_text_view_get_buffer(
            GTK_TEXT_VIEW(context->text_edit_text_view));

    gtk_text_buffer_get_bounds(text_edit_text_buffer, &text_edit_buffer_start,
            &text_edit_buffer_end);

    text_edit_text_buffer_text = gtk_text_buffer_get_text(
            text_edit_text_buffer, &text_edit_buffer_start,
            &text_edit_buffer_end, FALSE);

    error_reporting_text_buffer = gtk_text_view_get_buffer(
            GTK_TEXT_VIEW(context->error_reporting_text_view));

    ParseResult result = processText(text_edit_text_buffer_text);

    if (result.error_code != 0)
    {
        gtk_text_buffer_set_text(error_reporting_text_buffer,
                result.error_message, -1);
    }

    g_free(text_edit_text_buffer_text);
}

static void
doStopButtonCallback(GtkWidget *widget, ApplicationContext* data)
{
}

static void
doUpdateButtonCallback(GtkWidget *widget, ApplicationContext* data)
{
}

static void
doSaveButtonCallback(GtkWidget *widget, ApplicationContext* data)
{
}
