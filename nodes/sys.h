#ifndef ARDUINO_SYS_H
#define ARDUINO_SYS_H ARDUINO_SYS_H

#include <cstdint>
#include <rt/cpu_ticks.h>

extern int __heap_start, *__brkval;

namespace arduino{

//@node:auto
//@raster: 1s
struct sysinfo{
    uint16_t out_mem_free;
    uint16_t out_load;
    uint16_t param_zero_load_ticks = 13000;
    //uint16_t out_idle_ticks = 0;
    
    void tick(){
        int stack_top; 
    	out_mem_free = (int) &stack_top - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
 
    	//out_idle_ticks = idle_ticks;
    	if(idle_ticks > param_zero_load_ticks){
    	    idle_ticks = param_zero_load_ticks;
    	}
    	out_load = ((param_zero_load_ticks-idle_ticks)*(uint32_t)100)/param_zero_load_ticks;
    }
};


} // namespace arduino

#endif // ARDUINO_SYS_H
