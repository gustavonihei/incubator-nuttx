/****************************************************************************
 * arch/risc-v/src/esp32c3/esp32c3_rtc.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#ifndef __ARCH_RISCV_SRC_ESP32C3_ESP32C3_RTC_H
#define __ARCH_RISCV_SRC_ESP32C3_ESP32C3_RTC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/esp32c3_soc.h"

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Cycles for RTC Timer clock source (internal oscillator) calibrate */

#define RTC_CLK_SRC_CAL_CYCLES           (10)

/* Various delays to be programmed into power control state machines */

#define RTC_CNTL_XTL_BUF_WAIT_SLP_US            (250)
#define RTC_CNTL_PLL_BUF_WAIT_SLP_CYCLES        (1)
#define RTC_CNTL_CK8M_WAIT_SLP_CYCLES           (4)
#define RTC_CNTL_WAKEUP_DELAY_CYCLES            (5)

#define RTC_SLOW_CLK_CAL_REG    RTC_CNTL_STORE1_REG
#define RTC_BOOT_TIME_LOW_REG   RTC_CNTL_STORE2_REG
#define RTC_BOOT_TIME_HIGH_REG  RTC_CNTL_STORE3_REG
#define RTC_XTAL_FREQ_REG       RTC_CNTL_STORE4_REG
#define RTC_APB_FREQ_REG        RTC_CNTL_STORE5_REG
#define RTC_ENTRY_ADDR_REG      RTC_CNTL_STORE6_REG
#define RTC_RESET_CAUSE_REG     RTC_CNTL_STORE6_REG
#define RTC_MEMORY_CRC_REG      RTC_CNTL_STORE7_REG

#define RTC_SLEEP_PD_DIG                BIT(0)  /* Deep sleep */
#define RTC_SLEEP_PD_RTC_PERIPH         BIT(1)  /* Power down RTC peripherals */
#define RTC_SLEEP_PD_RTC_SLOW_MEM       BIT(2)  /* Power down RTC SLOW memory */
#define RTC_SLEEP_PD_RTC_FAST_MEM       BIT(3)  /* Power down RTC FAST memory */

/* RTC FAST and SLOW memories are automatically
 * powered up and down along with the CPU
 */

#define RTC_SLEEP_PD_RTC_MEM_FOLLOW_CPU BIT(4)
#define RTC_SLEEP_PD_VDDSDIO            BIT(5)  /* Power down VDDSDIO regulator */
#define RTC_SLEEP_PD_WIFI               BIT(6)  /* Power down Wi-Fi */
#define RTC_SLEEP_PD_BT                 BIT(7)  /* Power down BT */
#define RTC_SLEEP_PD_CPU                BIT(8)  /* Power down CPU when in light-sleep */
#define RTC_SLEEP_PD_DIG_PERIPH         BIT(9)  /* Power down DIG peripherals */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Clock source to be calibrated using rtc_clk_cal function */

enum esp32c3_rtc_cal_sel_e
{
  RTC_CAL_RTC_MUX = 0,       /* Currently selected RTC SLOW_CLK */
  RTC_CAL_8MD256 = 1,        /* Internal 8 MHz RC oscillator, divided by 256 */
  RTC_CAL_32K_XTAL = 2       /* External 32 kHz XTAL */
};

/* CPU clock source */

enum esp32c3_rtc_cpu_freq_src_e
{
  RTC_CPU_FREQ_SRC_XTAL,  /* XTAL */
  RTC_CPU_FREQ_SRC_PLL,   /* PLL (480M or 320M) */
  RTC_CPU_FREQ_SRC_8M,    /* Internal 8M RTC oscillator */
  RTC_CPU_FREQ_SRC_APLL   /* APLL */
};

/* Possible main XTAL frequency values.
 * Enum values should be equal to frequency in MHz.
 */

enum esp32c3_rtc_xtal_freq_e
{
  RTC_XTAL_FREQ_32M = 32,
  RTC_XTAL_FREQ_40M = 40,
};

/* RTC SLOW_CLK frequency values */

enum esp32c3_rtc_slow_freq_e
{
  RTC_SLOW_FREQ_RTC = 0,      /* Internal 150 kHz RC oscillator */
  RTC_SLOW_FREQ_32K_XTAL = 1, /* External 32 kHz XTAL */
  RTC_SLOW_FREQ_8MD256 = 2,   /* Internal 8 MHz RC oscillator, divided by 256 */
};

/* CPU clock configuration structure */

struct esp32c3_cpu_freq_config_s
{
  /* The clock from which CPU clock is derived */

  enum esp32c3_rtc_cpu_freq_src_e source;
  uint32_t source_freq_mhz;    /* Source clock frequency */
  uint32_t div;                /* Divider, freq_mhz = source_freq_mhz / div */
  uint32_t freq_mhz;           /* CPU clock frequency */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: esp32c3_rtc_clk_xtal_freq_get
 *
 * Description:
 *   Get main XTAL frequency
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   XTAL frequency (one of enum esp32c3_rtc_xtal_freq_e values)
 *
 ****************************************************************************/

enum esp32c3_rtc_xtal_freq_e esp32c3_rtc_clk_xtal_freq_get(void);

/****************************************************************************
 * Name: esp32c3_rtc_clk_slow_freq_get
 *
 * Description:
 *   Get the RTC_SLOW_CLK source
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Currently selected clock source
 *   (one of enum esp32c3_rtc_slow_freq_e values)
 *
 ****************************************************************************/

enum esp32c3_rtc_slow_freq_e esp32c3_rtc_clk_slow_freq_get(void);

/****************************************************************************
 * Name: esp32c3_rtc_clk_slow_freq_set
 *
 * Description:
 *   Select source for RTC_SLOW_CLK
 *
 * Input Parameters:
 *   slow_freq - clock source (one of esp32c3_rtc_slow_freq_e values)
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void esp32c3_rtc_clk_slow_freq_set(enum esp32c3_rtc_slow_freq_e slow_freq);

/****************************************************************************
 * Name: esp32c3_rtc_clk_set
 *
 * Description:
 *   Set RTC CLK frequency.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void esp32c3_rtc_clk_set(void);

/****************************************************************************
 * Name: esp32c3_rtc_init
 *
 * Description:
 *   Initialize RTC clock and power control related functions.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void esp32c3_rtc_init(void);

/****************************************************************************
 * Name: esp32c3_rtc_time_get
 *
 * Description:
 *   Get current value of RTC counter.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   current value of RTC counter
 *
 ****************************************************************************/

uint64_t esp32c3_rtc_time_get(void);

/****************************************************************************
 * Name: esp32c3_rtc_time_us_to_slowclk
 *
 * Description:
 *   Convert time interval from microseconds to RTC_SLOW_CLK cycles.
 *
 * Input Parameters:
 *   time_in_us      - Time interval in microseconds
 *   slow_clk_period -  Period of slow clock in microseconds
 *
 * Returned Value:
 *   number of slow clock cycles
 *
 ****************************************************************************/

uint64_t esp32c3_rtc_time_us_to_slowclk(uint64_t time_in_us,
                                        uint32_t period);

/****************************************************************************
 * Name: esp32c3_rtc_cpu_freq_set_xtal
 *
 * Description:
 *   Switch CPU clock source to XTAL
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void esp32c3_rtc_cpu_freq_set_xtal(void);

/****************************************************************************
 * Name: esp32c3_rtc_sleep_init
 *
 * Description:
 *   Prepare the chip to enter sleep mode
 *
 * Input Parameters:
 *   flags - sleep mode configuration
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void esp32c3_rtc_sleep_init(uint32_t flags);

/****************************************************************************
 * Name: esp32c3_rtc_sleep_start
 *
 * Description:
 *   Enter force sleep mode.
 *
 * Input Parameters:
 *   wakeup_opt - bit mask wake up reasons to enable
 *   reject_opt - bit mask of sleep reject reasons.
 *
 * Returned Value:
 *   non-zero if sleep was rejected by hardware
 *
 ****************************************************************************/

uint32_t esp32c3_rtc_sleep_start(uint32_t wakeup_opt, uint32_t reject_opt,
                                 uint32_t lslp_mem_inf_fpu);

/****************************************************************************
 * Name: esp32c3_rtc_clk_cal
 *
 * Description:
 *   Measure RTC slow clock's period, based on main XTAL frequency
 *
 * Input Parameters:
 *   cal_clk        - clock to be measured
 *   slowclk_cycles - number of slow clock cycles to average
 *
 * Returned Value:
 *   Average slow clock period in microseconds, Q13.19 fixed point format
 *   or 0 if calibration has timed out
 *
 ****************************************************************************/

uint32_t esp32c3_rtc_clk_cal(enum esp32c3_rtc_cal_sel_e cal_clk,
                             uint32_t slowclk_cycles);

/****************************************************************************
 * Name: esp32c3_rtc_time_slowclk_to_us
 *
 * Description:
 *   Convert time interval from RTC_SLOW_CLK to microseconds
 *
 * Input Parameters:
 *   rtc_cycles - Time interval in RTC_SLOW_CLK cycles
 *   period     - Period of slow clock in microseconds
 *
 * Returned Value:
 *   time interval in microseconds
 *
 ****************************************************************************/

uint64_t esp32c3_rtc_time_slowclk_to_us(uint64_t rtc_cycles,
                                                  uint32_t period);

/****************************************************************************
 * Name: esp32c3_rtc_deep_sleep_start
 *
 * Description:
 *   Enter deep sleep mode.
 *
 * Input Parameters:
 *   wakeup_opt - bit mask wake up reasons to enable
 *   reject_opt - bit mask of sleep reject reasons.
 *
 * Returned Value:
 *   non-zero if sleep was rejected by hardware
 *
 ****************************************************************************/

uint32_t esp32c3_rtc_deep_sleep_start(uint32_t wakeup_opt,
                                                uint32_t reject_opt);

/****************************************************************************
 * Name: esp32c3_rtc_clk_cpu_freq_set_config
 *
 * Description:
 *   Set CPU frequency configuration.
 *
 * Input Parameters:
 *   config - CPU frequency configuration
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void esp32c3_rtc_clk_cpu_freq_set_config(
               const struct esp32c3_cpu_freq_config_s *config);

/****************************************************************************
 * Name: esp32c3_rtc_sleep_low_init
 *
 * Description:
 *   Low level initialize for rtc state machine waiting
 *   cycles after waking up.
 *
 * Input Parameters:
 *   slowclk_period - Re-calibrated slow clock period
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void esp32c3_rtc_sleep_low_init(uint32_t slowclk_period);

/****************************************************************************
 * Name: esp32c3_rtc_clk_cpu_freq_get_config
 *
 * Description:
 *   Get the currently used CPU frequency configuration.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   CPU clock configuration structure
 *
 ****************************************************************************/

void esp32c3_rtc_clk_cpu_freq_get_config(
             struct esp32c3_cpu_freq_config_s *out_config);

/****************************************************************************
 * Name: esp32c3_rtc_sleep_set_wakeup_time
 *
 * Description:
 *   Set target value of RTC counter for RTC_TIMER_TRIG_EN wakeup source.
 *
 * Input Parameters:
 *   t - value of RTC counter at which wakeup from sleep will happen.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void esp32c3_rtc_sleep_set_wakeup_time(uint64_t t);

#ifdef __cplusplus
}
#endif
#undef EXTERN

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_RISCV_SRC_ESP32C3_ESP32C3_RTC_H */
