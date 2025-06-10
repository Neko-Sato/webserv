import sys
import os
import time
import datetime
import os

print("Status: 200\r", file=sys.stdout)
print("Content-type: text/plain\r", file=sys.stdout)
print("\r", file=sys.stdout)

i = 0
while True:
	print(f"{datetime.datetime.now()}", file=sys.stdout)
	sys.stdout.flush()
	time.sleep(0.1)
	i += 1
	if i > 4:
		print(os.environ, file=sys.stdout)
		sys.stdout.flush()
		# time.sleep(0.1)
		# os.kill(os.getpid(),11)
		print("end", file=sys.stdout)
		break

