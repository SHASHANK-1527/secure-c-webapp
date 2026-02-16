#include <sqlite3.h>
#include <string.h>
#include <stdio.h>
#include "../include/db.h"

#define DB_FILE "database/users.db"

int init_db() {
    sqlite3 *db;
    if (sqlite3_open(DB_FILE, &db)) return 0;

    const char *sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "email TEXT UNIQUE NOT NULL,"
        "password TEXT NOT NULL);";

    char *err = 0;
    if (sqlite3_exec(db, sql, 0, 0, &err) != SQLITE_OK) {
        sqlite3_free(err);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_close(db);
    return 1;
}

int create_user(const char *email, const char *password_hash) {
    sqlite3 *db;
    sqlite3_open(DB_FILE, &db);

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO users (email, password) VALUES (?, ?);";

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password_hash, -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return result == SQLITE_DONE;
}

int get_user_password(const char *email, char *password_hash) {
    sqlite3 *db;
    sqlite3_open(DB_FILE, &db);

    sqlite3_stmt *stmt;
    const char *sql = "SELECT password FROM users WHERE email = ?;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, email, -1, SQLITE_STATIC);

    int found = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        strcpy(password_hash,
               (const char *)sqlite3_column_text(stmt, 0));
        found = 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return found;
}
