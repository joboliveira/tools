# tools
Simple tools for projects use with NUAND BladeRF2 A4 and A9

Converts Sample Files Binary Data (SC16Q11 format) int to .hex


This is a simple code to export a intel Hex file. This file was used by Quartus embedded ROM in Signal generators for embedded IPs used as signal generators.

When this files are used, the internal user IP is changed to default use. In NUAND original files, the BladeRF are use a USB por as a default sink source.

The USB2.0 or 3.0 has some interupts in the streaming file sink by external source, like a PC. We use a NVIDIA Jetson nano to sink this streaming, but the result is the same as core-I7 gen7 with 32GB of DDR4 2666 using Nvme 512GB.

This results was significant changes in the project to put the "Sample" file inside of FPGA, and use an external trigger to start the sample.

This is very critical for time when we use RADAR aplications.

References.
1. https://www.radartutorial.eu/01.basics/Radar%20Principle.en.html
2. https://www.nuand.com/bladerf-2-0-micro/
3. https://github.com/Nuand/bladeRF/wiki/bladeRF-CLI-Tips-and-Tricks#Binary_Data_SC16Q11_format
