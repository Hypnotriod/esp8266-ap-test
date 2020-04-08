
#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "portmacro.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "tcpip_adapter.h"
#include "lwip/ip4_addr.h"
#include "esp_http_server.h"
#include "driver/gpio.h"
#include "esp8266/gpio_register.h"

#include "uri_parser.h"

#define SSID "Test Station"
#define PASS "12345678"

#define REQUEST_URI "/test"
#define RESPONSE_LENGTH_MAX 100

#define LED_PIN_IO_MASK (1ULL << GPIO_NUM_2)

esp_err_t http_get_handler(httpd_req_t *req);

httpd_uri_t http_get_uri = {
    .uri = REQUEST_URI,
    .method = HTTP_GET,
    .handler = http_get_handler,
};
httpd_handle_t http_server = NULL;

esp_err_t http_get_handler(httpd_req_t *req)
{
    static uri_params_t uri_params;
    static char response[RESPONSE_LENGTH_MAX];
    static char key_value_row[RESPONSE_LENGTH_MAX];
    uint8_t i;

    *response = 0;

    strcat(response, "<H1>GET Params:</H1><H2>");

    uri_parser_parse(&uri_params, req->uri);

    for (i = 0; i < uri_params.size; i++)
    {
        sprintf(key_value_row, "Key=%s, Value=%s<br>", uri_params.params[i].key, uri_params.params[i].value);
        strcat(response, key_value_row);
        if (strcmp(uri_params.params[i].key, "led") == 0)
        {
            if (strcmp(uri_params.params[i].value, "on") == 0)
            {
                printf("Led on\n");
                GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, LED_PIN_IO_MASK);
            }
            else if (strcmp(uri_params.params[i].value, "off") == 0)
            {
                printf("Led off\n");
                GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, LED_PIN_IO_MASK);
            }
        }
    }

    strcat(response, "</H2>");

    httpd_resp_send(req, response, strlen(response));
    return ESP_OK;
}

void start_webserver(void)
{
    const httpd_config_t http_server_config = HTTPD_DEFAULT_CONFIG();

    printf("Starting server on port: '%d'\n", http_server_config.server_port);
    esp_err_t err = httpd_start(&http_server, &http_server_config);
    if (err == ESP_OK)
    {
        printf("Registering URI handlers\n");
        httpd_register_uri_handler(http_server, &http_get_uri);
    }
    else
    {
        printf("Error starting server! %0xh\n", err);
    }
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
        printf("IP assigned: %s\n", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));

        if (http_server == NULL)
        {
            start_webserver();
        }
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        printf("Connected\n");
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        printf("Disconnected\n");
        break;
    default:
        break;
    }
    return ESP_OK;
}

void init_wifi()
{
    const wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    const wifi_ap_config_t wifi_ap_config = {
        .ssid = SSID,
        .ssid_len = strlen(SSID),
        .password = PASS,
        .channel = 0,
        .authmode = WIFI_AUTH_WPA_WPA2_PSK,
        .max_connection = 2,
    };

    tcpip_adapter_init();
    esp_event_loop_init(event_handler, NULL);
    esp_wifi_init(&wifi_init_config);
    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(ESP_IF_WIFI_AP, (wifi_config_t *)&wifi_ap_config);

    esp_wifi_start();
}

void init_gpio()
{
    const gpio_config_t led_gpio_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = LED_PIN_IO_MASK,
        .pull_down_en = 0,
        .pull_up_en = 0,
    };

    GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, LED_PIN_IO_MASK);
    gpio_config(&led_gpio_config);
}

void app_main()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        nvs_flash_erase();
        nvs_flash_init();
    }

    init_gpio();
    init_wifi();
}
