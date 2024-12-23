

/*

Peripheral:

Устройство в режиме Peripheral будет рекламировать себя и ожидать подключения.
Шаги:
    Настраиваем параметры рекламы.
    Ожидаем соединения.

*/

#include "build/config/sdkconfig.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt.h"

// Структура для настроек рекламы
esp_ble_adv_params_t adv_params = {
    .adv_int_min = 0x20,       // Минимальный интервал рекламы (в 0.625 мс)
    .adv_int_max = 0x40,       // Максимальный интервал рекламы (в 0.625 мс)
    .adv_type = ADV_TYPE_IND,  // Тип рекламы: обычная
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC, // Публичный адрес устройства
    .peer_addr_type = BLE_ADDR_TYPE_PUBLIC, // Адрес удаленного устройства (если необходимо)
    // .adv_channel_map = ADV_CHNL_ALL,  
    .channel_map = ADV_CHNL_ALL, // Используем все каналы
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY // Все устройства могут видеть рекламу
};

// Обработчик событий GATT
void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
    case ESP_GATTS_CONNECT_EVT:
        printf("Устройство подключилось!\n");
        break;

    case ESP_GATTS_DISCONNECT_EVT:
        printf("Устройство отключилось.\n");
        esp_ble_gap_start_advertising(&adv_params); // Перезапускаем рекламу после отключения устройства
        break;
    }
}

void app_main() {
    esp_err_t ret;


    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();


    // Инициализация Bluetooth
    esp_bt_controller_init(&bt_cfg);
    esp_bt_controller_enable(ESP_BT_MODE_BLE);
    esp_bluedroid_init();
    esp_bluedroid_enable();

    // Настройка рекламных данных
    esp_ble_gap_config_adv_data(&adv_params); // adv_data должна быть предварительно инициализирована

    // Запуск рекламы
    esp_ble_gap_start_advertising(&adv_params);

    // Регистрация callback-функции для обработки событий GATT
    esp_ble_gatts_register_callback(gatts_event_handler);
}



/*
Central:

Устройство в режиме Central будет сканировать и подключаться к выбранному устройству.
Шаги:

    Настраиваем параметры сканирования.
    Обрабатываем событие подключения.
*/

#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_bt.h"

// Структура для настроек сканирования
esp_ble_scan_params_t scan_params = {
    .scan_type = BLE_SCAN_TYPE_ACTIVE,  // Тип сканирования: активное (для получения информации о устройствах)
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC, // Публичный адрес устройства
    .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL, // Разрешение сканировать все устройства
    .scan_interval = 0x50, // Интервал сканирования в 0.625 мс (0x50 = 80, 80 * 0.625 = 50 мс)
    .scan_window = 0x30,   // Окно сканирования в 0.625 мс (0x30 = 48, 48 * 0.625 = 30 мс)
};

// Обработчик событий GATT Client (для получения данных от серверов)
void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param) {
    switch (event) {
    case ESP_GATTC_CONNECT_EVT:
        printf("Подключено к устройству.\n");
        break;

    case ESP_GATTC_DISCONNECT_EVT:
        printf("Отключено от устройства.\n");
        break;
    }
}

void app_main() {
    esp_err_t ret;

    // Инициализация Bluetooth
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_bt_controller_init(&bt_cfg);
    esp_bt_controller_enable(ESP_BT_MODE_BLE);
    esp_bluedroid_init();
    esp_bluedroid_enable();

    // Регистрация callback-функции для обработки событий GAP (поиск устройств)
    esp_ble_gap_register_callback(gattc_event_handler);

    // Настройка параметров сканирования
    esp_ble_gap_set_scan_params(&scan_params); // Передаем заранее инициализированную структуру scan_params для начала сканирования
}
