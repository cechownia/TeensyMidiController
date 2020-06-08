#include <Arduino.h>
#include <config.h>
#include <TM1638.h>
#ifndef RGB_LED_H

#define RGB_LED_H

//TODO: i hate statics - get rid of those.
static byte currentButtons = 0;
static byte previousButtons = 0;
static unsigned long pastRGBLedMilis = 0;
static char LEDbuffer[8];
static int currentBank = 0;

namespace RGBKeyButton
{
  enum state
  {
    RISING_EDGE = 0,
    FALLING_EDGE,
    STABLE_UP,
    STABLE_DOWN,
    UNKNOWN
  };

  /**
   * @brief structure for a RGBLED button debouncing - contains current state - and information about state change timing
   * TODO: create code for button debounce timing 
   * 
   */
  struct button
  {
    state currentState;
    unsigned long stateChangeMilis;
    unsigned long previousStateChangeMilis;
  };
};

//current state of all buttons on RGB_LED.
static RGBKeyButton::button buttonsRGBKey[config::RGB_BUTTON_BUTTONS_CNT];

/**
 * @brief This function determines the state of each button on TM1638
 * 
 * @param cButtons current buttond byte
 * @param pButtons previous button byte
 * @param shifter what bit to check byte
 * @return RGBKeyButton::state 
 */
RGBKeyButton::state calculateState(byte cButtons, byte pButtons, byte shifter);

/**
 * @brief Determine if we are in a stable button state for each button
 * 
 * @param buttons current not null button states
 * @param numButtons how many buttons do we have
 * @param cButtons current byte of buttons
 * @param pButtons previous state of buttons
 * @param currentMillis current elapsed milis
 */
void debounceRGBKeys(RGBKeyButton::button *buttons, int numButtons, byte cButtons, byte pButtons, unsigned long currentMillis);

/**
 * @brief Perform single read of TM1638 keys
 * 
 * @param LEDKEY8 initialized TM1638 object
 * @param currentMillis current elapsed milis.
 */
void readRGBKey(TM1638 &LEDKEY8, unsigned long currentMillis);

#endif // !RGB_LED_H
