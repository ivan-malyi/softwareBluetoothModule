#include <stdio.h> // стандартная библиотека ввода-вывода 
#include <string.h> // для использования сток 

// #include "esp_bt.h"
// #include "esp_bt_main.h"
// #include "esp_gap_ble_api.h"
// #include "esp_log.h"


#include "esp_gatts_api.h"
#include "esp_gap_ble_api.h"


#define TAG "BLE_SCANNER"
#define TARGET_DEVICE_NAME "test-Bluetooth" // Имя устройства, к которому подключаемся




// Callback-функция для обработки событий GAP (например, сканирование BLE)
void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param) {
    switch (event) {
        case ESP_GAP_BLE_SCAN_RESULT_EVT:  // Событие: найдено устройство
            if (param->scan_rst.search_evt == ESP_GAP_SEARCH_INQ_RES_EVT) {
                printf("Найдено устройство с адресом: ");
                for (int i = 0; i < 6; i++) {
                    printf("%02X:", param->scan_rst.bda[i]);
                }
                printf("\n");
            }
            break;
        case ESP_GAP_BLE_SCAN_START_COMPLETE_EVT:  // Событие: сканирование начато
            printf("Сканирование началось!\n");
            break;
        default:
            printf("Событие GAP: %d\n", event);
    }
}






void app_main(void)
{
    // Регистрируем callback для GAP
    esp_ble_gap_register_callback(gap_event_handler);
    
    // Начинаем сканирование
    esp_ble_gap_start_scanning(30);  // Сканируем 30 секунд
}