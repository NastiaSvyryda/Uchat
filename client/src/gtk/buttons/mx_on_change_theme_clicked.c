#include "uchat_client.h"

void mx_dark(__attribute__((unused)) GtkWidget *button, gpointer data){
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    GtkCssProvider *provider = gtk_css_provider_new ();

    gtk_css_provider_load_from_path (provider,
                                     "./client/src/gui/css/dark.css", NULL);
    gtk_style_context_add_provider_for_screen (gdk_screen_get_default(),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_image_set_from_file (GTK_IMAGE(gtk_builder_get_object(mwo->builder, "send_image")), "./client/src/gui/images/bright_sent.png");
    gtk_image_set_from_file (GTK_IMAGE(gtk_builder_get_object(mwo->builder, "plus_image")), "./client/src/gui/images/bright_plus.png");
    gtk_image_set_from_file (GTK_IMAGE(gtk_builder_get_object(mwo->builder, "back_image1")), "./client/src/gui/images/bright_back.png");
    gtk_image_set_from_file (GTK_IMAGE(gtk_builder_get_object(mwo->builder, "back_image")), "./client/src/gui/images/bright_back.png");
    gtk_image_set_from_file (GTK_IMAGE(gtk_builder_get_object(mwo->builder, "settings_image")), "./client/src/gui/images/bright_settings.png");
}

void mx_bright(__attribute__((unused)) GtkWidget *button, gpointer data){
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    GtkCssProvider *provider = gtk_css_provider_new ();

    gtk_css_provider_load_from_path (provider,
                                     "./client/src/gui/css/bright.css", NULL);
    gtk_style_context_add_provider_for_screen (gdk_screen_get_default(),
                                               GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_image_set_from_file (GTK_IMAGE(gtk_builder_get_object(mwo->builder, "send_image")), "./client/src/gui/images/dark_sent.png");
    gtk_image_set_from_file (GTK_IMAGE(gtk_builder_get_object(mwo->builder, "plus_image")), "./client/src/gui/images/dark_plus.png");
    gtk_image_set_from_file (GTK_IMAGE(gtk_builder_get_object(mwo->builder, "back_image1")), "./client/src/gui/images/dark_back.png");
    gtk_image_set_from_file (GTK_IMAGE(gtk_builder_get_object(mwo->builder, "back_image")), "./client/src/gui/images/dark_back.png");
    gtk_image_set_from_file (GTK_IMAGE(gtk_builder_get_object(mwo->builder, "settings_image")), "./client/src/gui/images/dark_setting.png");
}

