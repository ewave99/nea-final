#include <gtk/gtk.h>
#include "visuals.h"

typedef struct {
    GtkWidget *text_edit_text_view;
    GtkWidget *error_reporting_text_view;
    GtkWidget *drawing_area;
    int num_modules;
    AbstractModule *abstract_modules;
    VisualModule *visual_modules;
    int drawing;
    int num_connections;
    Connection *connections;
} ApplicationContext;

