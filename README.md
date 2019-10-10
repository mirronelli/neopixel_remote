# neopixel_remote

A wifi based remote control for sending commands to the [mirronelli/neopixel](https://github.com/mirronelli/neopixel) based projects.
This 2 board based solution deliver absolute flicker free neopixel experience.  

Use idf.py menuconfig in this repo, to configure the mqtt broker address and topic for listening to commands.
Connect the first ESP32 board running the neopixel_remote project to another ESP32 board running the mirronelli/neopixel led driver project via 3 wires.
By default connect:
- GND on remote to GND on driver
- GPIO5 on remote to GPIO4 on driver
- GPIO4 on remote to GPIO5 on driver.

Afterwards use your favorourite mqtt client to send commands to the remote, and it will relay them to the led driver. To see the command reference, see the [mirronelli/neopixel](https://github.com/mirronelli/neopixel).
