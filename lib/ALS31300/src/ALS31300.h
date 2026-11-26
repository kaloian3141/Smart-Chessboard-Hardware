#ifndef ALS31300_H
#define ALS31300_H

#define ALS31300_DEFAULT_ADDRESS 0x00
#define ALS31300_REG_CONFIG 0x02
#define ALS31300_REG_ACCESS 0x35
#define ALS31300_ACCESS_CODE 0x2C413534UL
#define ALS31300_SLAVE_ADDR_SHIFT 10
#define ALS31300_SLAVE_ADDR_MASK 0x7F
#define ALS31300_DISABLE_SLAVE_ADC_BIT 17
#define ALS31300_EEPROM_WRITE_DELAY_MS 50
#include <Wire.h>
#include <Arduino.h>

class ALS31300 {
    uint8_t _address;
    TwoWire *_wire;
    bool _access_mode_entered;
public:
    //Constructor
    ALS31300(uint8_t address = ALS31300_DEFAULT_ADDRESS, TwoWire &wire = Wire);

    bool begin();
    bool enterCustomerAccessMode();

    //Slave address management
    bool readStoredAddress(uint8_t &out_address);
    bool writeStoredAddress(uint8_t new_address);

    // Disable Slave ADC bit (bit 17 of 0x02)
    bool setDisableSlaveADC(bool disable);
    bool getDisableSlaveADC(bool &out_disabled);

    //Cached state (no I²C)
    uint8_t getConfiguredAddress() const;

    //Low-level register access
    bool readRegister(uint8_t register_address, uint32_t &value);
    bool writeRegister(uint8_t register_address, uint32_t value);
};

#endif