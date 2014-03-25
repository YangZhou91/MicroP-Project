

#define SIGNAL_DISP_TEMPERATURE 0x05 // signal sent to temperature thread
#define SIGNAL_ALARM_ON_OFF 0x02     // signal sent to alarm thread

extern int GLB_Temperature_display_flag; // Flag that display the string "Temperature" once every mode change

void Alarm_LED_thread(void const * argument);
void temperature_detection_thread(void const * argument);
void temperature_DispFlag_thread(void const * argument);

/*
 * Function: LCD_DisplayTemperature
 * ----------------------------
 * Display the temperature in LCD
 * Calls LCD API
 *
 *   @param_1: float temperature
 *   returns: void
 */
void LCD_DisplayTemperature(float temperature);

