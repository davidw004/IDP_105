#define LINESENSOR1 2
#define LINESENSOR2 3
#define LINESENSOR3 4
#define LINESENSOR4 5
#define REDLED 6 //indicates robot is moving
#define GREENLED 7
#define BLUELED 8
#define LIMITSWITCH 9 //test button
#define LIMITSWITCH2 10
#define BUTTON1 11 //Not currently used
#define ULTRASOUND A0

#define MAX_RANG (520)//the max measurement value of the module is 520cm(a little bit longer than effective max range)
#define ADC_SOLUTION (1023.0)//ADC accuracy of Arduino UNO is 10bit