# ArduinoProjects

Repository of Arduino sketches I develope



1. adc_test

  configuring the Atmega328 ADC internal registers.
  Allows more freedom in ADC calibration
  - clock frequency selection
  - but adjustment
  - as well as auto-trigger mode


2. adc_intr_test

  Triggering interrupts on complete ADC conversion
  can be used for low power, interrupt based program flows, monitoring an analog value

3. Timer1_int_test

  test setting up TIMER1 on ATmega328(p) with interrupts
  not complete

4. lcd_ref

  Reference library for LCD screen as well as demo application that
  - displays text and variables
  - reads Analog pin and decodes button pressed


5. lcd_int_test

  example of interrupt driven ADC program
  ADC is running high speed in auto-trigger mode where a new conversion begins after each is completed, and triggers the ISR on completion. The ISR therefore drives the code.

  This however is only as example for the ADC as in this program the ADC ISR is interrupting the main loop code though the button timing is not top priority.

6. LCD_basic_counter

  First version of a work to break, time-focusing device.
  Simple state machine based design:
  - waits for button press to begin
  - counts 30 minutes work period
  - can be paused during work period
  - counts 5 minutes break time
  - waits for button press before starting new work period

  it uses millis() to detect elapsed time and counts the seconds and minutes.
  plenty of room to improve:
  > low power design with timers and sleep mode
  > allow adjustment of time intervals and store in memory

7. DSP_freq_3

  cloned [source] library of DSP based frequency detection on arduino.
  - Added frequency detection to musical note conversion
  - using ADC ref above with higher ADC frequency to run quicker.

~
