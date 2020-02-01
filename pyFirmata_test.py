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

def x_stop():
	In1.write(0)
	In2.write(0)
def y_stop():
	In3.write(0)
	In4.write(0)

def x_left(dotime):
	start = time.time()
	while time.time() - start <= dotime:
		In1.write(1)
		In2.write(0)
	x_stop()
def x_right(dotime):
	start = time.time()
	while time.time() - start <= dotime:
		In1.write(0)
		In2.write(1)
	x_stop()
def y_left(dotime):
	start = time.time()
	while time.time() - start <= dotime:
		In3.write(1)
		In4.write(0)
	y_stop()
def y_right(dotime):
	start = time.time()
	while time.time() - start <= dotime:
		In3.write(0)
		In4.write(1)
	y_stop()


while True:
	x_left(2)
	time.sleep(2)
	y_left(2)
	time.sleep(2)
	x_right(2)
	time.sleep(2)
	y_right(2)
	time.sleep(2)