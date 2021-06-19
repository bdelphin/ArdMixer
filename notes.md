# Notes ArdMixer

## Python module : pulsectl

import pulsectl

pulse = pulsectl.Pulse('my-client-name')

### choose sink
master = pulse.sink_list()[0]

### get sink volume
master.volume.value_flat

### set sink volume
pulse.volume_set(master, PulseVolumeInfo(1, 2))
pulse.volume_set(master, PulseVolumeInfo(0.5, 2))

### get sink input name
sink1 = pulse.sink_input_list()[0]
sink1.proplist['application.name']

### set volume
pulse.volume_set(sink1, PulseVolumeInfo(0.5, 2))

### mute sink
pulse.mute(sink1, True)
pulse.mute(sink1, False)

### get mute status
sink1.mute
