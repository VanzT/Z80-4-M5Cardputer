#pragma once
const String banner[] = {           //Startup / logon banner
  " EEEEEE   SSSSSS    PPPPPP    888888    000000  ",
  "E        S      S  P      P  8      8  0     00 ",
  "E        S         P      P  8      8  0    0 0 ",
  "EEEEEE    SSSSSS   PPPPPPP    888888   0   0  0 ",
  "E               S  P         8      8  0  0   0 ",
  "E               S  P         8      8  0 0    0 ",
  " EEEEEE   SSSSSS   P          888888    000000  ",
  "                                                ",
  "           Z80 Emulator for ESP32 V2.2a         ",
  " David Bottrill - Shady Grove Electronics 2023  ",
  " Vance Thompson - M5Cardputer tweaks added 2024 ",
  "                                                "
  };

WiFiServer server(23);
WiFiClient serverClient;
const char *hostName = "esp80";   //Hostname
extern bool telnetReady;
extern bool useLED;
extern String ipString;
extern bool APMode;
String ipString = "";  // Default initialization
bool APMode = false;   // Default initialization
const int BUTTON_PIN = 0;

#define LED_PIN 21     // The GPIO pin connected to the LED data line
#define NUM_LEDS 1     // Number of LEDs (assuming 1)
#define BRIGHTNESS 255  // Adjust the brightness
CRGB leds[NUM_LEDS];

#define S3


#ifdef S3

#define SS    12
#define MOSI  14
#define MISO  39
#define SCK   40
SPIClass sdSPI(SPI);

//Define pins to use as virtual GPIO ports, -1 means not implemented
int PortA[8] = {  4,  5,  6,  7, 15, 16, 17, 18};   //Virtual GPIO Port A
int PortB[8] = {  8,  3, -1, -1, -1, -1, -1, -1};   //Virtual GPIO Port B


//BreakPoint switches
#define swA 47                   //Breakpoints on / Off

#endif
//*********************************************************************************************


//*********************************************************************************************
//Virtual GPIO Port
const uint8_t GPP = 0;

//Virtual 8250 UART ports
const uint8_t UART_PORT = 0x80;
const uint8_t UART_LSR  = UART_PORT + 5;

//Virtual disk controller ports
const uint8_t DCMD  = 0x20;
const uint8_t DPARM = 0x30;

//Z80 Registers
uint8_t   A = 0;
uint8_t   Fl = 0;
uint8_t   B = 0;
uint8_t   C = 0;
uint8_t   D = 0;
uint8_t   E = 0;
uint8_t   H = 0;
uint8_t   L = 0;
uint16_t IX = 0;
uint16_t IY = 0;
uint16_t PC = 0;
uint16_t SP = 0;

//Z80 alternate registers
uint8_t   Aa = 0;
uint8_t   Fla = 0;
uint8_t   Ba = 0;
uint8_t   Ca = 0;
uint8_t   Da = 0;
uint8_t   Ea = 0;
uint8_t   Ha = 0;
uint8_t   La = 0;

//Z80 flags
bool Zf = false;                //Zero flag
bool Cf = false;                //Carry flag
bool Sf = false;                //Sign flag (MSBit of A Set)
bool Hf = false;                //Half Carry flag
bool Pf = false;                //Parity / Overflow flag
bool Nf = false;                //Add / Subtract flag

bool RUN = false;               //RUN flag
bool SingleStep = false;        //Single Step flag
bool intE = false;              //Interrupt enable
uint16_t BP = 0xffff;           //Breakpoint
uint8_t BPmode = 0;             //Breakpoint mode
bool bpOn = false;              //BP passed flag
uint8_t OC;                     //Opcode store
uint8_t JR;                     //Signed relative jump
uint8_t V8;                     //8 Bit operand temp storge
uint16_t V16;                   //16 Bit operand temp storge
uint16_t V16a;                  //16 Bit operand temp storge
uint32_t V32;                   //32 Bit operand temp storage used for 16 bit addition and subration
uint8_t v1;                     //Temporary storage
uint8_t v2;                     //Temporary storage
bool cfs;                       //Temp carry flag storage
bool dled;                      //Disk activity flag


uint8_t RAM[65536] = {};        //RAM
uint8_t pOut[256];              //Output port buffer
uint8_t pIn[256];               //Input port buffer
uint8_t rxBuf[1024];            //Serial receive buffer
uint16_t rxInPtr;               //Serial receive buffer input pointer
uint16_t rxOutPtr;              //Serial receive buffer output pointer
uint8_t txBuf[1024];            //Serial transmit buffer
uint16_t txInPtr;               //Serial transmit buffer input pointer
uint16_t txOutPtr;              //Serial transmit buffer output pointer

int vdrive;                     //Virtual drive number
char sdfile[50] = {};           //SD card filename
char sddir[50] = {"/z80/xfer"}; //SD card path
bool sdfound = true;            //SD Card present flag

TaskHandle_t Task1, Task2, Task3, Task4, Task5, Task6;      //Task handles
SemaphoreHandle_t baton;        //Process Baton, currently not used

//Flags to indicate task startup sequences are complete, used to synchronise task startup order
bool ota_t = false;
bool cpu_t = false;
bool serial_t = false;
bool telnet_t = false;

uint32_t POP[256] ={};
uint32_t POPcb[256] ={};


