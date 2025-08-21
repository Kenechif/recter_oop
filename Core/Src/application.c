/*
 * application.c
 *
 *  Created on: 10 Mar 2025
 *      Author: Fidel
 */



#include "application.h"
#include <stdio.h>


// --------------------------
// 9. Complete Workflow Simulation
// --------------------------
void application_simulate_workflow(Nozzle nozzles[NOZZLES]) {
    // 1. Lift nozzles
    for(int i = 0; i < NOZZLES; i++) {
        nozzles[i].vtable->handle_lift(&nozzles[i]);
        nozzles[i].vtable->display_status(&nozzles[i]);
    }

    // 2. Authorize nozzles
    for(int i = 0; i < NOZZLES; i++) {
        nozzles[i].vtable->handle_authorize(&nozzles[i]);
        nozzles[i].vtable->display_status(&nozzles[i]);
    }

    // 3. Start pumping
    for(int i = 0; i < NOZZLES; i++) {
        nozzles[i].vtable->handle_pump_trigger(&nozzles[i]);
        nozzles[i].vtable->display_status(&nozzles[i]);
    }

    // 4. Pump fuel (3 cycles)
    for(int cycle = 0; cycle < 3; cycle++) {
        printf("\n--- Pump Cycle %d ---\n", cycle+1);
        for(int i = 0; i < NOZZLES; i++) {
            nozzles[i].vtable->update_state(&nozzles[i]);
            nozzles[i].vtable->display_status(&nozzles[i]);
        }
    }

    // 5. Release pumps
    for(int i = 0; i < NOZZLES; i++) {
        nozzles[i].vtable->handle_pump_release(&nozzles[i]);
        nozzles[i].vtable->display_status(&nozzles[i]);
    }

    // 6. Process payments
    printf("\n--- Processing Payments ---\n");
    for(int i = 0; i < NOZZLES; i++) {
        nozzles[i].vtable->handle_payment(&nozzles[i]);
        nozzles[i].vtable->display_status(&nozzles[i]);
    }
}





//// --------------------------
//// 10. Main Function
//// --------------------------
//
//#include "hardware.h"
//#include "application.h"
//
//
//int main() {
//    Nozzle nozzles[NOZZLES];
//
//    // Initialize hardware and business logic
//    hardware_init_nozzles(nozzles);
//
//    // Run complete application workflow
//    application_simulate_workflow(nozzles);
//
//    return 0;
//}


