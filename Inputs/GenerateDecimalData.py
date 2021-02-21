import random
import struct

for i in range(1, 10):
    file = open(f'Input{i}.bin', "wb")
    for i in range(1, random.randint(10, 10000)):
        file.write(struct.pack("f", random.random()))#writes random data, c++ will read bytes and interpret as chars which will still be random
    file.close()