
#define SIGNAL_TILT_READY 0x3
#define SIGNAL_DISP_TILT 0x04
#define SIGNAL_TRANSMIT 0x07
extern int GLB_TiltAngle_RollPitch_display_flag;

void tilt_DispFlag_thread(void const * argument);
void tilt_detection_thread(void const * argument);
void LCD_DisplayTiltAngle(float roll, float pitch);
void transmitTiltAngles(void const * argument);
void transmitSequence (void const * argument);

void sendSequence(char*);
