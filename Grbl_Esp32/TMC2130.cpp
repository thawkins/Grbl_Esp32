
/*
	TMC2130.cpp - Support for TMC2130 Stepper Drivers SPI Mode
  Part of Grbl_ESP32  

  Copyright (c) 2019 Barton Dring for Buildlog.net LLC   	

  GrblESP32 is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "grbl.h"

#ifdef X_CS_PIN
	TMC2130Stepper TMC2130_X = TMC2130Stepper(X_CS_PIN);
#endif

#ifdef Y_CS_PIN
	TMC2130Stepper TMC2130_Y = TMC2130Stepper(Y_CS_PIN);
#endif

#ifdef Z_CS_PIN
	TMC2130Stepper TMC2130_Z = TMC2130Stepper(Z_CS_PIN);
#endif

void TMC2130_Init()
{
	#ifdef X_CS_PIN
		TMC2130_X.begin(); // Initiate pins and registries		
		TMC2130_X.toff(3);
		TMC2130_X.tbl(1);
		TMC2130_X.stealthChop(1); // stealthchop unless going faster than
		TMC2130_X.coolstep_min_speed(200); // when to turn on coolstep
		TMC2130_X.hysteresis_start(4);
		TMC2130_X.hysteresis_end(-2);
		TMC2130_X.rms_current(X_TMC2130_I); // mA
		TMC2130_X.microsteps(X_TMC2130_uSTEPS);
		TMC2130_X.diag1_stall(1);
		TMC2130_X.diag1_active_high(0);		
		TMC2130_X.THIGH(0);
		TMC2130_X.sg_filter(1);
		TMC2130_X.semin(5);
		TMC2130_X.semax(2);
		TMC2130_X.sedn(0b01);
		TMC2130_X.sg_stall_value(X_TMC2130_SG);	
	#endif	
		
	#ifdef Y_CS_PIN
		TMC2130_Y.begin(); // Initiate pins and registries		
		TMC2130_Y.toff(3);
		TMC2130_Y.tbl(1);
		TMC2130_Y.stealthChop(1); // stealthchop unless going faster than
		TMC2130_Y.coolstep_min_speed(200); // when to turn on coolstep
		TMC2130_Y.hysteresis_start(4);
		TMC2130_Y.hysteresis_end(-2);
		TMC2130_Y.rms_current(Y_TMC2130_I); // mA
		TMC2130_Y.microsteps(Y_TMC2130_uSTEPS);
		TMC2130_Y.diag1_stall(1);
		TMC2130_Y.diag1_active_high(0);
		TMC2130_Y.THIGH(0);
		TMC2130_Y.sg_filter(1);
		TMC2130_Y.semin(5);
		TMC2130_Y.semax(2);
		TMC2130_Y.sedn(0b01);
		TMC2130_Y.sg_stall_value(Y_TMC2130_SG);
	#endif
	
	#ifdef Z_CS_PIN
		TMC2130_Z.begin(); // Initiate pins and registries
		TMC2130_Z.microsteps(32);
		TMC2130_Z.setCurrent(200, 0.11, 0.5);
		TMC2130_Z.stealthChop(1); // Enable extremely quiet stepping
	#endif
	
	#ifdef TMC2130_VERBOSE
	// setup a periodic task to monitor motor current
	xTaskCreatePinnedToCore(  rptCurrentTask,    // task
                              "rptCurrentTask", // name for task
                              2048,   // size of task stack
                              NULL,   // parameters
                              1, // priority
                              &rptCurrentTaskTaskHandle,
                              0 // core
                         );
	#endif
		
}

// this is the task
void rptCurrentTask(void *pvParameters)
{ 
  TickType_t xLastWakeTime;
  const TickType_t xRptFrequency = RPT_CURRENT_FREQ;  // in ticks (typically ms)
  
 
  
  while(true) { // don't ever return from this or the task dies

    vTaskDelayUntil(&xLastWakeTime, xRptFrequency);
	
		#ifdef TMC2130_VERBOSE
		if (settings.status_report_mask == 2)		
			TMC2130_Status();
		#endif
  }
} 

void TMC2130_Status() {
	static uint32_t x_last_val = 0;
	static uint32_t y_last_val = 0;
	uint32_t drv_status;
	uint32_t tstep;
	#ifdef X_CS_PIN_GOO
			drv_status = TMC2130_X.DRV_STATUS();
			tstep = TMC2130_X.TSTEP();
			if (tstep != 0xFFFFF) {
				// Ex:  [MSG:X...SG:786 , TSTEP:1451, SG:0]
				grbl_sendf(CLIENT_SERIAL, "[MSG:X...SG:%d , TSTEP:%05d, SG:%d]\r\n", 
								(drv_status & SG_RESULT_bm)>>SG_RESULT_bp,
								tstep,
								(drv_status & STALLGUARD_bm)>>STALLGUARD_bp);				
				x_last_val = drv_status;
			}
	#endif
	#ifdef Y_CS_PIN
			drv_status = TMC2130_Y.DRV_STATUS();
			tstep = TMC2130_Y.TSTEP();
			if (tstep != 0xFFFFF) {
				// Ex:  [MSG:X...SG:786 , TSTEP:1451, SG:0]
				grbl_sendf(CLIENT_SERIAL, "[MSG:Y...SG:%d , TSTEP:%05d, SG:%d]\r\n", 
								(drv_status & SG_RESULT_bm)>>SG_RESULT_bp,
								tstep,
								(drv_status & STALLGUARD_bm)>>STALLGUARD_bp);				
				y_last_val = drv_status;
			}
	#endif
	
}
