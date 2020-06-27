#ifndef UCHAT_SERVER_H
#define UCHAT_SERVER_H
#include <stdlib.h>
#include <stdio.h>
#include "../../libmx/inc/libmx.h"

///Models
//model_channel
char *model_channel_database(void);
char *model_channel_name_table(void);
char **model_channel_fill_table(void);
//model_delivery_user
char *model_delivery_user_database(void);
char *model_delivery_user_name_table(void);
char **model_delivery_user_fill_table(void);
//model_message
char *model_message_database(void);
char **model_message_name_table(void);
char **model_message_fill_table(void);
//model_user
char *model_user_database(void);
char *model_user_channel_name_table(void);
char **model_user_fill_table(void);
//model_user
char *model_user_channel_database(void);
char *model_user_name_table(void);
char **model_user_channel_fill_table(void);
///end models
///Migrations

#endif //UCHAT_SERVER_H
