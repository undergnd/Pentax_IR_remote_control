# Pentax_IR_remote_control
Tiny IR remote controler for Pentax camera, based on BlueNRG-LP uC

PENTAX_K30_REMOTE_CONTROLLER

PROGRAM FUNCTION

The project's goal is to allow remote control for Pentax K-30 camera.

RESULTS

The remote controller is able to send focus and shooter commands to Pentax camera with a range of a few meters. In standby it consumes less than 1 uA so it allows to use small 2032 battery. It has been tested with Pentax K-30 camera.

A SHORT DESCRIPTION OF HOW THE PROGRAM WORKS

All uC related names are valid for BlueNRG-LP controller.

switch 1 -> PA15 + RESET - triggers focus command 
switch 2 -> PA14 + RESET - triggers shooter command
PA4 - IR diode output
PA0 - LED output

On pressing switch 1 low level is present on RESET pin and high level on PA15. After reliesing switch 1, RESET comes back to high and uC starts. However on PA15 high level is kept for some time, what is determined RC timing circuit.
On pressing switch 2 low level is present on RESET pin and high level on PA14. After reliesing switch 2, RESET comes back to high and uC starts. However on PA14 high level is kept for some time, what is determined RC timing circuit.
uC tests the PA14 and PA15 pins after reset and according to above procedure it recignized which switch has been pressed. Respectively it sends command focus or shooter.
The commads base 38kHz i gnerated by TIM1 in PWM mode. The respective signale sequences are build by simple LL_mDelay function.
For focus we have:
11111111111110001010101010100010
For shooter:
111111111111100010101010101010
where: 
- 1 means 1 ms 38kHz pulse
- 0 means 1 ms delay (no 38kHz pulse)
