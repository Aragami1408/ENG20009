//https://forum.arduino.cc/t/timer-interrupt-on-arduino-zero-board/409166/6
#include <Arduino.h>
boolean toggle1 = 0;

void setup() {
	Serial.begin(9600);
	pinMode(11, OUTPUT); // setup output

	PORT->Group[PORTA].DIRSET.reg = PORT_PA06;       // Set-up digital pin D8 as an output

	// Set up the generic clock (GCLK4) used to clock timers
	GCLK->GENDIV.reg = GCLK_GENDIV_DIV(4) |          // Divide the 48MHz clock source by divisor 1: 48MHz/1=48MHz
		GCLK_GENDIV_ID(4);             // Select Generic Clock (GCLK) 4
	while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

	GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
		GCLK_GENCTRL_GENEN |         // Enable GCLK4
		GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
		GCLK_GENCTRL_ID(4);          // Select GCLK4
	while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

	// Feed GCLK4 to TC4 and TC5
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TC4 and TC5
		GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
		GCLK_CLKCTRL_ID_TC4_TC5;     // Feed the GCLK4 to TC4 and TC5
	while (GCLK->STATUS.bit.SYNCBUSY);               // Wait for synchronization

	TC4->COUNT16.CC[0].reg = 0xB71A;                 // Set the TC4 CC0 register as the TOP value in match frequency mode
	while (TC4->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization

	//NVIC_DisableIRQ(TC4_IRQn);
	//NVIC_ClearPendingIRQ(TC4_IRQn);
	NVIC_SetPriority(TC4_IRQn, 0);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
	NVIC_EnableIRQ(TC4_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC)

	TC4->COUNT16.INTFLAG.bit.OVF = 1;                        // Clear the interrupt flags
	TC4->COUNT16.INTENSET.bit.OVF = 1;                       // Enable TC4 interrupts

	TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024 |   // Set prescaler to 1024, 48MHz/1024 = 46.875kHz
		TC_CTRLA_WAVEGEN_MFRQ |        // Put the timer TC4 into match frequency (MFRQ) mode
		TC_CTRLA_ENABLE;               // Enable TC4
	while (TC4->COUNT16.STATUS.bit.SYNCBUSY);

}
void loop() {
	// put your main code here, to run repeatedly:

}


void TC4_Handler()                                 // Interrupt Service Routine (ISR) for timer TC4
{     
	// Check for overflow (OVF) interrupt
	if (TC4->COUNT16.INTFLAG.bit.OVF && TC4->COUNT16.INTENSET.bit.OVF)             
	{
		// Put your timer overflow (OVF) code here:     
		// ...
		//   PORT->Group[PORTA].OUTTGL.reg = PORT_PA06;     // Toggle the D8 pin HIGH LOW
		Serial.println("true");
		toggle1 = !toggle1;
		if (toggle1){
			digitalWrite(11,HIGH);
			TC4->COUNT16.CC[0].reg = 0x0D13;
		}
		else{
			digitalWrite(11,LOW);
			TC4->COUNT16.CC[0].reg = 0x2DC5;
		}

		TC4->COUNT16.INTFLAG.bit.OVF = 1;              // Clear the OVF interrupt flag
	}
}

/*
   void TC3_Handler()
   {
   Serial.println("test");  
   if (toggle1){
   digitalWrite(11,HIGH);
   toggle1 = 0;
   }
   else{
   digitalWrite(11,LOW);
   toggle1 = 1;
   }
   }
 */
