/*
 * nozzle_vtable.c
 *
 *  Created on: 10 Mar 2025
 *      Author: Fidel
 */


#include "nozzle.h"
#include "business_logic.h"
#include "presentation.h"
#include "control_logic.h"

const NozzleVTable nozzle_vtable = {
    .handle_lift = business_handle_lift,
    .handle_authorize = business_handle_authorize,
    .handle_pump_trigger = business_handle_pump_trigger,
    .handle_pump_release = business_handle_pump_release,
    .handle_payment = business_handle_payment,
    .display_status = presentation_show_status,
    .update_state = control_update_state
};

