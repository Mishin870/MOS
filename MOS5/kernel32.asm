;=========САМО ЯДРО СИСТЕМЫ (для 32-битного граф режима)========
;загрузочный сектор (boot.asm) проверяет на 32/24-битность и запускает
;32 или 24-битное ядро
use32
org 500h

include 'kernel_main.inc'

;hlt останавливает процессор до прихода следующего прерывание
;которое после окончания работы возобновит работу на этом же месте
;следовательно, приходится сделать бесконечный цикл
hltLoop:
hlt
jmp hltLoop

include 'random.inc'
include 'screen32.inc'
include 'keyboard.inc'

include 'kernel_idt.inc'

include 'kernel_footer.inc'