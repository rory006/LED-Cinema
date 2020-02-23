#number of leds per strip type on wall and ceiling
#number are guessed, need to be measured:
W_LONG_STRIP = 200
W_DOUBLE_STRIP = 0
W_SHORT_STRIP = 0

WL0_OFFSET = 0
WL1_OFFSET = 0
WL5_OFFSET = 0
WL6_OFFSET = 0

WR0_OFFSET = 0
WR1_OFFSET = 0
WR5_OFFSET = 0
WR6_OFFSET = 0

C_LONG_STRIP = 0
C_DOUBLE_STRIP = 0 #72cm
C_SHORT_STRIP = 0  #50cm

CL0_OFFSET = 0
CL1_OFFSET = 0
CL5_OFFSET = 0
CL6_OFFSET = 0

CR0_OFFSET = 0
CR1_OFFSET = 0
CR5_OFFSET = 0
CR6_OFFSET = 0

#---------------------------------------
# From here on, everything is computed
# --------------------------------------
#number of strips per wall": so e.g. 12 long strips total because 2 walls)
N_LONG = 6
N_DOUBLE = 4
N_SHORT = 6
NUM_TOTAL_LEDS = (N_LONG*W_LONG_STRIP+N_DOUBLE*W_DOUBLE_STRIP+N_SHORT*W_SHORT_STRIP+N_LONG*C_LONG_STRIP+N_DOUBLE*C_DOUBLE_STRIP+N_SHORT*C_SHORT_STRIP)
# memory layout:
# wlong*n_lon wshort*n_short wdouble*n_double clong*n_lon cshort*n_short cdouble*n_double

#memory positions
W_LONG_SIZE = (W_LONG_STRIP*4)
W_DOUBLE_SIZE = (W_DOUBLE_STRIP*4)
W_SHORT_SIZE = (W_SHORT_STRIP*4)
C_LONG_SIZE = (C_LONG_STRIP*4)
C_DOUBLE_SIZE = (C_DOUBLE_STRIP*4)
C_SHORT_SIZE = (C_SHORT_STRIP*4)

W_LONG_START = 0
W_SHORT_START = (W_LONG_SIZE*N_LONG)
W_DOUBLE_START = (W_SHORT_START+W_SHORT_SIZE*N_SHORT)
C_LONG_START = (W_DOUBLE_START+W_DOUBLE_SIZE*N_DOUBLE)
C_SHORT_START = (C_LONG_SIZE*N_LONG)
C_DOUBLE_START = (C_SHORT_START+C_SHORT_SIZE*N_SHORT)

#memory offsets for each strip section
offsets = {
"WL0": (W_DOUBLE_START+WL0_OFFSET, W_DOUBLE_SIZE),
"WL1": (W_DOUBLE_START+WL1_OFFSET, W_DOUBLE_SIZE),
"WL2": (W_SHORT_START, W_SHORT_SIZE),
"WL3": (W_SHORT_START+W_SHORT_SIZE, W_SHORT_SIZE),
"WL4": (W_SHORT_START+W_SHORT_SIZE*2, W_SHORT_SIZE),
"WL5": (W_DOUBLE_START+W_DOUBLE_SIZE+WL5_OFFSET, W_DOUBLE_SIZE),
"WL6": (W_DOUBLE_START+W_DOUBLE_SIZE+WL6_OFFSET, W_DOUBLE_SIZE),

"WR0": (W_DOUBLE_START+W_DOUBLE_SIZE*2+WR0_OFFSET, W_DOUBLE_SIZE),
"WR1": (W_DOUBLE_START+W_DOUBLE_SIZE*2+WR1_OFFSET, W_DOUBLE_SIZE),
"WR2": (W_SHORT_START+W_SHORT_SIZE*3, W_SHORT_SIZE),
"WR3": (W_SHORT_START+W_SHORT_SIZE*4, W_SHORT_SIZE),
"WR4": (W_SHORT_START+W_SHORT_SIZE*5, W_SHORT_SIZE),
"WR5": (W_DOUBLE_START+W_DOUBLE_SIZE*3+WR5_OFFSET, W_DOUBLE_SIZE),
"WR6": (W_DOUBLE_START+W_DOUBLE_SIZE*3+WR6_OFFSET, W_DOUBLE_SIZE),

"WH0": (W_LONG_START, W_LONG_SIZE),
"WH1": (W_LONG_START + W_LONG_SIZE, W_LONG_SIZE),
"WH2": (W_LONG_START + W_LONG_SIZE*2, W_LONG_SIZE),
"WH3": (W_LONG_START + W_LONG_SIZE*3, W_LONG_SIZE),
"WH4": (W_LONG_START + W_LONG_SIZE*4, W_LONG_SIZE),
"WH5": (W_LONG_START + W_LONG_SIZE*5, W_LONG_SIZE),

"CL0": (C_DOUBLE_START+CL0_OFFSET, C_DOUBLE_SIZE),
"CL1": (C_DOUBLE_START+CL1_OFFSET, C_DOUBLE_SIZE),
"CL2": (C_SHORT_START, C_SHORT_SIZE),
"CL3": (C_SHORT_START+C_SHORT_SIZE, C_SHORT_SIZE),
"CL4": (C_SHORT_START+C_SHORT_SIZE*2, C_SHORT_SIZE),
"CL5": (C_DOUBLE_START+C_DOUBLE_SIZE+CL5_OFFSET, C_DOUBLE_SIZE),
"CL6": (C_DOUBLE_START+C_DOUBLE_SIZE+CL6_OFFSET, C_DOUBLE_SIZE),

"CR0": (C_DOUBLE_START+C_DOUBLE_SIZE*2+CR0_OFFSET, C_DOUBLE_SIZE),
"CR1": (C_DOUBLE_START+C_DOUBLE_SIZE*2+CR1_OFFSET, C_DOUBLE_SIZE),
"CR2": (C_SHORT_START+C_SHORT_SIZE*3, C_SHORT_SIZE),
"CR3": (C_SHORT_START+C_SHORT_SIZE*4, C_SHORT_SIZE),
"CR4": (C_SHORT_START+C_SHORT_SIZE*5, C_SHORT_SIZE),
"CR5": (C_DOUBLE_START+C_DOUBLE_SIZE*3+CR5_OFFSET, C_DOUBLE_SIZE),
"CR6": (C_DOUBLE_START+C_DOUBLE_SIZE*3+CR6_OFFSET, C_DOUBLE_SIZE),

"CH0": (C_LONG_START, C_LONG_SIZE),
"CH1": (C_LONG_START + C_LONG_SIZE, C_LONG_SIZE),
"CH2": (C_LONG_START + C_LONG_SIZE*2, C_LONG_SIZE),
"CH3": (C_LONG_START + C_LONG_SIZE*3, C_LONG_SIZE),
"CH4": (C_LONG_START + C_LONG_SIZE*4, C_LONG_SIZE),
"CH5": (C_LONG_START + C_LONG_SIZE*5, C_LONG_SIZE)
}

import socket
import time

def init(color = None):
    if color is None:
        color = [0, 0, 0x10, 0]
    data = [0xFF]  # start code in server, used to show packet start
    for i in range(NUM_TOTAL_LEDS):
        data.extend(color)
    return data

def connect():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("192.168.2.254", 23))
    return s
    
def send(d, s = None):
    if s is None:
        s = connect()
    s.sendall(bytes(d))
    print("sent data for %d+1 bytes (%d LEDs)" % (len(d)-1, (len(d)-1)/4))

def set_all(d, color):
    for i in range(1, len(d), 4):
        d[i: i+4] = color
    send(d)

def set_strip(d, code, color, s = None):
    if code in offsets.keys():
        offset, lsize = offsets[code]
        offset += 1   # the first byte in data is start code
        print("offset, size is", offset, lsize)
        for i in range(offset, offset+lsize, 4):
            d[i: i+4] = color
        send(d, s)
        return d
    else:
        raise RuntimeError("Code %s does not exist" % code)

def set_pixel(d, code, index, color, s = None):
    if code in offsets.keys():
        offset, lsize = offsets[code]
        offset += 1   # the first byte in data is start code
        print("offset, size is", offset, lsize)
        d[offset+4*index: offset+4*index+4] = color
        send(d, s)
        return d
    else:
        raise RuntimeError("Code %s does not exist" % code)

def run_command(cmd):
    if cmd == "o":
        data = init()
        set_all(data, [0, 0, 0, 0])
    elif cmd == "w":
        data = init()
        set_all(data, [0xFE, 0xFE, 0xFE, 0xFE])
    elif cmd == "or":
        data = init()
        set_all(data, [0x50, 0xFE, 0, 0])
    elif cmd == "db":
        data = init()
        set_all(data, [0x05, 0x10, 0, 0])
    elif cmd == "wh":
        s = connect()
        data = init()
        set_all(data, [0, 0, 0, 0])
        time.sleep(1)
        for code in ["WH"+str(i) for i in range(6)]:
            print(code)
            data = set_strip(data, code, [0xFE, 0, 0, 0], s)
            time.sleep(3)
    elif cmd == "animate":
        s = connect()
        data = init([0, 0, 0, 0])
        set_all(data, [0, 0, 0, 0])
        for i in range(144):
            print("pixel index", i)
            data = init([0, 0, 0, 0])
            data = set_pixel(data, "WH0", i, [0xFE, 0, 0, 0], s)
            time.sleep(0.2)
    else:
        s = connect()
        data = init()
        for code in offsets.keys():
            print(code)
            data = set_strip(data, code, [0xFE, 0, 0, 0], s)
            time.sleep(0.3)
    return data
            
print("Total LEDs:", NUM_TOTAL_LEDS)
print("Total bytes:", NUM_TOTAL_LEDS*4)
#data = run_command("o")
data = run_command("or")
#print(data[0:int((len(data)-1)/4)])
