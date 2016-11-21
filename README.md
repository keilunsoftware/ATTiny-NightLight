# ATTiny-NightLight
ATTiny85 Based Night Light
Circuit using an LDR and ATTiny85 to make a very small light that turns on in the dark
Useful for bikes, backpacks, etc... Could be easily ported to Adafruit Gemma or similar devices

 *  Measure the resistance of the LDR at the relevant light levels, 
 *  and decide for a value of resistance that will serve as a threshold 
 *  for switching on and off.
 *  Set LDR_PULLUP_RES to the exact value of your pullup resistor.
 *  Do not use any pullup less than 1kOhm, I recommend a 10kOhm to begin with.
 *  Decide how often the program will check for the lights being turned on. Change the watchdog value according to the legend. 
 *  Assign both a digital and an analog pin for measuring the LDR. These will be LDR_DRIVE_PIN and  LDR_SENSE_PIN respectively. 
 *  Connect the LDR between LDR_SENSE_PIN and ground; connect the pullup resistor between LDR_SENSE_PIN and LDR_DRIVE_PIN.
'''                                                             
                                                             Power
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
    LED 	                      |                 |        | |
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
'''
