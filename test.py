import serial
import pulsectl
import time
from pulsectl import PulseVolumeInfo

pulse = pulsectl.Pulse('ArdMixer')
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=0.1)

master = pulse.sink_list()[0]

last_vol = 0

time.sleep(2)

mute = False

while(1):
    rcv = ser.readline()
    if(len(rcv) > 0):
        last_vol = int(rcv.decode('utf-8').split(' ')[1][:-2])
        print(str(last_vol))
        pulse.volume_set(pulse.sink_list()[0], PulseVolumeInfo(last_vol/100, 2))
    elif(mute):
        if(pulse.sink_list()[0].mute == 0):
            mute = False
            ser.write(str.encode('SET_MASTER_UNMUTE\n'))
    else:
        if(pulse.sink_list()[0].mute):
            mute = True
            ser.write(str.encode('SET_MASTER_MUTE\n'))
        else:
            vol = round(pulse.sink_list()[0].volume.value_flat * 100)
            if (vol != last_vol):
                ser.write(str.encode('SET_MASTER_VOL '+str(vol)+'\n'))
                last_vol = vol
                time.sleep(0.5)
    time.sleep(0.1)
