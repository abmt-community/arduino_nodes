#include "gpio.h"
#include <avr/io.h>
#include <abmt/os.h>

using namespace std;
using namespace arduino;

void avr_pin::from_pin_name(pin_name p){
    if(p >= PB_0 && p <= PB_7){
        pin_num  = p - PB_0;
        port_adr = &PORTB;
        ddr_adr  = &DDRB;
        pin_adr  = &PINB;
    } else if(p >= PC_0 && p <= PC_7){
        pin_num  = p - PC_0;
        port_adr = &PORTC;
        ddr_adr  = &DDRC;
        pin_adr  = &PINC;
    } else if(p >= PD_0 && p <= PD_7){
        pin_num  = p - PD_0;
        port_adr = &PORTD;
        ddr_adr  = &DDRD;
        pin_adr  = &PIND;
    }
}

void avr_pin::make_output(){
    if(port_adr == 0) return;
    
    *ddr_adr |= (1 << pin_num);
}

void avr_pin::make_input(){
    if(port_adr == 0) return;
    
    *ddr_adr &= ~(1 << pin_num);
}

void avr_pin::set(bool b){
    if(port_adr == 0) return;
    
    if(b){
		*port_adr |= (1 << pin_num);

	}else{
		*port_adr &= ~(1 << pin_num);
	}
}

bool avr_pin::get(){
    if(port_adr == 0) return false;
    
	return *pin_adr & (1 << pin_num);
}


void analog_in::init(){
    ADMUX  = (1 << REFS0); // vref = vcc
    ADCSRA = (1 << ADEN) | (1<<ADPS2) ;//| (1<<ADPS1) | (1<<ADPS0);
}

void analog_in::tick(){
    ADMUX = (ADMUX & ~(0x1F)) | ( (param_pin - A0) & 0x1F );
    ADCSRA |= (1 << ADSC); // start measurement
    while (ADCSRA & (1 << ADSC)); // wait
    out = ADC;
}

struct avr_ipin:public abmt::io::ipin{
    avr_pin p;
    avr_ipin(pin_name pn){
        p.from_pin_name(pn);
        p.make_input();
    }
    
    virtual bool get(){
		return p.get();
	}
};

struct avr_opin:public abmt::io::opin{
    avr_pin p;
    avr_opin(pin_name pn){
        p.from_pin_name(pn);
        p.make_output();
    }
    
    virtual void set(bool val = true){
        p.set(val);
	}
};

void ipin::init(){
    out_ipin.reset(new avr_ipin(param_pin));
}

void opin::init(){
    out_opin.reset(new avr_opin(param_pin));
}
