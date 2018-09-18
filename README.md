# kiwikee

>*kiwikee* is a small, low cost 3D printable robot , for fun & educational purposes for kids !
- a remake/ expansion of the [SMARS modular robot](https://www.thingiverse.com/thing:2662828) in OpenJSCAD!
- a custom emitter/ reciever using the Wemos D1 Mini Esp8266 modules
- a custom PCB for the robot itself

![overview](./cad/kiwikee/docs/overview2.png)

[![stability-experimental](https://img.shields.io/badge/stability-experimental-orange.svg)](https://github.com/emersion/stability-badges#experimental)


## Table of Contents

- [Features](#features)
- [Usage](#usage)
- [License](#license)
- [Screenshots](#screenshots)

## Features

- robotic body based on the great [SMARS modular robot](https://www.thingiverse.com/thing:2662828), but entirely parametric !
  - with added 
  - change the design to fit your needs !
  - integrated battery holder for 2 X AA batteries (LifePO4)
  - removeable electronics holder
- simple & quick to 3d print
- either RC mode or autonomous (based on your wants & needs & available sensors)
- attach different sensors for fun & learning (ultrasonic distance sensor, a led matrix 'face' etc)
- custom PCB design for the robot/reciever exposing 
  - 2 x I2C ports
  - 4 x general IO
  - use grove or standard pins for connectors

## Usage

### CAD: 
This package can currently only be used in the new version of JSCAD web V2 (written as npm module)
- install the web ui for JSCAD V2 (branch web-overhaul for now)
- clone this repository
- drag & drop this [folder](./cad/kiwikee) onto the JSCAD UI

### Firmware

both parts of the firmware where developped using [PlatformIo](https://platformio.org/) so ...
- install that first
  -  emitter (optional): you can find the code [here](./firmware-emitter)
  -  reciever/ robot (needed): you can find the code [here](./firmware-reciever)

- install 
- connect the electronics (either emitter or reciever based on what you need)
- go into the adapted folder above & type ```pio run && pio run -t upload``` (to compile & upload)
- have fun!

## License

All parts based on the original SMARS have the same license as the original: 
 Creative Commons - Attribution - Non-Commercial - Share Alike
All new parts 
- remote control 
- firmware
- pcb design
- etc
use the [The MIT License (MIT)](./LICENSE)
(unless specified otherwise)

## Screenshots

![overview](./cad/kiwikee/docs/overview.png)
![overview2](./cad/kiwikee/docs/overview2.png)
![chassis](./cad/kiwikee/docs/chassis.png)
![emitter](./cad/kiwikee/docs/emitter-internal.png)
![emitter2](./cad/kiwikee/docs/emitter-overview.png)
![ultrasonic-sensor](./cad/kiwikee/docs/ultrasonic-sensor.png)
