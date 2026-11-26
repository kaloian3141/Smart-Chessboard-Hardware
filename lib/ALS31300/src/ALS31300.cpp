#include "ALS31300.h"


//Constructor
ALS31300::ALS31300(uint8_t address, TwoWire &wire)
    : _address(address), _wire(&wire), _access_mode_entered(false) {}

bool ALS31300::begin()
{
    uint32_t read_value;
    return readRegister(ALS31300_REG_CONFIG, read_value);
}

bool ALS31300::enterCustomerAccessMode()
{
    if(!writeRegister(ALS31300_REG_ACCESS, ALS31300_ACCESS_CODE))
        return false;
    _access_mode_entered = true;
    return true;
}

//Slave address management
bool ALS31300::readStoredAddress(uint8_t &out_address)
{
    uint32_t value;
    if (!readRegister(ALS31300_REG_CONFIG, value))
        return false;
    out_address = (value >> ALS31300_SLAVE_ADDR_SHIFT) & ALS31300_SLAVE_ADDR_MASK;
    return true;
}

bool ALS31300::writeStoredAddress(uint8_t new_address)
{
    if(new_address < 0x08 || new_address > 0x77)
        return false;
    
    if(!_access_mode_entered && !enterCustomerAccessMode())
        return false;
    
    uint32_t value;
    if(!readRegister(ALS31300_REG_CONFIG, value))
        return false;
    
    value &= ~((uint32_t)ALS31300_SLAVE_ADDR_MASK << ALS31300_SLAVE_ADDR_SHIFT);
    value |= ((uint32_t)(new_address & ALS31300_SLAVE_ADDR_MASK)) << ALS31300_SLAVE_ADDR_SHIFT;
    
    if(!writeRegister(ALS31300_REG_CONFIG, value))
        return false;
    
    delay(ALS31300_EEPROM_WRITE_DELAY_MS);
    
    return true;
}

// Disable Slave ADC bit (bit 17 of 0x02)
bool ALS31300::setDisableSlaveADC(bool disable)
{
    if(!_access_mode_entered && !enterCustomerAccessMode())
        return false;
    uint32_t value;
    if(!readRegister(ALS31300_REG_CONFIG, value))
        return false;
    if(disable)
        value |= (1UL << ALS31300_DISABLE_SLAVE_ADC_BIT);
    else    
        value &= ~(1UL << ALS31300_DISABLE_SLAVE_ADC_BIT);
    
    if(!writeRegister(ALS31300_REG_CONFIG, value))
        return false;
    
    delay(ALS31300_EEPROM_WRITE_DELAY_MS);
    return true;
}

bool ALS31300::getDisableSlaveADC(bool &out_disabled)
{
    uint32_t value;
    if(!readRegister(ALS31300_REG_CONFIG, value))
        return false;
    out_disabled = (value >> ALS31300_DISABLE_SLAVE_ADC_BIT) & 1;
    return true;
}

//Cached state (no I²C)
uint8_t ALS31300::getConfiguredAddress() const 
{ 
    return _address; 
}

//Low-level register access
bool ALS31300::readRegister(uint8_t register_address, uint32_t &value)
{
    _wire->beginTransmission(_address);
    _wire->write(register_address); 
    if(_wire->endTransmission(false) != 0)
        return false;
    if(_wire->requestFrom(_address, (uint8_t)4) != 4)
        return false;
    value = 0;
    for(int i = 0; i < 4; i++)
    {
        value = value << 8;
        value |= _wire->read();
    }
    return true;
}

bool ALS31300::writeRegister(uint8_t register_address, uint32_t value)
{
    _wire->beginTransmission(_address);
    _wire->write(register_address);
    for(int i = 3; i >= 0; --i)
    {
        _wire->write((value >> (8 * i)) & 0xFF);
    }
    return _wire->endTransmission() == 0;
}

