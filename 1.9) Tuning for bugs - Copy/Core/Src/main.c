/* USER CODE BEGIN Header */
/**
	******************************************************************************
	* @file           : main.c
	* @brief          : Main program body
	******************************************************************************
	* @attention
	*
	* <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
	* All rights reserved.</center></h2>
	*
	* This software component is licensed by ST under BSD 3-Clause license,
	* the "License"; You may not use this file except in compliance with the
	* License. You may obtain a copy of the License at:
	*                        opensource.org/licenses/BSD-3-Clause
	*
	******************************************************************************
	*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
////////////////////////includes////////////////////////////////////////

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>



////////////////////////////////////////////////////////////////
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
////////////////////////defines////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/


/* USER CODE BEGIN PV */

////////////////////////vaibales///////////////////////////////////////
///FOR NEXTION///////
char rxbuf_nextion[4];
char rxbuf_nextion_previous[4];
char buf_nextion[15];
int t_for_nextion = 0;
unsigned char ending_array_for_nextion[3] = {0xFF,0xFF,0xFF};
bool vent_start = false; // To get Patient details (Age and Weight and others) before start of Ventilation
int LpM_Sensor4_for_nextion = 0;
double LpM_Sensor4_for_trigger = 0.0;
double pre1_results_4 = 0;
double pre2_results_4 = 0;
double pre3_results_4 = 0;
double pre4_results_4 = 0;
float cmH2O_Sensor3_for_nextion = 0;
//int t=0;
int VT_for_nextion = 30;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// FOR Rotary Knob ///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float counter_for_knob = 0;
int button_press = 1;
int previous_button_press = 1;
int t_for_rotary = 0;
int previous_t_for_rotary = 0;
int p_for_knob= 8;
int vt_for_knob = 200;
int pif_for_knob = 10;
float ie_for_knob = 20.0;
int rr_for_knob = 10;
int age_for_knob = 28;
int weight_for_knob = 50;
int PS_for_knob = 5;
int PEEP_for_knob = 0;
bool Peep_nextion_flag = false;
int FiO2_for_knob = 21;
int trigger_for_knob = 2;
int trigger_count = 0;
int trigger_taken = 0;

int change_mode_variable = 0; // To remember the current mode
int current_page = 0;					// To remember the current page
int Previous_page = 0;				// To remember the previous page
int current_pic = 0;					// To remember the current pic
int knob_value = 0;						// For knob counter
int previous_knob_value = 0;	// To keep the last value of knob counter
bool change_value_flag= false;// For changing the value of any paramter
bool confirm_flag = false;	  // For back window
bool display_appear = true;		// To call display function when mode changed
bool enter_flag = true;
bool controls_flag = false;
bool counter_flag = false;
bool touch_set_flag = false;
bool Auto_fill_limits = false;
bool vt_pip_flag = false;
bool pic_change = false;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// buffer /////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int p_for_buffer = 8;
int vt_for_buffer = 200;
int pif_for_buffer = 10;
float ie_for_buffer = 20.0;
int rr_for_buffer = 10;
int PS_for_buffer = 5;
int PEEP_for_buffer = 0;
int FiO2_for_buffer = 21;
int previous_FiO2 = 21;
int trigger_for_buffer = 2;
int 	RR_min;
float E_I_ratio;
int 	PIP_cmH2O;
int 	Flow_rate;
int 	Pressure_support;
int pressure_support_measured=0;
float I_plus_E;
float Insp_secs;
float Expr_secs;
float	Insp_msecs;
float Expr_msecs;
int previous_change_mode_touch_variable=0;
int change_mode_touch_variable_buffer=0;
int peep_set = 0;
int FiO2_set = 21;
uint16_t pwm_for_defined_flow_curve;
int fankari = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// ADS Initialzation////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char ADSwrite[6];
signed char ADSwrite_1[6];
uint32_t reading_ended;
int32_t reading;
unsigned char buffer_for_sensirion[3] = {0};

//////////////////////////////////////////////////////////
//////////////Sensors Variables//////////////////////////
//////////////////////////////////////////////////////////
///////////////////////sensirion//////////////////

double LpM_Sensor4= 0.00;
double LpM_Max_real = 0.00;
int LpM_Max_real_display = 0;
float  EXP_PWM = 0.0;

//////////////////////lung sensor/////////

int	ADC_Sensor3;
float  ADC_Volts_Sensor3;
float  kpa_Sensor3;
double cmH2O_Sensor3;
double PIP_real;
uint32_t results_3_calib = 0;
uint32_t results_3;
int PEEP_cmH2O = 0;
int PIP_display = 8;
int Plateau;

//////////////////////lung sensor/////////

int	ADC_Sensor2;
float  ADC_Volts_Sensor2;
float  kpa_Sensor2;
float cmH2O_Sensor2=0;
uint32_t	results_2_calib = 0;
uint32_t results_2;
uint32_t results_4_calib =0;
uint32_t results_4 = 0;

//////////////////////lung sensor/////////

uint32_t results_1;
float ADC_Volts_to_FiO2_Slope_Sensor1 = 2.0789;
float Offset_Sensor1 = -3.7;
float ADC_Volts_Sensor1;
float ADC_Sensor1;
uint8_t FiO2_Sensor1;

//////////////////////////////////////////////////////////
//////////////Variables for time control//////////////////
//////////////////////////////////////////////////////////

unsigned long Time_in_ms;
unsigned long previousMillis;
unsigned long current_time;
unsigned long prev_time;
unsigned long dt;
bool valve4_state;
bool valve5_state;
int valve4_state_for_prop;

//////////////////////////////////////////////////////////
////////////Variables for tidal volume calculation////////////
//////////////////////////////////////////////////////////

int LpM_for_tidal_volume;
double mLpms;
double VT_ml;
double VT = 0;
long VT_max;
long VT_max_display = 200;
double Compliance = 15.0;
double air_way_resistance = 25.0;
int sample_time = 20;
int time_last_taken = 0;

//////////////////////////////////////////////////////////
//////////////// Variables for AC & SIMV /////////////////
//////////////////////////////////////////////////////////

bool  Insp_start_time_flag = true;
bool	volume_complete_flag = true;
float Insp_start_time;
bool  Insp_flag = true;
bool  Expr_flag = true;
bool  Flowrate_zero_time_flag = false;
float Flowrate_zero_time;
float Exp_start_time;
float Breath_time_wo_exp_pause = 0.00;
float Expiratory_pause;
float Trigger_time_absolute;
float Trigger_time;
float Time_after_trigger;
float Insp_Exp_centre_millis;
bool  Supported_window_flag = false;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////Monitoring variables initialization/////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned long currentMillisIE = 0;
float TCT_monitoring;
float Insp_monitoring;
float Exp_monitoring;
float IE_monitoring_final;
float IE_monitoring_for_display = 1.0;
bool IE_monitoring_flag = true;
unsigned long previousMillisRR = 0;        // will store last time LED was updated
unsigned long currentMillisRR = 0;
bool RR_monitoring_flag = true;
float RR_monitoring_final = 10.0;
float Minute_volume_real = 0.0;
float Minute_volume_set;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// for ps SIMV /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool 	flag_to_detect_PS_SIMV_trigger;
float Insp_start_time_SIMV_trigger;
float Expr_msecs_PS_SIMV = 1000000000000000000;
float Insp_msecs_PS_SIMV = 100000000;
bool 	Flowrate_zero_time_flag_SIMV_trigger;
float Exp_start_time_SIMV_trigger;
float Flowrate_zero_time_SIMV_trigger;
bool  PS_for_SIMV_flag = false;
float Expr_time_wo_pause;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// for ps SIMV /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int count = 0;
int z=1;
int y=0;
int xy=8;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////// for proportional valve control /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// Pressure control for PC control modes///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float defined_rise_time = 250;
float Flow_rate_for_pressure_rise_time;
float Flow_rate_for_pressure_control;
float Previous_Flow_rate_for_pressure_control;
float Flow_rate_for_volume=0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// PID Variable Initialzation///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long lastTime;
double Input, Output, Setpoint;
double error;
double ITerm, lastInput;
double kp, ki, kd;
int SampleTime = 20; //1 sec
double outMin, outMax;
bool inAuto = false;
int outputPC = 0;
float pid_out_max;
#define MANUAL 0
#define AUTOMATIC 1

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////c-pap bi-pap///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
int ipap = 5;
int epap = 5;

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Alarms ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
int display_counter = 0;
int beep = 0;
bool pip_alarm = false;
bool vt_alarm = false;
bool rr_alarm = false;
bool peep_alarm = false;
bool fio2_alarm = false;
bool ve_alarm = false;
bool parameter_value_change = false;
bool skip_one_cycle = false;
bool disconnection_alarm = false;
bool pressure_exceed_max = false;
uint8_t disconnection_time_counter = 0;
uint8_t no_of_alarms = 0;
uint8_t alarms_id[7]= {"\0"};
uint8_t turn[7] = {0,0,0,0,0,0};

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// Motor of Blender /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t blender_millis=0;
int encoder_skipping = 0;
int current_encoder = 0;
int previous_encoder = 0;
short pwm_for_blender_motor = 95;
int current_blender_position=0;
int required_encoder=0;
bool move_motor = false;
bool for_back_flag=true;
bool upper_bound=false;
bool lower_bound=false;
double blender_temp;

//////////////////////////////////// ADC Variables for Feedback Voltages ////////////////////////////////////
uint8_t FB_volt[2];
uint8_t alternate_text = 0;
uint16_t power_loss = 0;

//////////////////////////////////// Alarms Limit Page ////////////////////////////////////
uint8_t  pip_max = 40;
uint8_t  pip_low = 8;
uint16_t vt_max = 1000;
uint16_t vt_low = 200;
uint8_t  rr_max  = 30;
uint8_t  rr_low  = 10;
uint8_t  ve_max  = 30;
uint8_t  ve_low  = 2;
uint8_t  pip_max_buffer = 40;
uint8_t  pip_low_buffer = 8;
uint16_t vt_max_buffer = 1000;
uint16_t vt_low_buffer = 200;
uint8_t  rr_max_buffer  = 30;
uint8_t  rr_low_buffer  = 10;
uint16_t ve_max_buffer  = 30;
uint16_t ve_low_buffer  = 2;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/////////////////////functions/////////////////////////////////
void Flash_Write (uint32_t Flash_address, uint32_t Flash_data)
{
	HAL_FLASH_Unlock();
	FLASH_Erase_Sector(FLASH_SECTOR_11,VOLTAGE_RANGE_3);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Flash_address, Flash_data);
	HAL_FLASH_Lock();
}

uint32_t Flash_Read(uint32_t Flash_address)
{
	uint32_t Flash_data;
	Flash_data = *(uint32_t*)Flash_address;
	return Flash_data;
}
double millis()
{
	Time_in_ms =  (double) HAL_GetTick();
	return Time_in_ms;
}
void default_values()
{
	previousMillis = millis();
	PIP_real = 0;
	LpM_Max_real = 0;
	Insp_msecs = 0;
	Expr_msecs = 0;
	PS_for_SIMV_flag = false;
	VT_max_display = vt_for_buffer;
	PIP_display 	 = PIP_cmH2O;
	no_of_alarms = 0;
}
void NEXTION_SendString(char *ID, char *string)///////////////////////////to send strings to nextion
{
		sprintf(buf_nextion,"%s.txt=\"%s\"",ID,string);
		HAL_UART_Transmit(&huart1,(uint8_t *)buf_nextion, strlen(buf_nextion),100);
		HAL_UART_Transmit(&huart1,ending_array_for_nextion, sizeof(ending_array_for_nextion),100);
}
void NEXTION_Text_Background (char *ID, int val)///////////////////////////sending color code to change text background in nextion //63488 is for red // 0 is for black
{
	sprintf(buf_nextion,"%s.bco=%d",ID,val);
	HAL_UART_Transmit(&huart1, (uint8_t *)buf_nextion, strlen(buf_nextion), 50);
	HAL_UART_Transmit(&huart1, ending_array_for_nextion, sizeof(ending_array_for_nextion), 50);
}
void NEXTION_Text_FontColor (char *ID, int val)///////////////////////////sending color code to change text background in nextion //63488 is for red // 0 is for black
{
	sprintf(buf_nextion,"%s.pco=%d",ID,val);
	HAL_UART_Transmit(&huart1, (uint8_t *)buf_nextion, strlen(buf_nextion), 50);
	HAL_UART_Transmit(&huart1, ending_array_for_nextion, sizeof(ending_array_for_nextion), 50);
}
void NEXTION_SendInteger (char *ID, int val)///////////////////////////to send integer values to nextion
{
	sprintf(buf_nextion,"%s.val=%d",ID,val);
	HAL_UART_Transmit(&huart1, (uint8_t *)buf_nextion, strlen(buf_nextion), 50);
	HAL_UART_Transmit(&huart1, ending_array_for_nextion, sizeof(ending_array_for_nextion), 50);
}
void NEXTION_SendFloat (char *ID, int val)///////////////////////////to send float values to nextion
{
	sprintf(buf_nextion,"%s.val=%d",ID, val);
	HAL_UART_Transmit(&huart1, (uint8_t *)buf_nextion, strlen(buf_nextion), 100);
	HAL_UART_Transmit(&huart1, ending_array_for_nextion, sizeof(ending_array_for_nextion), 100);
}
void NEXTION_PageChange(int page_no)/////////////////////////////////to change page of nextion
{
		sprintf(buf_nextion,"page %d",page_no);
		HAL_UART_Transmit(&huart1,(uint8_t *)buf_nextion, strlen(buf_nextion),100);
		HAL_UART_Transmit(&huart1,ending_array_for_nextion, sizeof(ending_array_for_nextion),100);
}

void NEXTION_PicChange(int pic_no)/////////////////////////////////to change pic of nextion
{
		sprintf(buf_nextion,"p0.pic=%d",pic_no);
		HAL_UART_Transmit(&huart1,(uint8_t *)buf_nextion, strlen(buf_nextion),100);
		HAL_UART_Transmit(&huart1,ending_array_for_nextion, sizeof(ending_array_for_nextion),100);
}
void NEXTION_SendGraph(int ID,int channel, int value)///////////////to send graph values to nextion
{
		sprintf(buf_nextion,"add %d,%d,%d",ID,channel,value);
		HAL_UART_Transmit(&huart1,(uint8_t *)buf_nextion, strlen(buf_nextion),100);
		HAL_UART_Transmit(&huart1,ending_array_for_nextion, sizeof(ending_array_for_nextion),100);
}
void display_graph()
{
	NEXTION_SendGraph(2,0,cmH2O_Sensor3_for_nextion); // Pressure Graph
	NEXTION_SendGraph(3,0,LpM_Sensor4_for_nextion);   // Flow Rate Graph
	NEXTION_SendGraph(4,0,VT_for_nextion);   					// Volume Graph
}
void display()
{
		switch (current_page)
		{
			case 0:
					NEXTION_SendInteger("n0", PIP_display );         // Measured PIP
					NEXTION_SendFloat  ("x2", Minute_volume_real);   // Measured Minute Volume
					NEXTION_SendInteger("n2", VT_max_display );			 // Measured VT
					NEXTION_SendInteger("n3", RR_monitoring_final);  // Measured RR
					if (change_mode_variable%2!=0)	NEXTION_SendInteger("n6", p_for_knob);	// User Set PIP (p_for_knob)
					else 														NEXTION_SendInteger("n6", vt_for_knob); // User Set VT  (vt_for_knob)
					NEXTION_SendInteger("n7", PEEP_for_knob); 			 // User Set PEEP
					NEXTION_SendInteger("n8", FiO2_for_knob); 			 // User Set FiO2
				break;
			case 1:
					NEXTION_SendInteger("n9", rr_for_knob);								// User Set RR
					NEXTION_SendFloat  ("x0", ie_for_knob ); 							// User Set I:E Ratio
					NEXTION_SendInteger("n13",trigger_for_knob); 					// User Set Trigger
					// Extra buttons above controls window
					NEXTION_SendInteger("n0", PIP_display );         // Measured PIP
					NEXTION_SendFloat  ("x2", Minute_volume_real);   // Measured Minute Volume
					NEXTION_SendInteger("n2", VT_max_display );			 // Measured VT
					NEXTION_SendInteger("n3", RR_monitoring_final);  // Measured RR
					if (change_mode_variable%2!=0)	NEXTION_SendInteger("n6", p_for_knob);	// User Set PIP (p_for_knob)
					else 														NEXTION_SendInteger("n6", vt_for_knob); // User Set VT  (vt_for_knob)
					NEXTION_SendInteger("n7", PEEP_for_knob); 			 // User Set PEEP
  			break;
			case 2:
					NEXTION_SendInteger("n9", rr_for_knob);								// User Set RR
					NEXTION_SendFloat  ("x0", ie_for_knob); 							// User Set I:E Ratio
					NEXTION_SendInteger("n13",trigger_for_knob); 					// User Set Trigger
					NEXTION_SendInteger("n12",pif_for_knob);							// User Set PIF
					// Extra buttons above controls window
					NEXTION_SendInteger("n0", PIP_display );         // Measured PIP
					NEXTION_SendFloat  ("x2", Minute_volume_real);   // Measured Minute Volume
					NEXTION_SendInteger("n2", VT_max_display );			 // Measured VT
					NEXTION_SendInteger("n3", RR_monitoring_final);  // Measured RR
					if (change_mode_variable%2!=0)	NEXTION_SendInteger("n6", p_for_knob);	// User Set PIP (p_for_knob)
					else 														NEXTION_SendInteger("n6", vt_for_knob); // User Set VT  (vt_for_knob)
					NEXTION_SendInteger("n7", PEEP_for_knob); 			 // User Set PEEP
				break;
			case 3:
					NEXTION_SendInteger("n9", rr_for_knob);								// User Set RR
					NEXTION_SendFloat  ("x0", ie_for_knob); 							// User Set I:E Ratio
					NEXTION_SendInteger("n13",trigger_for_knob); 					// User Set Trigger
					NEXTION_SendInteger("n10",PS_for_knob);								// User Set PS
					// Extra buttons above controls window
					NEXTION_SendInteger("n0", PIP_display );         // Measured PIP
					NEXTION_SendFloat  ("x2", Minute_volume_real);   // Measured Minute Volume
					NEXTION_SendInteger("n2", VT_max_display );			 // Measured VT
					NEXTION_SendInteger("n3", RR_monitoring_final);  // Measured RR
					if (change_mode_variable%2!=0)	NEXTION_SendInteger("n6", p_for_knob);	// User Set PIP (p_for_knob)
					else 														NEXTION_SendInteger("n6", vt_for_knob); // User Set VT  (vt_for_knob)
					NEXTION_SendInteger("n7", PEEP_for_knob); 			 // User Set PEEP
				break;
			case 4:
					NEXTION_SendInteger("n9", rr_for_knob);								// User Set RR
					NEXTION_SendFloat  ("x0", ie_for_knob); 							// User Set I:E Ratio
					NEXTION_SendInteger("n13",trigger_for_knob); 					// User Set Trigger
					NEXTION_SendInteger("n12", pif_for_knob);							// User Set PIF
					NEXTION_SendInteger("n10", PS_for_knob);							// User Set PS
					// Extra buttons above controls window
					NEXTION_SendInteger("n0", PIP_display );         // Measured PIP
					NEXTION_SendFloat  ("x2", Minute_volume_real);   // Measured Minute Volume
					NEXTION_SendInteger("n2", VT_max_display );			 // Measured VT
					NEXTION_SendInteger("n3", RR_monitoring_final);  // Measured RR
					if (change_mode_variable%2!=0)	NEXTION_SendInteger("n6", p_for_knob);	// User Set PIP (p_for_knob)
					else 														NEXTION_SendInteger("n6", vt_for_knob); // User Set VT  (vt_for_knob)
					NEXTION_SendInteger("n7", PEEP_for_knob); 			 // User Set PEEP
				break;
			case 6:
					NEXTION_SendInteger("n0",  PIP_display);         			// Measured PIP
					NEXTION_SendFloat  ("x2",  Minute_volume_real); 		  // Measured Minute Volume
					NEXTION_SendInteger("n2",  VT_max_display);				  	// Measured VT
					NEXTION_SendInteger("n3",  RR_monitoring_final);  		// Measured RR
					NEXTION_SendInteger("n4",  0);			  			 					// Measured FiO2 (FiO2_Sensor1)
					NEXTION_SendInteger("n5",  PEEP_cmH2O);    			 			// Measured PEEP (PEEP_cmH2O)
					NEXTION_SendFloat  ("x1",  IE_monitoring_for_display);// Measured I:E Ratio
					NEXTION_SendInteger("n22", pressure_support_measured);// Measured PS
					NEXTION_SendInteger("n23", trigger_taken); 						// Measured Trigger
					NEXTION_SendInteger("n24", LpM_Max_real_display); 		// Measured PIF
					if (change_mode_variable%2!=0)	NEXTION_SendInteger("n6", p_for_knob);	// User Set PIP (p_for_knob)
					else 														NEXTION_SendInteger("n6", vt_for_knob); // User Set VT  (vt_for_knob)
					NEXTION_SendInteger("n7", PEEP_for_knob); 			 // User Set PEEP
				break;
			case 7:
					NEXTION_SendInteger("n25", pip_max); 						// PIP Max limit
					NEXTION_SendInteger("n26", pip_low); 						// PIP Low limit
					NEXTION_SendInteger("n27", vt_max); 						// VT Max limit
					NEXTION_SendInteger("n28", vt_low); 						// VT Low limit
					NEXTION_SendInteger("n29", rr_max); 						// RR Max limit
					NEXTION_SendInteger("n30", rr_low); 						// RR Low limit
					NEXTION_SendFloat  ("x3",  ve_max); 						// VE Max limit
					NEXTION_SendFloat  ("x4",  ve_low); 						// VE Low limit
					// Extra buttons above controls window
					NEXTION_SendInteger("n0", PIP_display );         												// Measured PIP
					if (change_mode_variable%2!=0)	NEXTION_SendInteger("n6", p_for_knob);	// User Set PIP (p_for_knob)
					else 														NEXTION_SendInteger("n6", vt_for_knob); // User Set VT  (vt_for_knob)
			break;
			case 8:
					NEXTION_SendInteger("n1", trigger_count); 						// Noting Trigger Count
					// Extra buttons above controls window
					NEXTION_SendInteger("n0", PIP_display );         // Measured PIP
					NEXTION_SendFloat  ("x2", Minute_volume_real);   // Measured Minute Volume
					NEXTION_SendInteger("n2", VT_max_display );			 // Measured VT
					if (change_mode_variable%2!=0)	NEXTION_SendInteger("n6", p_for_knob);	// User Set PIP (p_for_knob)
					else 														NEXTION_SendInteger("n6", vt_for_knob); // User Set VT  (vt_for_knob)
					NEXTION_SendInteger("n7", PEEP_for_knob); 			 // User Set PEEP
			break;
		}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) ////////////////////////To get Touch Values of Nextion
{
		HAL_UART_Receive_DMA(&huart1,(uint8_t*)rxbuf_nextion,1);
		if (rxbuf_nextion[0]!=rxbuf_nextion_previous[0])
		{
			switch(rxbuf_nextion[0])
			{
				case 'A': change_mode_variable = 0; change_value_flag = false; current_page = 0; TIM8->CNT = 0; break; ////Enter in CMV_Default///////
				case 'B': change_mode_variable = 1; change_value_flag = false; current_page = 0; TIM8->CNT = 0; break; ////Enter in CMV_PC////////////
				case 'C': change_mode_variable = 2; change_value_flag = false; current_page = 0; TIM8->CNT = 0; break; ////Enter in CMV_VC////////////
				case 'D': change_mode_variable = 3; change_value_flag = false; current_page = 0; TIM8->CNT = 0; break; ////Enter in AC_PC/////////////
				case 'E': change_mode_variable = 4; change_value_flag = false; current_page = 0; TIM8->CNT = 0; break; ////Enter in AC_VC/////////////
				case 'F': change_mode_variable = 5; change_value_flag = false; current_page = 0; TIM8->CNT = 0; break; ////Enter in SIMV_PC///////////
				case 'G': change_mode_variable = 6; change_value_flag = false; current_page = 0; TIM8->CNT = 0; break; ////Enter in SIMV_VC///////////
				case 'H': change_mode_variable = 7; change_value_flag = false; current_page = 0; TIM8->CNT = 0; break; ////Enter in CPAP//////////////
				case 'I': change_mode_variable = 8; change_value_flag = false; current_page = 0; TIM8->CNT = 0; break; ////Enter in BiPAP/////////////
				case 'J': change_value_flag = true; vt_pip_flag=true; current_pic=1; break;												 ////To Change PIP or VT///////////	changed pic so it take button press
				case 'j': touch_set_flag 		= true; break;												 ////To Set VT or PIP///////////////
				case 'K': change_value_flag = true; t_for_rotary = 3; current_pic=1; break;												 ////To Change PEEP//////////
				case 'k': touch_set_flag 		= true; break;												 ////To Set PEEP/////////////
				case 'L': change_value_flag = true; t_for_rotary = 4; current_pic=1; break;												 ////To Change FiO2//////////
				case 'l': touch_set_flag 		= true; break;												 ////To Set FiO2/////////////
				case 'M': change_value_flag = true; t_for_rotary = 5; current_pic=1; break;												 ////To Change RR////////////
				case 'm': touch_set_flag 		= true; break;												 ////To Set RR///////////////
				case 'N': change_value_flag = true; t_for_rotary = 6; current_pic=1; break;												 ////To Change I/E///////////
				case 'n': touch_set_flag 		= true; break;												 ////To Set I/E//////////////
				case 'R': change_value_flag = true; t_for_rotary = 7; current_pic=1; break;												 ////To Change Trigger///////
				case 'r': touch_set_flag 		= true; break;												 ////To Set Trigger//////////
				case 'Q': change_value_flag = true; t_for_rotary = 8; current_pic=1; break;												 ////To Change PIF///////////
				case 'q': touch_set_flag 		= true; break;												 ////To Set PIF//////////////
				case 'O': change_value_flag = true; t_for_rotary = 9; current_pic=1; break;												 ////To Change PS////////////
				case 'o': touch_set_flag 		= true; break;												 ////To Set PS///////////////
				case 'S': current_page = 5;	TIM8->CNT = 53; break; 								 ////FOR Modes///////////////
				case 'T': current_page = 0;	TIM8->CNT = 0; break; 								 ////FOR Confirm and cross button//////
				case 'P': controls_flag = true; break;														 ////For Controls////////////
				case 'V': current_page = 6;	TIM8->CNT = 64; break;								 ////For Monitoring /////////
				case 's': current_page = 8;	TIM8->CNT = 87; break;								 ////To go Second Page of MOnitoring /////////
				case 't': current_page = 6;	TIM8->CNT = 64; break;								 ////Went back to first Page of MOnitoring/////////
				case 'U': current_page = 7;	TIM8->CNT = 67; break; 								 ////FOR Alarms Limit Page//////
				case 'W': change_value_flag = true; t_for_rotary = 10; current_pic = 1; break; ////To Change Max limit of PIP////
				case 'w': touch_set_flag 		= true; break;
				case '1': change_value_flag = true; t_for_rotary = 11; current_pic = 1; break; ////To Change Low limit of PIP////
				case '2': touch_set_flag 		= true; break;
				case 'X': change_value_flag = true; t_for_rotary = 12; current_pic = 1; break; ////To Change Max limit of VT////
				case 'x': touch_set_flag 		= true; break;
				case '3': change_value_flag = true; t_for_rotary = 13; current_pic = 1; break; ////To Change Low limit of VT////
				case '4': touch_set_flag 		= true; break;
				case 'Y': change_value_flag = true; t_for_rotary = 14; current_pic = 1; break; ////To Change Max limit of RR////
				case 'y': touch_set_flag 		= true; break;
				case '5': change_value_flag = true; t_for_rotary = 15; current_pic = 1; break; ////To Change Low limit of RR////
				case '6': touch_set_flag 		= true; break;
				case 'Z': change_value_flag = true; t_for_rotary = 16; current_pic = 1; break; ////To Change Max limit of VE////
				case 'z': touch_set_flag 		= true; break;
				case '7': change_value_flag = true; t_for_rotary = 17; current_pic = 1; break; ////To Change Low limit of VE////
				case '8': touch_set_flag 		= true; break;
				case '9': Auto_fill_limits 	= true; break;
			}
			rxbuf_nextion[0] = 255;
		}
		rxbuf_nextion_previous[0] = rxbuf_nextion[0];
}
void Blender()
{
	current_encoder  = TIM4->CNT;
	if(((current_encoder<required_encoder && for_back_flag) || (current_encoder>required_encoder && !for_back_flag)) && move_motor)
	{
		blender_temp = 0.019f*current_encoder + 1.0f;
		current_blender_position = ceil(blender_temp);
		if (current_encoder == previous_encoder)pwm_for_blender_motor++;
		else
		{
			switch(current_blender_position)
			{
				case 2: pwm_for_blender_motor = 150; break; // To move motor from  21%  to  30%
				case 3:	pwm_for_blender_motor = 115; break; // To move motor from  30%  to  40%
				case 4:	pwm_for_blender_motor = 130; break; // To move motor from  40%  to  50%
				case 5:	pwm_for_blender_motor = 140; break; // To move motor from  50%  to  60%
				case 6:	pwm_for_blender_motor = 210; break; // To move motor from  60%  to  70%
				case 7:	pwm_for_blender_motor = 300; break; // To move motor from  70%  to  80%
				case 8:	pwm_for_blender_motor = 370; break; // To move motor from  80%  to  90%
				case 9: pwm_for_blender_motor = 400; break; // To move motor from  90%  to 100%
			}
		}
			htim3.Instance->CCR2 = pwm_for_blender_motor;
			previous_encoder = current_encoder;
			blender_millis = millis();
	}
	else if(upper_bound && (millis()-blender_millis)<=300)htim3.Instance->CCR2 = 350;
	else if(lower_bound && (millis()-blender_millis)<=300)htim3.Instance->CCR2 = 150;
	else
	{
		htim3.Instance->CCR2 = 0;
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET); // To stop the motor
		move_motor  = false;
		upper_bound = false;
		lower_bound = false;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////// Knob ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void counter_position()
{
	switch (current_page)
	{
		case 0: TIM8->CNT = 0; break;
		case 1: TIM8->CNT = 13; break;
		case 2: TIM8->CNT = 21; break;
		case 3: TIM8->CNT = 31; break;
		case 4: TIM8->CNT = 41; break;
		case 7: TIM8->CNT = 67; break;
	}
}
void controls_page()
{
	switch(change_mode_variable)
		{
			case 0:	current_page = 1; TIM8->CNT = 13; break;
			case 1: current_page = 1; TIM8->CNT = 13; break;
			case 3: current_page = 1; TIM8->CNT = 13; break;
			case 2: current_page = 2; TIM8->CNT = 21; break;
			case 4: current_page = 2; TIM8->CNT = 21; break;
			case 5: current_page = 3; TIM8->CNT = 31; break;
			case 6: current_page = 4; TIM8->CNT = 41; break;
		}
}
void orange_pic()
{
	switch(current_page){
		case 0: if(current_pic==7) NEXTION_PicChange(current_pic+5); else if(current_pic<=3) NEXTION_PicChange(current_pic+8); break;
		case 1: NEXTION_PicChange(current_pic+4) ; break;
		case 2: NEXTION_PicChange(current_pic+5) ; break;
		case 3: NEXTION_PicChange(current_pic+5) ; break;
		case 4: NEXTION_PicChange(current_pic+6) ; break;
		case 7: NEXTION_PicChange(current_pic+10); break;}
		//default: NEXTION_PicChange(current_pic+5); for page no. 2&3
}

void Auto_limits()
{
	pip_max = 40;
	pip_max_buffer = 40;  
	pip_low 			 = 8;
	pip_low_buffer = 8;	  
	vt_max  			 = 1000;
	vt_max_buffer  = 1000;
	vt_low	  		 = 200;
	vt_low_buffer  = 200;
	rr_max  			 = 30;
	rr_max_buffer  = 30;
	rr_low  			 = 10;
	rr_low_buffer  = 10;
}
void knob()
{
	if((((int)TIM8->CNT)-knob_value) >= 65435) TIM8->CNT = 0;
	knob_value = TIM8->CNT;
	button_press = HAL_GPIO_ReadPin (GPIOC, GPIO_PIN_8);
	if 			(vt_pip_flag)			 {if(change_mode_variable%2!=0) t_for_rotary= 1; else t_for_rotary= 2; vt_pip_flag = false;}
	else if (controls_flag)		 {controls_flag = false; controls_page();}
	else if (Auto_fill_limits) {Auto_limits(); Auto_fill_limits = false;}
	if (!change_value_flag)
	{
		switch (current_page)
		{
			case 0:
				if (knob_value>8 && knob_value<65435) {TIM8->CNT = 0; knob_value = 8;}
				else if (knob_value>65435){TIM8->CNT=8;knob_value = 8;}
				current_pic = knob_value;
				switch(current_pic){
					case 1: if(change_mode_variable%2!=0) t_for_rotary = 1; else t_for_rotary = 2; break; // 1 for PIP and 2 for VT
					case 2: t_for_rotary = 3; break;  // For PEEP
					case 3: t_for_rotary = 4; break;  // For FiO2
					default:t_for_rotary = 0; break;}
				break;
			case 1:
				if (knob_value>17) 			{TIM8->CNT = 13; knob_value = 17;} // To rotate forwards
				else if (knob_value<13) {TIM8->CNT = 17; knob_value = 17;} // To rotate backwards
				current_pic = knob_value;
				if (current_pic<17 && current_pic>13) {t_for_rotary = knob_value - 9; confirm_flag = false;} // Updating t_for_rotary to note the position where the rotary knob is standing
				else	if (current_pic==17) confirm_flag = true;
				else t_for_rotary = 0;
				break;
			case 2:
				if (knob_value>26) 			{TIM8->CNT = 21; knob_value = 26;} // To rotate forwards
				else if (knob_value<21) {TIM8->CNT = 26; knob_value = 26;} // To rotate backwards
				current_pic = knob_value;
				if (current_pic<26 && current_pic>21) {t_for_rotary = knob_value - 17; confirm_flag = false;} // Updating t_for_rotary to note the position where the rotary knob is standing
				else	if (current_pic==26) confirm_flag = true;
				else t_for_rotary = 0;
				break;
			case 3:
				if (knob_value>36)		  {TIM8->CNT = 31; knob_value = 36;} // To rotate forwards
				else if (knob_value<31) {TIM8->CNT = 36; knob_value = 36;} // To rotate backwards
				current_pic = knob_value;
				if (current_pic<36 && current_pic>31) {if(current_pic!=35)t_for_rotary = knob_value - 27; else t_for_rotary = 8; confirm_flag = false;} // Updating t_for_rotary to note the position where the rotary knob is standing
				else	if (current_pic==36) confirm_flag = true;
				else t_for_rotary = 0;
				break;
			case 4:
				if (knob_value>47) 			{TIM8->CNT = 41; knob_value = 47;} // To rotate forwards
				else if (knob_value<41) {TIM8->CNT = 47; knob_value = 47;} // To rotate backwards
				current_pic = knob_value;
				if (current_pic<47 && current_pic>41) {t_for_rotary = knob_value - 37; confirm_flag = false;} // Updating t_for_rotary to note the position where the rotary knob is standing
				else	if (current_pic==47) confirm_flag = true;
				else t_for_rotary = 0;
				break;
			case 5:
				if (knob_value>63) 			{TIM8->CNT = 53; knob_value = 63;} // To rotate forwards
				else if (knob_value<53) {TIM8->CNT = 63; knob_value = 63;} // To rotate backwards
				current_pic = knob_value;
				if (current_pic==63) confirm_flag = true;
				break;
			case 6:
				if (knob_value>66) { TIM8->CNT = 64; knob_value = 66;} // Main window of page 6 (updated t_for_rotary in case button is pressed in this window)
				else if (knob_value<64) {TIM8->CNT = 64; knob_value = 64;} // To remain at Main window of page 3 (updated t_for_rotary in case button is pressed in this window)
				current_pic = knob_value;
				if (current_pic==65) confirm_flag = true;
			break;
			case 7:
				if (knob_value>77) { TIM8->CNT = 67; knob_value = 77;} // Main window of page 6 (updated t_for_rotary in case button is pressed in this window)
				else if (knob_value<67) {TIM8->CNT = 67; knob_value = 67;} // To remain at Main window of page 3 (updated t_for_rotary in case button is pressed in this window)
				current_pic = knob_value;
				if (current_pic<76 && current_pic>67) {t_for_rotary = knob_value - 58; confirm_flag = false;} // Updating t_for_rotary to note the position where the rotary knob is standing
				else if      (current_pic==76) Auto_fill_limits = true;
				else if (current_pic==77) confirm_flag = true;
				else t_for_rotary = 0;
			break;
			case 8:
				if (knob_value>89) { TIM8->CNT = 87; knob_value = 89;} // Main window of page 6 (updated t_for_rotary in case button is pressed in this window)
				else if (knob_value<87) {TIM8->CNT = 87; knob_value = 87;} // To remain at Main window of page 3 (updated t_for_rotary in case button is pressed in this window)
				current_pic = knob_value;
				if (current_pic==88) confirm_flag = true;
			break;
		}
		if(knob_value!=previous_knob_value || pic_change){pic_change = false; NEXTION_PicChange(current_pic);}
		previous_knob_value = knob_value;
	}
	else
 	{
		uint16_t low_limit_bound = 0;
		switch(t_for_rotary)
			{
				case 1:
				if(enter_flag) {TIM8->CNT = p_for_knob - 8; enter_flag = false;}
				if(TIM8->CNT>32)	TIM8->CNT = 32;
			  p_for_knob = 8+TIM8->CNT;
				if (button_press != previous_button_press || touch_set_flag)
				{
					p_for_buffer = p_for_knob;
					parameter_value_change = true;
					skip_one_cycle = true;
					counter_position();
					enter_flag = true;
				}
				break;

				case 2:
				if(enter_flag) {TIM8->CNT = (vt_for_knob - 200)/10; enter_flag = false;}
				if(TIM8->CNT>80) TIM8->CNT = 80;
				vt_for_knob = 200+(TIM8->CNT*10);
				if (button_press != previous_button_press || touch_set_flag)
				{
					vt_for_buffer = vt_for_knob;
					parameter_value_change = true;
					skip_one_cycle = true;
					counter_position();
					enter_flag = true;
				}
				break;

				case 3:
				if(enter_flag) {TIM8->CNT = PEEP_for_knob; enter_flag = false;}
				if(TIM8->CNT>20) TIM8->CNT = 20;
				PEEP_for_knob = TIM8->CNT;
				if (button_press != previous_button_press || touch_set_flag)
				{
					PEEP_for_buffer = PEEP_for_knob;
					parameter_value_change = true;
					skip_one_cycle = true;
					counter_position();
					enter_flag = true;
					if (PEEP_for_buffer > 0)  Peep_nextion_flag = true;
					else Peep_nextion_flag = false;
				}
			  break;

				case 4:
				if(enter_flag) {TIM8->CNT = (FiO2_for_knob - 20)/10; enter_flag = false;}
				if(TIM8->CNT>8) TIM8->CNT = 8;
				if(TIM8->CNT==0) FiO2_for_knob = 21;
				else FiO2_for_knob = 20 +(10*TIM8->CNT);
				if (button_press != previous_button_press || touch_set_flag)
				{
					FiO2_for_buffer = FiO2_for_knob;
					parameter_value_change = true;
					skip_one_cycle = true;
					counter_position();
					enter_flag = true;
				}
				break;

				case 5:
				if(enter_flag) {TIM8->CNT = rr_for_knob - 10; enter_flag = false;}
				if(TIM8->CNT>20) TIM8->CNT = 20;
				rr_for_knob = 10+TIM8->CNT;
				if (button_press != previous_button_press || touch_set_flag)
				{
					rr_for_buffer = rr_for_knob;
					parameter_value_change = true;
					skip_one_cycle = true;
					counter_position();
					enter_flag = true;
				}
				break;

				case 6:
				if(enter_flag) {TIM8->CNT = ie_for_knob - 10; enter_flag = false;}
				if(TIM8->CNT>20) TIM8->CNT = 20;
					ie_for_knob = 10+TIM8->CNT;
				if (button_press != previous_button_press || touch_set_flag)
				{
					ie_for_buffer = ie_for_knob;
					counter_position();
					enter_flag = true;
				}
				break;

				case 7:
				if(enter_flag) {TIM8->CNT = trigger_for_knob - 2; enter_flag = false;}
				if(TIM8->CNT>18) TIM8->CNT = 18;
				trigger_for_knob = 2+ TIM8->CNT;
				if (button_press != previous_button_press || touch_set_flag)
				{
					trigger_for_buffer = trigger_for_knob;
					counter_position();
					enter_flag = true;
				}
				break;

				case 8:
				if(enter_flag) {TIM8->CNT = (pif_for_knob - 10)/5; enter_flag = false;}
				if(TIM8->CNT>18) TIM8->CNT = 18;
				pif_for_knob = 10 +(TIM8->CNT*5);
				if (button_press != previous_button_press || touch_set_flag)
				{
					pif_for_buffer = pif_for_knob;
					counter_position();
					enter_flag = true;
				}
				break;

				case 9:
				if(enter_flag) {TIM8->CNT = PS_for_knob-5; enter_flag = false;}
				if(TIM8->CNT>20)  TIM8->CNT = 20;
				PS_for_knob = 5+TIM8->CNT;
				if (button_press != previous_button_press || touch_set_flag)
				{
					PS_for_buffer = PS_for_knob;
					counter_position();
					enter_flag = true;
				}
			  break;

				case 10:
				if(enter_flag) 	 {TIM8->CNT = pip_max-15; enter_flag = false;}
				if(TIM8->CNT>25)  TIM8->CNT = 25;
				pip_max = 15+TIM8->CNT;
				if (button_press != previous_button_press || touch_set_flag)
				{
					pip_max_buffer = pip_max;
					counter_position();
					enter_flag = true;
				}
			  break;

				case 11:
				low_limit_bound = pip_max-9;
				if(enter_flag) 	 {TIM8->CNT = pip_low-8; enter_flag = false;}
				if(TIM8->CNT>low_limit_bound)  TIM8->CNT = low_limit_bound;
				pip_low = 8+TIM8->CNT;
				if (button_press != previous_button_press || touch_set_flag)
				{
					pip_low_buffer = pip_low;
					counter_position();
					enter_flag = true;
				}
			  break;

				case 12:
				if(enter_flag) 	 {TIM8->CNT = (vt_max-300)/50; enter_flag = false;}
				if(TIM8->CNT>14)  TIM8->CNT = 14;
				vt_max = 300+(50*TIM8->CNT);
				if (button_press != previous_button_press || touch_set_flag)
				{
					vt_max_buffer = vt_max;
					counter_position();
					enter_flag = true;
				}
			  break;

				case 13:
				low_limit_bound = (vt_max-250)/50;
				if(enter_flag) 	 {TIM8->CNT = vt_low-8; enter_flag = false;}
				if(TIM8->CNT>low_limit_bound)  TIM8->CNT = low_limit_bound;
				vt_low = 200+(50*TIM8->CNT);
				if (button_press != previous_button_press || touch_set_flag)
				{
					vt_low_buffer = vt_low;
					counter_position();
					enter_flag = true;
				}
			  break;

				case 14:
				if(enter_flag) 	 {TIM8->CNT = rr_max-15; enter_flag = false;}
				if(TIM8->CNT>15)  TIM8->CNT = 15;
				rr_max = 15+TIM8->CNT;
				if (button_press != previous_button_press || touch_set_flag)
				{
					rr_max_buffer = rr_max;
					counter_position();
					enter_flag = true;
				}
			  break;

				case 15:
				low_limit_bound = rr_max-11;
				if(enter_flag) 	 {TIM8->CNT = rr_low-10; enter_flag = false;}
				if(TIM8->CNT>low_limit_bound)  TIM8->CNT = low_limit_bound;
				rr_low = 10+TIM8->CNT;
				if (button_press != previous_button_press || touch_set_flag)
				{
					rr_low_buffer = rr_low;
					counter_position();
					enter_flag = true;
				}
			  break;

				case 16:
				if(enter_flag) 	 {TIM8->CNT = (ve_max-25)/5; enter_flag = false;}
				if(TIM8->CNT>55)  TIM8->CNT = 55;
				ve_max = 25+(TIM8->CNT*5);
				if (button_press != previous_button_press || touch_set_flag)
				{
					ve_max_buffer = ve_max;
					counter_position();
					enter_flag = true;
				}
			  break;
				case 17:
				low_limit_bound = (ve_max-25)/5;
				if(enter_flag) 	 {TIM8->CNT = ve_low-20; enter_flag = false;}
				if(TIM8->CNT>low_limit_bound)  TIM8->CNT = low_limit_bound;
				ve_low = 20+(TIM8->CNT*5);
				if (button_press != previous_button_press || touch_set_flag)
				{
					ve_low_buffer = ve_low;
					counter_position();
					enter_flag = true;
				}
			  break;
			 }
			if (touch_set_flag) {change_value_flag = false; touch_set_flag = false;}
	}

	if (button_press != previous_button_press && previous_button_press == 0)
	{
		if (current_pic==4)			 {change_value_flag = false; enter_flag = true; controls_page(); NEXTION_PageChange(current_page);}
		else if (current_pic==8) {change_value_flag = false; enter_flag = true; TIM8->CNT = 64; current_page = 6; NEXTION_PageChange(current_page);}
		else if (current_page==5){change_value_flag = false; enter_flag = true; TIM8->CNT = 0;  current_page = 0; NEXTION_PageChange(current_page); change_mode_variable = current_pic-53;}
		else if (current_pic==5) {change_value_flag = false; enter_flag = true; TIM8->CNT = 53; current_page = 5; NEXTION_PageChange(current_page);}
		else if (current_pic==6) {change_value_flag = false; enter_flag = true; TIM8->CNT = 67; current_page = 7; NEXTION_PageChange(current_page);}
		else if (current_pic==66){change_value_flag = false; enter_flag = true; TIM8->CNT = 87; current_page = 8; NEXTION_PageChange(current_page);}
		else if (current_pic==89){change_value_flag = false; enter_flag = true; TIM8->CNT = 64; current_page = 6; NEXTION_PageChange(current_page);}
		else if (confirm_flag)	 {change_value_flag = false; confirm_flag = false ; enter_flag = true; TIM8->CNT = 0;  current_page = 0; NEXTION_PageChange(current_page);}
		else if (current_pic!=0 && current_pic!=13 && current_pic!=21 && current_pic!=31 && current_pic!=41 && current_pic!=53 && current_pic!=67) {change_value_flag=!change_value_flag; if (change_value_flag) orange_pic();else pic_change = true;}
	}
	previous_button_press = button_press;
}
////////////////////////////////////////////////////////
////////////////////////Knob Buffer/////////////////////
////////////////////////////////////////////////////////
///// creating buffer to change the values at the end of breath

void buffer_knob()
{
	change_mode_touch_variable_buffer = change_mode_variable;
	if(change_mode_variable!= previous_change_mode_touch_variable) default_values();
	previous_change_mode_touch_variable = change_mode_variable;
	display_appear = true; // To restart to keep the change in change mode variable

////////Knob Settings//////////////

		///RR//
		RR_min	= rr_for_buffer;

		///IE///
		E_I_ratio = ie_for_buffer/10;//E_I_ratio

		///PIP///
		PIP_cmH2O	= p_for_buffer;
		ipap = PIP_cmH2O;

		///VT///
		 VT_ml = vt_for_buffer;

		///Flow Rate///
		Flow_rate	= pif_for_buffer;

		///PS///
		Pressure_support	= PS_for_buffer;

		 ////////// PEEP ////////////////
		 peep_set = PEEP_for_buffer;
		 epap = peep_set;//.................................correct this

		//////////// VT //////////////////
		VT_ml = vt_for_buffer ;

		///////////// FiO2 Settings for Blender Motor //////////////////////////
		FiO2_set = FiO2_for_buffer;
		if (FiO2_for_buffer!=previous_FiO2)
		{
			if (FiO2_for_buffer>previous_FiO2){ for_back_flag = true;  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET); HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);} //To move the motor in Forward direction
			else 															{ for_back_flag = false; HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET); HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_SET);} //To move the motor in Backward direction
			required_encoder = 5.52*FiO2_for_buffer - 110;
			Flash_Write(0x080E0000,required_encoder);
			if (FiO2_for_buffer<previous_FiO2 && FiO2_for_buffer==21) 			{required_encoder=required_encoder + 20;lower_bound=true;Flash_Write(0x080E0000,0);}
			else if (FiO2_for_buffer>previous_FiO2 && FiO2_for_buffer==100) {required_encoder=required_encoder - 20;upper_bound=true;Flash_Write(0x080E0000,440);}
			move_motor = true;
		}
		previous_FiO2 = FiO2_for_buffer;
		///IE-Time calculation//
		I_plus_E = (60.0f/ RR_min);
		Insp_secs = I_plus_E / (1 + E_I_ratio);
		Expr_secs = I_plus_E - Insp_secs;
		Insp_msecs = 1000 * Insp_secs;
		Expr_msecs = 1000 * Expr_secs;
}
//////////////////////////////////////////////////////
///////////////////////////Sensirion Function////////
//////////////////////////////////////////////////////
float peep_generator(uint8_t pressure)
{
		EXP_PWM = (0.0022*pow(pressure,3)) - (0.2057*pow(pressure,2)) + (17.168*pressure) + (5.339); //(0.0002*(PEEP_for_knob*PEEP_for_knob*PEEP_for_knob*PEEP_for_knob*PEEP_for_knob)) - (0.0107*(PEEP_for_knob*PEEP_for_knob*PEEP_for_knob*PEEP_for_knob)) + (0.2288*(PEEP_for_knob*PEEP_for_knob*PEEP_for_knob)) - (2.2078*(PEEP_for_knob*PEEP_for_knob)) + (22.123*PEEP_for_knob) + 13.872;
	//EXP_PWM = (-0.1741)*(PEEP_for_knob*PEEP_for_knob) + (15.671*PEEP_for_knob) + 16.732;
		return EXP_PWM;
}
float ads_read_Sensirion ()
{
			ADSwrite[0] = 0x10;
			ADSwrite[1] = 0x00;

			HAL_I2C_Master_Transmit(&hi2c3, 0x40 << 1, ADSwrite, 0x02, 0xFF);
			HAL_Delay(1);
			HAL_I2C_Master_Receive(&hi2c3, 0x40 <<1, buffer_for_sensirion, 0x03, 0xFF);
			reading_ended = (buffer_for_sensirion[0] << 8 | buffer_for_sensirion[1]);
			return reading_ended;
}
uint8_t lpm_counter = 0;
void read_flow_sensor()
{
		uint8_t lpm_diff = 0;
		//LpM_Sensor4 = (ads_read_Sensirion() - results_4_calib) / 120;
		LpM_Sensor4 = (ads_read_Sensirion() - 32768) / 120;
		LpM_Sensor4_for_trigger = LpM_Sensor4;
		//LpM_Sensor4_for_nextion = LpM_Sensor4+90;
		lpm_diff = fabs(LpM_Sensor4_for_nextion - LpM_Sensor4);
		if(lpm_diff < 1)
		{
			lpm_counter += 1;
			LpM_Sensor4_for_nextion = LpM_Sensor4_for_nextion + LpM_Sensor4/lpm_counter;
		}
		else
		{
			LpM_Sensor4_for_nextion =  LpM_Sensor4;
			lpm_counter=1;
		}
			///////////////PIF/////////////////////
		if (LpM_Sensor4 > LpM_Max_real)
		{
			LpM_Max_real = LpM_Sensor4;
		}
		LpM_Sensor4_for_nextion = 87 + LpM_Sensor4_for_nextion*0.83;
		if (LpM_Sensor4_for_nextion < 87 && valve4_state == 1) LpM_Sensor4_for_nextion = 87;
		else if (LpM_Sensor4_for_nextion < 5) LpM_Sensor4_for_nextion = 5;
		else if (LpM_Sensor4_for_nextion > 170) LpM_Sensor4_for_nextion = 170;
}
////////////////////////container sensor/////////////////
float ads_readADC_SingleEnded_0()
{
			ADSwrite[0] = 0x01;
			ADSwrite[1] = 0xBF;  // Combination for decide ADS datasheet.
			ADSwrite[2] = 0x83; //10000011 LSB
			HAL_I2C_Master_Transmit(&hi2c3, 0x49 << 1, ADSwrite, 3, 100);
			ADSwrite[0] = 0x00;
			HAL_I2C_Master_Transmit(&hi2c3, 0x49 << 1 , ADSwrite, 1 ,100);
			HAL_Delay(1);
			HAL_I2C_Master_Receive(&hi2c3, 0x49 <<1, ADSwrite, 2, 100);
			reading_ended = (ADSwrite[0] << 8 | ADSwrite[1] );
			return reading_ended;
}
////////////////////////controling container pressure/////////////////
void cont_p()
{
			//////////////////////////////////////////////////////
		///////////////////container pressure sensor//////////
		//////////////////////////////////////////////////////
	results_2=ads_readADC_SingleEnded_0();
	ADC_Sensor2 = results_2 - results_2_calib;
	ADC_Volts_Sensor2 = ADC_Sensor2 * 0.0078125f;
	kpa_Sensor2 = ADC_Volts_Sensor2 / 0.3041f; // division by sensitivity to convert into kpa
	cmH2O_Sensor2 = kpa_Sensor2 * 6.895f* 10.1972f;
	if (cmH2O_Sensor2 <= 2000)
	{
		htim2.Instance->CCR1 = 1000; //digitalWrite(HF_solen_Container_valve2, valve3_state);
	}
	else
	{
	 htim2.Instance->CCR1 = 0;
	}
}
//////////////////////////lung pressure sensor///////////////////
float ads_readADC_SingleEnded_1 ()
{
			ADSwrite[0] = 0x01;
			ADSwrite[1] = 0xE1;  // Combination for decide ADS datasheet.
			ADSwrite[2] = 0x83; //10000011 LSB
			HAL_I2C_Master_Transmit(&hi2c3, 0x48 << 1, ADSwrite, 3, 100);
			ADSwrite[0] = 0x00;
			HAL_I2C_Master_Transmit(&hi2c3, 0x48 << 1 , ADSwrite, 1 ,100);
			HAL_Delay(1);
			HAL_I2C_Master_Receive(&hi2c3, 0x48 <<1, ADSwrite, 2, 100);
			reading_ended = (ADSwrite[0] << 8 | ADSwrite[1] );
			return reading_ended;
}
uint8_t P_counter=0;
float previous_cmH2O_Sensor3_for_nextion = 0.0f;
void read_lung_Pressure_sensor()
{
	results_3 = ads_readADC_SingleEnded_1 ();
	ADC_Sensor3 = results_3 - results_3_calib;
	ADC_Volts_Sensor3 = ADC_Sensor3 * 0.1875f; // multiplier_ads2_L; // 16 gain
	kpa_Sensor3 = ADC_Volts_Sensor3 / 44.901f; // division by sensitivity to convert into kpa
	cmH2O_Sensor3 = kpa_Sensor3 * 10.1972f; // kpa to cmh2o converstion factor
//	uint8_t P_diff  = fabs(cmH2O_Sensor3 - cmH2O_Sensor3_for_nextion);
//		if(P_diff < 1)
//		{
//			P_counter += 1;
//			cmH2O_Sensor3_for_nextion = cmH2O_Sensor3_for_nextion + cmH2O_Sensor3/P_counter;
//		}
//		else
//		{
//			cmH2O_Sensor3_for_nextion =  cmH2O_Sensor3;
//			P_counter=1;
//		}

	 if (cmH2O_Sensor3 > PIP_real)
		{
			PIP_real = cmH2O_Sensor3;
		}
		cmH2O_Sensor3_for_nextion = (cmH2O_Sensor3 + pre1_results_4 + pre2_results_4 + pre3_results_4 + pre4_results_4)/5;
		pre4_results_4 = pre3_results_4;
		pre3_results_4 = pre2_results_4;
		pre2_results_4 = pre1_results_4;
		pre1_results_4 = cmH2O_Sensor3;
//    kpa_Sensor3 = ADC_Volts_Sensor3 /  44.901f; // division by sensitivity to convert into kpa
//    cmH2O_Sensor3_for_nextion = kpa_Sensor3 * 10.1972f;
		float lower_clipping = PEEP_for_buffer+0.5; // Clipping the waveform by force
		if ( cmH2O_Sensor3_for_nextion <= lower_clipping && valve4_state == 0 && Peep_nextion_flag == true) cmH2O_Sensor3_for_nextion = lower_clipping;
		else if (valve4_state == 1 && cmH2O_Sensor3_for_nextion < lower_clipping && Peep_nextion_flag == true) cmH2O_Sensor3_for_nextion = lower_clipping;
		cmH2O_Sensor3_for_nextion = 27 + (cmH2O_Sensor3_for_nextion*2.3f);//(cmH2O_Sensor3_for_nextion*4);
//	  float check_nextion_cmH2O = fabs(previous_cmH2O_Sensor3_for_nextion - cmH2O_Sensor3_for_nextion);
//	  if ( cmH2O_Sensor3_for_nextion <= 10 && valve4_state == 0 && Peep_nextion_flag == true) cmH2O_Sensor3_for_nextion = previous_cmH2O_Sensor3_for_nextion; // to flat the graph of pressure in case of peep > 0
//	  previous_cmH2O_Sensor3_for_nextion = cmH2O_Sensor3_for_nextion;
		if(cmH2O_Sensor3_for_nextion<27) cmH2O_Sensor3_for_nextion = 27;
		else if(cmH2O_Sensor3_for_nextion>165) cmH2O_Sensor3_for_nextion = 165;
}
float ads_read_for_oxygen ()
{
			ADSwrite[0] = 0x01;
			ADSwrite[1] = 0xF7;  // Combination for decide ADS datasheet.
			ADSwrite[2] = 0x83; //10000011 LSB
			HAL_I2C_Master_Transmit(&hi2c3, 0x48 << 1, ADSwrite, 3, 100);
			ADSwrite[0] = 0x00;
			HAL_I2C_Master_Transmit(&hi2c3, 0x48 << 1 , ADSwrite, 1 ,100);
			HAL_Delay(1);
			HAL_I2C_Master_Receive(&hi2c3, 0x48 <<1, ADSwrite, 2, 100);
			reading_ended = (ADSwrite[0] << 8 | ADSwrite[1] );
			return reading_ended;
}
void read_oxygen()
{
		//////////////////////////////////////////////////////
		///////////////////oxygen sensor//////////////////////
		//////////////////////////////////////////////////////
  results_1 = ads_read_for_oxygen ();
	ADC_Sensor1 = (int32_t) (results_1);// oxygen sensor
	ADC_Volts_Sensor1 = ADC_Sensor1 * 0.03125F;
	FiO2_Sensor1 = 25*ADC_Volts_Sensor1 - 312.5f;
  //FiO2_Sensor1 = (uint8_t) ((ADC_Volts_Sensor1 * ADC_Volts_to_FiO2_Slope_Sensor1) + Offset_Sensor1);
}
double previous_VT = 0;
void Tidal_Volume()
{
	  LpM_for_tidal_volume = (10.0*LpM_Sensor4);//Rounding up flow rate for 1 decimal point. When use this lpm don't forget to divide this value by 10
		//LpM_for_tidal_volume = LpM_for_tidal_volume/10;
		if (LpM_Sensor4 < 1 && LpM_Sensor4 > -1) LpM_for_tidal_volume = 0;
		mLpms = (1000 * LpM_for_tidal_volume/10.0) / 60000.0;// to be used to calculate Tidal Volume
		current_time = millis();
		dt = current_time - prev_time;
		VT +=  mLpms*dt;
		prev_time = current_time;
		VT_for_nextion = 29+(VT*0.136);
		if (VT_for_nextion < 29)VT_for_nextion = 29;
		else if(VT_for_nextion > 165) VT_for_nextion = 165;
		if (VT > VT_max)
		{
			VT_max = VT;
		}
		previous_VT = VT;
		Minute_volume_set = (float)(VT_ml * RR_min) / 1000.0f;
		read_lung_Pressure_sensor();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// RR_IE_VE_Monitoring ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RR_IE_VE_Monitoring()
{
	if ((valve5_state == 0) && (RR_monitoring_flag == true))
	{
		currentMillisRR = millis();
		RR_monitoring_final = ceil(60000.0f /(currentMillisRR - previousMillisRR));
		RR_monitoring_flag = false;
		previousMillisRR = currentMillisRR;
	}
	if ((valve5_state == 1) && (IE_monitoring_flag == true))
	{
		currentMillisIE = millis();
		TCT_monitoring = (60.0f / RR_monitoring_final);
		Insp_monitoring =  (float)(currentMillisIE - previousMillisRR) / 1000.0f;
		Exp_monitoring  =  TCT_monitoring - Insp_monitoring;
		IE_monitoring_final = Exp_monitoring / Insp_monitoring;
		IE_monitoring_for_display = IE_monitoring_final*10.0f;
		IE_monitoring_flag  = false;
	}
	Minute_volume_real = ((VT_max_display * RR_monitoring_final) / 1000.0f)*10; // Minute Ventilation // Multiply by 10 to send as float for nextion
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// flowrate calculation ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Claculate_Flowrate()
{
		read_flow_sensor();//to compute tidal volume first read flow rate
		Tidal_Volume();
		Flow_rate_for_volume =  Flow_rate;
		switch((Flow_rate+7)/11)
		{
			case 0:		pwm_for_defined_flow_curve = 500;	break;
			case 1:		pwm_for_defined_flow_curve = 2.4924f*Flow_rate_for_volume + 603.40f ;	break; // 4 to 14		//593.40f
			case 2:		pwm_for_defined_flow_curve = 2.2722f*Flow_rate_for_volume + 607.29f ; break; //15 to 25   //597.29f
			case 3: 	pwm_for_defined_flow_curve = 1.0325f*Flow_rate_for_volume + 636.02f ; break; //26 to 36		//631.02f
			case 4: 	pwm_for_defined_flow_curve = 1.0325f*Flow_rate_for_volume + 636.02f ; break; //37 to 47
			case 5: 	pwm_for_defined_flow_curve = 1.0325f*Flow_rate_for_volume + 636.02f ; break; //48 to 58
			case 6:	 	pwm_for_defined_flow_curve = 0.8317f*Flow_rate_for_volume + 647.79f ; break; //59 to 69		//642.79f
			case 7: 	pwm_for_defined_flow_curve = 0.6236f*Flow_rate_for_volume + 662.06f ; break; //70 to 80		//657.06f
			case 8: 	pwm_for_defined_flow_curve = 0.6236f*Flow_rate_for_volume + 662.06f ; break; //81 to 91
			case 9: 	pwm_for_defined_flow_curve = 0.6236f*Flow_rate_for_volume + 662.06f ; break; //92 to 102
			case 10: 	pwm_for_defined_flow_curve = 0.6236f*Flow_rate_for_volume + 662.06f ; break; //103 to 113
			default: 	pwm_for_defined_flow_curve = 730;	break;
		}
		pwm_for_defined_flow_curve = pwm_for_defined_flow_curve-5.0f;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////  Alarms /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void alarm_display()
{
	int turn_samples = 80;
	int reset_turns = no_of_alarms*turn_samples;
	if 			(alarms_id[0] == 1  && turn[0] <= turn_samples) {turn[0]++;	display_counter++; NEXTION_SendString("t12","High Pressure!"); 		 		return;}
	else if (alarms_id[0] == 2  && turn[0] <= turn_samples) {turn[0]++;	display_counter++; NEXTION_SendString("t12","Low Pressure!"); 		 		return;}
  else if (alarms_id[1] == 3  && turn[1] <= turn_samples) {turn[1]++; display_counter++; NEXTION_SendString("t12","High Tidal Volume!"); 		return;}
	else if (alarms_id[1] == 4  && turn[1] <= turn_samples) {turn[1]++; display_counter++; NEXTION_SendString("t12","Low Tidal VOlume!");  		return;}
  else if (alarms_id[2] == 5  && turn[2] <= turn_samples) {turn[2]++; display_counter++; NEXTION_SendString("t12","High Breath Rate!");  		return;}
	else if (alarms_id[2] == 6  && turn[2] <= turn_samples) {turn[2]++; display_counter++; NEXTION_SendString("t12","Low Breath Rate!");      return;}
	else if (alarms_id[3] == 7  && turn[3] <= turn_samples) {turn[3]++; display_counter++; NEXTION_SendString("t12","High PEEP!");   			    return;}
	else if (alarms_id[3] == 8  && turn[3] <= turn_samples) {turn[3]++; display_counter++; NEXTION_SendString("t12","Low PEEP!");   			    return;}
	else if (alarms_id[4] == 9  && turn[4] <= turn_samples) {turn[4]++; display_counter++; NEXTION_SendString("t12","High Inspired Oxygen!"); return;}
	else if (alarms_id[4] == 10 && turn[4] <= turn_samples) {turn[4]++; display_counter++; NEXTION_SendString("t12","Low  Inspired Oxygen!"); return;}
	else if (alarms_id[5] == 11 && turn[5] <= turn_samples) {turn[5]++; display_counter++; NEXTION_SendString("t12","Air Leakage!");  			  return;}
	else if (alarms_id[6] == 12 && turn[6] <= turn_samples) {turn[6]++; display_counter++; NEXTION_SendString("t12","High Minute Volume!");  	return;}
	else if (alarms_id[6] == 13 && turn[6] <= turn_samples) {turn[6]++; display_counter++; NEXTION_SendString("t12","Low  Minute Volume!");  	return;}
	if(display_counter>reset_turns)
	{
		for(int i=0;i<7;i++) turn[i] = 0;
		display_counter = 0;
	}
}
void alarm_beep (uint8_t on_off)
{
	switch(on_off)
			{
///////////////////case for silent////////////////////////////
				case 0:
						htim10.Instance->CCR1=0;
						beep = 0;
				break;

///////////////////case for alternate ringing////////////////////////////
				case 1:
					beep++;
					switch (beep)
					{
//						case 1:  htim10.Instance->CCR1 = 1000;  break; //on for 20 samples
//						case 21: htim10.Instance->CCR1 = 0;    break;	//off for 20 samples
//						case 41: htim10.Instance->CCR1 = 1000; break;	//on for 10 samples
//						case 51: htim10.Instance->CCR1 = 0;    break;	//off for 10 samples
//						case 61: htim10.Instance->CCR1 = 1000; break;	//on for 5 samples
//						case 66: htim10.Instance->CCR1 = 0;    break;	//off for 5 samples
//						case 71: htim10.Instance->CCR1 = 1000; break;	//on for 3 samples
//						case 74: htim10.Instance->CCR1 = 0;    break;	//off for 3 samples
//						case 77: beep = 0; break;
						case 1:  htim10.Instance->CCR1 = 1000;  break; //on for 20 samples
						case 21: htim10.Instance->CCR1 = 0;    break;	//off for 20 samples
						case 26: htim10.Instance->CCR1 = 1000; break;	//on for 10 samples
						case 46: htim10.Instance->CCR1 = 0;    break;	//off for 10 samples
						case 56: htim10.Instance->CCR1 = 1000; break;	//on for 5 samples
						case 76: htim10.Instance->CCR1 = 0;    break;	//off for 5 samples
						case 96: htim10.Instance->CCR1 = 1000; break;	//on for 3 samples
						case 116: htim10.Instance->CCR1 = 0;    break;	//off for 3 samples
						case 136: htim10.Instance->CCR1 = 1000; break;	//on for 3 samples
						case 156: htim10.Instance->CCR1 = 0;    break;	//off for 3 samples
						case 176: beep = 0; break;
					}
					break;
			}

}
void alarms_PC(void)
{
////////////////////////////////////////////// For Peak Inspiratory Pressure //////////////////////////////////////
		if (PIP_display >= PIP_cmH2O+3)
		{
			pip_alarm = true;
			alarms_id[0] = 1;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n0",63488); // To change the color of the text to Red
		}
		else if (PIP_display <= PIP_cmH2O-3)
		{
			pip_alarm = true;
			alarms_id[0] = 2;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n0",63488); // To change the color of the text to Red
		}
		else if (pip_alarm)
		{
			pip_alarm = false;
			alarms_id[0] = 0;
			no_of_alarms -= 1;
			NEXTION_Text_FontColor("n0",65535); // To change the color of the text to Red
		}

////////////////////////////////////////////// For Tidal Volume ///////////////////////////////////////////////////

		if (VT_max_display > vt_max_buffer)  // For Max Limit of VT
		{
			vt_alarm = true;
			alarms_id[1] = 3;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n2",63488); // To change the color of the text to Red
		}
		else if (VT_max_display < vt_low_buffer)  // For Min Limit of VT
		{
			vt_alarm = true;
			alarms_id[1] = 4;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n2",63488); // To change the color of the text to Red
		}
		else if (vt_alarm)
		{
			vt_alarm = false;
			alarms_id[2] = 0;
			no_of_alarms -= 1;
			NEXTION_Text_FontColor("n2",65535); // To change the color of the text to Red
		}
		if (vt_alarm || pip_alarm || rr_alarm || peep_alarm || fio2_alarm || disconnection_alarm)
		{
			//alarm_beep(1);
			alarm_display();
			NEXTION_Text_Background("t12",63488);
		}
		else
		{
			alarm_beep(0);
			NEXTION_Text_Background("t12",0);
			NEXTION_SendString("t12"," ");
		}
}
void alarms_VC(void)
{
////////////////////////////////////////////////// For Peak Inspiratory Pressure //////////////////////////////////////

		if (pressure_exceed_max) // For Max Limit of PIP
		{
			pip_alarm = true;
			alarms_id[0] = 1;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n0",63488); // To change the color of the text to Red
		}
		else if (PIP_display < pip_low_buffer) // For Min Limit of PIP
		{
			pip_alarm = true;
			alarms_id[0] = 2;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n0",63488); // To change the color of the text to Red
		}
		else if (pip_alarm)
		{
			pip_alarm = false;
			alarms_id[0] = 0;
			no_of_alarms -= 1;
			NEXTION_Text_FontColor("n0",65535); // To change the color of the text to White
		}

////////////////////////////////////////////// For Tidal Volume ///////////////////////////////////////////////////

		if (VT_max_display > vt_for_buffer+50)
		{
			vt_alarm = true;
			alarms_id[1] = 3;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n2",63488); // To change the color of the text to Red
		}
		else if (VT_max_display < vt_for_buffer-50)
		{
			vt_alarm = true;
			alarms_id[1] = 4;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n2",63488); // To change the color of the text to Red
		}
		else if (vt_alarm)
		{
			vt_alarm = false;
			alarms_id[1] = 0;
			no_of_alarms -= 1;
			NEXTION_Text_FontColor("n2",65535); // To change the color of the text to White
		}
		if (vt_alarm || pip_alarm || rr_alarm || peep_alarm || fio2_alarm || disconnection_alarm)
		{
			alarm_beep(1);
			alarm_display();
			NEXTION_Text_Background("t12",63488);
		}
		else
		{
			alarm_beep(0);
			NEXTION_Text_Background("t12",0);
			NEXTION_SendString("t12"," ");
		}
}
void alarms_both(void)
{
///////////////////////////////// For RR //////////////////////////////////
		if (RR_monitoring_final > rr_for_knob+2)
		{
			rr_alarm = true;
			alarms_id[2] = 5;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n3",63488); // To change the color of the text to Red
		}
		else if (RR_monitoring_final < rr_for_knob-2)
		{
			rr_alarm = true;
			alarms_id[2] = 6;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n3",63488); // To change the color of the text to Red
		}
		else if (rr_alarm)
		{
			rr_alarm = false;
			alarms_id[2] = 0;
			no_of_alarms -= 1;
			NEXTION_Text_FontColor("n3",65535); // To change the color of the text to Red
		}

/////////////////////////////////// For PEEP ////////////////////////////////////

		if (PEEP_cmH2O > PEEP_for_buffer+2)
		{
			peep_alarm = true;
			alarms_id[3] = 7;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n5",63488); // To change the color of the text to Red
		}
		else if (PEEP_cmH2O < PEEP_for_buffer-2)
		{
			peep_alarm = true;
			alarms_id[3] = 8;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n5",63488); // To change the color of the text to Red
		}
		else if (peep_alarm)
		{
			peep_alarm = false;
			alarms_id[3] = 0;
			no_of_alarms -= 1;
			NEXTION_Text_FontColor("n5",65535); // To change the color of the text to Red
		}

////////////////////////////////////// For FiO2 /////////////////////////////////////////

		if (FiO2_Sensor1 > FiO2_for_buffer+5)
		{
			fio2_alarm = true;
			alarms_id[4] = 9;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n4",63488); // To change the color of the text to Red
		}
		else if (FiO2_Sensor1 < FiO2_for_buffer-5)
		{
			fio2_alarm = true;
			alarms_id[4] = 10;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n4",63488); // To change the color of the text to Red
		}
		else if (fio2_alarm)
		{
			fio2_alarm = false;
			alarms_id[4] = 0;
			no_of_alarms -= 1;
			NEXTION_Text_FontColor("n4",65535); // To change the color of the text to Red
		}

/////////////////////////////////////// For Disconnection/Air Leakage /////////////////////////////////

		if ((cmH2O_Sensor3 < (FiO2_for_buffer - 2.5) || cmH2O_Sensor3 < (p_for_buffer-10)))
		{
			if (disconnection_time_counter>200)
			{
				disconnection_alarm = true;
				alarms_id[5] = 11;
				no_of_alarms += 1;
			}
			else disconnection_time_counter++;
		}
		else if (!disconnection_alarm) disconnection_time_counter=0;
		else
		{
			disconnection_alarm = false;
			alarms_id[5] = 0;
			no_of_alarms -= 1;
		}
//////////////////////////////////// Main Power Supply Loss //////////////////////////////
//			if (FB_volt[1]<255)
//			{
//				switch(alternate_text)
//				{
//					case   0:	NEXTION_SendString("t12","Power Failure")  ; NEXTION_Text_Background("t12",63488);
//					case  80:	NEXTION_SendString("t12","On Backup Power");
//					case 160: alternate_text = 0;
//				}
//				switch(power_loss)
//				{
//					case 0:  htim10.Instance->CCR1 = 1000; break;	//on for 20 samples
//					case 20: htim10.Instance->CCR1 = 0;    break;	//off for 20 samples
//					case 40: htim10.Instance->CCR1 = 1000; break;	//on for 10 samples
//					case 60: htim10.Instance->CCR1 = 0;    break;	//off for 10 samples
//				}
//				alternate_text++;
//				power_loss++;
//				if (power_loss>12060)power_loss = 0;
//			}
//			else
//			{
//				NEXTION_SendString("t12"," ");
//				NEXTION_Text_Background("t12",0);
//			}
		/////////////////////// For Minute Volume ///////////////////////////
		if (Minute_volume_set>ve_max_buffer)
		{
			ve_alarm = true;
			alarms_id[6] = 12;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n4",63488); // To change the color of the text to Red

		}
		else if (Minute_volume_set<ve_low_buffer)
		{
			ve_alarm = true;
			alarms_id[6] = 13;
			no_of_alarms += 1;
			NEXTION_Text_FontColor("n4",63488); // To change the color of the text to Red
		}
		else if (ve_alarm)
		{
			ve_alarm = false;
			alarms_id[6] = 0;
			no_of_alarms -= 1;
			NEXTION_Text_FontColor("n4",65535); // To change the color of the text to Red
		}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////// proportional control //////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double FV_AM[30][2];
double P_AM[30];
double resistance_ML,compliance_ML;
int index_counter = 0;

void compute_lung_parameters()
{
	double elastance = 0.00;
	double det_arr = 0.00;
	double inverse[2][2] = {{0,0},{0,0}};
	double FVT_x_FV[2][2] = {{0,0},{0,0}}; // x * x-transpose
	double FVT_x_P[2] = {0,0}; // x-transpose * y
	for (int g=0;g<index_counter;g++)
	{
		FVT_x_FV[0][0] += pow(FV_AM[g][0],2);
		FVT_x_FV[1][1] += pow(FV_AM[g][1],2);
		FVT_x_FV[0][1] += FV_AM[g][0] * FV_AM[g][1];
	}
	FVT_x_FV[1][0] = FVT_x_FV[0][1];

	det_arr = (FVT_x_FV[0][0]*FVT_x_FV[1][1])-(FVT_x_FV[1][0]*FVT_x_FV[0][1]);

	inverse[0][0] = FVT_x_FV[1][1] / det_arr;
	inverse[1][1] = FVT_x_FV[0][0] / det_arr;
	inverse[0][1] = FVT_x_FV[0][1]*(-1) / det_arr;
	inverse[1][0] = FVT_x_FV[1][0]*(-1) / det_arr;

	for (int h=0;h<index_counter;h++)
	{
		FVT_x_P[0] +=  FV_AM[h][0]*P_AM[h];
		FVT_x_P[1] +=  FV_AM[h][1]*P_AM[h];
	}
	resistance_ML = (FVT_x_P[0]*inverse[0][0]+FVT_x_P[1]*inverse[0][1]); // 1 cmH2O/LPM = 60 cmH2O/LPS
	elastance = FVT_x_P[0]*inverse[1][0]+FVT_x_P[1]*inverse[1][1];
	compliance_ML = (1/elastance)*1000.0;
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////  PID_functions //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//*working variables*//

double Compute(double Setpoint, double Input)
{
  // if(!inAuto) return;
  // unsigned long now = millis();
   //int timeChange = (now - lastTime);
	//if(timeChange>=SampleTime)
   //{
      /*Compute all the working error variables*/
			//if(Input>=0)
      //{
				error = Setpoint - Input;
      //}
      double dInput = (Input - lastInput);
      /*Compute PID Output*/
      Output = kp * error + ITerm- kd * dInput;
			if(Output> outMax) Output = outMax;
      else if(Output < outMin) Output = outMin;
			ITerm+= (ki * error);
      if(ITerm> outMax) ITerm= outMax;
      else if(ITerm< outMin) ITerm= outMin;


      /*Remember some variables for next time*/
      lastInput = Input;
     // lastTime = now;
// }
	 return Output;
}

void SetTunings(double Kp, double Ki, double Kd)
{
  double SampleTimeInSec = ((double)SampleTime)/1000;
   kp = Kp;
   ki = Ki * SampleTimeInSec;
   kd = Kd / SampleTimeInSec;
}

void SetSampleTime(int NewSampleTime)
{
   if (NewSampleTime > 0)
   {
      double ratio  = (double)NewSampleTime
                      / (double)SampleTime;
      ki *= ratio;
      kd /= ratio;
      SampleTime = (unsigned long)NewSampleTime;
   }
}

void SetOutputLimits(double Min, double Max)
{
   if(Min > Max) return;
   outMin = Min;
   outMax = Max;

   if(Output > outMax) Output = outMax;
   else if(Output < outMin) Output = outMin;

   if(ITerm> outMax) ITerm= outMax;
   else if(ITerm< outMin) ITerm= outMin;
}
float Upper_limit()
{
	float max_value;
	switch ((PIP_cmH2O-8)/3)
	{
		case 0:  max_value = 0.4;  break;
		case 1:  max_value = 0.45f; break;
		case 2:  max_value = 0.5f;  break;
		case 3:  max_value = 0.55f;	break;
		case 4:  max_value = 0.6f;	break;
		case 5:  max_value = 0.65f;	break;
		case 6:  max_value = 0.7f;	break;
		case 7:  max_value = 0.75f;	break;
		case 8:  max_value = 0.8f;	break;
		case 9:  max_value = 0.85f;	break;
		case 10: max_value = 9.0f;	break;
	}
	return max_value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////  CMV Defaut /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMV_default ()
{
		/////////////////////// switching inspiratory and expiratory valve /////////////////
		
		Claculate_Flowrate();
		cont_p();
		Blender();
		if (change_value_flag) display();
		if  (valve4_state == 1)     //  if ( valve4_state == HIGH )
		{
			htim3.Instance->CCR1 = 620; //inspiratory
			htim1.Instance->CCR4 = 750; //peep_generator(34); //expiratory
			if ( (millis() - previousMillis) >= 2000)
			{
				valve4_state = 0;          // valve4_state = LOW; change the state of LED // inspiration valve ko off na karna
				valve5_state = 1;          // valve5_state = HIGH;
				previousMillis = millis();
				Plateau = cmH2O_Sensor3;
				IE_monitoring_flag = true;
				Insp_flag = false;
			}
		}
		else
		{
				htim3.Instance->CCR1=0;  //pwmWriteHR(HF_solen_Inspiratory_valve1,    0);  //use this functions instead of analogWrite on 'initialized' pins
				//htim1.Instance->CCR4= peep_generator(PEEP_for_knob); //digitalWrite(Expiration_valve5, valve5_state);
				htim1.Instance->CCR4 = 0;
				if ( (millis() - previousMillis) >= 4000)
				{
					valve4_state = 1;                    //valve4_state = HIGH;
					valve5_state = 0;                    //valve5_state = low;
					PIP_display = (int)PIP_real;
					VT_max_display = VT_max;
					PEEP_cmH2O = cmH2O_Sensor3;
					if (!skip_one_cycle && parameter_value_change)parameter_value_change = false;
					LpM_Max_real_display = ceil(LpM_Max_real);
					PIP_real = 0;
					LpM_Max_real = 0;
					VT = 0;
					VT_max = 0;
					previousMillis = millis();
					RR_monitoring_flag = true;
					Insp_flag = true;
					buffer_knob();
					display();
				}
		}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// CMV_PC //////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMV_PC()
{
		SetOutputLimits(0.0,Upper_limit());
		//SetTunings(0.22,0,0); //kp, ki, kd
		SetTunings(0.0097,0.779,0);
		//if (PIP_cmH2O==12)	SetTunings(0.0197,0.779,0);
		Flow_rate = (Compute(PIP_cmH2O,cmH2O_Sensor3)*60.00);//LPS TO LPM
		Claculate_Flowrate();
		cont_p();
		Blender();
		if (change_value_flag) display();
		if  (valve4_state == 1)
		{
			htim3.Instance->CCR1 = pwm_for_defined_flow_curve;  //Signal for Inspiratory Valve to Open
			htim1.Instance->CCR4=928; 													//Signal for Expiratory Valve to Close
			if ( (millis() - previousMillis) >= Insp_msecs)
			{
				valve4_state = 0;     //End Inspiration
				valve5_state = 1;  //Start Expiration
				Insp_flag = false;
				previousMillis = millis();
				IE_monitoring_flag = true;
			}
			else
			{
				if (cmH2O_Sensor3>=0 && cmH2O_Sensor3<=PIP_cmH2O && LpM_Sensor4>0.5)
				{
					FV_AM[index_counter][0] = LpM_Sensor4/60.0; //flow rate in liter per second
					FV_AM[index_counter][1] = VT/1000.0; 				// volume in literes
					P_AM[index_counter] = cmH2O_Sensor3;
					index_counter++;
				}
			}
		}

		else
		{

			htim3.Instance->CCR1= 500; //Signal for Inspiratory Valve to Close
			htim1.Instance->CCR4= peep_generator(PEEP_for_knob);	 //Signal for Expiratory Valve to Open
			//htim1.Instance->CCR4= 0;
			if ((millis() - previousMillis) >= 150 && LpM_Sensor4 >= trigger_for_buffer) trigger_count++;
			if ((millis() - previousMillis) >= Expr_msecs)
			{
				compute_lung_parameters();
				index_counter = 0;
				ITerm = 0;
				valve4_state = 1; //Start Inspiration
				valve5_state = 0;
				PIP_display = (int)PIP_real;
				VT_max_display = VT_max;
				PEEP_cmH2O = cmH2O_Sensor3;
				if (!skip_one_cycle && parameter_value_change)parameter_value_change = false;
				skip_one_cycle = false;
				PIP_real = 0;
				LpM_Max_real_display = ceil(LpM_Max_real);
				LpM_Max_real = 0;
				VT = 0;
				VT_max = 0;
				Insp_flag = true;
				previousMillis = millis(); // Starting time of Inspiration
				RR_monitoring_flag = true;
				buffer_knob();
				display();
			}
		}//else ends here
		/////////////// Alarms //////////////
		if(!parameter_value_change){
		//alarms_PC();
		//alarms_both();
		}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// CMV_VC //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float vt_time;
float static_compliance = 0.0;
uint16_t static_resistance = 0;
void CMV_VC()
{
		vt_time = Insp_msecs * 0.75f;
		Flow_rate = vt_for_buffer * (60/vt_time);
		if (Flow_rate<8) Flow_rate = 8;
		Claculate_Flowrate();		 // calculating flow rate
		cont_p();
		Blender();
		if (change_value_flag) display();
		if  (valve4_state == 1)
		{
			if ( (millis() - previousMillis) >= Insp_msecs)
			{
				valve4_state = 0;			// End Inspiration
				valve5_state = 1;			// Start Expiration
				Insp_flag = false;
				previousMillis = millis(); // Staring time of Expiration
				IE_monitoring_flag = true;
				valve4_state_for_prop = 500;
				static_compliance = (float)VT_max/((float)cmH2O_Sensor3-PEEP_cmH2O);
				static_resistance = (uint16_t)((PIP_real-cmH2O_Sensor3)/(LpM_Max_real/60.0f));
			}
			if(VT<VT_ml && volume_complete_flag == true)
			{
				valve4_state_for_prop = pwm_for_defined_flow_curve;
			}
			else if (VT >= VT_ml)
			{
				valve4_state_for_prop = 500;
				volume_complete_flag = false;
			}
			if (cmH2O_Sensor3>pip_max_buffer || pressure_exceed_max) {pressure_exceed_max = true; htim1.Instance->CCR4=peep_generator(pip_max_buffer-6);}
			else {htim1.Instance->CCR4 = 900;} //Signal for Expiratory Valve to Close
			htim3.Instance->CCR1 = valve4_state_for_prop; 	  //Signal for Inspiratory Valve to Open
			
			if(!volume_complete_flag && (millis() - previousMillis) >= (0.9*Insp_msecs)) // To calculate Static compliance & resistance before 10% time remaining for inspiration 
			{
				static_compliance = (float)VT/((float)cmH2O_Sensor3-PEEP_cmH2O);
				static_resistance = (ceil)((PIP_display-cmH2O_Sensor3)/(LpM_Max_real/60.0f));
			}
		}
		else
		{
				 htim3.Instance->CCR1= 500;
				 htim1.Instance->CCR4= peep_generator(PEEP_for_knob);
				 //htim1.Instance->CCR4= 0;
				 if ((millis() - previousMillis) >= 150 && LpM_Sensor4 >= trigger_for_buffer) trigger_count++;
				 if ((millis() - previousMillis) >= Expr_msecs)
			 {
					valve4_state = 1;
					valve5_state = 0;
					PIP_display = (int)PIP_real;
					VT_max_display = VT_max;
					if(cmH2O_Sensor3>=0) PEEP_cmH2O = cmH2O_Sensor3; else PEEP_cmH2O = 0;
					if (!skip_one_cycle && parameter_value_change)parameter_value_change = false;
					skip_one_cycle = false;
					PIP_real = 0;
					LpM_Max_real_display = ceil(LpM_Max_real);
					LpM_Max_real = 0;
					VT = 0;
					VT_max = 0;
					Insp_flag = true;
					previousMillis = millis();
					RR_monitoring_flag = true;
					volume_complete_flag = true;
					buffer_knob();
					display();
					pressure_exceed_max = false;
			}
		}

	/////////////// Alarms //////////////
		if(!parameter_value_change){
			alarms_VC();
		//alarms_both();
		}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////(9) AC_PC //////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double test_trigger = 0;
void AC_PC ()
{
		SetOutputLimits(0.0,Upper_limit());
		//SetTunings(0.22,0,0); //kp, ki, kd
		SetTunings(0.0097,0.779,0);
		Flow_rate = (Compute(PIP_cmH2O,cmH2O_Sensor3)*60.00);//LPS TO LPM
		Claculate_Flowrate();
		cont_p();
		Blender();
		if (change_value_flag) display();
 ///////////////////////controlling container pressure/////////////////

	if ( valve4_state == 1)
	{
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// AC Control Paramters ////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (Insp_start_time_flag == true)
			{
				Insp_start_time = millis();  // Starting time of inspiration
				Insp_start_time_flag = false;
			}
		  if ( (millis() - previousMillis) >= Insp_msecs)
			{
			valve4_state = 0;
			valve5_state = 1;
			Insp_flag = false;
			previousMillis = millis();
			Plateau = cmH2O_Sensor3;
			IE_monitoring_flag = true;

			/////////////////////// AC Control Paramters//////////////////////////////////////////////////////

			Insp_start_time_flag = true;
			Flowrate_zero_time_flag = true;
			Exp_start_time = millis();
			}

			htim3.Instance->CCR1 = pwm_for_defined_flow_curve;
			htim1.Instance->CCR4=928;
	}
	else
	{

			htim3.Instance->CCR1= 500; //pwmWriteHR(HF_solen_Inspiratory_valve1,    0);  //use this functions instead of analogWrite on 'initialized' pins
			htim1.Instance->CCR4= peep_generator(PEEP_for_knob);
			//htim1.Instance->CCR4= 0;	//expiratory valve controlling peep


		if ((millis() - Exp_start_time) > 150 && LpM_Sensor4_for_trigger<=1 && LpM_Sensor4_for_trigger >=-1 && Flowrate_zero_time_flag == true)
		{
			Flowrate_zero_time = millis();
			Flowrate_zero_time_flag = false;
			Breath_time_wo_exp_pause = (Flowrate_zero_time - Insp_start_time) / 1000.0f;
			Expiratory_pause = TCT_monitoring - Breath_time_wo_exp_pause; // TCT= Breath_time_wo_exp_pause + exp_pause ==> exp_pause =  TCT - Breath_time_wo_exp_pause
			if (Supported_window_flag == true)
			{
				previousMillis = Insp_Exp_centre_millis;
				Supported_window_flag = false;
			}
		}
		if( LpM_Sensor4 >= trigger_for_buffer)  // Patient's making effort and trying to take a breath //if (cmH2O_Sensor3 < -1.5 ) // Pressure_trigger_dec
		{
			trigger_count++;
			trigger_taken = LpM_Sensor4;
			Trigger_time_absolute = millis();
			Trigger_time = (Trigger_time_absolute - Insp_start_time) / 1000.0f;
			Time_after_trigger = TCT_monitoring - Trigger_time;

			if (Time_after_trigger <= Breath_time_wo_exp_pause) // Synchornized breathing
			{
				previousMillis = Trigger_time_absolute - Expr_msecs;
			}
			else //else if (Time_after_trigger > Breath_time_wo_exp_pause || LpM_Sensor4 >= trigger_for_buffer) // Supportted breathing
			{
				Insp_Exp_centre_millis = previousMillis;
				previousMillis = Trigger_time_absolute - Expr_msecs;
				Supported_window_flag = true;
			}
		 }
		if ( (millis() - previousMillis) >= Expr_msecs)
			{
				ITerm = 0;
				valve4_state = 1;
				valve5_state = 0;
				PIP_display = (int)PIP_real;
				VT_max_display = VT_max;
				PEEP_cmH2O = cmH2O_Sensor3;
				if (!skip_one_cycle && parameter_value_change)parameter_value_change = false;
				skip_one_cycle = false;
				PIP_real = 0;
				LpM_Max_real_display = ceil(LpM_Max_real);
				LpM_Max_real = 0;
				VT = 0;
				VT_max = 0;
				Insp_flag = true;
				previousMillis = millis();
				RR_monitoring_flag = true;
				buffer_knob();
				display();
			}
	}
	/////////////// Alarms //////////////
	if(!parameter_value_change){
		//alarms_PC();
		//alarms_both();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////// (10) AC_VC /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AC_VC ()
{
		vt_time = Insp_msecs * 0.75f;
		Flow_rate = vt_for_buffer * 60 / vt_time;
		if(Flow_rate<8) Flow_rate = 8;
		Claculate_Flowrate();
		cont_p();
		Blender();
		if (change_value_flag) display();
		if ( valve4_state == 1)
		{
			/////////////////////////////////AC Control Paramters/////////////////////////////

			if (Insp_start_time_flag == true)
			{
				Insp_start_time = millis();
				Insp_start_time_flag = false;
			}

			if ( (millis() - previousMillis) >= Insp_msecs)
			{
				valve4_state = 0;
				valve5_state = 1;
				Insp_flag = false;
				previousMillis = millis();
				IE_monitoring_flag = true;
				valve4_state_for_prop = 500;

				/////////////////////// AC Control Paramters///////////////////////////////////

				Insp_start_time_flag = true;
				Flowrate_zero_time_flag = true;
				Plateau = cmH2O_Sensor3;
				Exp_start_time = millis();
		 }
			if(VT<VT_ml && volume_complete_flag == true)
			{
				valve4_state_for_prop = pwm_for_defined_flow_curve;
			}

			else if (VT >= VT_ml)
			{
				valve4_state_for_prop = 500;
				volume_complete_flag = false;
			}
			if (cmH2O_Sensor3>pip_max_buffer || pressure_exceed_max) {pressure_exceed_max = true; htim1.Instance->CCR4=peep_generator(pip_max_buffer-6);}
			else 									{htim1.Instance->CCR4 = 900;} //Signal for Expiratory Valve to Close
			htim3.Instance->CCR1 = valve4_state_for_prop;
			
			if(!volume_complete_flag && (millis() - previousMillis) >= (0.9*Insp_msecs)) // To calculate Static compliance & resistance before 10% time remaining for inspiration 
			{
				static_compliance = (float)VT/((float)cmH2O_Sensor3-PEEP_cmH2O);
				static_resistance = (uint16_t)((PIP_display-cmH2O_Sensor3)/(LpM_Max_real/60.0f));
			}
		}
		else
		{
			 htim3.Instance->CCR1= 500;// insp valve
			 htim1.Instance->CCR4= peep_generator(PEEP_for_knob);;// exp valve
			 //htim1.Instance->CCR4= 0;


			if ((millis() - Exp_start_time) > 150 && LpM_Sensor4_for_trigger <=1 && LpM_Sensor4_for_trigger >=-1 && Flowrate_zero_time_flag == true)
			{
				Flowrate_zero_time = millis();
				Flowrate_zero_time_flag = false;
				Breath_time_wo_exp_pause = (Flowrate_zero_time - Insp_start_time) / 1000.0f;
				Expiratory_pause = TCT_monitoring - Breath_time_wo_exp_pause; // TCT= Breath_time_wo_exp_pause + exp_pause ==> exp_pause =  TCT - Breath_time_wo_exp_pause
				if (Supported_window_flag == true)
				{
					previousMillis = Insp_Exp_centre_millis;
					Supported_window_flag = false;
				}
			}
			if( LpM_Sensor4 >= trigger_for_buffer)  // Patient's making effort and trying to take a breath //if (cmH2O_Sensor3 < -1.5 ) // Pressure_trigger_dec
			{
				trigger_count++;
				trigger_taken = LpM_Sensor4;
				Trigger_time_absolute = millis();
				Trigger_time = (Trigger_time_absolute - Insp_start_time) / 1000.0f;
				Time_after_trigger = TCT_monitoring - Trigger_time;

				if (Time_after_trigger <= Breath_time_wo_exp_pause)//Synchornized breathing
				{
					previousMillis = Trigger_time_absolute - Expr_msecs;
				}
				if (Time_after_trigger > Breath_time_wo_exp_pause) //Supported breathing
				{
					Insp_Exp_centre_millis = previousMillis;
					previousMillis = Trigger_time_absolute - Expr_msecs;
					Supported_window_flag = true;
				}
			 }
			if ( (millis() - previousMillis) >= Expr_msecs)
					{
						valve4_state = 1;
						valve5_state = 0;
						PIP_display = (int)PIP_real;
						VT_max_display = VT_max;
						PEEP_cmH2O = cmH2O_Sensor3;
						if (!skip_one_cycle && parameter_value_change)parameter_value_change = false;
						skip_one_cycle = false;
						PIP_real = 0;
						LpM_Max_real_display = ceil(LpM_Max_real);
						LpM_Max_real = 0;
						VT = 0;
						VT_max = 0;
						Insp_flag = true;
						previousMillis = millis();
						RR_monitoring_flag = true;
						buffer_knob();
						display();
						volume_complete_flag = true;
						pressure_exceed_max = false;
					}
		}
	/////////////// Alarms //////////////
		if(!parameter_value_change){
		//alarms_VC();
		//alarms_both();
		}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////(10) PS_FOR_SIMV ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PS_FOR_SIMV()
{
	if (flag_to_detect_PS_SIMV_trigger == true)
	{
		valve4_state = 1;
		valve5_state = 0;
		Insp_start_time_SIMV_trigger = millis();
		Expr_msecs_PS_SIMV = 1000000000000000000;
		Insp_msecs_PS_SIMV = 100000000;
		flag_to_detect_PS_SIMV_trigger = false;
	}

		SetOutputLimits(0.0,Upper_limit());
		//SetTunings(0.22,0,0); //kp, ki, kd
		SetTunings(0.0097,0.779,0);
		Flow_rate = (Compute(PIP_cmH2O,cmH2O_Sensor3)*60.00);//LPS TO LPM
		Claculate_Flowrate();
		cont_p();
	if ( valve4_state == 1)
	{
		htim3.Instance->CCR1 = pwm_for_defined_flow_curve;
		htim1.Instance->CCR4=928;
		previousMillis = millis() - 1.0;
		pressure_support_measured = cmH2O_Sensor3;
		if (LpM_Sensor4_for_trigger <=1 && LpM_Sensor4_for_trigger >-1  && cmH2O_Sensor3 > Pressure_support+2)
		{
			previousMillis = Insp_msecs_PS_SIMV;
		}
		if ( previousMillis == Insp_msecs_PS_SIMV)
		{
			valve4_state = 0;
			Insp_flag = false;
			IE_monitoring_flag = true;
			htim3.Instance->CCR1=0;
			/////////////////////////////// AC Control Paramters ///////////////////////////////////////////////
			Insp_start_time_flag = true;
			Flowrate_zero_time_flag_SIMV_trigger = true;
			previousMillis= millis();
		}
	}

	else
	 {
				htim3.Instance->CCR1=500;
				if((millis() - previousMillis) >=5500)
				{
					valve5_state = 1;
					Exp_start_time_SIMV_trigger = millis();
					htim1.Instance->CCR4 = 0;
				}

				if (cmH2O_Sensor3 >= Pressure_support+3 )
				{
					valve5_state = 1;
					Exp_start_time_SIMV_trigger = millis();
					htim1.Instance->CCR4 = 0;
				}

				if ( LpM_Sensor4_for_trigger <=1 && LpM_Sensor4_for_trigger >-1 && Flowrate_zero_time_flag_SIMV_trigger == true && valve5_state == 1)
				{
					Flowrate_zero_time_SIMV_trigger = millis();
					previousMillis = Flowrate_zero_time_SIMV_trigger - Expr_time_wo_pause;
					Flowrate_zero_time_flag_SIMV_trigger = false;
					Breath_time_wo_exp_pause = (Flowrate_zero_time - Insp_start_time) / 1000.0f;
					Expiratory_pause = TCT_monitoring - Breath_time_wo_exp_pause;
					PS_for_SIMV_flag = false;
					PEEP_cmH2O = 0;
					buffer_knob();
					display();
				}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////(11) SIMV_PC ////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SIMV_PC ()
{
	if (change_value_flag) display();
	if (PS_for_SIMV_flag == true)
	{
		PS_FOR_SIMV();
	}
	else
	{
		SetOutputLimits(0.0,Upper_limit());
		//SetTunings(0.22,0,0); //kp, ki, kd
		SetTunings(0.0097,0.779,0);
		Flow_rate = (Compute(PIP_cmH2O,cmH2O_Sensor3)*60.00);//LPS TO LPM
		Claculate_Flowrate();
		cont_p();
		Blender();

	if ( valve4_state == 1)
	{
		////////////// AC Control Paramters/////////////////////////////////////

		if (Insp_start_time_flag == true)
		{
			Insp_start_time = millis();
			Insp_start_time_flag = false;
		}
		  if ( (millis() - previousMillis) >= Insp_msecs)
			{
			valve4_state = 0;
			valve5_state = 1;
			Insp_flag = false;
			previousMillis = millis();
			Plateau = cmH2O_Sensor3;
			IE_monitoring_flag = true;
			Insp_start_time_flag = true;
			Flowrate_zero_time_flag = true;
			Exp_start_time = millis();
			}
			htim3.Instance->CCR1 = pwm_for_defined_flow_curve;
			htim1.Instance->CCR4=928;
	}

	else
	{
					htim3.Instance->CCR1=500;
					htim1.Instance->CCR4= peep_generator(PEEP_for_knob);
					//htim1.Instance->CCR4 = 0;

					if ((millis() - Exp_start_time) > 150 && LpM_Sensor4_for_trigger <=0.2 && LpM_Sensor4_for_trigger >=-0.2 && Flowrate_zero_time_flag == true)
					{
						Flowrate_zero_time = millis();
						Flowrate_zero_time_flag = false;
						Breath_time_wo_exp_pause = (Flowrate_zero_time - Insp_start_time) / 1000.0f;
						Expiratory_pause = TCT_monitoring - Breath_time_wo_exp_pause;
						Expr_time_wo_pause = Flowrate_zero_time - Exp_start_time;
					}
					if( LpM_Sensor4 >= trigger_for_buffer)  // Patient's making effort and trying to take a breath //if (cmH2O_Sensor3 < -1.5 ) // Pressure_trigger_dec
					{
						trigger_count++;
						trigger_taken = LpM_Sensor4;
						Trigger_time_absolute = millis();
						Trigger_time = (Trigger_time_absolute - Insp_start_time) / 1000.0f; // trigger time w. r. to inspiration
						Time_after_trigger = TCT_monitoring - Trigger_time;

						if (Time_after_trigger <= Breath_time_wo_exp_pause) //Synchornized breathing
						{
							previousMillis = Trigger_time_absolute - Expr_msecs;
						}
						if (Time_after_trigger > Breath_time_wo_exp_pause) //Supported breathing
						{
							flag_to_detect_PS_SIMV_trigger = true;
							PS_for_SIMV_flag = true;
						}
					}

					if ( (millis() - previousMillis) >= Expr_msecs)
					{
						ITerm = 0;
						valve4_state = 1;
						valve5_state = 0;
						PIP_display = (int)PIP_real;
						VT_max_display = VT_max;
						PEEP_cmH2O = cmH2O_Sensor3;
						if (!skip_one_cycle && parameter_value_change)parameter_value_change = false;
						skip_one_cycle = false;
						PIP_real = 0;
						LpM_Max_real_display = ceil(LpM_Max_real);
						LpM_Max_real = 0;
						VT = 0;
						VT_max = 0;
						Insp_flag = true;
						previousMillis = millis();
						RR_monitoring_flag = true;
						buffer_knob();
						display();
						}
		}
	}
	/////////////// Alarms //////////////
	if(!parameter_value_change){
		//alarms_PC();
		//alarms_both();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////(12) SIMV_VC ////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SIMV_VC ()
{
	 if (change_value_flag) display();
	 if (PS_for_SIMV_flag == true)
		{
			PS_FOR_SIMV();
		}

	else
		{
			vt_time = Insp_msecs * 0.75f;
			Flow_rate = vt_for_buffer * 60 / vt_time;
			if (Flow_rate<8) Flow_rate = 8;
			Claculate_Flowrate(); //Calculating Flow Rate
			cont_p();
			Blender();

			if ( valve4_state == 1)
			{
					if (Insp_start_time_flag == true)
					{
						Insp_start_time = millis();
						Insp_start_time_flag = false;
					}

					if ( (millis() - previousMillis) >= Insp_msecs)
					{
						valve4_state = 0;
						valve5_state = 1;
						Insp_flag = false;
						previousMillis = millis();
						IE_monitoring_flag = true;

						/////////////////////// AC Control Paramters//////////////////////////

						Insp_start_time_flag = true;
						Flowrate_zero_time_flag = true;
						Plateau = cmH2O_Sensor3;
						Exp_start_time = millis();
				 }
					if(VT<VT_ml && volume_complete_flag == true)
					{
						valve4_state_for_prop = pwm_for_defined_flow_curve;
					}

					else if (VT >= VT_ml)
					{
						valve4_state_for_prop = 500;
						volume_complete_flag = false;
					}
					if (cmH2O_Sensor3>pip_max_buffer || pressure_exceed_max) {pressure_exceed_max = true; htim1.Instance->CCR4=peep_generator(pip_max_buffer-6);}
					else 									{htim1.Instance->CCR4 = 900;} //Signal for Expiratory Valve to Close
					htim3.Instance->CCR1 = valve4_state_for_prop;
					
					if(!volume_complete_flag && (millis() - previousMillis) >= (0.9*Insp_msecs)) // To calculate Static compliance & resistance before 10% time remaining for inspiration 
					{
						static_compliance = (float)VT/((float)cmH2O_Sensor3-PEEP_cmH2O);
						static_resistance = (uint16_t)((PIP_display-cmH2O_Sensor3)/(LpM_Max_real/60.0f));
					}
			}
			else
			{
				 htim3.Instance->CCR1= 500;	// insp valve
				 htim1.Instance->CCR4= peep_generator(PEEP_for_knob);		// exp valve

					if ((millis() - Exp_start_time) > 150 && LpM_Sensor4_for_trigger <=1 && LpM_Sensor4_for_trigger >=-1 && Flowrate_zero_time_flag == true)
						{
							Flowrate_zero_time = millis();
							Flowrate_zero_time_flag = false;
							Breath_time_wo_exp_pause = (Flowrate_zero_time - Insp_start_time) / 1000.0f;
							Expiratory_pause = TCT_monitoring - Breath_time_wo_exp_pause;
							Expr_time_wo_pause = Flowrate_zero_time - Exp_start_time;
						}
					if( LpM_Sensor4 >= trigger_for_buffer)  // Patient's making effort and trying to take a breath //if (cmH2O_Sensor3 < -1.5 ) // Pressure_trigger_dec
					{
						trigger_count++;
						trigger_taken = LpM_Sensor4;
						Trigger_time_absolute = millis();
						Trigger_time = (Trigger_time_absolute - Insp_start_time) / 1000.0f; // trigger time w. r. to inspiration
						Time_after_trigger = TCT_monitoring - Trigger_time;

						if (Time_after_trigger <= Breath_time_wo_exp_pause)//Synchornized breathing
						{
							previousMillis = Trigger_time_absolute - Expr_msecs;
						}
						if (Time_after_trigger > Breath_time_wo_exp_pause) //Supported breathing
						{
							flag_to_detect_PS_SIMV_trigger = true;
							PS_for_SIMV_flag = true;
						}
					}

					if ( (millis() - previousMillis) >= Expr_msecs)
					{
						valve4_state = 1;
						valve5_state = 0;
						PIP_display = (int)PIP_real;
						VT_max_display = VT_max;
						PEEP_cmH2O = cmH2O_Sensor3;
						if (!skip_one_cycle && parameter_value_change)parameter_value_change = false;
						skip_one_cycle = false;
						PIP_real = 0;
						LpM_Max_real_display = ceil(LpM_Max_real);
						LpM_Max_real = 0;
						VT = 0;
						VT_max = 0;
						Insp_flag = true;
						previousMillis = millis();
						RR_monitoring_flag = true;
						buffer_knob();
						display();
						volume_complete_flag = true;
						pressure_exceed_max = false;
					}
			}
	}
	/////////////// Alarms //////////////
		if(!parameter_value_change){
		//alarms_VC();
		//alarms_both();
		}
}


void PS()
{
//			peep_set = PEEP_for_buffer;
//			if (count == 0)
//			{
//				valve4_state = 0;
//				Exp_start_time = millis();
//				Flowrate_zero_time_flag = true;
//				previousMillis = millis();
//				Expr_msecs = 100000;
//				Insp_msecs = 100000;
//			}
//			count++;

//			htim3.Instance->CCR1= 0;
//			if (cmH2O_Sensor2<= 2000)
//			{
//				htim2.Instance->CCR1 = 1000;
//			}
//			else
//			{
//				htim2.Instance->CCR1 = 0;
//			}
//
//			if  (valve4_state == 1) //&& (Insp_flag == true))//valve khula rahay for inspiration //
//			{
//				xy=0;
//				///////////////// AC Control Paramters/////////////////////////
//
//				if (Insp_start_time_flag == true) // to note the time at the start of inspiration
//				{
//					Insp_start_time = millis();// save the exact time at the start of inspiration
//					Insp_start_time_flag = false;// avoid update of insp start time till the next inspiration
//				}
//
//				/////////////////////////////////////////////
//				//////////////PID////////////////////////////
//				/////////////////////////////////////////////
//
//				htim3.Instance->CCR1 = ((1-(0.30f*cmH2O_Sensor3/Pressure_support))*777);
//				htim1.Instance->CCR4 = 923; 		//expiratory valve full closed

//				previousMillis = millis() - 1.0;
//				if (LpM_Sensor4_for_trigger <=1 && LpM_Sensor4_for_trigger >-1&& cmH2O_Sensor3>= Pressure_support)
//				{
//				 y=1;
//					xy=1;
//				}
//				if ( z==y)
//					{
//					valve4_state = 0; // Inspiration ka valve ko off na karna
//					Insp_flag = false;
//					previousMillis = millis(); // remember Current millis() time
//					IE_monitoring_flag = true;
//					htim3.Instance->CCR1= 0;
//
//					/////////////////////// AC Control Paramters////////////////////////////////////////////
//
//					Insp_start_time_flag = true;
//					Flowrate_zero_time_flag = true;
//					y=0;
//					xy=2;
//				}
//			}

//			else
//			{
//						xy=5;
//						if (cmH2O_Sensor3 >= Pressure_support + 3)
//						{
//							valve5_state = 1;// valve khul jaey for expiration
//							Exp_start_time = millis();// saving the value of expr start time so that flowrate=0 not to be noted soon after the end of insp & start of exp.
//							htim1.Instance->CCR4= 0;
//						}
//
//						if (cmH2O_Sensor3 <= -1.5)
//						{
//								y=1;
//								xy=3;
//						}
//
//						if  (z==y)
//						{
//							valve4_state = 1;
//							valve5_state = 0;
//							PIP_display = (int)PIP_real;
//							PIP_real = 0;
//							LpM_Max_real_display = ceil(LpM_Max_real);
//							LpM_Max_real = 0;
//							VT = 0;
//							VT_max_display = VT_max;
//							VT_max = 0;
//							Insp_flag = true;
//							previousMillis = millis();
//							RR_monitoring_flag = true;
//							y=0;
//							xy=4;
//							buffer_knob();
//						}
//			}

}


void CPAP()
{
//			buffer_knob();
//			cont_p();
//			z=600+(peep_set/cmH2O_Sensor3)*10;
//			htim3.Instance->CCR1=z;
}
void BIPAP()
{
//	buffer_knob();
//	cont_p();
//
//		if(cmH2O_Sensor3<= (ipap) && Insp_flag == true)
//			{
//				z=123;
//			}
//	  if(cmH2O_Sensor3>(ipap))
//			{
//				Insp_flag = false;
//			}
//
//		if(Insp_flag == false)
//			{
//				z=234;
//				htim1.Instance->CCR4=(epap-5)*10;
//				htim3.Instance->CCR1=600;
//
//			if(cmH2O_Sensor3<(epap-1))
//			{
//				Insp_flag = true;
//			}
//	}
}
/* USER CODE END 0 */
/**
	* @brief  The application entry point.
	* @retval int
	*/



	
	
	
	int pwm_increment = 0;
	int reuired_encoder_for_mean = 0;
	int speed = 0;
	bool start_check = true;
	int testt = 0;
	uint16_t Last_encoder_Value=0;

int main(void)
{
	/* USER CODE BEGIN 1 */
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */

		HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
		SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
		MX_GPIO_Init();
		MX_DMA_Init();
		MX_ADC1_Init();
		MX_TIM1_Init();
		MX_TIM2_Init();
		MX_TIM3_Init();
		MX_TIM4_Init();
		MX_TIM5_Init();
		MX_TIM8_Init();
		MX_TIM10_Init();
		MX_TIM11_Init();
		MX_TIM14_Init();
		MX_USART1_UART_Init();
		MX_I2C3_Init();
		/* USER CODE BEGIN 2 */
/////////////////function init////////////////////////////////////
		HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1); // pwm intialization of channels
		HAL_TIM_PWM_Start(&htim11, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
		HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // pwm intialization of channels
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
		HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	//HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

		HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
		HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);

		HAL_UART_Receive_DMA(&huart1,(uint8_t*)rxbuf_nextion,1);
		HAL_ADC_Start_DMA(&hadc1,(uint32_t*)FB_volt,2);
	//NEXTION_SendInteger("n4",100);
/////////////////////////////////////////////////////
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	//////////////////////////////////user code////////
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);		//Motor Driver IN1
		HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_RESET);	// Motor Driver IN2
		htim10.Instance->CCR1=0;
		htim10.Instance->CCR1=0;
		htim3.Instance->CCR1=840;//digitalWrite(Inspiration_valve4, LOW); // turn the LED ON or OFF
		htim1.Instance->CCR4=0; //digitalWrite(Expiration_valve5, valve5_state); // turn the LED ON or OFF
		htim2.Instance->CCR1=0;		//digitalWrite(CONTAINER_valve5, valve5_state); // turn the LED ON or OFF
		htim2.Instance->CCR2=0;	// Container Pressure release valve is set to zero
		htim1.Instance->CCR4=0;
		htim3.Instance->CCR2=0; ///Motor Driver PWM
		
		//while(1){display_graph(),display();}
		//testt = Flash_Read(0x080E0000);
			
//			TIM4->CNT = 0;
//			current_encoder = TIM4->CNT;
//			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
//			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
//      htim3.Instance->CCR2 = 300;
//			while(current_encoder<=60)current_encoder = TIM4->CNT;
//			htim3.Instance->CCR2 = 0;
//			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
//			while(1)current_encoder = TIM4->CNT;
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////// To Return The Motor of Blender to its mean position //////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
//			TIM4->CNT = 0;
//			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
//			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
//      htim3.Instance->CCR2 = 210;
//			while (current_encoder<278)	current_encoder = TIM4->CNT;
//			htim3.Instance->CCR2 = 0;
//			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
//			Flash_Write(0x080E0000,current_encoder);
//			while(1);
//			Last_encoder_Value = Flash_Read(0x080E0000);
//			while(1);
//			TIM4->CNT = 0;
//			current_encoder = TIM4->CNT;
//			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
//			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
//      htim3.Instance->CCR2 = 300;
//			while(current_encoder<=222)current_encoder = TIM4->CNT;
//			htim3.Instance->CCR2 = 0;
//			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
//			Flash_Write(0x080E0000,current_encoder);
//			HAL_Delay(1000);
*/			
///////////////////////////////////////// By accessing The EEPROM //////////////////////////////////////////////
			
			Last_encoder_Value = Flash_Read(0x080E0000);
			TIM4->CNT = Last_encoder_Value;
			current_encoder = TIM4->CNT;
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_SET);
      htim3.Instance->CCR2 = 300;
			while(current_encoder>20)current_encoder = TIM4->CNT;
			htim3.Instance->CCR2 = 0;
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_SET);
			previousMillis = millis();
      htim3.Instance->CCR2 = 150;
     	while((millis()-previousMillis)<300 && Last_encoder_Value>0);
			htim3.Instance->CCR2 = 0;
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
			TIM4->CNT = 0;
			current_encoder = TIM4->CNT;
			Flash_Write(0x080E0000,current_encoder);
			
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*///////////////////////////////////////// Logic Base ///////////////////////////////////////////////////////////
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
//		previousMillis = millis();
//		int motor_for_temp = millis()-previousMillis;
//
//		while (motor_for_temp<55) { motor_for_temp = millis()-previousMillis; htim3.Instance->CCR2 = 300;}
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
//		htim3.Instance->CCR2 = 0;
//		HAL_Delay(1000);
//
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
//		previous_encoder = 1000;
//		TIM4->CNT = 0;
//		current_encoder = TIM4->CNT;
//
//		while (current_encoder < 40)
//		{
//			current_encoder = TIM4->CNT;
//			htim3.Instance->CCR2 = 95;
//			HAL_Delay(100);
//			if (current_encoder==previous_encoder) {start_check = false; break;}
//			previous_encoder = current_encoder;
//		}
//
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
//		htim3.Instance->CCR2 = 0;
//		HAL_Delay(200);

//		if (!start_check)
//		{
//			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_SET);
//			TIM4->CNT = 65535;
//			current_encoder = 65535 - TIM4->CNT;
//			while (current_encoder<50) // We can predict the position of blender motor by calculating the pwm reuired to move the motor.
//			{
//				current_encoder = 65535 - TIM4->CNT;
//				htim3.Instance->CCR2 = pwm_for_blender_motor + pwm_increment;
//				HAL_Delay(25);
//				if (current_encoder==previous_encoder) pwm_increment++;
//				previous_encoder = current_encoder;
//			}
//
//			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
//			htim3.Instance->CCR2 = 0;
//			pwm_for_blender_motor += pwm_increment;
//
//			if (pwm_for_blender_motor<=110)
//			{
//				if(pwm_for_blender_motor<=95) blender_temp = 60;
//				else blender_temp = 160;
//			}
//			else blender_temp = (-0.00002*pow(pwm_for_blender_motor,3))+(0.0058*pow(pwm_for_blender_motor,2))+(1.2256*pwm_for_blender_motor)+41.348; // Calcultaing the required encoder value to get the knob to its mean position
//
//			reuired_encoder_for_mean = blender_temp;
//			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_SET);
//
//			while(true)
//			{
//				current_encoder = 65535 - TIM4->CNT;
//				if (current_encoder<(reuired_encoder_for_mean-40))
//				{
//					blender_temp = 0.019*(reuired_encoder_for_mean-current_encoder) + 1.0;
//					current_blender_position = ceil(blender_temp);
//					switch(current_blender_position)
//					{
//						case 2: pwm_for_blender_motor = 95;  break; //To move motor from 21%  to  30%
//						case 3:	pwm_for_blender_motor = 95;  break; //To move motor from 30%  to  40%
//						case 4:	pwm_for_blender_motor = 105; break; //To move motor from 40%  to  50%
//						case 5:	pwm_for_blender_motor = 110; break; //To move motor from 50%  to  60%
//						case 6:	pwm_for_blender_motor = 140; break; //To move motor from 60%  to  70%
//						case 7:	pwm_for_blender_motor = 200; break; //To move motor from 70%  to  80%
//						case 8:	pwm_for_blender_motor = 240; break; //To move motor from 80%  to  90%
//						case 9: pwm_for_blender_motor = 300; break; //To move motor from 90%  to 100%
//					}
//					htim3.Instance->CCR2 = pwm_for_blender_motor;
//				}
//				else break;
//			}
//		}
//
//		else
//		{
//			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_SET);
//			previous_encoder = 1000;
//			current_encoder = TIM4->CNT;
//			while (current_encoder > 10)
//			{
//				current_encoder = TIM4->CNT;
//				htim3.Instance->CCR2 = 95;
//				HAL_Delay(25);
//				if (current_encoder==previous_encoder) break;
//				previous_encoder = current_encoder;
//			}
//		}
//
//		previousMillis = millis();
//		TIM4->CNT = 65535;
//		previous_encoder = 1000;
//		motor_for_temp = millis()-previousMillis;
//
//		while (motor_for_temp <= 2000)
//		{
//			motor_for_temp = millis()-previousMillis;
//			current_encoder = 65535 - TIM4->CNT;
//			htim3.Instance->CCR2 = 95;
//			HAL_Delay(100);
//			if (current_encoder==previous_encoder) break;
//			previous_encoder = current_encoder;
//		}
//
//		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_14,GPIO_PIN_RESET);
//		htim3.Instance->CCR2 = 0;
//		TIM4->CNT=0;
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
		
		HAL_Delay(1000);
		NEXTION_SendString("t9","VT");
		NEXTION_PageChange(0);
		uint16_t i=0;
		read_flow_sensor();
		while(i<100)
		{
			//results_1 = ads2_readADC_SingleEnded_3();     //oxygen sensor being used here
			results_2 =  ads_readADC_SingleEnded_0();     //6-pin mpx5100  container pressure sensor being used here
			if (results_2 > 60000)
			{
			results_2 =0;
			}
			results_4 =  ads_read_Sensirion ();  //4-pin mpx2100 flowrate sensor for flowrate sensor being used here
			results_3 =  ads_readADC_SingleEnded_1 ();   //4-pin mpx2050 lung pressure being used here
			//results_5 = ads_readADC_SingleEnded_1();

			//results_1_calib += results_1;
			results_2_calib += results_2;
			results_3_calib += results_3;
			results_4_calib += results_4;
			//results_5_calib += results_5;
			++i;
			HAL_Delay(20);
		}

	//results_1_calib = results_1_calib/10.0;
		results_2_calib = results_2_calib/i;
		results_3_calib = results_3_calib/i;
	//results_5_calib = results_5_calib/10.0;

		results_4_calib = results_4_calib/i;

		htim3.Instance->CCR1=0;
		htim10.Instance->CCR1=0;
	  cmH2O_Sensor2=0;
		display();
		display_graph();
		
	while(1)
	{
		read_oxygen();
		read_lung_Pressure_sensor();
		read_flow_sensor();
		knob();
		display_graph();
		display();
		cont_p();
		if (cmH2O_Sensor2>2500) cmH2O_Sensor2 = 0;
		else if(cmH2O_Sensor2>2000)break;
	}
	
///////////////////////////////////////////////////////////////////
///////////////// getting patient details//////////////////////////
///////////////////////////////////////////////////////////////////

/*	while(vent_start == false)
	{
		knob();
		NEXTION_SendInteger("n8",age_for_knob);
		NEXTION_SendInteger("n9",weight_for_knob);
	}
*/

	buffer_knob();
	display_graph();
	display();

	while (true)
	{

		///////////////////////////////////////////////////////////////////
		//////////////////////// calling funtions//////////////////////////
		///////////////////////////////////////////////////////////////////

		RR_IE_VE_Monitoring();
		knob();
		if (display_appear && change_mode_variable!= previous_change_mode_touch_variable){trigger_count=0; display_appear = false; display(); alarm_beep(0);NEXTION_Text_Background("t12",0);NEXTION_SendString("t12"," ");}
		if (current_page!=Previous_page) display();
		Previous_page = current_page;
		display_graph();
		read_oxygen();
		sample_time = millis() - time_last_taken;
		time_last_taken = millis();

	///////////////////////////////////////////////////////////////////
	//////////////////////// changing modes ///////////////////////////
	///////////////////////////////////////////////////////////////////
		switch(change_mode_touch_variable_buffer)
		{
			case 0:	CMV_default(); NEXTION_SendString("t0","DEFAULT"); NEXTION_SendString("t9","VT");  break;
			//case 1:	CMV_PC();			 NEXTION_SendString("t0","CMV_PC");  NEXTION_SendString("t9","PIP"); break;
			case 2:	CMV_VC();			 NEXTION_SendString("t0","CMV_VC");	 NEXTION_SendString("t9","VT");  break;
			//case 3: AC_PC();   		 NEXTION_SendString("t0","AC_PC");	 NEXTION_SendString("t9","PIP"); break;
			//case 4: AC_VC();			 NEXTION_SendString("t0","AC_VC");	 NEXTION_SendString("t9","VT");  break;
			//case 5: SIMV_PC(); 		 NEXTION_SendString("t0","SIMV_PC"); NEXTION_SendString("t9","PIP"); break;
			//case 6: SIMV_VC();		 NEXTION_SendString("t0","SIMV_VC"); NEXTION_SendString("t9","VT");  break;
			case 7: CPAP();				 NEXTION_SendString("t0","CPAP");    break;
			case 8: BIPAP();			 NEXTION_SendString("t0","BIPAP");	 break;
			case 9: PS();					 NEXTION_SendString("t0","P S");		 break;
		}
		//display();

		//////////////////////////////////////////////////
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		////////////////////////////main logic////////////////

		///////////////////////////////////////////////////////
	}
		/* USER CODE END 3 */
}
/**
	* @brief System Clock Configuration
	* @retval None
	*/
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	*/
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
															|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
	* @brief  This function is executed in case of error occurrence.
	* @retval None
	*/
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
	* @brief  Reports the name of the source file and the source line number
	*         where the assert_param error has occurred.
	* @param  file: pointer to the source file name
	* @param  line: assert_param error line source number
	* @retval None
	*/
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
		 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
