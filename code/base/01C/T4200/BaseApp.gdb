set remotetimeout 10
target remote localhost:2345
add-symbol-file F:\AYD\SOURCE\ON_ME\ParameterSet\Sources\T4200\Debug\BaseApp.elf  0xa0815330 -s .rodata 0xa0965d50 -s .data 0xa09929f0 -s .dynamic 0xa09a0974 -s .ctors 0xa09a0a0c -s .dtors 0xa09a0a2c -s .got 0xa09a0a44 -s .sbss 0xa09a4bd0 -s .bss 0xa09a4bd0 
hbreak main
hbreak NU_Main

