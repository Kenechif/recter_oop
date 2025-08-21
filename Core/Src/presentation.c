/*
 * presentation.c
 *
 *  Created on: 10 Mar 2025
 *      Author: Fidel
 */


#include "presentation.h"
#include "business_logic.h"
#include <stdio.h>

void presentation_show_status(const Nozzle* self) {
    const char* states[] = {"IDLE", "AUTHORIZING", "PUMPING", "PAYMENT"};
    printf("Nozzle %d [%s]: %.1fL ($%.2f)\n",
           self->id + 1, states[self->state],
           self->fuel_dispensed, business_calculate_payment(self));
}

