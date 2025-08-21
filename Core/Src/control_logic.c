/*
 * control_logic.c
 *
 *  Created on: 10 Mar 2025
 *      Author: Fidel
 */


#include "control_logic.h"

void control_update_state(Nozzle* self) {
    if (self->state == STATE_PUMPING && self->pump_active) {
        self->fuel_dispensed += 0.1f; // Simulate fuel flow
    }
}

