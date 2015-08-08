# RaspberryIR
IR sender and receiver using rpi


This is a program than can decode IR signal in the air, and send IR signals to machines such as TVs.
There are two programs, how to build them:

	gcc ir_encode.c -lwiringPi
	gcc ir_decode.c -lwiringPi
	

Before build, wiringPi must be installed on rpi, refer to:

	http://wiringpi.com/download-and-install/

I use two peripherals bought on taobao:


	![](https://raw.githubusercontent.com/tanhangbo/RaspberryIR/master/IR.png)

	
	
The sender is a simple diode, with resister and something I dont know.You must use PWM to generate 38K output, or use UART to send 0x55(a trick) to generate PWM.
The receiver is more complex, with a filter for 38K in the little black head. When 38K is received, DAT sends '1' or 'DAT' falls to '0'.
Both modules are 3.3V driven, which match the Rpi's GPIO.


