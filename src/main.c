#include <microhttpd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/db.h"
#include "../include/auth.h"

#define PORT 8080

struct MHD_Response* serve_file(const char *filename);


static enum MHD_Result handle_request(void *cls,
                          struct MHD_Connection *connection,
                          const char *url,
                          const char *method,
                          const char *version,
                          const char *upload_data,
                          size_t *upload_data_size,
                          void **con_cls)
{
    struct MHD_Response *response;
    int ret;

    /* LOGIN PAGE */
    if (strcmp(url, "/") == 0 || strcmp(url, "/login") == 0) {
        response = serve_file("static/login.html");
        if (!response) return MHD_NO;

        ret = MHD_queue_response(connection, 200, response);
        MHD_destroy_response(response);
        return ret;
    }

    /* REGISTER PAGE */
    if (strcmp(url, "/register") == 0) {
        response = serve_file("static/register.html");
        if (!response) return MHD_NO;

        ret = MHD_queue_response(connection, 200, response);
        MHD_destroy_response(response);
        return ret;
    }

    /* DASHBOARD PAGE */
    if (strcmp(url, "/dashboard") == 0) {
        response = serve_file("static/dashboard.html");
        if (!response) return MHD_NO;

        ret = MHD_queue_response(connection, 200, response);
        MHD_destroy_response(response);
        return ret;
    }

    /* 404 */
    const char *not_found = "<h1>404 Not Found</h1>";
    response = MHD_create_response_from_buffer(
        strlen(not_found),
        (void*) not_found,
        MHD_RESPMEM_PERSISTENT
    );

    ret = MHD_queue_response(connection, 404, response);
    MHD_destroy_response(response);
    return ret;
}

struct MHD_Response* serve_file(const char *filename) {
	FILE *fp = fopen(filename, "rb");
	if (!fp) return NULL;

	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	rewind(fp);

	char *buffer = malloc(size + 1);
	fread(buffer, 1, size, fp);
	buffer[size] = '\0';
	fclose(fp);

	return MHD_create_response_from_buffer(
		size,
		buffer,
		MHD_RESPMEM_MUST_FREE
	);
} 


int main() {
    if (!init_db()) {
        printf("Database initialization failed\n");
        return 1;
    }

    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY,
                              PORT,
                              NULL, NULL,
                              &handle_request, NULL,
                              MHD_OPTION_END);

    if (!daemon) return 1;

    printf("Server running on http://localhost:%d\n", PORT);
    getchar();

    MHD_stop_daemon(daemon);
    return 0;
}
