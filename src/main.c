#include <microhttpd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/db.h"
#include "../include/auth.h"

#define PORT 8080

static enum MHD_Result handle_request(void *cls,
                          struct MHD_Connection *connection,
                          const char *url,
                          const char *method,
                          const char *version,
                          const char *upload_data,
                          size_t *upload_data_size,
                          void **con_cls)
{
    if (strcmp(url, "/") == 0) {
        const char *page = "<h1>Secure C WebApp Running</h1>";
        struct MHD_Response *response =
            MHD_create_response_from_buffer(strlen(page),
                                            (void*) page,
                                            MHD_RESPMEM_PERSISTENT);
        int ret = MHD_queue_response(connection, 200, response);
        MHD_destroy_response(response);
        return ret;
    }

    return MHD_NO;
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
