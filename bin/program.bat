avrdude -c usbasp-clone -p m1284p -u -U lfuse:w:0xD7:m
avrdude -c usbasp-clone -p m1284p -u -U hfuse:w:0x91:m
avrdude -c usbasp-clone -p m1284p -u -U efuse:w:0xFD:m
avrdude -c usbasp-clone -p m1284p -u -U flash:w:HardMPU.hex
