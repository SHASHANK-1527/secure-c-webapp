#include <openssl/sha.h>
#include <string.h>
#include <stdio.h>
#include "../include/auth.h"

int hash_password(const char *password, char *output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password, strlen(password), hash);

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        sprintf(output + (i * 2), "%02x", hash[i]);

    output[64] = 0;
    return 1;
}

int verify_password(const char *password, const char *hash) {
    char computed[65];
    hash_password(password, computed);
    return strcmp(computed, hash) == 0;
}
