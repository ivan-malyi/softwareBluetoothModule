/*
Пример использования Bluetooth-GAP: Broadcaster - Observer 
*/



#include "build/config/sdkconfig.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt.h"



/*
Broadcaster 
*/




void app_main() {
    esp_err_t ret;

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    // Инициализация BLE
    esp_bt_controller_init(&(esp_bt_controller_config_t)BT_CONTROLLER_INIT_CONFIG_DEFAULT());
    esp_bt_controller_enable(ESP_BT_MODE_BLE);
    esp_bluedroid_init();
    esp_bluedroid_enable();

    // Настройка GAP в режиме Broadcaster
    esp_ble_gap_config_adv_data(&adv_data);

    esp_ble_gap_start_advertising(&adv_params);
}

// Параметры рекламы
esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,             // Не используем ответ на сканирование
    .include_name = true,              // Включаем имя устройства
    .manufacturer_len = 0,             // Данные производителя (опционально)
    .service_data_len = 0,             // Сервисные данные (опционально)
};

esp_ble_adv_params_t adv_params = {
    .adv_int_min = 0x20,               // Минимальный интервал
    .adv_int_max = 0x40,               // Максимальный интервал
    .adv_type = ADV_TYPE_IND,          // Тип рекламы
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .peer_addr = {0},                  // Адрес другого устройства (не используется)
};




/*
Observer 
*/


void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    switch (event) {
    case ESP_GAP_BLE_SCAN_RESULT_EVT:
        if (param->scan_rst.adv_data_len > 0) {
            printf("Найдено устройство: %s\n", param->scan_rst.ble_adv);
        }
        break;

    case ESP_GAP_BLE_SCAN_PARAM_SET_COMPLETE_EVT:
        esp_ble_gap_start_scanning(30);  // Сканируем 30 секунд
        break;
    }
}

void app_main() {
    esp_err_t ret;

    // Инициализация BLE
    esp_bt_controller_init(0);
    esp_bt_controller_enable(ESP_BT_MODE_BLE);
    esp_bluedroid_init();
    esp_bluedroid_enable();

    // Регистрация callback-функции
    esp_ble_gap_register_callback(gap_event_handler);

    // Параметры сканирования
    esp_ble_gap_set_scan_params(&scan_params);
}

// Параметры сканирования
esp_ble_scan_params_t scan_params = {
    .scan_type = BLE_SCAN_TYPE_ACTIVE,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
    .scan_interval = 0x50,
    .scan_window = 0x30,
};

