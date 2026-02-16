#ifndef AUTH_H
#define AUTH_H

int hash_password(const char *password, char *output);
int verify_password(const char *password, const char *hash);

#endif
