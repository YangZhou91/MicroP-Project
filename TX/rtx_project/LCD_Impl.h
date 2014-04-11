
#define LCD_DATA_REG_SIZE 8 // lcd data reg size

#define LCD_REG_SEL_WRITE_COMMAND 0  // sel reg for write command
#define LCD_REG_SEL_WRITE_TEXT 1     // sel reg for write text

#define LCD_DISPLAY_FIRST_LINE_SIZE 16        // first line lim 16 for lcd 16x2
#define LCD_WAIT_BETWEEN_DISPLAY 60           // wait between display of temperature or tilt
#define LCD_ONLY_TEMPERATURE_SIZE_IN_DIGITS 5 // display 4 digit and '\0' for temperature
#define LCD_WAIT_CLEAR_DISPLAY 100            // wait after clear

// LCD Commands data bits
#define LCD_CLEAR_DISPLAY 	0x01
#define LCD_CHAR_ENTRY_MODE 0x06
#define LCD_CURSOR_HOME 		0x02
#define LCD_FUNCTION_SET 		0x38
#define LCD_SET 						0x80
#define LCD_DISPLAY_ON_OFF  0x0F


/*
 * Function: LCD_GPIO_Config
 * ----------------------------
 * Initializes the GPIO config for the LCD
 *
 *   @param: void
 *   returns: void
 */
void LCD_GPIO_Config(void);

/*
 * Function: LCD_clear_display
 * ----------------------------
 * Clears the LCD screen
 *
 *   @param: void
 *   returns: void
 */
void LCD_clear_display(void);

/*
 * Function: LCD_DisplayOnOff
 * ----------------------------
 * Turns LCD Display On/Off
 *
 *   @param: void
 *   returns: void
 */
void LCD_DisplayOnOff(void);

/*
 * Function: LCD_FuncSet
 * ----------------------------
 * Sets Function Set
 *
 *   @param: void
 *   returns: void
 */
void LCD_FuncSet(void);

/*
 * Function: LCD_CharacterEntryMode
 * ----------------------------
 * Sets character Entry mode
 *
 *   @param: void
 *   returns: void
 */
void LCD_CharacterEntryMode(void);

/*
 * Function: LCD_display_CursorHome
 * ----------------------------
 * Cursor home
 *
 *   @param: void
 *   returns: void
 */
void LCD_display_CursorHome(void);

/*
 * Function: LCD_write
 * ----------------------------
 * Writes text to the screen 
 * by setting data bits from num.
 *
 *   @param: num -> to write in the data regs
 *   returns: void
 */
void LCD_write(uint8_t num);

/*
 * Function: LCD_set
 * ----------------------------
 * Sets cursor to the position
 * set by pos.
 *
 *   @param: pos -> position in the display
 *   returns: void
 */
void LCD_set(uint8_t pos);

/*
 * Function: LCD_Command_dataRegs
 * ----------------------------
 * Writes "data" to the data regs
 * to send command
 *
 *   @param: data -> to write in the data regs
 *   returns: void
 */
void LCD_Command_dataRegs(uint8_t data);

/*
 * Function: LCD_SelectRegs
 * ----------------------------
 * write text for RegSel_RW --> 1
 * write command for RegSel_RW --> 0
 *
 *   @param: int
 *   returns: void
 */
void LCD_SelectRegs(int RegSel_RW);

/*
 * Function: LCD_Delay_Longer
 * ----------------------------
 * Dlonger delay counter
 *
 *   @param: uint32_t
 *   returns: void
 */
void LCD_Delay_Longer(uint32_t count);

/*
 * Function: LCD_Delay
 * ----------------------------
 * Delay counter
 *
 *   @param: uint32_t
 *   returns: void
 */
void LCD_Delay(uint32_t count);

/*
 * Function: LCD_Pulse
 * ----------------------------
 * Produces shorter Enable pulse 
 *
 *   @param: uint32_t
 *   returns: void
 */
void LCD_Pulse (uint32_t delay);

/*
 * Function: LCD_Pulse_Longer
 * ----------------------------
 * Produces longer Enable pulse 
 *
 *   @param: uint32_t
 *   returns: void
 */
void LCD_Pulse_Longer (uint32_t delay_longer);

/*
 * Function: LCD_Display_String
 * ----------------------------
 * Display the string sentence 
 *
 *   @param_1: char* string
 *   @param_2: int string size
 *   returns: void
 */
void LCD_Display_String(char* sentence, int sentenceSize);

/*
 * Function: LCD_DISPLAY_UPDATE_POS
 * ----------------------------
 * Display the string sentence 
 * on the position pos without clearing.
 *
 *   @param_1: uint8_t pos - position
 *   @param_2: char* string
 *   returns: void
 */
void LCD_DISPLAY_UPDATE_POS(uint8_t pos, char* sentence);

/*
 * Function: LCD_Init
 * ----------------------------
 * Initializes the LCD
 *
 *   @param: void
 *   returns: void
 */
void LCD_Init(void);
