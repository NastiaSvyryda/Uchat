#include "uchat_client.h"

void *input(void *data);




enum
{
    COL_NAME = 0,
    COL_AGE,
    NUM_COLS
} ;


static GtkTreeModel *
create_and_fill_model (void)
{
    GtkListStore  *store;
    GtkTreeIter    iter;

    store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);

    /* Append a row and fill in some data */
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter,
                        COL_NAME, "channel_1",
                        COL_AGE, 76458,
                        -1);

    /* append another row and fill in some data */
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter,
                        COL_NAME, "channel_2",
                        COL_AGE, 23,
                        -1);

    /* ... and a third row */
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter,
                        COL_NAME, "channel_3",
                        COL_AGE, time(NULL),
                        -1);

    return GTK_TREE_MODEL (store);
}

static GtkWidget *
create_view_and_model (void)
{
    GtkCellRenderer     *renderer;
    GtkTreeModel        *model;
    GtkWidget           *view;

    view = gtk_tree_view_new ();

    /* --- Column #1 --- */

    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                                 -1,
                                                 "channel",
                                                 renderer,
                                                 "text", COL_NAME,
                                                 NULL);

    /* --- Column #2 --- */

    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                                 -1,
                                                 "time",
                                                 renderer,
                                                 "text", COL_AGE,
                                                 NULL);

    model = create_and_fill_model ();

    gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

    /* The tree view has acquired its own reference to the
     *  model, so we can drop ours. That way the model will
     *  be freed automatically when the tree view is destroyed */

    g_object_unref (model);

    return view;
}




void create_main_window(struct s_MainWindowObjects *mwo) {
    GtkBuilder *builder;
    GError     *error = NULL;
    pthread_t thread = NULL;
    GtkWidget *view;

    /* Init GTK+ */
    //gtk_init( &argc, &argv );

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if(!gtk_builder_add_from_file( builder, "menu.glade", &error)) {
        g_warning("%s", error->message);
        g_free(error);
    }

    /* Get main window pointer from UI */
    mwo->mainWindow = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    if (NULL == mwo->mainWindow) {
        /* Print out the error. You can use GLib's message logging  */
        fprintf(stderr, "Unable to file object with id \"window1\" \n");
        /* Your error handling code goes here */
    }
    view = create_view_and_model ();
    gtk_container_add (GTK_CONTAINER (gtk_builder_get_object(builder, "messages_tree")), view);

    /* Connect signals */
    gtk_builder_connect_signals(builder, mwo);

    /* Destroy builder, since we don't need it anymore */
    g_object_unref(G_OBJECT(builder));

    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show_all (GTK_WIDGET(mwo->mainWindow));

    /* CREATING THREAD FOR RECIEVEING RESPONSES */

    if (pthread_create(&thread, NULL, input, mwo) != 0) {
        mx_printerr("uchat_server: thread creating error");
        exit(1);
    }
    /* Start main loop */
    gtk_main();

}

void *input(void *data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
    char buf[1024];
    while (1) {
        memset(buf, '\0', 1024);
        read(mwo->fd, buf, 1024);
        mx_printstr("Response recieved");
        if (strcmp(buf, "success") == 0) {
            gtk_window_close(mwo->loginWindow);
            create_main_window(mwo);
        }
    }
}

void create_login_window(char **argv) {
    GtkBuilder *builder;
    GError     *error = NULL;
    int sockfd = 0;
    struct sockaddr_in serv;
    t_mainWindowObjects mainObjects;

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if (!gtk_builder_add_from_file( builder, "login.glade", &error)  ) {
        g_warning("%s", error->message);
        g_free(error);
    }
//CONNECT TO SERVER

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        mx_printerr("uchat: couldn't create socket");
        exit(1);
    }

    serv.sin_family = AF_INET;
    serv.sin_port = htons(atoi(argv[2]));


    if ((inet_pton(AF_INET, argv[1], &serv.sin_addr)) <= 0) {
        mx_printerr("uchat: network adress isn't valid");
        exit(1);
    }
    if ((connect(sockfd, (struct sockaddr *)&serv, sizeof(serv))) < 0) {
        mx_printerr("uchat: connection failed\n");
        exit(1);
    }


//END CONNECT

    /* Get main window pointer from UI */
    mainObjects.loginWindow = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    if (NULL == mainObjects.loginWindow) {
        /* Print out the error. You can use GLib's message logging  */
        fprintf(stderr, "Unable to file object with id \"window1\" \n");
        /* Your error handling code goes here */
    }
    mainObjects.entryLogin_l = GTK_ENTRY( gtk_builder_get_object(builder, "login_entry_l") );
    mainObjects.entryPass_l = GTK_ENTRY( gtk_builder_get_object(builder, "password_entry_l") );
    mainObjects.fd = sockfd;

    /* Connect signals */
    gtk_builder_connect_signals(builder, &mainObjects);

    /* Destroy builder, since we don't need it anymore */
    g_object_unref(G_OBJECT(builder));

    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show_all (GTK_WIDGET(mainObjects.loginWindow));

    /* Start main loop */
    gtk_main();
}

void create_registre_window(struct s_MainWindowObjects* mwo) {
    GtkBuilder *builder;
    GError     *error = NULL;

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if(!gtk_builder_add_from_file( builder, "registre.glade", &error)) {
        g_warning("%s", error->message);
        g_free(error);
    }

    /* Get main window pointer from UI */
    mwo->registreWindow = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    if (NULL == mwo->registreWindow) {
        /* Print out the error. You can use GLib's message logging  */
        fprintf(stderr, "Unable to file object with id \"window1\" \n");
        /* Your error handling code goes here */
    }
    mwo->entryLogin_r = GTK_ENTRY( gtk_builder_get_object(builder, "login_entry_r") );
    mwo->entryPass_r = GTK_ENTRY( gtk_builder_get_object(builder, "password_entry_r") );
    mwo->entryPass_r2 = GTK_ENTRY( gtk_builder_get_object(builder, "password_entry_r2") );
    mwo->entryName_r = GTK_ENTRY( gtk_builder_get_object(builder, "name_entry_r") );
    mwo->entrySurname_r = GTK_ENTRY( gtk_builder_get_object(builder, "surname_entry_r") );
    /* Connect signals */
    gtk_builder_connect_signals(builder, mwo);

    /* Destroy builder, since we don't need it anymore */
    g_object_unref(G_OBJECT(builder));

    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show_all (GTK_WIDGET(mwo->registreWindow));
//
    /* Start main loop */
    gtk_main();
}

void on_butLogin_clicked(GtkWidget *button, gpointer data) {
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    if (button != NULL) {

        t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
        t_login login;
        login.type = strdup("login");
        login.login = (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryLogin_l));
        login.password = (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_l));
        printf("login = %s\npassword = %s\n", login.login, login.password);
        strcpy(json->pers_info.login, login.login);
        strcpy(json->pers_info.password, login.password);
        json_str = mx_json_make_json(JS_LOG_IN, json);
        write(mwo->fd, json_str, mx_strlen(json_str + 4) + 4);
        char buf[1024];
        read(mwo->fd, buf, 1024);
        if (strcmp(buf, "success") == 0) {
            gtk_window_close(mwo->loginWindow);

            create_main_window(mwo);
        }
    }
}

void on_butRegistreIn_clicked(GtkWidget *button, gpointer data) {
    if (button != NULL) {
        t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;
        gtk_window_close(mwo->loginWindow);
        create_registre_window(mwo);
    }
}

void on_butRegistre_clicked(GtkWidget *button, gpointer data) {
    char *json_str = NULL;
    t_json_data *json = calloc(1, sizeof(t_json_data));

    if (button != NULL) {
        t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;

        t_registre registre;
        puts("kjjhbjhbvhj");
        registre.type = strdup("register");
        registre.login = (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryLogin_r));
        if (strcmp((char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_r)), (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_r2))) == 0)
            registre.password = (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryPass_r));
        else
            printf("different passwords\n");
        registre.name = (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entryName_r));
        registre.surname = (char *) gtk_entry_get_text(GTK_ENTRY(mwo->entrySurname_r));
        strcpy(json->pers_info.login, registre.login);
        strcpy(json->pers_info.password, registre.password);
        strcpy(json->pers_info.first_name, registre.name);
        strcpy(json->pers_info.last_name, registre.surname);
        json_str = mx_json_make_json(JS_REG, json);
        write(mwo->fd, json_str, mx_strlen(json_str + 4) + 4);
        printf("login = %s\npassword = %s\nname = %s\nsurname = %s\n",
               registre.login, registre.password, registre.name,
               registre.surname);
        gtk_window_close(mwo->registreWindow);
        create_main_window(mwo);
    }
}

int main(int argc, char **argv) {
    /* Init GTK+ */
    gtk_init(&argc, &argv);

    if (argc != 3) {
        mx_printerr("uchat_server: error args\n");
        mx_printerr("example: ./uchat ip port\n");
        exit(1);
    }

    create_login_window(argv);
}
