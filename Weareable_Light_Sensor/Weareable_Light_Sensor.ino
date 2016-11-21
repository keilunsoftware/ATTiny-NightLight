#include <avr/sleep.h>
#include <avr/wdt.h> //Needed to enable/disable watch dog timer


// Pin assignment
#define LDR_DRIVE_PIN        2  // digital 2
#define LDR_SENSE_PIN        0  // analog 0
#define LED_PIN              3  // digital 3

/* Threshold values
 *  Measure the resistance of the LDR at the relevant light levels, 
 *  and decide for a value of resistance that will serve as a threshold 
 *  for switching on and off.
 *  Set LDR_PULLUP_RES to the exact value of your pullup resistor.
 *  Do not use any pullup less than 1kOhm, I recommend a 10kOhm to begin with.
 *  Decide how often the program will check for the lights being turned on. Change the watchdog value according to the legend. 
 *  Assign both a digital and an analog pin for measuring the LDR. These will be LDR_DRIVE_PIN and  LDR_SENSE_PIN respectively. 
 *  Connect the LDR between LDR_SENSE_PIN and ground; connect the pullup resistor between LDR_SENSE_PIN and LDR_DRIVE_PIN.
 *                                                            
 *                                                            Power
                                                             Switch
                                  +-----------------+ VCC     _____
                                  |                 +---------+   +---------+
                   220R           |                 |
                 +-------+ PIN 3  |                 | PIN 2 (Digital)
     +-----------+       +--------+                 +---------+
     |           +-------+        |                 |         |
     |                            |                 |        +++
     |                            |    ATTINY85     |        | |
    +++                           |                 |        | | PULLUP Resistor
LED | |                           |                 |        | |
    +++                           |                 |        +++
     |                      GND   |                 |         |
     +--------------------+-------+                 +---------+---------+
                          |PIN 4  |                 | PIN 0 (Analog)    |
                          |       +-----------------+                   |
                          |                                             |
                          |                                             |
                          |                           LDR               |
                          |                          +---+              |
                          +--------------------------+   +--------------+
                          |                          +---+
                          |
                         +++
                         GND

*/

#define LDR_THRESHOLD_RES 25e3  // 25 kOhm
#define LDR_PULLUP_RES    10e3  // 10 kOhm
#define LDR_THRESHOLD_RATIO (1.0*LDR_PULLUP_RES/LDR_THRESHOLD_RES)
#define LDR_THRESHOLD_READING ((int)(1024 / (1 + LDR_THRESHOLD_RATIO)))


//This runs each time the watch dog wakes us up from sleep
ISR(WDT_vect) {
  //Don't do anything. This is just here so that we wake up.
}

void setup() {
  pinMode(LDR_DRIVE_PIN, OUTPUT);
  pinMode(LDR_SENSE_PIN, INPUT);
  digitalWrite(LDR_SENSE_PIN, HIGH); //Hack for getting around INPUT_PULLUP
  pinMode(LED_PIN, OUTPUT);
  //During power up, blink the LED to let the world know we're alive
  if(digitalRead(LED_PIN) == LOW)
      digitalWrite(LED_PIN, HIGH);
  else
      digitalWrite(LED_PIN, LOW);
  delay(50);
}

bool checkForLight(){
  // Read LDR sensor and return true if above threshold
  // Sense light level.
        digitalWrite(LDR_DRIVE_PIN, HIGH);
        int reading = analogRead(LDR_SENSE_PIN);
        digitalWrite(LDR_DRIVE_PIN, LOW);

        // Low reading means the light is on.
        if (reading < LDR_THRESHOLD_READING) return true;
        else
          return false;
}

//Sets the watchdog timer to wake us up, but not reset
//0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
//6=1sec, 7=2sec, 8=4sec, 9=8sec
//From: http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/
void setup_watchdog(int timerPrescaler) {

  if (timerPrescaler > 9 ) timerPrescaler = 9; //Limit incoming amount to legal settings

  byte bb = timerPrescaler & 7; 
  if (timerPrescaler > 7) bb |= (1<<5); //Set the special 5th bit if necessary

  //This order of commands is important and cannot be combined
  MCUSR &= ~(1<<WDRF); //Clear the watch dog reset
  WDTCR |= (1<<WDCE) | (1<<WDE); //Set WD_change enable, set WD enable
  WDTCR = bb; //Set new watchdog timeout value
  WDTCR |= _BV(WDIE); //Set the interrupt enable, this will keep unit from resetting after each int
}

void loop() {
    // Sleep and use watchdog to wakeup
    ADCSRA &= ~(1<<ADEN); //Disable ADC, saves ~230uA
    setup_watchdog(6); //Setup watchdog to go off after 1sec
    sleep_mode(); //Go to sleep! Wake up 1sec later and check light
    ADCSRA |= (1<<ADEN); //Enable ADC

    //Now check for light!
    if (checkForLight() == true)
      digitalWrite(LED_PIN, HIGH);
    else
      digitalWrite(LED_PIN, LOW);
}
