#include <gtk/gtk.h>
#include <cairo/cairo.h>

static void doDrawing(cairo_t *canvas)
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

static gboolean onDrawEvent(GtkWidget *widget, cairo_t *canvas,
        gpointer user_data)
{
    doDrawing(canvas);

    return FALSE;
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *view;
    GtkWidget *drawing_area;
    GtkTextBuffer *buffer;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    GdkRectangle workarea = {0};
    gdk_monitor_get_workarea(
            gdk_display_get_primary_monitor(gdk_display_get_default()),
            &workarea);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), workarea.width,
            workarea.height);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_title(GTK_WINDOW(window), "NEA: Music Interface");

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    view = gtk_text_view_new();
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
            GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    gtk_container_add(GTK_CONTAINER(scrolled_window), view);

    int HALF_WIDTH = workarea.width / 2;

    gtk_widget_set_size_request(GTK_WIDGET(scrolled_window),
            workarea.width / 2, workarea.height);
    gtk_widget_set_size_request(GTK_WIDGET(view), HALF_WIDTH,
            workarea.height);
    gtk_widget_set_size_request(GTK_WIDGET(grid), HALF_WIDTH,
            workarea.height);

    gtk_box_pack_start(GTK_BOX(box), view, 1, 1, 1);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD_CHAR);
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    gtk_text_buffer_set_text(buffer, "he", -1);

    gtk_text_view_set_monospace(GTK_TEXT_VIEW(view), TRUE);

    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 1, 1, 1);
    gtk_widget_set_hexpand_set(GTK_WIDGET(view), FALSE);

    drawing_area = gtk_drawing_area_new();
    gtk_grid_attach(GTK_GRID(grid), drawing_area, 1, 1, 1, 1);
    gtk_widget_set_size_request(drawing_area, HALF_WIDTH,
            workarea.height);

    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(onDrawEvent),
            NULL);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),
            NULL);

    gtk_widget_show_all(window);

    gtk_widget_queue_draw(drawing_area);

    gtk_main();
}

int main(int argc, char *argv[])
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.nea", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
