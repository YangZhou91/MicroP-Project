  /*******************************************************************
  * @file    SevenSegLED.h
  * @author  Tashreef Anowar, Yang Zhou
  * @date    17-February-2014
  * @brief   This file provides functions to manage the following 
  *          functionalities:
  *           - Initialization and Configuration of LED GPIOs
  *           - Display floating point number on 7 segment
  *           - Display temperature
  *           - Turn PWM LED on/off
  *           - Trigger Alarm through PWM mechanism
  *           - Delay countdown counter
	********************************************************************/

/* 7 Segment Select line */
#define SEGMENT_1 3
#define SEGMENT_2 2
#define SEGMENT_3 1

#define SEGMENT_DELAY 0x430EB    //delay between corresponding segments

/*
 * struct SET_ALARM
 * ----------------------------
 * Alarms configuration for PWM
 *
 *   int Period - Period
 *	 int DutyCycle - Duty cycle
 *	 int dutyCycleAdjust - decrements or increments duty cycle by this value
 *	 int incr_Flag - increment from low dutycycle to high 
 *	 int decr_Flag - decrements from high dutycycle to low 
 *	 int count - number of times duty clycle increases or decreases 
 *	 int width - increase/reduce duty cycle by this value
 */
typedef struct SET_ALARM
{
	int Period;
	int DutyCycle;
	int dutyCycleAdjust;
	int incr_Flag;
	int decr_Flag;
	int count;
	int width;
} set_alarm_t;


enum {
	 ALARM_OFF = 0,
	 ALARM_ON
};

/*
 * Function: SevenSegLED
 * ----------------------------
 * Displays single digit number in a segment 
 *   Returns: void
 *
 *   @param: int number - Number to display (0 - 9) and 10 for Dot
 *   @param: int position - segment (1, 2, 3) 
 *
 *   returns: void
 */
void SevenSegLED(int number,  int position);

/*
 * Function: LED_GPIO_Config
 * ----------------------------
 * Configures GPIO port D pins(0 - 10, 14) for 7-segment and PWM LED 
 *   Returns: void
 *
 *   @param: void
 */
void LED_GPIO_Config(void);

/*
 * Function: Delay
 * ----------------------------
 * Countdown timer
 *   Returns: void
 *
 *   @param: uint32_t nCount - countds down from this value
 *   returns: void
 */
void Delay(uint32_t nCount);

/*
 * Function: DisplayTemperature
 * ----------------------------
 * Breaks the floating point temperature into four segments
 *
 *   @param: float temperature - temperature in float
 *   returns: void
 */
void DisplayTemperature(float temperature);


/*
 * Function: TURN_ON_OFF_PWM_LED
 * ----------------------------
 * Turns LED on(1) or off(0)
 *
 *   @param: int flip - writesBit on pin on(1) or off(0)
 *   returns: void
 */
void TURN_ON_OFF_PWM_LED(int flip);

/*
 * Function: trigger_alarm
 * ----------------------------
 * Displays single digit number in a segment 
 *
 *   @param: set_alarm_t* alarm - address of struct alarm 
 *
 *   returns: void
 */
void trigger_alarm(set_alarm_t* alarm);

/*
 * Function: DisplayAngle
 * ----------------------------
 * Breaks the floating point angle into 2 segments
 *
 *   @param: float angle - tilt angle in float
 *   @param: uint8_t segment to display
 *   returns: void
 */
void DisplayAngle(float angle, uint8_t SegToDisplay);

