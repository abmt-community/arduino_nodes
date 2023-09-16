#ifndef ARDUINO_GPIO_H
#define ARDUINO_GPIO_H ARDUINO_GPIO_H

#include <cstdint>
#include <abmt/io/pin.h>

enum pin_name{
    PB_0,
    PB_1,
    PB_2,
    PB_3,
    PB_4,
    PB_5,
    PB_6,
    PB_7,
    PC_0,
    PC_1,
    PC_2,
    PC_3,
    PC_4,
    PC_5,
    PC_6,
    PC_7,
    PD_0,
    PD_1,
    PD_2,
    PD_3,
    PD_4,
    PD_5,
    PD_6,
    PD_7,

    A0 = PC_0,
    A1 = PC_1,
    A2 = PC_2,
    A3 = PC_3,
    A4 = PC_4,
    A5 = PC_5,
    
    D0 = PD_0,
    D1 = PD_1,
    D2 = PD_2,
    D3 = PD_3,
    D4 = PD_4,
    D5 = PD_5,
    D6 = PD_6,
    D7 = PD_7,
    D8 = PB_0,
    D9 = PB_1,
    D10 = PB_2,
    D11 = PB_3,
    D12 = PB_4,
    D13 = PB_5,
    // special pins

    NC = 0xffffffff
};

namespace arduino{

struct avr_pin{
    uint8_t  pin_num = 0;
    volatile uint8_t* port_adr = 0;
    volatile uint8_t* ddr_adr = 0;
    volatile uint8_t* pin_adr = 0;
    
    void from_pin_name(pin_name p);
    void make_output();
    void make_input();
    void set(bool b);
    bool get();
};

//@node: auto
struct gpio_output{
    pin_name param_pin = NC;
    bool in;
    
    avr_pin p;
    
    void init(){
        p.from_pin_name(param_pin);
        p.make_output();
    }
    
    void tick(){
        p.set(in);
    }
};

//@node: auto
struct gpio_input{
    pin_name param_pin = NC;
    bool out;
    
    avr_pin p;
    
    void init(){
        p.from_pin_name(param_pin);
        p.make_input();
    }
    
    void tick(){
        out = p.get();
    }
};


//@node:auto
struct analog_in{
    pin_name param_pin = NC;
    uint16_t out;
    
    void init();
    void tick();
};

//@node: auto
struct ipin{
    pin_name param_pin = NC;
    
    abmt::io::ipin_ptr out_ipin;
    void init();
};

//@node: auto
struct opin{
    pin_name param_pin = NC;
    
    abmt::io::opin_ptr out_opin;
    void init();
};




} // namespace arduino

#endif // ARDUINO_GPIO_H
