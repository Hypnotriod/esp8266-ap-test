#include "uri_parser.h"
#include "esp_heap_caps.h"

void uri_parser_parse(uri_params_t *uri_params, const char *uri)
{
    char *p_buffer = uri_params->buffer;
    char *p_key = uri_params->buffer;
    uri_params->size = 0;

    while (*uri != '\0')
    {
        if (*uri == '?')
        {
            uri++;
            break;
        }
        uri++;
    }

    while (*uri != '\0')
    {
        if (p_key != NULL && *uri == '=')
        {
            *p_buffer++ = 0;
            uri_params->params[uri_params->size].key = p_key;
            uri_params->params[uri_params->size].value = p_buffer;
            uri_params->size++;
            p_key = NULL;
        }
        else if (*uri == '&')
        {
            if (uri_params->size == URI_PARAMS_COUNT_MAX)
            {
                break;
            }
            *p_buffer++ = '\0';
            p_key = p_buffer;
        }
        else
        {
            *p_buffer++ = *uri;
        }

        uri++;
    }

    *p_buffer = '\0';
}