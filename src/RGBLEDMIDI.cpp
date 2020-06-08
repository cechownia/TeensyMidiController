#include <RGBLEDMIDI.h>
#include <config.h>

RGBKeyButton::state calculateState(byte cButtons, byte pButtons, byte shifter)
{
    if((cButtons & shifter) != 0 && (previousButtons & shifter) != 0)
    {//stable up /¯¯

      return RGBKeyButton::state::STABLE_UP;
    }
    if((cButtons & shifter) == 0 && (previousButtons & shifter) == 0)
    {//stable down \__

      return RGBKeyButton::state::STABLE_DOWN;
    }
    if((cButtons & shifter) == 0 && (previousButtons & shifter) != 0)
    {//falling ¯\_

      return RGBKeyButton::state::FALLING_EDGE;
    }        
    if((cButtons & shifter) != 0 && (previousButtons & shifter) == 0)
    {//rising _/¯

      return RGBKeyButton::state::RISING_EDGE;
    }

    return RGBKeyButton::state::UNKNOWN;
}

void debounceRGBKeys(RGBKeyButton::button *buttons, int numButtons, byte cButtons, byte pButtons, unsigned long currentMillis)
{
  if(buttons == nullptr || numButtons <=0)
  {
    return;
  }

  byte shifter = 1;
  for (int butIdx = 0; butIdx < numButtons; ++butIdx, shifter = shifter << 1)
  {
    buttons[butIdx].currentState = calculateState(cButtons, pButtons, shifter); 
  }

}

void readRGBKey(TM1638 &LEDKEY8, unsigned long currentMillis)
{
  if(currentMillis - pastRGBLedMilis > config::RGB_LED_DEBOUNCE_TIME)
  {
    currentButtons = LEDKEY8.getButtons();
    debounceRGBKeys(buttonsRGBKey, config::RGB_BUTTON_BUTTONS_CNT, currentButtons, previousButtons, currentMillis);
    previousButtons = currentButtons;
    pastRGBLedMilis = currentMillis;
  

    if(buttonsRGBKey[0].currentState == RGBKeyButton::state::FALLING_EDGE)
    {//bank up
      if(currentBank < config::BANK_LIMIT)
      {
        ++currentBank;
        Serial.printf("Bank changed %d\n", currentBank);
        sprintf(LEDbuffer, "B.%.2d", currentBank);
        LEDKEY8.setDisplayToString(LEDbuffer);
      }

    }

    if(buttonsRGBKey[1].currentState == RGBKeyButton::state::FALLING_EDGE)
    {//bank down
      if(currentBank > 0)
      {
        --currentBank;
        Serial.printf("Bank changed %d\n", currentBank);
        sprintf(LEDbuffer, "B.%.2d", currentBank);
        LEDKEY8.setDisplayToString(LEDbuffer);
      }
    }
  }
}