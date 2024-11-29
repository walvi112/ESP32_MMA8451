#include <stdio.h>
#include "driver/i2c_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "MMA8451.h"

#define I2C_PORT        0
#define I2C_SCL_PIN     17
#define I2C_SDA_PIN     16

const char* TAG = "MMA8451";

void app_main(void)
{
    i2c_master_bus_config_t i2c_mst_config = {
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .i2c_port = I2C_PORT,
    .scl_io_num = I2C_SCL_PIN,
    .sda_io_num = I2C_SDA_PIN,
    .glitch_ignore_cnt = 7,
    .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = MMA8451_DEFAULT_ADDRESS,
        .scl_speed_hz = 100000,
    };

    i2c_master_dev_handle_t dev_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));

    MMA8451 mma8451;

    if(!MMA8451_Init(&mma8451, dev_handle, MMA8451_DEFAULT_ADDRESS))
    {
        ESP_LOGE(TAG, "Error init accelerometer MMA8451");
    }

    MMA8451_setRange(&mma8451, MMA8451_RANGE_2_G);
    ESP_LOGI(TAG, "Range = %d", MMA8451_getRange(&mma8451));

    while(1)
    {
        MMA8451_read(&mma8451);
        ESP_LOGI(TAG, "X = %.2f | Y = %.2f | Z = %.2f  m/s^2", mma8451._xg * SENSORS_GRAVITY_STANDARD, mma8451._yg * SENSORS_GRAVITY_STANDARD, mma8451._zg * SENSORS_GRAVITY_STANDARD);
        switch(MMA8451_getOrientation(&mma8451))
        {
            case MMA8451_PL_PUF:
                ESP_LOGI(TAG, "Portrait Up Front");
                break;
            case MMA8451_PL_PUB:
                ESP_LOGI(TAG, "Portrait Up Back");
                break;
            case MMA8451_PL_PDF:
                ESP_LOGI(TAG, "Portrait Down Front");
                break;
            case MMA8451_PL_PDB:
                ESP_LOGI(TAG, "Portrait Down Back");
                break;
            case MMA8451_PL_LRF:
                ESP_LOGI(TAG, "Landscape Right Front");
                break;
            case MMA8451_PL_LRB:
                ESP_LOGI(TAG, "Landscape Right Back");
                break;
            case MMA8451_PL_LLF:
                ESP_LOGI(TAG, "Landscape Left Front");
                break;
            case MMA8451_PL_LLB:
                ESP_LOGI(TAG, "Landscape Left Back");
                break;
        };
    }

}