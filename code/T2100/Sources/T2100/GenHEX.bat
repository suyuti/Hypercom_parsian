@echo off

set eski_VRELEASE=PARS___
set VRELEASE=PRSSRV_
set eski_VRELEASE_A=PARS__A
set VRELEASE_A=PRSSRVA
set VREV=01
set VMOD=B
set VLET=


echo ********************************************************************
echo *                                                                  *
echo *   Build Optimum T2100 Base Application  %VREV%%VMOD%                      *
echo *                                                                  *
echo ********************************************************************
echo *                                                                  *
echo *        !!! Do use Bce2ppx.exe ver.10.1.3 or lather  !!!          *
echo *                                                                  *
echo ********************************************************************


echo Strip and convert HEX to PPX file format
objcopy  -j .data_alignment -j .text -j .ctors -j .dtors -j .rodata -j .data -j .nocache -j .reset -S Release\optimum.elf %VRELEASE%%VREV%%VMOD%.elf


echo .
echo Generate TM dll file

if EXIST %VRELEASE%%VREV%%VMOD%.hxs (
echo Add Signature
findstr /v ":" %VRELEASE%%VREV%%VMOD%.hxs >%VRELEASE%%VREV%%VMOD%.sig
Bce2ppx %VRELEASE%%VREV%%VMOD%.elf %VRELEASE%.%VREV%%VMOD% /SL 1 /JP 1 /SR 57343 /AN !BAA%VREV%%VMOD% /SG %VRELEASE%%VREV%%VMOD%.sig
@del /q %VRELEASE%%VREV%%VMOD%.sig
) else (
Bce2ppx %VRELEASE%%VREV%%VMOD%.elf %VRELEASE%.%VREV%%VMOD% /SL 1 /JP 1 /SR 57343 /AN !BA_%VREV%%VMOD%%VLET%
)


