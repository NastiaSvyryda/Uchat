#include "uchat_client.h"

//GObject *
//g_object_clone(GObject *src)
//{
//    GObject *dst;
//    GParameter *params;
//    GParamSpec **specs;
//    guint n, n_specs, n_params;
//
//    specs = g_object_class_list_properties(G_OBJECT_GET_CLASS(src), &n_specs);
//    params = g_new0(GParameter, n_specs);
//    n_params = 0;
//
//    for (n = 0; n < n_specs; ++n)
//        if (strcmp(specs[n]->name, "parent") &&
//            (specs[n]->flags & G_PARAM_READWRITE) == G_PARAM_READWRITE) {
//            params[n_params].name = g_intern_string(specs[n]->name);
//            g_value_init(&params[n_params].value, specs[n]->value_type);
//            g_object_get_property(src, specs[n]->name, &params[n_params].value);
//            ++ n_params;
//        }
//
//    dst = g_object_newv(G_TYPE_FROM_INSTANCE(src), n_params, params);
//    g_free(specs);
//    g_free(params);
//
//    return dst;
//}

static void fill_channel_info(t_mainWindowObjects *mwo, t_json_data *json) {
    gchar *text;
    char *temp = NULL;
//    GtkWidget button;

    for (int i = 0; i < json->channels_arr_size; i++) {//задать первый
        temp = mx_strtrim_qouts(json->channels_arr[i].channel_name);
        strcpy(mwo->channel_info->channel_data.channel_name, temp);
        text = g_strdup_printf("%s", mwo->channel_info->channel_data.channel_name);
        mwo->channel_info->channel_data.channel_id = json->channels_arr[i].channel_id;
        mwo->channel_info->channel_data.last_mes_time = json->channels_arr[i].last_mes_time;
        mwo->channel_info->channel_data.user_ids_size = json->channels_arr[i].user_ids_size;
        mwo->channel_info->channel_data.user_ids = malloc(sizeof(int) * mwo->channel_info->channel_data.user_ids_size);
        for(int j = 0; j < json->channels_arr[i].user_ids_size; j++) {
            mwo->channel_info->channel_data.user_ids[j] = json->channels_arr[i].user_ids[j];
        }
        mwo->channel_info->message = NULL;
        mwo->channel_info->messageList = gtk_list_box_new();
        gtk_list_box_set_selection_mode(GTK_LIST_BOX(mwo->channel_info->messageList), GTK_SELECTION_NONE);
//        gtk_container_add(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "scrolled_chat")), mwo->curr_messageList);
//        mwo->channel_info->messageList = g_object_ref(mwo->curr_messageList);
//        gtk_container_remove(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "scrolled_chat")), mwo->curr_messageList);
//        gtk_container_add(GTK_CONTAINER(gtk_builder_get_object(mwo->builder, "scrolled_chat")), mwo->channel_info->messageList);
        mwo->channel_info->chat_button = mx_create_chat(mwo->channel_info->channel_data.channel_name, mwo);
        gtk_list_box_insert(GTK_LIST_BOX(mwo->chatList), mwo->channel_info->chat_button, 0);
        free(text);
        mx_strdel(&temp);
        mwo->channel_info->next = malloc(sizeof(t_channel_info));
        mwo->channel_info->next->first = mwo->channel_info->first;//????
        mwo->channel_info = mwo->channel_info->next;
        mwo->channel_info->next = NULL;
        mwo->channel_info->message = NULL;
    }
    gtk_widget_show_all(mwo->mainWindow);
}

static void fill_message_info_(t_mainWindowObjects *mwo, t_json_data *json) {
    t_message_list *temp_mess = NULL;
    t_message_list *temp = NULL;

    mwo->channel_info = mwo->channel_info->first;
    //if (mwo->channel_info != NULL) {
    while (mwo->channel_info->next != NULL) {
        if (mwo->channel_info->channel_data.channel_id ==
            json->message.channel_id)
            break;
        mwo->channel_info = mwo->channel_info->next;
    }
    //}
    if (mwo->curr_chat_users != NULL && mwo->curr_chat != NULL) {
        strcpy(mwo->channel_info->channel_data.channel_name, mwo->curr_chat);
        mwo->channel_info->channel_data.channel_id = json->message.channel_id;
        mwo->channel_info->channel_data.last_mes_time = json->message.delivery_time;
        mwo->channel_info->channel_data.user_ids_size = mx_arrlen(mwo->curr_chat_users) + 1;
        mwo->channel_info->channel_data.user_ids = malloc(sizeof(int) * mwo->channel_info->channel_data.user_ids_size);
        for (int i = 0; i < mwo->channel_info->channel_data.user_ids_size; i++) {
            mwo->channel_info->channel_data.user_ids[i] = mwo->user_ids[i];
        }
        mwo->channel_info->message = NULL;
        mwo->channel_info->messageList = gtk_list_box_new();
        gtk_list_box_set_selection_mode(GTK_LIST_BOX(mwo->channel_info->messageList), GTK_SELECTION_NONE);
        gtk_container_add(GTK_CONTAINER(mwo->viewPort), mwo->channel_info->messageList);
        mwo->channel_info->next = malloc(sizeof(t_channel_info));
        //mwo->channel_info->first = mwo->channel_info;
        mwo->channel_info->next->first = mwo->channel_info->first;
        mwo->channel_info->next->chat_button = NULL;
        mwo->channel_info->next->next = NULL;
        mwo->channel_info->next->message = NULL;
        mwo->channel_info->chat_button = mx_create_chat(mwo->curr_chat, mwo); //change signal connectors
        gtk_list_box_insert(GTK_LIST_BOX(mwo->chatList), mwo->channel_info->chat_button, 0);
        free(mwo->user_ids);
        mwo->user_ids = NULL;
        g_free(mwo->curr_chat);
        mwo->curr_chat = NULL;
        mx_del_strarr(&mwo->curr_chat_users);
    }
    temp_mess = malloc(sizeof(t_message_list));
    temp_mess->channel_id = json->message.channel_id;
    temp_mess->message_id = json->message.message_id;
    temp_mess->delivery_time = json->message.delivery_time;
    temp_mess->text = mx_strdup(mwo->curr_message);
    temp_mess->next = NULL;
    if (mwo->channel_info->message == NULL) {
        temp_mess->first = temp_mess;
        mwo->channel_info->message = temp_mess;
        mwo->channel_info->message->mess_row = mx_create_message(mwo->curr_message, mwo, 0); //change signal connectors
        gtk_list_box_insert(GTK_LIST_BOX(mwo->channel_info->messageList), mwo->channel_info->message->mess_row, -1);
    }
    else {
        temp = mwo->channel_info->message;
        mwo->channel_info->message = temp_mess;
        mwo->channel_info->message->next = temp;
        t_message_list *first = mwo->channel_info->message;
        while (mwo->channel_info->message != NULL) {
            mwo->channel_info->message->first = first;
            mwo->channel_info->message = mwo->channel_info->message->next;
        }
        mwo->channel_info->message = first;
        mwo->channel_info->message->mess_row = mx_create_message(mwo->curr_message, mwo, 0); //change signal connectors
        gtk_list_box_insert(GTK_LIST_BOX(mwo->channel_info->messageList), mwo->channel_info->message->mess_row, -1);
    }
    mwo->curr_channel_info = mwo->channel_info;
    mx_strdel(&mwo->curr_message);
    GtkWidget *scrolled_window = GTK_WIDGET(gtk_builder_get_object(mwo->builder, "chatScrolledWindow"));

    //gtk_container_add(GTK_CONTAINER(mainObjects->viewPort), mwo->channel_info->messageList);

    GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment((GtkScrolledWindow *)scrolled_window);
    gtk_adjustment_set_value(adjustment, gtk_adjustment_get_upper(adjustment));
    gtk_scrolled_window_set_vadjustment((GtkScrolledWindow *)scrolled_window, (GtkAdjustment *)adjustment);
    gtk_widget_show_all(GTK_WIDGET(mwo->Window));

}

static void push_front_message_list(t_message_list **list, t_json_data *json, int index, t_mainWindowObjects *mwo) {
    t_message_list *front_list = NULL;
    t_message_list *temp = NULL;
    char *temp_mess = NULL;

    front_list = malloc(sizeof(t_message_list));
    front_list->channel_id = json->message.channel_id;
    front_list->message_id = json->messages_arr[index].message_id;
    front_list->delivery_time = json->messages_arr[index].delivery_time;
    //front_list->text = mx_strdup(json->messages_arr[index].text);
    temp_mess = mx_strtrim_qouts(json->messages_arr[index].text);
    front_list->text = mx_strdup(temp_mess);
    strcpy(front_list->login, json->messages_arr[index].login);
//    if (json->messages_arr[index].client1_id == mwo->user_id)
//        front_list->mess_row = mx_create_message(front_list->text, mwo, 0); //change signal connectors
//    else
//        front_list->mess_row = mx_create_message(front_list->text, mwo, 1);
//    gtk_list_box_insert(GTK_LIST_BOX(mwo->channel_info->messageList), front_list->mess_row, 0);
    front_list->next = NULL;
    front_list->first = front_list;
    if (list == NULL)
        mwo->channel_info->message = front_list;
    else {
        temp = *list;
        front_list->next = temp;
        *list = front_list;
        while ((*list)->next != NULL) {
            (*list)->first = front_list;
            *list = (*list)->next;
        }
        (*list)->first = front_list;
        (*list) = front_list;
    }
    mx_strdel(&temp_mess);
}

gboolean mx_reciever(__attribute__((unused)) GIOChannel *chan, __attribute__((unused)) GIOCondition condition, void *data)
{
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    char *json_str = NULL;
    int length = 0;
    t_json_data *json = NULL;
    SSL_read(mwo->ssl, &length, 4);
    if ( length > 0) {
        json_str = mx_strnew(length);
        mx_SSL_read_to_str(mwo->ssl, json_str, length);
        json = mx_json_parse(json_str);
        mx_printstr("Response recieved:\n");
        mx_printstr(json_str);
        mx_printchar('\n');
        if (json != NULL) {
            if (json->type == JS_REG) {
                if (json->status == 200) {
                    mwo->user_id = json->user_id;
                    strcpy(mwo->login, json->pers_info.login);
                    strcpy(mwo->first_name, json->pers_info.first_name);
                    strcpy(mwo->last_name, json->pers_info.last_name);
                    strcpy(mwo->token, json->token);
                    gtk_label_set_text(GTK_LABEL(mwo->label_login),
                                       (const gchar *) mwo->login);
                    mx_set_component(mwo, mwo->mainWindow);
                }
                if (json->status == 412) {
                    mx_show_popup(mwo->Window, "Such user already exist!");
                }
            } else if (json->type == JS_LOG_IN) {
                if (json->status == 200) {
                    mwo->user_id = json->user_id;
                    strcpy(mwo->login, json->pers_info.login);
                    strcpy(mwo->first_name, json->pers_info.first_name);
                    strcpy(mwo->last_name, json->pers_info.last_name);
                    strcpy(mwo->token, json->token);
                    gtk_label_set_text(GTK_LABEL(mwo->label_login),
                                       (const gchar *) mwo->login);//change label
                    fill_channel_info(mwo, json);
                    mx_set_component(mwo, mwo->mainWindow);
                }
                if (json->status == 401) {
                    mx_show_popup(mwo->Window, "Invalid user or password!");
                }
            }
            else if (json->type == JS_MES_IN) {
                mx_printchar('\a');
                mx_add_out_message(mwo, json);
            }
            else if (json->type == JS_MES_OUT) {
                fill_message_info_(mwo, json);
            }
            else if (json->type == JS_MES_DEL_IN) {
                mwo->channel_info = mwo->channel_info->first;
                while (mwo->channel_info->next != NULL) {
                    if (mwo->channel_info->channel_data.channel_id ==
                        json->message.channel_id)
                        break;
                    mwo->channel_info = mwo->channel_info->next;
                }
                if (mwo->channel_info->message != NULL) {
                    mwo->channel_info->message = mwo->channel_info->message->first;
                    while (mwo->channel_info->message->next != NULL) {
                        if (mwo->channel_info->message->message_id ==
                            json->message.message_id)
                            break;
                        mwo->channel_info->message = mwo->channel_info->message->next;
                    }

                    gtk_widget_destroy(mwo->channel_info->message->mess_row);
                    gtk_widget_show_all(GTK_WIDGET(mwo->Window));
                }
            }
            else if (json->type == JS_MES_EDIT_IN) {
                mwo->channel_info = mwo->channel_info->first;
                while (mwo->channel_info->next != NULL) {
                    if (mwo->channel_info->channel_data.channel_id ==
                        json->message.channel_id)
                        break;
                    mwo->channel_info = mwo->channel_info->next;
                }
                if (mwo->channel_info->message != NULL) {
                    mwo->channel_info->message = mwo->channel_info->message->first;
                    while (mwo->channel_info->message->next != NULL) {
                        if (mwo->channel_info->message->message_id ==
                            json->message.message_id)
                            break;
                        mwo->channel_info->message = mwo->channel_info->message->next;
                    }

                    GList *children, *iter;
                    GtkWidget *label_new;
                    GtkWidget *label_login;
                    GtkWidget *button;
                    mx_strdel(&mwo->channel_info->message->text);
                    mwo->channel_info->message->text = mx_strdup(json->message.text);
                    children = gtk_container_get_children(GTK_CONTAINER(mwo->channel_info->message->mess_row));
                    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
                        gtk_container_remove(GTK_CONTAINER(mwo->channel_info->message->mess_row), GTK_WIDGET(iter->data));
                    }
                    button = gtk_button_new();
                    label_new = gtk_label_new(mwo->channel_info->message->text);
                    gtk_label_set_line_wrap((GtkLabel *)label_new, TRUE);
                    gtk_label_set_line_wrap_mode((GtkLabel *)label_new, PANGO_WRAP_WORD_CHAR);
                    gtk_container_add(GTK_CONTAINER(button), label_new);
                    gtk_widget_set_hexpand(button, TRUE);
                    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
                    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);

                    char *tmp = mx_strtrim_qouts(mwo->channel_info->message->login);
                    label_login = gtk_label_new(tmp);//
                    gtk_container_add(GTK_CONTAINER(mwo->channel_info->message->mess_row), label_login);//
                    gtk_widget_set_halign(label_login, GTK_ALIGN_START);//

                    gtk_container_add(GTK_CONTAINER(mwo->channel_info->message->mess_row), button);
                    //gtk_widget_set_halign(mwo->channel_info->message->mess_row, GTK_ALIGN_END);
//                    g_object_set_data(G_OBJECT(button), (gchar *)"message_info", mwo->channel_info->message);
//                    g_signal_connect(button, "button-press-event", G_CALLBACK(mx_on_message_clicked), mwo);
                    gtk_widget_show_all(GTK_WIDGET(mwo->Window));




                }
            }
            else if (json->type == JS_GET_USERS) {//для добавления юзеров
                puts("GET_USERS reciever");
                GtkWidget *row;
                GtkWidget *label;
                gchar *text;
                char *temp = NULL;
                int k = 0;
                mwo->ids_logins_arr = malloc(
                        sizeof(t_id_login) * (json->ids_logins_arr_size - 1));
                mwo->ids_logins_arr_size = json->ids_logins_arr_size - 1;
                for (int i = 0; i < json->ids_logins_arr_size; i++) {
                    if (mwo->user_id != json->ids_logins_arr[i].user_id) {
                        temp = mx_strtrim_qouts(json->ids_logins_arr[i].login);
                        strcpy(mwo->ids_logins_arr[k].login, temp);
                        mwo->ids_logins_arr[k].user_id = json->ids_logins_arr[i].user_id;
                        text = g_strdup_printf("%s",
                                               mwo->ids_logins_arr[k].login);
                        row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
                        label = gtk_label_new(text);
                        gtk_container_add(GTK_CONTAINER(row), label);
                        gtk_list_box_insert(GTK_LIST_BOX(mwo->usersList), row,
                                            0);
                        free(text);
                        mx_strdel(&temp);
                        k++;
                    }
                }
                gtk_widget_show_all(mwo->addChat_Dialog);
            }
            else if (json->type ==
                       JS_MES_HIST) {

                //mwo->channel = 1;
                mwo->channel_info = mwo->channel_info->first;
                while (mwo->channel_info->next != NULL) {
                    if (mwo->channel_info->channel_data.channel_id ==
                        json->message.channel_id)
                        break;
                    mwo->channel_info = mwo->channel_info->next;
                }
                for (int i = 0; i < json->messages_arr_size; i++) {
                    if (mwo->channel_info->message == NULL)
                        push_front_message_list(NULL, json, i,mwo);
                    else
                        push_front_message_list(&mwo->channel_info->message,
                                                json, i, mwo);
                    if (json->messages_arr[i].client1_id == mwo->user_id)
                        mwo->channel_info->message->mess_row = mx_create_message(
                                mwo->channel_info->message->text, mwo,
                                0); //change signal connectors
                    else
                        mwo->channel_info->message->mess_row = mx_create_message(
                                mwo->channel_info->message->text, mwo, 1);
                    gtk_list_box_insert(
                            GTK_LIST_BOX(mwo->channel_info->messageList),
                            mwo->channel_info->message->mess_row, 0);
                }
                GtkWidget *scrolled_window = GTK_WIDGET(
                        gtk_builder_get_object(mwo->builder,
                                               "chatScrolledWindow"));

                //gtk_container_add(GTK_CONTAINER(mainObjects->viewPort), mwo->channel_info->messageList);

                GtkAdjustment *adjustment = gtk_scrolled_window_get_vadjustment(
                        (GtkScrolledWindow *) scrolled_window);
                gtk_adjustment_set_value(adjustment,
                                         gtk_adjustment_get_upper(adjustment));
                gtk_scrolled_window_set_vadjustment(
                        (GtkScrolledWindow *) scrolled_window,
                        (GtkAdjustment *) adjustment);
                gtk_widget_show_all(GTK_WIDGET(mwo->Window));
                mwo->curr_channel_info = mwo->channel_info;
            }
            if (json->message.text != NULL) {
                mx_strdel(&json->message.text);
            }
        }
    }
    return TRUE;
}
