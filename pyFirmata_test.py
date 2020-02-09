import pyfirmata
import time
board = pyfirmata.Arduino('COM3')


In1 = board.digital[4]
In2 = board.digital[5]
In3 = board.digital[6]
In4 = board.digital[7]

In1.mode = pyfirmata.OUTPUT
In2.mode = pyfirmata.OUTPUT
In3.mode = pyfirmata.OUTPUT
In4.mode = pyfirmata.OUTPUT

period_time = 0.001

def x_stop():
	In1.write(0)
	In2.write(0)
def y_stop():
	In3.write(0)
	In4.write(0)

def pin_speed(pin, speed):
	pin.write(1)
	time.sleep(period_time * speed)
	pin.write(0)
	# time.sleep(period_time)
	# time.sleep(period_time * (1 - speed))
def x_left(speed, dotime):
	start = time.time()
	In2.write(0)
	while time.time() - start <= dotime:
		pin_speed(In1, speed)
	x_stop()
def x_right(speed, dotime):
	start = time.time()
	In1.write(0)
	while time.time() - start <= dotime:
		pin_speed(In2, speed)
	x_stop()
def y_left(speed, dotime):
	start = time.time()
	In4.write(0)
	while time.time() - start <= dotime:
		pin_speed(In3, speed)
	y_stop()
def y_right(speed, dotime):
	start = time.time()
	In3.write(0)
	while time.time() - start <= dotime:
		pin_speed(In4, speed)
	y_stop()


if True:
# while True:
	for j in range(5):
		for i in range(20):
			x_left(3.0, 0.005)
			y_left(12.0, 0.005)
		time.sleep(0.3)
	for j in range(5):
		for i in range(20):
			x_right(3.0, 0.005)
			y_right(12.0, 0.005)
		time.sleep(0.3)
