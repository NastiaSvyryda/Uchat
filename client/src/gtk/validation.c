#include "uchat_client.h"

//rename file

void mx_hide_show_password(__attribute__((unused))GtkWidget *button, gpointer data) {
    t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;

    if (mwo->visibility == 1) {
        gtk_entry_set_visibility(GTK_ENTRY(mwo->entryPass_l), TRUE);
        mwo->visibility = 0;
    }
    else {
        gtk_entry_set_visibility(GTK_ENTRY(mwo->entryPass_l), FALSE);
        mwo->visibility = 1;
    }
}

char *mx_handle_user_input(const char *s)
{
    char *oldW = "\\";
    char *newW = "\\\\";
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    for (i = 0; s[i] != '\0'; i++)
    {
        if (strstr(&s[i], oldW) == &s[i])
        {
            cnt++;
            i += oldWlen - 1;
        }
    }
    result = mx_strnew(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s)
    {
        if (strstr(s, oldW) == s)
        {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }

    for (int y = 0; result[y]; ++y)
        if (result[y] == '"' || result[y] == '\'')
            result[y] = '`';
    result[i] = '\0';
    return result;
}

void mx_show_popup(void *parent_window, char *msg)
{
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent_window),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_CLOSE, "%s",
                                               msg);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

_Bool mx_validate_login(char *str, void *window) {
    _Bool valid = (str && *str && strlen(str) > 0 && strlen(str) < MX_LOGIN_LEN);
    char *msg;

    for (int i = 0; str[i]; i++)
        if (str[i] == ' ')
            valid = 0;
    asprintf(&msg, "Login cannot contain space and must be more than 0 symbols and less than %d symbols!", MX_LOGIN_LEN);
    if (!valid)
        mx_show_popup(window, msg);
    return valid;
}

_Bool mx_validate_password(char *str, void *window)
{
    _Bool valid = (str && *str && strlen(str) > 7 && strlen(str) < MX_MAX_PASSWORD_LEN);
    char *msg;
    asprintf(&msg, "Password must be more than 7 symbols and less than %d symbols!", MX_MAX_PASSWORD_LEN);
    if (!valid)
        mx_show_popup(window, msg);
    return valid;
}

_Bool mx_validate_chat_name(char *str, void *window)
{
    _Bool valid = (str && *str && strlen(str) > 0 && strlen(str) < MX_MAX_NAME_LEN);
    char *msg;
    asprintf(&msg, "Chat name must be more than 0 symbols and less than %d symbols!", MX_MAX_NAME_LEN);

    if (!valid)
        mx_show_popup(window, msg);
    return valid;
}

_Bool mx_validate_message(char *str, void *window)
{
    _Bool valid = (str && *str && strlen(str) > 0 && strlen(str) < MX_MAX_MESSAGE_LEN);
    char *msg;

    asprintf(&msg, "Message must be more than 0 symbols and less than %d symbols!", MX_MAX_MESSAGE_LEN);
    if (!valid)
        mx_show_popup(window, msg);
    return valid;
}

_Bool mx_validate_user_name(char *str, void *window)
{
    _Bool valid = (str && *str && strlen(str) > 0 && strlen(str) < MX_FIRST_NAME_LEN);
    char *msg;

    asprintf(&msg, "User name must be more than 0 symbols and less than %d symbols!", MX_FIRST_NAME_LEN);
    if (!valid)
        mx_show_popup(window, msg);
    return valid;
}

_Bool mx_validate_user_surname(char *str, void *window)
{
    _Bool valid = (str && *str && strlen(str) > 0 && strlen(str) < MX_LAST_NAME_LEN);
    char *msg;

    asprintf(&msg, "User surname must be more than 0 symbols and less than %d symbols!", MX_LAST_NAME_LEN);
    if (!valid)
        mx_show_popup(window, msg);
    return valid;
}
