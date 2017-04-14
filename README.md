# midi-controller

Arduino interfacing with modified 32 key CASIO SA-20 keyboard to work as a MIDI controller

# Modifications in keyboard

- 8x4 key matrix scanned by Arduino using 4 GPIO for columns and 74HC595 shift register for columns
- PS2 analog joystick (2 analog axis + button) connected to arduino for [pitch bend](https://en.wikipedia.org/wiki/Pitch_wheel) (Up-Down) and MIDI program change (Left - Right)

# Schematics
To-Do

# Images
![SA-20](Photos/SA-20.jpg "SA-20")
![Key matrix PCB](Photos/Key%20matrix%20PCB.jpg "Key matrix PCB")
![Arduino to key matrix](Photos/Arduino%20to%20key%20matrix.jpg "Arduino to key matrix")
![Joystick](Photos/Joystick.jpg "Joystick")
