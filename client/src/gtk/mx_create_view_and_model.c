#include "uchat_client.h"

enum
{
    COL_NAME = 0,
    COL_TIME,
    NUM_COLS
} ;

GtkListStore  *mx_fill_model(GtkListStore  *store) {
    GtkTreeIter    iter;

    /* Append a row and fill in some data */
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter, COL_NAME, "channel_1", COL_TIME, 76458, -1);
    /* append another row and fill in some data */
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter, COL_NAME, "channel_2", COL_TIME, 23, -1);
    /* ... and a third row */
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter, COL_NAME, "channel_3", COL_TIME, time(NULL), -1);
    return store;
}

static GtkTreeModel *create_model(void) {
    GtkListStore  *store;

    store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);
    /* Append a row and fill in some data */
    store = mx_fill_model(store);
    return GTK_TREE_MODEL (store);
}

GtkWidget *mx_create_view_and_model (void) {
    GtkCellRenderer     *renderer;
    GtkTreeModel        *model;
    GtkWidget           *view;

    view = gtk_tree_view_new ();
    /* --- Column #1 --- */
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "channel", renderer, "text", COL_NAME, NULL);
    /* --- Column #2 --- */
    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view), -1, "time", renderer, "text", COL_TIME, NULL);
    model = create_model();
    gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);
    /* The tree view has acquired its own reference to the
     *  model, so we can drop ours. That way the model will
     *  be freed automatically when the tree view is destroyed */
    g_object_unref (model);
    return view;
}

