import sys
import os
import time

i = 0
while True:
	sys.stdout.write(f"{time.time()}\n")
	sys.stdout.flush()
	time.sleep(0.1)
	i += 1
	if i > 30:
		os.kill(os.getpid(), 11)