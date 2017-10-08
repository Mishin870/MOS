;=====СМ kernel32.asm==========
use32
org 500h

include 'kernel_main.inc'

hltLoop:
hlt
jmp hltLoop

include 'random.inc'
include 'screen24.inc'
include 'keyboard.inc'

include 'kernel_idt.inc'

include 'kernel_footer.inc'