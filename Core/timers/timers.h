/*
 * timers.h
 *
 *  Created on: May 4, 2022
 *      Author: tunjo
 */

#ifndef TIMERS_TIMERS_H_
#define TIMERS_TIMERS_H_

#ifdef __cplusplus
extern "C" {
#endif

  void toggle_led(void);

  bool dec_prn_tm(int d);
  void set_prn_tm(int tmm__);

#ifdef __cplusplus
}
#endif

#endif /* TIMERS_TIMERS_H_ */
