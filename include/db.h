#ifndef DB_H
#define DB_H

int init_db();
int create_user(const char *email, const char *password_hash);
int get_user_password(const char *email, char *password_hash);

#endif
