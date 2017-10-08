;==============TSS=====================
TestTSS:
tprev_tss dd ?			;The previous TSS - if we used hardware task switching this would form a linked list.
tesp0 dd ?			;The stack pointer to load when we change to kernel mode.
tss0 dd ?				;The stack segment to load when we change to kernel mode.
tesp1 dd ?			;Unused...
tss1 dd ?
tesp2 dd ?
tss2 dd ?
tcr3 dd ?
teip dd ?
teflags dd ?
teax dd ?
tecx dd ?
tedx dd ?
tebx dd ?
tesp dd ?
tebp dd ?
tesi dd ?
tedi dd ?
tes dd ?				;The value to load into ES when we change to kernel mode.
tcs dd ?				;The value to load into CS when we change to kernel mode.
tss dd ?				;The value to load into SS when we change to kernel mode.
tds dd ?				;The value to load into DS when we change to kernel mode.
tfs dd ?				;The value to load into FS when we change to kernel mode.
tgs dd ?				;The value to load into GS when we change to kernel mode.
tldt dd ?				;Unused...
ttrap dw ?
tiomap_base dw ?
TestTSSEND: