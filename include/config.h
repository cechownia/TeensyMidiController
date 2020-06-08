#ifndef CONFIG_H
#define CONFIG_H


namespace config
{
    //Pin order does matter - denotes the id of the button
    static const int BUTTON_PINS[]  = {3,
                                       4,
                                       5,
                                       6,
                                       7,
                                       8,
                                       9,
                                       10,
                                       11,
                                       12,
                                       18,
                                       14,
                                       15,
                                       16,
                                       17};
    static const unsigned int N_BUTTONS = 15;
    static const unsigned int LED_AND_KEY_STROBE = 2;
    static const unsigned int LED_AND_KEY_DATA   = 0;
    static const unsigned int LED_AND_KEY_CLOCK  = 1;
    static const unsigned int N_MIDI_BANKS = 4;
    static const unsigned int DEBOUNCE_TIME = 100;
    static const byte LED_MIDI_CHANNEL = 2;
    static const unsigned int RGB_BUTTON_BUTTONS_CNT = 8;
    static const unsigned long RGB_LED_DEBOUNCE_TIME = 10;
    static const int BANK_LIMIT = 15;
};

#endif // !CONFIG_H