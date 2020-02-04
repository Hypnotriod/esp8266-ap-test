#ifndef __URI_PARSER
#define __URI_PARSER

#include "esp_http_server.h"

#define URI_PARAMS_COUNT_MAX 16

typedef struct get_param
{
    const char *key;
    const char *value;
} get_param_t;

typedef struct uri_params
{
    char buffer[HTTPD_MAX_URI_LEN];
    get_param_t params[URI_PARAMS_COUNT_MAX];
    uint8_t size;
} uri_params_t;

void uri_parser_parse(uri_params_t *uri_params, const char *uri);

#endif
