#include <avr/sleep.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";
const byte LED_PIN = 9;
const char textOpen[] = "door_alarm/open";
const char textClosed[] = "door_alarm/closed";

void wake ()
{
  // cancel sleep as a precaution
  sleep_disable();
  // precautionary while we do other stuff
  detachInterrupt (0);
}  // end of wake

void setup ()
{
  digitalWrite (2, HIGH);  // enable pull-up
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}  // end of setup


void transmit() {
  radio.write(&textOpen, sizeof(textOpen));
  radio.write(&textClosed, sizeof(textClosed));
}

void loop ()
{
  radio.powerUp();

  pinMode (LED_PIN, OUTPUT);
  for (byte i = 0; i < 5; i++) { //flash the LED
    digitalWrite(LED_PIN, HIGH);
    delay(10);
    digitalWrite(LED_PIN, LOW);
    delay(10);
  }

  transmit();
  pinMode (LED_PIN, INPUT);

  // disable ADC
  ADCSRA = 0;

  set_sleep_mode (SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  radio.powerDown();


  // Do not interrupt before we go to sleep, or the
  // ISR will detach interrupts and we won't wake.
  noInterrupts ();

  // will be calLED_PIN when pin D2 goes low
  attachInterrupt (0, wake, FALLING);
  EIFR = bit (INTF0);  // clear flag for interrupt 0

  // turn off brown-out enable in software
  // BODS must be set to one and BODSE must be set to zero within four clock cycles
  MCUCR = bit (BODS) | bit (BODSE);
  // The BODS bit is automatically cleared after three clock cycles
  MCUCR = bit (BODS);

  // We are guaranteed that the sleep_cpu call will be done
  // as the processor executes the next instruction after
  // interrupts are turned on.
  interrupts ();  // one cycle
  sleep_cpu ();   // one cycle

}
