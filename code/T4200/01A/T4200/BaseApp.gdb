set remotetimeout 10
target remote localhost:2345
add-symbol-file G:\projeler\Parsian\code\T4200\01A\T4200\Debug\BaseApp.elf  0xa0810e70 -s .rodata 0xa095e270 -s .data 0xa0980bc0 -s .dynamic 0xa098dfdc -s .ctors 0xa098e074 -s .dtors 0xa098e094 -s .got 0xa098e0ac -s .sbss 0xa0992170 -s .bss 0xa0992170 
hbreak main
hbreak NU_Main

