#include <Include.h>

void ISR();
void deep_sleep();
void enable_deep_sleep();
void set_EIC_general_clock();

volatile bool isAlarmSet = false;
volatile int ISRCALLED = 0;

void setup() {
  Serial.begin(9600);
  setup_ds3231();
  clear_alarms();
  enable_deep_sleep();
  attachInterrupt(digitalPinToInterrupt(EXT_INT), ISR, FALLING);
  set_EIC_general_clock();
}

void loop() {
  delay(5000);
  if(ISRCALLED){
    Serial.println();
  }
  Serial.print("ISR called: ");
  Serial.println(ISRCALLED);
  char date[10] = "hh:mm:ss";
  now().toString(date);
  Serial.print("Current time: ");
  Serial.println(date);
  if(!isAlarmSet){
    Serial.print("SETTING ALARM: ");
    isAlarmSet = set_alarm(now() + TimeSpan(10), DS3231_A1_Second);
    Serial.print("Is Alarm set: ");
    Serial.println(isAlarmSet);
  }
  DateTime alarm1 = getAlarm1();
  Ds3231Alarm1Mode alarm1mode = getAlarm1Mode();
  char alarm1Date[12] = "DD hh:mm:ss";
  alarm1.toString(alarm1Date);
  Serial.print("  [Alarm1: ");
  Serial.print(alarm1Date);
  Serial.print(", Mode: ");
  switch (alarm1mode) {
    case DS3231_A1_PerSecond: Serial.print("PerSecond"); break;
    case DS3231_A1_Second: Serial.print("Second"); break;
    case DS3231_A1_Minute: Serial.print("Minute"); break;
    case DS3231_A1_Hour: Serial.print("Hour"); break;
    case DS3231_A1_Date: Serial.print("Date"); break;
    case DS3231_A1_Day: Serial.print("Day"); break;
  }
  Serial.println("]");
  deep_sleep();
}


void set_EIC_general_clock()
{
  // Enable GCLK for EIC (External Interrupt Controller)
  GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2 | GCLK_CLKCTRL_ID( GCLK_CLKCTRL_ID_EIC_Val ) );
  while (GCLK->STATUS.bit.SYNCBUSY);

}

void ISR() {
  clearAlarm(ALARM_1);
  isAlarmSet = false;
  ISRCALLED++;
}


void enable_deep_sleep(){
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;// | SCB_SCR_SLEEPONEXIT_Msk;  // Put the SAMD21 in deep sleep upon executing the __WFI() function
}

void deep_sleep(){
  // USBDevice.detach();
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;           // Disable SysTick interrupts
  __DSB();                                              // Complete outstanding memory operations - not required for SAMD21 ARM Cortex M0+
  __WFI();                                              // Put the SAMD21 into deep sleep, Zzzzzzzz...
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;            // Enable SysTick interrupts
  // USBDevice.attach();
}

// void clear_floating_pins(){
//   	USBDevice.detach();
// 	USBDevice.end();
// 	USBDevice.standby();

// 	Serial.println();

// 	for (uint8_t n = 0; n < PORT_GROUPS; n++)
// 	{
// 		regDir[n] = PORT->Group[n].DIR.reg;
// 		regOut[n] = PORT->Group[n].OUT.reg;
// 	}
	
// 	// Save current pin config and set all INPUT pins to INPUT_PULLUP
// 	for (uint8_t n=0; n<NUM_DIGITAL_PINS; n++)
// 	{
// 		this->regPinCFG[n] = PORT->Group[g_APinDescription[n].ulPort].PINCFG[g_APinDescription[n].ulPin].reg;
		
// 		if ((PORT->Group[g_APinDescription[n].ulPort].PINCFG[g_APinDescription[n].ulPin].reg & (uint8_t)(PORT_PINCFG_INEN)) == (uint8_t)(PORT_PINCFG_INEN) &&
// 			(PORT->Group[g_APinDescription[n].ulPort].DIR.reg & (uint32_t)(1<<g_APinDescription[n].ulPin)) == 0 &&
// 			(PORT->Group[g_APinDescription[n].ulPort].PINCFG[g_APinDescription[n].ulPin].reg & (uint8_t)(PORT_PINCFG_PULLEN)) == 0)
// 		{
// 			pinMode(n, INPUT_PULLUP);
// 		}
// 	}

// 	// Save MISO pin config
// 	this->pinSPICFG[0] = PORT->Group[g_APinDescription[PIN_SPI_MISO].ulPort].PINCFG[g_APinDescription[PIN_SPI_MISO].ulPin].reg;
	
// 	// Set MISO pin to non-floating states
// 	pinMode(PIN_SPI_MISO, INPUT_PULLUP);
	
// 	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk; //cth to fix hangs

// 	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
// 	__DSB();
// 	__WFI();
	
// 	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //restore

// 	for (uint8_t n = 0; n < PORT_GROUPS; n++)
// 	{
// 		PORT->Group[n].DIR.reg = regDir[n];
// 		PORT->Group[n].OUT.reg = regOut[n];
// 	}

// 	// Restore previously saved pin config
// 	for (uint8_t n=0; n<NUM_DIGITAL_PINS; n++)
// 	{
// 		PORT->Group[g_APinDescription[n].ulPort].PINCFG[g_APinDescription[n].ulPin].reg = this->regPinCFG[n];
// 	}

// 	// Restore previously saved MISO pin config
// 	PORT->Group[g_APinDescription[PIN_SPI_MISO].ulPort].PINCFG[g_APinDescription[PIN_SPI_MISO].ulPin].reg = this->pinSPICFG[0];

// 	USBDevice.init();
// 	USBDevice.attach();
// }

