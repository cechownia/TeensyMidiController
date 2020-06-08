# TeensyMidiController

One to two paragraph statement about your product and what it does.

![Prototype image](https://github.com/cechownia/TeensyMidiController/blob/master/doc/img/prototype.jpg?raw=true)

## Maker resources

Hints on how to create this controler can be found in the [doc](doc) folder

## Breadboard schematic
![Prototype schematic](https://github.com/cechownia/TeensyMidiController/blob/master/doc/schematics/MidiControllerSchematic_bb.png?raw=true)

## Drillout pattern printout, 3d printable panel

The drillout pattern or 3d printable panel can be found in the [doc](doc) folder

## Installation, Use, Development

**TODO:** change random number bat to python file

All systems:

Open project in *Visual Studio Code* with installed *PlatformIO*, flash your teensy.

Install *python 3.7* at the moment of writing PyGame did not work with *3.8*
Run:
```pip install pygame```

At the time of writing version of *pygame* was *1.9.6*

``` pushd listener & python listener.py & popd```

This script runs untill you kill it. Now you can press buttons on the midi controller and the python script should react and LED's should work like on this video

[![Play video](https://media.giphy.com/media/gfx0BmaPcHfDG8a9HZ/giphy.gif)](https://vimeo.com/427071713 "Prototype in action")

With the project opened in *Visual Studio Code* you should be ready to develop

## Meta

Maciej Lichon – makerspace cechownia

Distributed under the MIT license. See [LICENSE](LICENSE) for more information.

[https://github.com/maciejlichon](https://github.com/maciejlichon/)

## Contributing

1. Fork it (<https://github.com/cechownia/TeensyMidiController/fork>)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request