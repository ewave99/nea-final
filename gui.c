#include <gtk/gtk.h>
#include <cairo/cairo.h>
#include "drawing.h"
#include "text.h"
//#include "application.h"

void activate(GtkApplication *app, gpointer user_data);
static void createWindow();
static GdkRectangle getScreenDimensions();
static GtkWidget* createLeftPane(GtkWidget **button_run_ptr,
        GtkWidget **button_stop_ptr, GtkWidget **button_update_ptr,
        GtkWidget **button_save_ptr, GtkWidget **button_load_ptr,
        GtkWidget **text_editing_area_view_ptr,
        GtkWidget **error_reporting_area_view_ptr);
static GtkWidget* createTextEditingArea(GtkWidget **text_view);
static GtkWidget* createControlButtonArea(GtkWidget **button_run,
        GtkWidget **button_stop, GtkWidget **button_update,
        GtkWidget **button_save, GtkWidget **button_load);
static GtkWidget* createErrorReportingArea(GtkWidget **text_view);
static GtkWidget* createRightPane();

static void doRunButtonCallback(GtkWidget *widget, ApplicationContext* context);
static void doStopButtonCallback(GtkWidget *widget, ApplicationContext* context);
static void doUpdateButtonCallback(GtkWidget *widget, ApplicationContext* context);
static void doSaveButtonCallback(GtkWidget *widget, ApplicationContext* context);
static void doLoadButtonCallback(GtkWidget *widget, ApplicationContext* data);

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
quitApplication(GtkWidget* window, ApplicationContext* context)
{
    if (context->abstract_modules != NULL)
        free(context->abstract_modules);
    if (context->visual_modules != NULL)
        free(context->visual_modules);

    free(context);

    gtk_main_quit();
}

static void
createWindow()
{
    GdkRectangle screen_dimensions;

    GtkWidget *window;
    GtkWidget *grid;

    GtkWidget *left_pane;
    GtkWidget *right_pane;

    GtkWidget *button_run;
    GtkWidget *button_stop;
    GtkWidget *button_update;
    GtkWidget *button_save;
    GtkWidget *button_load;

    GtkWidget *text_editing_area_view;
    GtkWidget *error_reporting_area_view;

    ApplicationContext *context;

    int canvas_width;
    int canvas_height;

    screen_dimensions = getScreenDimensions();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), screen_dimensions.width,
            screen_dimensions.height);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_title(GTK_WINDOW(window), "NEA: Music Interface");

    grid = gtk_grid_new();

    left_pane = createLeftPane(&button_run, &button_stop, &button_update,
            &button_save, &button_load, &text_editing_area_view,
            &error_reporting_area_view);
    right_pane = createRightPane(&canvas_width, &canvas_height);

    gtk_grid_attach(GTK_GRID(grid), left_pane, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), right_pane, 1, 1, 1, 1);

    gtk_container_add(GTK_CONTAINER(window), grid);

    context = (ApplicationContext*) malloc(sizeof(ApplicationContext));

    context->text_edit_text_view = text_editing_area_view;
    context->error_reporting_text_view = error_reporting_area_view;
    context->drawing_area = right_pane;
    context->canvas_width = canvas_width;
    context->canvas_height = canvas_height;

    context->visual_modules = (VisualModule*) malloc(
            sizeof(VisualModule) * context->num_modules);

    context->drawing = 0; //don't draw initially

    context->num_connections = 2;

    context->connections = (Connection*) malloc(
            sizeof(Connection) * context->num_connections);
    context->connections[0] = (Connection){ 0, 0, 1, 0 };
    context->connections[1] = (Connection){ 1, 0, 2, 0 };

    g_signal_connect(button_run, "clicked",
            G_CALLBACK(doRunButtonCallback), context);
    g_signal_connect(button_stop, "clicked",
            G_CALLBACK(doStopButtonCallback), context);
    g_signal_connect(button_update, "clicked",
            G_CALLBACK(doUpdateButtonCallback), context);
    g_signal_connect(button_save, "clicked",
            G_CALLBACK(doSaveButtonCallback), context);
    g_signal_connect(button_load, "clicked",
            G_CALLBACK(doLoadButtonCallback), context);

    g_signal_connect(G_OBJECT(right_pane), "draw", G_CALLBACK(onDrawEvent),
            context);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(quitApplication),
            context);

    //this is what actually makes it draw. We don't want it to draw anything
    //right at the beginning.
    //gtk_widget_queue_draw(drawing_area);

    gtk_widget_show_all(window);
}

static GtkWidget*
createLeftPane(GtkWidget **button_run_ptr, GtkWidget** button_stop_ptr,
        GtkWidget **button_update_ptr, GtkWidget **button_save_ptr, GtkWidget
        **button_load_ptr, GtkWidget **text_editing_area_view_ptr, GtkWidget
        **error_reporting_area_view_ptr)
{
    GtkWidget* grid;

    GtkWidget* text_editing_area;
    GtkWidget* control_button_area;
    GtkWidget* error_reporting_area;

    grid = gtk_grid_new();
    text_editing_area = createTextEditingArea(text_editing_area_view_ptr);
    control_button_area = createControlButtonArea(button_run_ptr,
            button_stop_ptr, button_update_ptr, button_save_ptr,
            button_load_ptr);
    error_reporting_area = createErrorReportingArea(
            error_reporting_area_view_ptr);

    gtk_grid_attach(GTK_GRID(grid), text_editing_area, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), control_button_area, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), error_reporting_area, 0, 3, 1, 1);

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
        GtkWidget **button_update, GtkWidget **button_save, GtkWidget **button_load)
{
    GtkWidget* hbox;

    hbox = gtk_hbox_new(TRUE, 10);

    *button_run    = gtk_button_new_with_label("Run");
    *button_stop   = gtk_button_new_with_label("Stop");
    *button_update = gtk_button_new_with_label("Update");
    *button_save   = gtk_button_new_with_label("Save");
    *button_load   = gtk_button_new_with_label("Load");

    gtk_box_pack_start(GTK_BOX(hbox),    *button_run, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),   *button_stop, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), *button_update, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),   *button_save, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox),   *button_load, TRUE, TRUE, 0);

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
createRightPane(int *canvas_width, int *canvas_height)
{
    GtkWidget* drawing_area;
    GdkRectangle screen_dimensions;

    screen_dimensions = getScreenDimensions();
    *canvas_width = screen_dimensions.width / 2;
    *canvas_height = screen_dimensions.height;

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, *canvas_width, *canvas_height);

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

    error_reporting_text_buffer = gtk_text_view_get_buffer(
            GTK_TEXT_VIEW(context->error_reporting_text_view));

    text_edit_text_buffer = gtk_text_view_get_buffer(
            GTK_TEXT_VIEW(context->text_edit_text_view));

    gtk_text_buffer_get_bounds(text_edit_text_buffer, &text_edit_buffer_start,
            &text_edit_buffer_end);

    text_edit_text_buffer_text = gtk_text_buffer_get_text(
            text_edit_text_buffer, &text_edit_buffer_start,
            &text_edit_buffer_end, FALSE);

    ParseContext parsed = processText(text_edit_text_buffer_text);
    gtk_text_buffer_set_text(error_reporting_text_buffer,
            parsed.errmsg, -1);
    g_free(text_edit_text_buffer_text);

    if (parsed.err != 0)
        return;
    if (parsed.result.num_modules == 0)
        return;

    context->num_modules = parsed.result.num_modules;

    context->abstract_modules = (AbstractModule*) realloc(
            context->abstract_modules, 0);
    context->abstract_modules = parsed.result.modules;

    context->num_connections = parsed.result.num_connections;
    context->connections = (Connection*) realloc(context->connections, 0);
    context->connections = parsed.result.connections;

    context->visual_modules = (VisualModule*) realloc(context->visual_modules,
            sizeof(VisualModule) * context->num_modules);

    convertAbstractModulesToVisualModules(context->num_modules,
            context->abstract_modules, context->visual_modules);

    gtk_widget_queue_draw(context->drawing_area);
}

static void
doStopButtonCallback(GtkWidget *widget, ApplicationContext* context)
{
    GtkTextBuffer* error_reporting_text_buffer;

    error_reporting_text_buffer = gtk_text_view_get_buffer(
            GTK_TEXT_VIEW(context->error_reporting_text_view));

    gtk_text_buffer_set_text(error_reporting_text_buffer,
            "Stopped successfully.", -1);
}

static void
doUpdateButtonCallback(GtkWidget *widget, ApplicationContext* context)
{
    GtkTextBuffer* text_edit_text_buffer;
    GtkTextIter text_edit_buffer_start;
    GtkTextIter text_edit_buffer_end;
    gchar* text_edit_text_buffer_text;

    GtkTextBuffer* error_reporting_text_buffer;

    error_reporting_text_buffer = gtk_text_view_get_buffer(
            GTK_TEXT_VIEW(context->error_reporting_text_view));

    text_edit_text_buffer = gtk_text_view_get_buffer(
            GTK_TEXT_VIEW(context->text_edit_text_view));

    gtk_text_buffer_get_bounds(text_edit_text_buffer, &text_edit_buffer_start,
            &text_edit_buffer_end);

    text_edit_text_buffer_text = gtk_text_buffer_get_text(
            text_edit_text_buffer, &text_edit_buffer_start,
            &text_edit_buffer_end, FALSE);

    ParseContext parsed = processText(text_edit_text_buffer_text);
    gtk_text_buffer_set_text(error_reporting_text_buffer,
            parsed.errmsg, -1);
    g_free(text_edit_text_buffer_text);

    if (parsed.err != 0)
        return;
    if (parsed.result.num_modules == 0)
        return;

    context->num_modules = parsed.result.num_modules;

    context->abstract_modules = (AbstractModule*) realloc(
            context->abstract_modules, 0);
    context->abstract_modules = parsed.result.modules;

    context->num_connections = parsed.result.num_connections;
    context->connections = (Connection*) realloc(context->connections, 0);
    context->connections = parsed.result.connections;

    context->visual_modules = (VisualModule*) realloc(context->visual_modules,
            sizeof(VisualModule) * context->num_modules);

    convertAbstractModulesToVisualModules(context->num_modules,
            context->abstract_modules, context->visual_modules);

    gtk_widget_queue_draw(context->drawing_area);
}

static void
doSaveButtonCallback(GtkWidget *widget, ApplicationContext* context)
{
    GtkTextBuffer* text_edit_text_buffer;
    GtkTextIter text_edit_buffer_start;
    GtkTextIter text_edit_buffer_end;
    gchar* text_edit_text_buffer_text;

    GtkTextBuffer* error_reporting_text_buffer;

    int error_value;

    FILE* file_ptr;

    text_edit_text_buffer = gtk_text_view_get_buffer(
            GTK_TEXT_VIEW(context->text_edit_text_view));

    gtk_text_buffer_get_bounds(text_edit_text_buffer, &text_edit_buffer_start,
            &text_edit_buffer_end);

    text_edit_text_buffer_text = gtk_text_buffer_get_text(
            text_edit_text_buffer, &text_edit_buffer_start,
            &text_edit_buffer_end, FALSE);

    error_reporting_text_buffer = gtk_text_view_get_buffer(
            GTK_TEXT_VIEW(context->error_reporting_text_view));

    gtk_text_buffer_set_text(error_reporting_text_buffer,
            "Saving file.", -1);

    file_ptr = fopen("saved.txt", "w");

    if (file_ptr == NULL)
    {
        gtk_text_buffer_set_text(error_reporting_text_buffer,
                "Could not open or create the file for writing.", -1);
        g_free(text_edit_text_buffer_text);
        return;
    }

    error_value = fprintf(file_ptr, "%s", text_edit_text_buffer_text);
    
    if (error_value < 0)
        gtk_text_buffer_set_text(error_reporting_text_buffer,
                "There was an error writing the text to the file.", -1);

    fclose(file_ptr);

    g_free(text_edit_text_buffer_text);

    gtk_text_buffer_set_text(error_reporting_text_buffer,
            "File saved successfully.", -1);
}

static void
doLoadButtonCallback(GtkWidget *widget, ApplicationContext* context)
{
    GtkTextBuffer* text_edit_text_buffer;
    GtkTextBuffer* error_reporting_text_buffer;

    int error_value;

    FILE* file_ptr;
    long num_bytes;

    char* file_buffer;

    text_edit_text_buffer = gtk_text_view_get_buffer(
            GTK_TEXT_VIEW(context->text_edit_text_view));

    error_reporting_text_buffer = gtk_text_view_get_buffer(
            GTK_TEXT_VIEW(context->error_reporting_text_view));

    gtk_text_buffer_set_text(error_reporting_text_buffer,
            "Loading file.", -1);

    file_ptr = fopen("saved.txt", "r");

    if (file_ptr == NULL)
    {
        gtk_text_buffer_set_text(error_reporting_text_buffer,
                "Could not open the file for reading.", -1);
        return;
    }

    // actually load the contents of the file into memory

    fseek(file_ptr, 0L, SEEK_END);
    num_bytes = ftell(file_ptr);
    fseek(file_ptr, 0L, SEEK_SET);

    file_buffer = (char*) calloc(num_bytes, sizeof(char));

    if (file_buffer == NULL)
    {
        gtk_text_buffer_set_text(error_reporting_text_buffer,
                "Error allocating memory for file.", -1);
        fclose(file_ptr);
        return;
    }

    fread(file_buffer, sizeof(char), num_bytes, file_ptr);

    fclose(file_ptr);

    gtk_text_buffer_set_text(text_edit_text_buffer,
            file_buffer, -1);

    gtk_text_buffer_set_text(error_reporting_text_buffer,
            "File loaded successfully.", -1);
}
