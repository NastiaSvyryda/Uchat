#include "uchat_client.h"

//rename file

void mx_hide_show_password(GtkWidget *entry, __attribute__((unused)) gpointer data)
{
    // t_mainWindowObjects *mwo = (t_mainWindowObjects *)data;
    gtk_entry_set_visibility(GTK_ENTRY(entry), TRUE);
}

char *mx_handle_user_input(const char *s)
{
    char *oldW = "\\";
    char *newW = "\\\\";
    char *result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);

    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++)
    {
        if (strstr(&s[i], oldW) == &s[i])
        {
            cnt++;

            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }

    // Making new string of enough length
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1);

    i = 0;
    while (*s)
    {
        // compare the substring with the result
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
    {
        if (result[y] == '"')
            result[y] = '`';
        if (result[y] == '\'')
            result[y] = '`';
    }

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

_Bool mx_validate_login(char *str, void *window)
{
    _Bool valid = (str && *str && strlen(str) > 0 && strlen(str) < 256);
    if (!valid)
    {
        mx_show_popup(window, "Login must be more than 0 symbols and less than 256 symbols!");
    }

    if (strcmp(str, "Valerka") == 0)
    {
        mx_show_popup(window, "Valerka is The Cool boy!!!");
    }
    if (strcmp(str, "Sasha") == 0)
    {
        mx_show_popup(window, "Sasha is The King!!!");
    }
    if (strcmp(str, "Vlad") == 0)
    {
        mx_show_popup(window, "Vlad is The Prince!!!");
    }
    if (strcmp(str, "Nastya") == 0)
    {
        mx_show_popup(window, "Nastya is The Qeen!!!");
    }
    if (strcmp(str, "Slava") == 0)
    {
        mx_show_popup(window, "Slava is The best Comedian!!!");
    }

    return valid;
}

_Bool mx_validate_password(char *str, void *window)
{
    _Bool valid = (str && *str && strlen(str) > 7 && strlen(str) < 256);
    if (!valid)
        mx_show_popup(window, "Password must be more than 7 symbols and less than 256 symbols!");
    return valid;
}

_Bool mx_validate_chat_name(char *str, void *window)
{
    _Bool valid = (str && *str && strlen(str) > 0 && strlen(str) < 256);
    if (!valid)
        mx_show_popup(window, "Chat name must be more than 0 symbols and less than 256 symbols!");
    return valid;
}

_Bool mx_validate_message(char *str, void *window)
{
    _Bool valid = (str && *str && strlen(str) > 0 && strlen(str) < 1024);
    if (!valid)
        mx_show_popup(window, "Message must be more than 0 symbols and less than 1024 symbols!");
    return valid;
}

_Bool mx_validate_user_name(char *str, void *window)
{
    _Bool valid = (str && *str && strlen(str) > 0 && strlen(str) < 256);
    if (!valid)
        mx_show_popup(window, "User name must be more than 0 symbols and less than 256 symbols!");
    return valid;
}

_Bool mx_validate_user_surname(char *str, void *window)
{
    _Bool valid = (str && *str && strlen(str) > 0 && strlen(str) < 256);
    if (!valid)
        mx_show_popup(window, "User surname must be more than 0 symbols and less than 256 symbols!");
    return valid;
}
