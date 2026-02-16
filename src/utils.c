#include <string.h>
#include "../include/utils.h"

void escape_html(const char *input, char *output) {
    while (*input) {
        switch (*input) {
            case '<': strcat(output, "&lt;"); break;
            case '>': strcat(output, "&gt;"); break;
            case '&': strcat(output, "&amp;"); break;
            case '"': strcat(output, "&quot;"); break;
            default:
                strncat(output, input, 1);
        }
        input++;
    }
}
