ldr r0,=0x20200000
mov r1,#1
lsl r1,#18
str r1,[r0,#4]
mov r1,#1
lsl r1,#16

loop:

str r1,[r0,#28]

ldr r5,=0x100000
sleep:
sub r5,r5,#1
cmp r5,#0
bne sleep

str r1,[r0,#40]

ldr r5,=0x100000
sleepsleep:
sub r5,r5,#1
cmp r5,#0
bne sleepsleep

b loop
