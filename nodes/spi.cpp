#include "spi.h"

using namespace std;
using namespace arduino;

#include <avr/io.h>
#include <avr/interrupt.h>
#include <abmt/os.h>

#define SPI_PORT                 PORTB
#define SPI_DDR                  DDRB
#define SPI_SCK                 (1 << 5) /* PB5 */
#define SPI_MOSI                (1 << 3) /* PB3 */
#define SPI_SS                  (1 << 2) /* PB2 */


struct avr_spi: public abmt::io::spi{

    
    avr_spi(int baud, int mode){
        uint8_t en_spi2x = 0;
        uint8_t en_spr0  = 0;
        uint8_t en_spr1  = 0;
        
        if(baud >= F_CPU/2){
            en_spi2x = 1;
        } else if(baud >= F_CPU/4){
            // default
        } else if(baud >= F_CPU/8){
            en_spi2x = 1;
            en_spr0  = 1;
        } else if(baud >= F_CPU/16){
            en_spr0  = 1;
        } else if(baud >= F_CPU/32){
            en_spi2x = 1;
            en_spr1  = 1;
        } else if(baud >= F_CPU/64){
            en_spr1  = 1;
        } else if(baud >= F_CPU/128){
            en_spr0  = 1;
            en_spr1  = 1;
        }
        
        uint8_t en_cpol = 0;
        uint8_t en_cpah = 0;
        
        if(mode == 1){
            en_cpah = 1;
        }if(mode == 2){
            en_cpol = 1;
        }if(mode == 3){
            en_cpah = 1;
            en_cpol = 1;
        }
        
        
        SPI_DDR |= SPI_MOSI | SPI_SCK | SPI_SS;
        SPCR = (1 << SPE) | (1 << MSTR) | (en_spr0 << SPR0) | (en_spr1 << SPR1) | (en_cpah << CPHA) | (en_cpol <<  CPOL);
        SPSR = (en_spi2x << SPI2X);
    }
    
    virtual uint8_t tranceive(uint8_t data){
        uint16_t time_out = 0;
        SPDR = data;
        while(!(SPSR & (1<<SPIF))){
            if(time_out++ > 200) return 0;
        }
        return SPDR;
	}
};

void spi::init(){
        out_spi_bus.reset(new avr_spi(param_baudrate, param_mode));
}