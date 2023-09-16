#ifndef ARDUINO_I2C_H
#define ARDUINO_I2C_H ARDUINO_I2C_H

#include <cstdint>
#include <initializer_list>
#include <abmt/io/i2c.h>
#include <type_traits>

namespace arduino{

//@node: auto
struct i2c{
	abmt::io::i2c_ptr out_i2c_bus;
	uint32_t param_baudrate = 400000;
    void init();
};


} // namespace arduino

#endif // ARDUINO_I2C_H
