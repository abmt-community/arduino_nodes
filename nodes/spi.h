#ifndef ARDUINO_SPI_H
#define ARDUINO_SPI_H ARDUINO_SPI_H

#include <abmt/io/spi.h>

namespace arduino{

//@node: auto
struct spi{

	int param_baudrate = 1000000;
	int param_mode = 0; /// bit1 = pol, bit2 = pah; google
	
	abmt::io::spi_ptr out_spi_bus;
	
    void init();
};

} // namespace arduino

#endif // ARDUINO_SPI_H
