#READ ME:
#For regular arrays data will be in Input.bin, it's just random bytes
#For linked lists data will be in InputList.bin generated with your selected types
#change lines 16 and 24 to change size of files
#not sure if it will work with doubles because of the floating point
import random
import struct
import string
import collections

size = input("Enter your type size in bytes: ")
file_count = input("Enter how many different files you want: ")

#generating data for any type for regular arrays
for i in range(1, int(file_count)):
    file = open(f'Input{i}.bin', "wb")
    for i in range(1, random.randint(10, 10000)):
        file.write(struct.pack("d", random.random()))#writes random data, c++ will read bytes and interpret as chars which will still be random
    file.close()

#generating data for sorting binary file that acts like a linked list
for i in range(1, int(file_count) + 1):
    file = open(f'InputList{i}.bin', "wb")

    nr_of_nodes = 11
    for i in range(1, nr_of_nodes + 1):
        if i % 2 == 0:
            file.write(struct.pack("i", i))
            for j in range(0, int(size)):#will write your data
                file.write(struct.pack("B", 0))
            file.write(struct.pack("i", 0))
            continue

        file.write(struct.pack("i", i))
        for j in range(0, int(size)):#will write your data
            file.write(struct.pack("B", random.randrange(0, 255)))
        if i == nr_of_nodes:
            file.write(struct.pack("i", 0))#point last element to 0(i will interpret as nullptr)
        else:
            file.write(struct.pack("i", i + 2))
