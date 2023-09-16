#include "i2c.h"

#include <avr/io.h>
#include <abmt/os.h>

using namespace std;
using namespace arduino;

struct avr_i2c: public abmt::io::i2c{
    avr_i2c(uint32_t baud){
        TWBR = (F_CPU / baud - 16) / 2;
    }
    
    
    virtual bool write_read(uint16_t addr, void* write_data, uint16_t w_len, void* read_data, uint16_t r_len){
        if(w_len == 0 && r_len == 0){
            return true;   
        }
        
        uint16_t timeout = 0;
        
        auto write = [&](uint8_t data){
            TWDR = data;
            TWCR = (1<<TWINT)|(1<<TWEN);
            timeout = 0;
            while ((TWCR & (1<<TWINT)) == 0){
                if(timeout++ > 500) return;
            }
        };
        
        auto read_ack = [&]()->uint8_t {
            TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
            timeout = 0;
            while (!(TWCR & (1 << TWINT))){
                if(timeout++ > 500) return 0;
            }
            return TWDR;
        };
        
        auto read_nack = [&]()->uint8_t {
            TWCR = (1<<TWINT)|(1<<TWEN);
            timeout = 0;
            while (!(TWCR & (1 << TWINT))){
                if(timeout++ > 500) return 0;
            }
            return TWDR;
        };
        
        auto rcv_ack = [&]()->bool {
            if((TWSR & 0xF8) == 0x40 || (TWSR & 0xF8) == 0x18){
                return true;
            } else {
                return false;
            }
        };
        
        
        
        // start
        if(w_len > 0){
            TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
            while ((TWCR & (1<<TWINT)) == 0){
                if(timeout++ > 200) return false;
            }
            write((addr << 1 ) + 0x00);
            if(rcv_ack() == false) return false;
            for(uint16_t i = 0; i < w_len; ++i){
                write( ((uint8_t*)write_data)[i] );
            }
        }
        
        //return false;
        
        if(r_len > 0){
            TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
            timeout = 0;
            while ((TWCR & (1<<TWINT)) == 0){
                if(timeout++ > 200) return false;
            }
            write( (addr << 1 ) + 0x01);
            if(rcv_ack() == false) return false;
            for(uint16_t i = 0; i < (r_len - 1); ++i){
                ((uint8_t*)read_data)[i] = read_ack();
            }
            ((uint8_t*)read_data)[r_len - 1] = read_nack();
        }
        
        // stop
        TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
    }
};

void i2c::init(){
    out_i2c_bus.reset(new avr_i2c(param_baudrate));
}