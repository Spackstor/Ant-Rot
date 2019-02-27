# Antenna Rotator - ESP8266-12e

This is annother ESP project. 

I live far from broadcasters,  so I have a number of yagi antenna to consistantly bring in a number of signals mostly from Watertown. But that is only required because we live far from the transmitters. For most people in larger centers, a rotating antenna is all that is required to get more than enough channels. Mostly in the summer months however, because the heat stratifies the troposphere, bonus channels are sometimes available to me from many directions. A rotating antenna is handy for those. 

The end result of this project is a browser based GUI that allows the rotation of an attic antenna. This is accomplished using electronics and a programmed ESP8266-12e in conjunction with a DIY light weight bowtie antenna and a number of printed 3D parts.

The wiring diagram should make the required components obvious. All components were aquired off ebay.

The STLfiles.7z is a zip file containing the rotator parts which can be printed. The STL files are in millimeters. Output from my CAD puts these to inches for simplify3D so they have to be rescaled if that is what you use to generate your gcode (its a 0-1 bit toggle interpreted differently depending on your program). The contained .sldprt files are exported solid works parts.

The antenna shown in the files was built for a friend. Subsequently I decided using the standard hanger construction you can find all over the web is faster and simpler. So the greatly compressed video shows one of those

The gist of the wifi and html is in many programs witch I have used (so no credit to me) but I did figure out the analog IR interface details, and make the multipage adjustment.

On the motor gear with the optical holes, I redrilled the holes on the part for better consistency but this was not really necessary. Adjustments on the values in the program can be made to work with quite variable holes.
