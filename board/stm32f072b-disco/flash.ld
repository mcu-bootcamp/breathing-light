MEMORY
{
    rom (xr)    : ORIGIN = 0x08000000, LENGTH = 128K
    ram (xrw)   : ORIGIN = 0x20000000, LENGTH = 16K
}

SECTIONS
{
    .rodata :
    {
        KEEP(*(.rodata.vector))
        *(.rodata .rodata*)
    } >rom

    .text :
    {
        *(.text .text*)
    } >rom
}