set remotetimeout 10
target remote localhost:2345
add-symbol-file C:\Farsi_implementation\Examples\HSDK_1.2.1\BaseApp\Sources\T4200\Debug\BaseApp.elf  0xa08166b0 -s .rodata 0xa095e310 -s .data 0xa09833a0 -s .dynamic 0xa099e084 -s .ctors 0xa099e11c -s .dtors 0xa099e138 -s .got 0xa099e14c -s .sbss 0xa09a2140 -s .bss 0xa09a2140 
hbreak main
hbreak NU_Main

