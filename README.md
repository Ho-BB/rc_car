# RC Car

![RC car image 1](/pictures/20200417_160458.jpg)
![RC car image 2](/pictures/20200417_160736.jpg)
![RC car image 3](/pictures/20200417_160641.jpg)

The mechanical parts of the rc car were modeled in blender and printed with a 3D printer.
Both the car and the remote are based on an STM32F103C8T6.
The chips were programmed using an STLINK dongle and using [st-flash](https://github.com/stlink-org/stlink)

Folders :
* 3D : Stl files for the various parts making up the car
* firmware : C code interfacing with the chip's peripheral as well as the NRF24L01 chip used for radio communication
* car : C code for the car
* remote : C code for the remote
* doc : The documentation I used to make this RC car.
