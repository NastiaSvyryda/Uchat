#include "uchat_client.h"

void *input(void *sock) {
    int *sockfd = (int *) sock;
    char buf[1024];

    while (1) {
        recv(*sockfd, buf, 1024, 0);
//        json = mx_json_parse(buf);

        mx_printstr("Message recived: ");
        mx_printstr(buf);
//        mx_printstr(json->data.message.text);
        mx_printstr("\n from: ");
//        mx_printint(json->data.message.client1_id);
        memset(buf, '\0', 1024);
//        mx_strdel(&json->data.message.text);
//        free(json);
    }

//    char recvBuff[1024];
//    char name[100];
//    char *json_str = NULL;
//    t_json_data *json = calloc(1, sizeof(t_json_data));
//    enum e_json_types type;
//
//    type = JS_LOG_IN;
//    json->type = JS_LOG_IN;
//    strcpy(json->pers_info.login, "arni");
//    strcpy(json->pers_info.password, "qwerty");
////    send messages
////    type = JS_MES_OUT;
////    json->message.client1_id = 1;
////    json->message.client2_id = 2;
////    json->type = JS_MES_OUT;
////    json->message.text = mx_strdup("hallo");
////        while(1) {
//    json_str = mx_json_make_json(type, json);
//    mx_printint(strlen(json_str + 4) + 4);
//    write(*sockfd, json_str, strlen(json_str + 4) + 4);
//    memset(recvBuff, '\0', 1024);
//    memset(name, '\0', 100);
////    }
//
//    return NULL;
}

void create_login_window(char **argv) {
    GtkBuilder *builder;
    GError     *error = NULL;
    int sockfd = 0;
    struct sockaddr_in serv;
    pthread_t thread = NULL;
    t_mainWindowObjects mainObjects;

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if( ! gtk_builder_add_from_file( builder, "login.glade", &error ) )
    {
        g_warning( "%s", error->message );
        g_free( error );
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
    if (pthread_create(&thread, NULL, input, &sockfd) != 0) {
        mx_printerr("uchat_server: thread creating error");
        exit(1);
    }


//END CONNECT
    /* Get main window pointer from UI */
    mainObjects.loginWindow = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    if (NULL == mainObjects.loginWindow)
    {
        /* Print out the error. You can use GLib's message logging  */
        fprintf(stderr, "Unable to file object with id \"window1\" \n");
        /* Your error handling code goes here */
    }
    mainObjects.entryLogin_l = GTK_ENTRY( gtk_builder_get_object(builder, "login_entry_l") );
    mainObjects.entryPass_l = GTK_ENTRY( gtk_builder_get_object(builder, "password_entry_l") );
    mainObjects.fd = sockfd;

    /* Connect signals */
    gtk_builder_connect_signals( builder, &mainObjects );

    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );

    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show_all ( GTK_WIDGET (mainObjects.loginWindow) );

    /* Start main loop */
    gtk_main();
}

void create_main_window(struct s_MainWindowObjects *mwo) {
    GtkBuilder *builder;
    GError     *error = NULL;

    /* Init GTK+ */
    //gtk_init( &argc, &argv );

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if( ! gtk_builder_add_from_file( builder, "menu.glade", &error ) )
    {
        g_warning( "%s", error->message );
        g_free( error );
    }

    /* Get main window pointer from UI */
    mwo->mainWindow = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    if (NULL == mwo->mainWindow)
    {
        /* Print out the error. You can use GLib's message logging  */
        fprintf(stderr, "Unable to file object with id \"window1\" \n");
        /* Your error handling code goes here */
    }

    /* Connect signals */
    gtk_builder_connect_signals( builder, mwo );

    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );

    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show_all ( GTK_WIDGET (mwo->mainWindow) );
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
        login.login = (char *) gtk_entry_get_text(
                GTK_ENTRY(mwo->entryLogin_l));
        login.password = (char *) gtk_entry_get_text(
                GTK_ENTRY(mwo->entryPass_l));
        printf("login = %s\npassword = %s\n", login.login, login.password);
        strcpy(json->pers_info.login, login.login);
        strcpy(json->pers_info.password, login.password);
        json_str = mx_json_make_json(JS_LOG_IN, json);
        write(mwo->fd, json_str, mx_strlen(json_str + 4) + 4);
        //
//        gtk_window_close(mwo->loginWindow);
//        //
//        create_main_window(mwo);
    }
}

void create_registre_window(struct s_MainWindowObjects* mwo) {
    //
    GtkBuilder *builder;
    GError     *error = NULL;

    /* Init GTK+ */
    //gtk_init( &argc, &argv );

    /* Create new GtkBuilder object */
    builder = gtk_builder_new();
    /* Load UI from file. If error occurs, report it and quit application.
     * Replace "tut.glade" with your saved project. */
    if( ! gtk_builder_add_from_file( builder, "registre.glade", &error ) )
    {
        g_warning( "%s", error->message );
        g_free( error );
    }

    /* Get main window pointer from UI */
    mwo->registreWindow = GTK_WINDOW(gtk_builder_get_object(builder, "main_window"));
    if (NULL == mwo->registreWindow)
    {
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
    gtk_builder_connect_signals( builder, mwo );

    /* Destroy builder, since we don't need it anymore */
    g_object_unref( G_OBJECT( builder ) );

    /* Show window. All other widgets are automatically shown by GtkBuilder */
    gtk_widget_show_all ( GTK_WIDGET (mwo->registreWindow) );
//
    /* Start main loop */
    gtk_main();
}

void on_butRegistreIn_clicked(GtkWidget *button, gpointer data) {
    if (button != NULL) {
        t_mainWindowObjects *mwo = (t_mainWindowObjects *) data;

        gtk_window_close(mwo->loginWindow);
        //
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
        registre.login = (char *) gtk_entry_get_text(
                GTK_ENTRY(mwo->entryLogin_r));
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
    gtk_init( &argc, &argv );

    if (argc != 3) {
        mx_printerr("uchat_server: error args\n");
        mx_printerr("example: ./uchat ip port\n");
        exit(1);
    }
//    write(sockfd, name, mx_strlen(name));

    create_login_window(argv);
}
