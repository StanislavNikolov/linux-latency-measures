#!/bin/bash

musb() { head -c1 /dev/usbmon6 > /dev/null && date -Ins; }

echo '----------------------'
musb &
/home/stjo/idk
