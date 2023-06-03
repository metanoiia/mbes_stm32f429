# STM32F429 MBES EDU PROJ

 - This project provide a functional cmake/ninja project for
   stm32f429 MCU using arm-none-eabi gcc toolchain and openOCD 

## Requeriments

 - CMake 3.16 or great
 - ninja build tool 1.10.0 or great
 - riscv none embed 8.2.0 or great (from WCH or xpack provider)

## Build steps

 - Create `build` directory with `mkdir build` and move to this (`cd build`)
 - Generate cmake project using ninja (or your preferred tool): `cmake ../ -G Ninja`
 - If you preffer gnu make try `cmake ../ -G 'Unix Makefile'` or replace to your preferred tool)
 - Launch ninja to build all of code: `ninja`
 - If all pass ok, the files `firmware.elf`, `firmware.elf.bin` and `firmware.elf.hex` appears in `build` directory
 - Additionally, you found the files `firmware.elf.map` and `firmware.elf.lst` containing memmory mapping and disassembler of the ELF file
# mbes_stm32f429
