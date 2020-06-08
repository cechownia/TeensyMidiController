import sys, pygame, pygame.midi
import os
from enum import IntEnum
import threading
import subprocess
import re

midi_output = None

#None in "expectedOutputRegex" means we're looking for success command line with exit code 0
#TODO add watch banks
LEDListeners = [
    {"id": 0, "type": "command", "value": ["randomexit.bat"], "timeoutms": 1100.0, "expectedOutputRegex": None,},
    {"id": 1, "type": "command", "value": ["randomexit.bat"], "timeoutms": 1200.0, "expectedOutputRegex": None,},
    {"id": 2, "type": "command", "value": ["randomexit.bat"], "timeoutms": 1300.0, "expectedOutputRegex": None,},
    {"id": 3, "type": "command", "value": ["randomexit.bat"], "timeoutms": 1400.0, "expectedOutputRegex": None,},
    {"id": 4, "type": "command", "value": ["randomexit.bat"], "timeoutms": 1500.0, "expectedOutputRegex": None,},
    {"id": 5, "type": "command", "value": ["randomexit.bat"], "timeoutms": 1600.0, "expectedOutputRegex": None,},
    {"id": 6, "type": "command", "value": ["randomexit.bat"], "timeoutms": 1700.0, "expectedOutputRegex": None,},
    {"id": 7, "type": "command", "value": ["randomexit.bat"], "timeoutms": 1800.0, "expectedOutputRegex": None,},
]


#None in value means don't react
Commands = {
    "NoteOff": [
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    {"type": "command", "value": None},
    ],
    "NoteOn" : [
    {"type": "command", "value": "echo command 1"},
    {"type": "command", "value": "echo command 2"},
    {"type": "command", "value": "echo command 3"},
    {"type": "command", "value": "echo command 4"},
    {"type": "command", "value": "echo command 5"},
    {"type": "command", "value": "echo command 6"},
    {"type": "command", "value": "echo command 7"},
    {"type": "command", "value": "echo command 8"},
    {"type": "command", "value": "echo command 9"},
    {"type": "command", "value": "echo command 10"},
    {"type": "command", "value": "echo command 11"},
    {"type": "command", "value": "echo command 12"},
    {"type": "command", "value": "echo command 13"},
    {"type": "command", "value": "echo command 14"},
    {"type": "command", "value": "echo command 15"},
    ]
}


def processNote(trigger, note_num):
    if trigger in Commands and note_num < len(Commands[trigger]) and Commands[trigger][note_num]["value"] is not None:
        print("Executing {type} on trigger {trigger} : {command}".format(type= Commands[trigger][note_num]["type"], trigger= trigger, command= Commands[trigger][note_num]["value"]))
        if(Commands[trigger][note_num]["type"] == "command"):
            os.system(Commands[trigger][note_num]["value"])
        else:
            print("Unknown type of command...")
    else:
        print("Command Undefined:{trigger}, {noteNum}".format(trigger= trigger, noteNum= note_num))

#On timer trigger process watch on something that should light up the LED
def LEDTimerTrigger(id, type, value, timeoutms, expectedOutputRegex):
    global midi_output
    #print("Timmer Trigger: {id}, {type}, {value}, {timeoutms}, {expectedOutputRegex}".format(id=id, type=type, value=value, timeoutms=timeoutms, expectedOutputRegex=expectedOutputRegex),)
    
    subprocessReturn = subprocess.run(value, capture_output=True, shell=True)
    if expectedOutputRegex is None:
        if(subprocessReturn.returncode == 0):
            #light up LED by sending note on channel 2
            midi_output.note_on(id, 99, 1)
        else:
            midi_output.note_off(id, 0, 1)
    else:
        if re.match(expectedOutputRegex,subprocessReturn.stdout) is not None:
            #light up LED by sending note on channel 2
            midi_output.note_on(id, 99, 1)
        else:
            midi_output.note_off(id, 0, 1)

    timerReDo = threading.Timer(timeoutms/1000.0, LEDTimerTrigger, None, {"id": id, "type": type, "value": value, "timeoutms": timeoutms, "expectedOutputRegex": expectedOutputRegex})
    timerReDo.start()


def main():
    global midi_output
    # set up pygame
    pygame.midi.init()
    # list all midi devices
    print("Avaliable devices:")
    for x in range( 0, pygame.midi.get_count() ):
        print("{number} - {device}".format(number= x, device= str(pygame.midi.get_device_info(x))))

    input_id = pygame.midi.get_default_input_id()
    output_id = pygame.midi.get_default_output_id()
    print("Default input id {}".format(input_id))
    print("Default output id {}".format(input_id))
    # open a specific midi device
    inp = pygame.midi.Input(input_id)
    #my output was 3 on id list printed on start - your may varry
    midi_output = pygame.midi.Output(3)
    #create concurent timers to wait for events on which we should play notes
    for value in LEDListeners:
        print("Starting timer: {}".format(str(value)) )
        timer = threading.Timer(value["timeoutms"]/1000.0, LEDTimerTrigger, None, value)
        timer.start()
    # run the event loop
    print("Polling...")
    while True:
        if inp.poll():
            # no way to find number of messages in queue
            # so we just specify a high max value
            red_inputs = inp.read(1000)
            for red_element in red_inputs:
                if red_element[0][0] == 144:
                    #NoteOn
                    trigger = "NoteOn"
                    note_num = red_element[0][1]
                    processNote(trigger, note_num)
                if red_element[0][0] == 128:
                    #NoteOff
                    trigger = "NoteOff"
                    note_num = red_element[0][1]
                    processNote(trigger, note_num)

        pygame.time.wait(10)

print("ended")
if __name__ == "__main__":
    main()