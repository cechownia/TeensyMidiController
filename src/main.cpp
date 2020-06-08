#include <Arduino.h>
#include <config.h>
#include <TM1638.h>
#include <JC_Button.h> 
#include <string>
#include <RGBLEDMIDI.h>

static Button** buttons; 
//byte dataPin, byte clockPin, byte strobePin
static TM1638 LEDKEY8(config::LED_AND_KEY_DATA, config::LED_AND_KEY_CLOCK, config::LED_AND_KEY_STROBE, true, 4);
//array of currently playing notes from the side of arduino
static bool notesPlaying[config::N_BUTTONS];

/**
 * @brief We received a note - we should power on proper TM1638 LED
 * 
 * @param channel MIDI channel
 * @param note MIDI note
 * @param velocity MIDI velocity
 */
void handleNoteOn(byte channel, byte note, byte velocity)
{
  Serial.printf("Handle note on %d %d\n", channel, note);
  if(channel == config::LED_MIDI_CHANNEL && note >=0 && note < 8)
  {
    LEDKEY8.setLED(TM1638_COLOR_RED, note);
  }
}
/**
 * @brief We received a note - we should power off proper TM1638 LED
 * 
 * @param channel MIDI channel
 * @param note MIDI note
 * @param velocity MIDI velocity
 */
void handleNoteOff(byte channel, byte note, byte velocity)
{
  Serial.printf("Handle note off %d %d\n", channel, note);
  if(channel == config::LED_MIDI_CHANNEL && note >=0 && note < 8)
  {
    LEDKEY8.setLED(TM1638_COLOR_NONE, note);
  }
}

/**
 * @brief Setup portion of Arduino code - initialize serial, buttons, TM1638
 * 
 */
void setup() {
  Serial.begin(0);
  buttons = new Button*[config::N_BUTTONS];
  // put your setup code here, to run once:
  for(unsigned int buttonCnt = 0; buttonCnt < config::N_BUTTONS; ++buttonCnt)
  {
    buttons[buttonCnt] = new Button(config::BUTTON_PINS[buttonCnt]);
    buttons[buttonCnt]->begin();
    buttons[buttonCnt]->read();
    notesPlaying[buttonCnt] = false;
    //pinMode(config::BUTTON_PINS[buttonCnt], INPUT);
  }
  LEDKEY8.setupDisplay(true, 4);
  sprintf(LEDbuffer, "B.%.2d", 0);
  
  LEDKEY8.setDisplayToString(LEDbuffer);
  usbMIDI.setHandleNoteOff(handleNoteOff);
  usbMIDI.setHandleNoteOn(handleNoteOn);
}

/**
 * @brief read normal buttons using JC_Button
 * 
 * @param currentMillis current elapsed milis
 */
void readButtons(unsigned long currentMillis)
{
  for(unsigned int buttonCnt = 0; buttonCnt < config::N_BUTTONS; ++buttonCnt)
  {
    buttons[buttonCnt]->read();
  }
}

/**
 * @brief turn on or off midi notes for buttons
 * 
 * @param buttonsIn array of pointers to buttons
 * @param countButtons how many buttons we have
 */
void playMidiButtons(Button** buttonsIn, unsigned int countButtons)
{
  int MIDI_NOTE_LENGTH = 10;
  for(unsigned int buttonCnt = 0; buttonCnt < countButtons; ++buttonCnt)
  {
    if(buttonsIn[buttonCnt]->wasReleased())
    {
      //Left for ease of debugging
      //Serial.printf("Pressed button %d was released - sending note on channel 1\n", buttonCnt + (currentBank * config::N_BUTTONS));
      usbMIDI.sendNoteOn(buttonCnt + (currentBank * config::N_BUTTONS), 99, 1);
      notesPlaying[buttonCnt] = true;
    }
    else
    {
      if(notesPlaying[buttonCnt] && buttonsIn[buttonCnt]->isReleased() && buttonsIn[buttonCnt]->releasedFor(MIDI_NOTE_LENGTH))
      {
        usbMIDI.sendNoteOff(buttonCnt + (currentBank * config::N_BUTTONS), 0, 1);
        notesPlaying[buttonCnt] = false;
        //Left for ease of debugging
        //Serial.printf("Note off on button %d\n", buttonCnt + (currentBank * config::N_BUTTONS));
      }
    }
  }
}
/**
 * @brief play midi notes based on pressed buttons
 * 
 * @param currentMillis current elapsed milis
 */
void actionButtons(unsigned long currentMillis)
{
  playMidiButtons(buttons, config::N_BUTTONS);
}

/**
 * @brief Main loop od arduino code - we read normal buttons and RGBLED key, we perform actions with playing midi notes - then we read the midi input and react on it
 * 
 */
void loop() {

  unsigned long currentMillis = millis();
  readButtons(currentMillis);
  readRGBKey(LEDKEY8, currentMillis);
  actionButtons(currentMillis);
  // MIDI Controllers should read what they need and discard other incoming MIDI messages.
  while (usbMIDI.read()){}
}