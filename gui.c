#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include "drawing.h"

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

void activate(GtkApplication *app, gpointer user_data);
static void createWindow();
static GdkRectangle getScreenDimensions();
static GtkWidget* createLeftPane();
static GtkWidget* createTextEditingArea();
static GtkWidget* createControlButtonArea();
static GtkWidget* createErrorReportingArea();
static GtkWidget* createRightPane();

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

    grid = gtk_grid_new();
    text_editing_area = createTextEditingArea();
    control_button_area = createControlButtonArea();

    gtk_grid_attach(GTK_GRID(grid), text_editing_area, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), control_button_area, 0, 2, 1, 1);

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
            screen_dimensions.width / 2, screen_dimensions.height * 2 / 3);

    text_view = gtk_text_view_new();
    gtk_widget_set_size_request(GTK_WIDGET(text_view),
            screen_dimensions.width / 2, screen_dimensions.height * 2 / 3);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(text_view), TRUE);

    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    return scrolled_window;
}

static GtkWidget*
createControlButtonArea()
{
    GtkWidget* hbox;
    GtkWidget* button_run;
    GtkWidget* button_stop;
    GtkWidget* button_update;
    GtkWidget* button_save;

    hbox = gtk_hbox_new(TRUE, 10);

    button_run    = gtk_button_new_with_label("Run");
    button_stop   = gtk_button_new_with_label("Stop");
    button_update = gtk_button_new_with_label("Update");
    button_save   = gtk_button_new_with_label("Save");

    gtk_box_pack_start(GTK_BOX(hbox),    button_run, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),   button_stop, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), button_update, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),   button_save, TRUE, TRUE, 0);

    return hbox;
}

static GtkWidget*
createErrorReportingArea()
{
    GtkWidget* scrolled_window;
    GtkWidget* text_view;
    GdkRectangle screen_dimensions;

    screen_dimensions = getScreenDimensions();

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
            GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window),
            screen_dimensions.width / 2, screen_dimensions.height / 6);

    text_view = gtk_text_view_new();
    gtk_widget_set_size_request(GTK_WIDGET(text_view),
            screen_dimensions.width / 2, screen_dimensions.height / 6);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(text_view), TRUE);

    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

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
