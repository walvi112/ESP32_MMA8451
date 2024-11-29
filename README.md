# Driver for MMA8451 accelerometer sensor on ESP-IDF framework
* Configure the i2c master bus via i2c_new_master_bus() and add i2c device via i2c_master_bus_add_device(). Then pass the i2c_master_dev_handle_t into MMA8451_Init(). 
* Create MMA8451 struct and call MMA8451_Init function to initialize.
