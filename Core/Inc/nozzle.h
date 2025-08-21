/*
 * nozzle.h
 *
 *  Created on: 10 Mar 2025
 *      Author: Fidel
 */

#ifndef INC_NOZZLE_H_
#define INC_NOZZLE_H_


//#ifndef NOZZLE_H
//#define NOZZLE_H

#include <stdbool.h>

#define NOZZLES 2

typedef enum {
    STATE_IDLE,
    STATE_AUTHORIZING,
    STATE_PUMPING,
    STATE_PAYMENT_PENDING
} DispenserState;


typedef enum
{
	prog_State,
    idle_State,
    inactive_State,
    nozzleup_waitingforauth_State,
	authorised_nozzledown_State,
    authorised_nozzleup_State,
	authorisation_paused_State,
	authorisation_resumed_State,
	filling_State,
	filling_paused_State,
	filling_resumed_State,
	keypad_entry_State,
	operator_State,
	savesettings_State,
	read_flash_State,
	write_flash_State,
	switchedoff_State,
	pnp_State,
	filledmamo_State,
	last_State,
} eSystemState;

typedef enum
{
	_progState_Handler,
	_idleState_Handler,
	_inactiveState_Handler,
	_nozzleup_waitingforauthState_Handler,
	_authorised_nozzledown_State_Handler,
	_authorised_nozzleup_State_Handler,
	_authorisation_paused_State_Handler,
	_authorisation_resumed_State_Handler,
	_filling_State_Handler,
	_filling_paused_State_Handler,
	_filling_resumed_State_Handler,
	_keypad_entry_State_Handler,
	_operator_State_Handler,
	_savesettings_State_Handler,
	_read_flash_State_Handler,
	_write_flash_State_Handler,
	_switchedoffState_Handler,
	_pnpState_Handler,
	_fillingcompletestate_Handler,
	_resetState_Handler,
	_filledmamo_State_Handler
}eSystemStateHandler;


//Different type events
typedef enum
{
    _timeout_Event = 1,
    _authorise_Event,
	_auth_command_Event,
    _nozzleup_Event,
	_nozzledown_Event,
    _pause_Event,
    _resume_Event,
	_keyup_Event,
	_keydown_Event,
	_keypress_Event,
	_tot_error_Event,
	_key19_Event,
	_filling_paused_Event,
	_filling_resumed_Event,
	_filling_pulse_Event,
	_error_clear_Event,
	_operator_Event,
	_function_key_Event,
	_stopcommand_Event,
	_resetcommand_Event,
	_fillingcomplete_Event,   //_priceupdate_Event,
	_switchoffcommand_Event,
	_authorisecommand_Event,
	_hardwarereset_Event,
	_hardwareerror_Event,
	_auth_suspendcommand_Event,
	_filling_suspendcommand_Event,
	_auth_resumecommand_Event,
	_filling_resumecommand_Event,
	_mamo_Event,
    _no_Event
} eSystemEvent;




//typedef of function pointer of eventHandlers.
typedef eSystemState (*pfEventHandler)(void);

//typedef of function pointer of stateHandlers.
typedef eSystemState (*pfStateHandler)(void);
//------------------------------------------------
//structure of  event with event handler
//state and
typedef struct
{
  //eSystemState eStateMachine;
  eSystemEvent   eStateMachineEvent;
  pfEventHandler pfEventMachineHandler;
}sEventMachine;
//-----------------------------------------------
//state and state handler...
typedef struct
{
  eSystemState   eStateMachine;
  pfStateHandler pfStateMachineHandler;
} sStateMachine;
//-----------------------------------------------
//state and state handler...
typedef struct
{
  eSystemState   eStateMachine;
  pfStateHandler pfStateMachineHandler;
  eSystemEvent  states[8];
} sStateEventMachine;
//-----------------------------------------------
eSystemEvent eNewEvent1;
//eSystemState eNextState1, eLastState1, ePrevState;

typedef enum
{
 non,
 level1,
 level2,
 level3
}access_lev;

typedef enum
{
	 pass1,
	 pass2,
	 pass3,
}pass_lev;

/*typedef enum
{
    side1 = 1,
	side2
}pump_side;     refer to settings.h file  */

typedef enum
{
    l1 = 1,
	l2
}sales_v;



typedef struct Nozzle Nozzle;

typedef struct {
    void (*handle_lift)(Nozzle*);
    void (*handle_authorize)(Nozzle*);
    void (*handle_pump_trigger)(Nozzle*);
    void (*handle_pump_release)(Nozzle*);
    void (*handle_payment)(Nozzle*);
    void (*display_status)(const Nozzle*);
    void (*update_state)(Nozzle*);


    sEventMachine* asEventMachinePtr [MAX_EVENTS_NO];
	sStateEventMachine* asStateEventMachinePtr [MAX_STATES_NO];
	void (*state_iniPtr)(void);
	void (*statesPtr)(struct Nozle*);

	eSystemState (*timeout_HandlerPtr)(struct Nozle*);
	eSystemState (*authorise_HandlerPtr)(struct Nozle*);
	eSystemState (*auth_command_HandlerPtr)(struct Nozle*);
	eSystemState (*nozzleup_HandlerPtr)(struct Nozle*);
	eSystemState (*nozzledown_HandlerPtr)(struct Nozle*);
	eSystemState (*pause_HandlerPtr)(struct Nozle*);
	eSystemState (*resume_HandlerPtr)(struct Nozle*);
	eSystemState (*keyup_HandlerPtr)(struct Nozle*);
	eSystemState (*keydown_HandlerPtr)(struct Nozle*);
	eSystemState (*keypress_HandlerPtr)(struct Nozle*);
	eSystemState (*tot_error_HandlerPtr)(struct Nozle*);
	eSystemState (*key19_HandlerPtr)(struct Nozle*);
	eSystemState (*filling_paused_HandlerPtr)(struct Nozle*);
	eSystemState (*filling_resumed_HandlerPtr)(struct Nozle*);
	eSystemState (*filling_pulse_HandlerPtr)(struct Nozle*);
	eSystemState (*error_clear_HandlerPtr)(struct Nozle*);
	eSystemState (*operator_HandlerPtr)(struct Nozle*);
	eSystemState (*function_key_HandlerPtr)(struct Nozle*);
	eSystemState (*stopcommand_HandlerPtr)(struct Nozle*);
	eSystemState (*resetcommand_HandlerPtr)(struct Nozle*);
	eSystemState (*fillingcomplete_HandlerPtr)(struct Nozle*);
	eSystemState (*switchoffcommand_HandlerPtr)(struct Nozle*);
	eSystemState (*authorisecommand_HandlerPtr)(struct Nozle*);
	eSystemState (*hardwarereset_HandlerPtr)(struct Nozle*);
	eSystemState (*hardwareerror_HandlerPtr)(struct Nozle*);
	eSystemState (*auth_suspendcommand_HandlerPtr)(struct Nozle*);
	eSystemState (*filling_suspendcommand_HandlerPtr)(struct Nozle*);
	eSystemState (*auth_resumecommand_HandlerPtr)(struct Nozle*);
	eSystemState (*filling_resumecommand_HandlerPtr)(struct Nozle*);
	eSystemState (*mamo_HandlerPtr)(struct Nozle*);
	eSystemState (*progState_HandlerPtr)(struct Nozle*);
	eSystemState (*idleState_HandlerPtr)(struct Nozle*);
	eSystemState (*inactiveState_HandlerPtr)(struct Nozle*);
	eSystemState (*nozzleup_waitingforauthState_HandlerPtr)(struct Nozle*);
	eSystemState (*authorised_nozzleup_State_HandlerPtr)(struct Nozle*);
	eSystemState (*authorised_nozzledown_State_HandlerPtr)(struct Nozle*);
	eSystemState (*authorisation_paused_State_HandlerPtr)(struct Nozle*);
	eSystemState (*authorisation_resumed_State_HandlerPtr)(struct Nozle*);
	eSystemState (*filling_State_HandlerPtr)(struct Nozle*);
	eSystemState (*filling_paused_State_HandlerPtr)(struct Nozle*);
	eSystemState (*filling_resumed_State_HandlerPtr)(struct Nozle*);
	eSystemState (*keypad_entry_State_HandlerPtr)(struct Nozle*);
	eSystemState (*operator_State_HandlerPtr)(struct Nozle*);
	eSystemState (*savesettings_State_HandlerPtr)(struct Nozle*);
	eSystemState (*filledmamo_State_HandlerPtr)(struct Nozle*);
	eSystemState (*pnpState_HandlerPtr)(struct Nozle*);
	eSystemState (*switchedoffState_HandlerPtr)(struct Nozle*);


	void (*reset_timerPtr)(struct Nozle*, int tm);
	void (*stop_timerPtr)(struct Nozle*);
	void (*start_timerPtr)(struct Nozle*, int tm);

	void (*do_calcsPtr)(struct Nozle*);
	uint32_t (*price2pulserPtr)(struct Nozle*, float price);
	uint32_t (*amt2pulserPtr)(struct Nozle*, float amt);
	float (*pulser2pricePtr)(struct Nozle*, uint32_t pulse_);
	float (*amt2pricePtr)(struct Nozle*, float amt_);
	float (*pulser2amtPtr)(struct Nozle*, uint32_t pulse_);
	float (*pulser2amt_RPtr)(struct Nozle*, uint32_t pulse_);
	void (*make_stringPtr)(struct Nozle*, sellmode_ sll,float pr);

	uint8_t (*long_press_keyPtr)(struct Nozle*);
	uint8_t (*long_press_progExitPtr)(struct Nozle*);

	uint8_t (*long_press_logPtr)(struct Nozle*);
	uint8_t (*long_press_totPtr)(struct Nozle*);

	bool (*leapYear_calcPtr)(struct Nozle*, int8_t year);
	void (*save_datePtr)(struct Nozle*, int8_t _entri);

	uint16_t (*get_ctTimePtr)(struct Nozle*);

	void (*retrieve_settings_framPtr)(struct Nozle*, pump_sid side);
	void (*save_settings_framPtr)(struct Nozle*, pump_sid side);
	void (*dp_initPtr)(struct Nozle*);
	void (*clr_pulserPtr)(struct Nozle*, pump_sid side);

	void (*keypad_iniPtr)(pump_sid side);
	int8_t (*read_keypadPtr)(struct Nozle*);
	int8_t (*scankeyPtr)(pump_sid side);
	uint8_t (*checkkeyPtr)(pump_sid side);
	void (*shiftPtr)(struct Nozle *noz, pump_sid side, uint8_t data_byte, uint8_t lat);
	void (*shift_Ptr)(pump_sid side, uint8_t data_byte, uint8_t lat);

	void (*load_settingsPtr)(struct Nozle*);
	void (*make_settingsPtr)(struct Nozle*);
//	void (*save_settings_framPtr)(struct Nozle*, pump_sid side);
	void (*pumpType_configure_framPtr)(struct Nozle*);
	void (*pumpType_write_framPtr)(struct Nozle*, pump_sid side);
	void (*firstTotalizerDay_write_framPtr)(struct Nozle*, pump_sid side);
	void (*clear_volumeTotaliser_framPtr)(struct Nozle*, pump_sid side);
//	void (*make_settingsPtr)(struct Nozle*);
	uint8_t (*readNozzlePtr)(pump_sid side);
	void (*drive_totaliserPtr)(struct Nozle*, pump_sid side, drive drv);
	uint8_t (*readtotaliser_statePtr)(pump_sid side);
	uint8_t (*readkey19_statePtr)(pump_sid side);
	uint8_t (*readsettingskey_statePtr)(pump_sid side);
	void (*firstTotalizer_day_framPtr)(void);
//	void (*load_settingsPtr)(struct Nozle*);
	void (*pumpName_parse_framPtr)(struct Nozle*);

	void (*send_keypadPtr)(struct Nozle*, pump_sid side, char lcd_[8]); //ln125

	void (*drive_fast_solePtr)(pump_sid side, drive drv);
	void (*drive_slow_solePtr)(pump_sid side, drive drv);
	void (*drive_motorPtr)(pump_sid side, drive drv);
	void (*stop_flowPtr)(struct Nozle*, pump_sid side);
//	void (*drive_motorPtr)(pump_sid side, drive drv);
//	void (*drive_slow_solePtXCDFr)(pump_sid side, drive drv);
//	void (*drive_fast_solePtr)(pump_sid side, drive drv);
//	void (*clear_screenPtr)(struct Nozle*, pump_sid side);
	void (*fast_flowPtr)(struct Nozle*, pump_sid side);
	void (*slow_flowPtr)(struct Nozle*, pump_sid side);
//	uint8_t (*readsettingskey_statePtr)(pump_sid side);

	void (*check_flowPtr)(struct Nozle*, pump_sid side);

	void (*dec_varPtr)(struct Nozle*);

	void (*clear_logPtr)(struct Nozle*);
	void (*clear_totPtr)(struct Nozle*, pump_sid side);

	void (*save_volumeTotaliser_framPtr)(struct Nozle*, pump_sid side);

	eSystemState (*read_flash_State_HandlerPtr)(struct Nozle *noz, pump_sid side);
	eSystemState (*write_flash_State_HandlerPtr)(struct Nozle *noz, pump_sid side);

	void (*update_infoPtr)(struct Nozle*);
	void (*flash_info_read_framPtr)(struct Nozle*, pump_sid side);
	void (*dec_timerPtr)(struct Nozle*);

} NozzleVTable;

struct Nozzle
{
    const NozzleVTable* vtable;

    TIM_HandleTypeDef *flow_counter;

    uint8_t id;
    DispenserState state;
    float fuel_dispensed;
    bool nozzle_lifted;
    bool authorized;
    bool pump_active;
    float price_per_liter;


	float price_real,
		  amt_real,
		  amt_,
		  price_;

	bool mth_success,
		 clock_save;

	float lastVolumeSale,
		  lastVolumeSale_cal,
		  lastAmountSale,
		  lastAmountSale_cal;

	uint8_t ctTimed_day;

	uint8_t overflow_count;

	ctTimed_settings ctTimed_settingsA;

	_calibrationData calibrationData[2];

	uint8_t batteryStatus;

	uint8_t mamo_reached_flag,
		   mamo_reached_flag_1,
		   stopFlag_source,
		   hardwareError_flag,
		   hardwareErrorFlag_source;


	Log_new generallog_new,
			synchedLog_new;

	eSystemState eNextState,
				 eLastState,
				 ePrevState;

	uint8_t prog_entry;

	int8_t index_menu;

	uint8_t  prog_revisit,
			 prog_revisitt,
			 firstTime_key19;

	uint8_t key19State,
			lastKey19State;


	uint32_t data_size;

	uint8_t calibration_flag,
			configMode;

	float price_dp;

	float temppp_;


	pump_status_enum pump_status;
	pump_status_enum_4G pump_status_4G;


	float price;
	float amt;
	float litre_price;
	float pulser_index;
	float pulser_index_c;

	uint8_t pwr;

	float price_upper,
		  amt_middle;

	float display_minimumPulser;

	uint8_t ep1a_priceChangeFlag;

	int8_t ttime[3],
		   ddate[4];

	uint32_t target_pulser,
			 current_pulser,
			 overall_currentPulser;

	uint8_t fastFlow1;

	uint16_t timeout_picknozzle,
			 timeout_dispense;

	uint8_t timer_flag,
			timer_flag_old;

	uint8_t tot_buttonpress_tmr,
			log_buttonpress_tmr,
			key_buttonpress_tmr,
			progExit_buttonpress_tmr;

   uint16_t t,
			_tt,
			totalizer1Timer,
			priceChange_timer1,
			timer_config1,
			key19Timer1;

	uint32_t num;

	uint32_t transaction_period;

	sellmode_ sellmode;

	char str_[10];

	//=== screen Arrays ====
	char upper[10],
		 middle[10];

	//=============== keyboard entry ======================
	char keyboard_entry[10],  //buffers the key press
		 keyboard[9];

	//=====================================================
	int8_t keypress_,
		   index_;

	float key_value;
	//=====================================================
	int pulser_rem;
	//=====================================================

	uint32_t calib_pulser;
	uint8_t calibr;     //used for signaling motor and solenoid activatn in prog mode.

	char password_level1[9],
		 password_level2[9],
		 password_level3[9];

	uint8_t max_dp;

	float totaliser_vol,
		  totaliser_vol_cal,
		  firstTotaliser_vol,
		  firstTotaliser_vol_cal;

	float totaliser_amt,
		  totaliser_amt_cal,
		  priceOld;

	float startShiftTotaliser_vol,
		  startShiftTotaliser_vol_cal;

	float startShiftTotaliser_amt1,
		  startShiftTotaliser_amt_cal;

	float working_volTotaliser,
		 running_volTotaliser,
		 working_volTotaliser_cal,
		 running_volTotaliser_cal;

   float working_amtTotaliser,
		 running_amtTotaliser,
		 working_amtTotaliser_cal,
		 running_amtTotaliser_cal;

   uint8_t dp_amount,
		   dp_vol,
		   dp_unitprice;

   uint8_t firstTime_nozz;

   uint8_t stop_flag;

   uint8_t irrecov_flag;

	//=================================================
	//=================================================

	uint8_t pulser_complete; //used from the interrupt to signify the completion of pump sales.

	//=================================================

	uint8_t volume_flag,
			pulser_flag,
			valve_flag;

	float original_pulse;

	uint32_t pulser_new;

	uint8_t filling;
	bool lock_clr;

	uint8_t firstTime_filling,
			totalizer_error,
			calibration_error;

	uint16_t fast_flow_threshold,
			 slowFlow_startThreshold,
			 slowFlow_endThreshold;

	//================================================
	//================================================
	uint8_t nozzle_bit,
			stop_fueling_bit;

	uint8_t change_price,
			change_volume;

	float auth_v,
		  auth_p;

	uint8_t change_p,
			change_v;

	uint8_t auth_from_ctrl,
			authorise_flag,
			change_price_flag;

   float set_p;

   uint8_t changeLitrePrice;

	//-------------------------------------------------
   flash_store_info flash_infoA,
					flash_infoB;
   uint32_t flash_read_idA,
			flash_read_idB;

	uint32_t flash_locc;

	pump disp_type;

	uint16_t shutdown_timer,
			 motor_tmr;

	uint8_t pump_LitreOverflow,
			display_overflow,
			_auth_p,
			_auth_v,
			idle_backwardPulse,
			idle_forwardPulse,
			flow_loss,
			changeLitrePrice1_2,
			firstTime_display_overflow,
			priceChange_timerMin;

	uint8_t _pump_max_litres,
			_litre_price,
			nonValid_sale,
			nozzleUp_inProgMode;

	float half_litre,
		  display_minimumCentilitrePrice;

	uint8_t nozzleDown_source,
			reset_flag;

	float pulser_totalizer;
	uint32_t pulser_benchMark;

	float display_minimumCentilitre;

	uint8_t calibrationCan_measure;

	uint32_t  r_volTotaliser;
	uint32_t  old_r_volTotaliser;

	float r_amtTotaliser,
		  old_r_amtTotaliser;

	uint8_t lat_cnt;
	uint8_t disp_len_pad;  // = 6;

	totaliser_store totaliser_vol_store,
					totaliser_amt_store,
					startShiftTotaliser_vol_store,
					startShiftTotaliser_amt_store;

	drive drive;

	char key_lcd[10];

	uint8_t keypress_flag;

	uint16_t //_tt,
			 timer_spi,
			 totalizerTimer,
			 priceChange_timer,
			 timer_config,
			 key19Timer;

	uint32_t flash_read_id;

	uint8_t fastFlow;

	flash_store_info flash_info;

	pump_names pumpName;

	const uint16_t flash_stoA_fram,
				   flash_stoB_fram;

	uint8_t statusChange_noz,
			statusChange_pump;

	uint8_t key;

};

extern const NozzleVTable nozzle_vtable;

void hardware_init_nozzles(Nozzle nozzles[NOZZLES]);
void application_simulate_workflow(Nozzle nozzles[NOZZLES]);


#define NOZZLES 2

typedef enum {
    STATE_IDLE,
    STATE_AUTHORIZING,
    STATE_PUMPING,
    STATE_PAYMENT_PENDING
} DispenserState;

typedef struct {
    int id;
    DispenserState state;
    float fuel_dispensed;
    bool nozzle_lifted;
    bool authorized;
    bool pump_active;
} Nozzle;

typedef struct {
    Nozzle nozzles[NOZZLES];
    float price_per_liter;
} FuelDispenser;

//#endif // NOZZLE_H


#endif /* INC_NOZZLE_H_ */
