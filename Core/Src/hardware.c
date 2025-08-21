/*
 * hardware.c
 *
 *  Created on: 10 Mar 2025
 *      Author: Fidel
 */




#include "hardware.h"
#include "nozzle.h"

void hardware_init_nozzles(Nozzle nozzles[NOZZLES]) {
    for (int i = 0; i < NOZZLES; i++) {
        nozzles[i] = (Nozzle){
            .vtable = &nozzle_vtable,
            .id = i,
            .state = STATE_IDLE,
            .price_per_liter = 1.50f
        };
    }
}
