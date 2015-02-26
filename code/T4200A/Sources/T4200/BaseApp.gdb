set remotetimeout 10
target remote localhost:2345
add-symbol-file G:\projeler\Parsian\code\T4200A\Sources\T4200\Debug\BaseApp.elf  0xa08180a4 -s .rodata 0xa09679b0 -s .data 0xa098eb20 -s .dynamic 0xa09aa05c -s .ctors 0xa09aa0f4 -s .dtors 0xa09aa110 -s .got 0xa09aa124 -s .sbss 0xa09ae220 -s .bss 0xa09ae220 
hbreak main
hbreak NU_Main

