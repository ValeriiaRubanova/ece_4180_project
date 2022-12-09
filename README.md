# Final project: ECE 4180 F22

Modern implementation of a 3D version of the arcade game “Asteroids” using mbed as a hand-held device for user input. Game graphics are displayed on a computer screen through serial port connection, additional gameplay effects, like sounds, are played on a speaker peripheral. High scores are stored on the SD card.

Demo: [include link]

## Technical project specifications

### Hardware used:

- [mbed LPC1768](https://os.mbed.com/platforms/mbed-LPC1768/)
- [microSD](https://www.sparkfun.com/products/544)
- [speaker](https://www.sparkfun.com/products/11089)
- [3 pushbuttons](https://os.mbed.com/users/4180_1/notebook/pushbuttons/)
- [class D audio amplifier](https://www.google.com/url?client=internal-element-cse&cx=005068852423125155704:gdwng-4isdi&q=https://os.mbed.com/components/TPA2005D1-Class-D-Audio-Amp/&sa=U&ved=2ahUKEwiRjuyiu-X7AhWmpXIEHZHIDcIQFnoECAQQAQ&usg=AOvVaw3Gh-2TOhToFa9RDK48rhbZ)
- [accelerometer MMA8452](https://os.mbed.com/components/MMA8452Q-Triple-Axis-Accelerometer/)
- microSD Card
- [Breadboard Power Supply 5V](https://www.sparkfun.com/products/114)
- [Wall Adapter - DC Barrel Jack](https://www.sparkfun.com/products/21249)
- Personal computer

### Software prerequisites:

[Windows serial port driver](https://os.mbed.com/handbook/Windows-serial-configuration) installed on the PC.

### Mbed connections schematics:

<img width="528" alt="Screen Shot 2022-12-09 at 12 20 22 PM" src="https://user-images.githubusercontent.com/81819912/206757661-224ebadf-63cd-4aff-8aa1-6354729a61eb.png">

<img width="326" alt="Screen Shot 2022-12-09 at 12 26 02 PM" src="https://user-images.githubusercontent.com/81819912/206758574-2e01965a-d928-4040-a9e5-be23e01f1816.png">


## How to run the program

Download ECE4180_Asteroids_MBED-OS2.LPC1768_FINAL.bin to the mbed

Plug in the mbed to usb port on computer and run x64/Debug/ECE4180_Final_Asteroids.exe

## Software specs

### MBED
Multi-threaded program:
- processing thread dedicated to reading and processing values from accelerometer and pushbuttons
- speaker thread which is playing appropriate sound effects 
- Serial COM thread dedicated to sending processed input data to the computer, receiving and processing game event data, and reading/writing to SD card 

### PC
Multi-threaded program:
- processing thread which renders graphics and updates the game
  - Graphics are rendered with opengl GLUT library
- Serial COM thread which sends and receives data from mbed through serial COM port


## Results and further improvement ideas

### Results Summary
- Full functionality for basic gameplay
- High scores recorded and saved on SD card

### Further Work
- Asteroids breaking apart into smaller asteroids
- Shield functionality
- UFO
- Background music

