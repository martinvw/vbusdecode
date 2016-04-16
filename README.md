=============================
Welcome to the vbusdecoder
=============================
[![Build Status](https://travis-ci.org/martinvw/vbusdecode.svg?branch=master)](https://travis-ci.org/martinvw/vbusdecode)

Originally developed by: andywhite

This is a utility for use with resol solar controllers (or rebranded alternatives)

## Details ##

Some Resol controllers have an interface called VBUS.  This interface is an RS485 serial interface.  These controllers transmit much of their state via this interface.

Resol have windows software that will log this, but I found a need to have something simpler and able to run on my WGT634U linux box.

So here is my first attempt, and instead of building in the specifics for the models, I have designed it so the parameters are passed via arguments.  Right now, there is no error checking around args, so you need to get them right.  Below is a table of how to use this for the models I can get info on;

For example;

Resol Deltasol BS Plus
> cat /dev/ttyUSB0 | vbusdecode 0,15,0.1 2,15,0.1 4,15,0.1 6,15,0.1 8,7,1 9,7,1
> Will give out, temp of S1,S2,S3,S4 and pump1 speed and pump2

Included is a sample of a serial port capture , raw.log, from a Resol Deltasol BS Plus, so can you test with;
> cat raw.log | vbusdecode 0,15,0.1 2,15,0.1 4,15,0.1 6,15,0.1 8,7,1 9,7,1

## Compiling ##

Tested under linux, osx and windows under cygwin.

just make

```
make all
```

## Arguments  ##

arguments [-d] [-s source addr] [-f filename] [-c count]  fields
* *-f filename*      will put last retrieved value to filename , overwriting anyother contents
* *-c* The number of full frames to decode, then exit
* *-d* Debug, prints more info
* *-s* filter for this source address/model address, value in hex
* fields can contain up to four values, seperated by commas, where value

1. offset from start of data, this value is bytes
2. length, how many bits
3. bitposition or multiplier
    * if length > 1 this is the multiplier
    * if length = 1 this is the bit position within the byte use for bit fields, like error mask, relay mask etc.
4. format, p = plus , add the returned value to the next field
    * t = time , format the output as time
    * f = convert to Fahrenheit
    * l = convert bit fields to True/False
    * y = convert bit fields to Yes/No
    * o = convert bit fields to On/Off
    * Any other value is ignored.

To get this information for your model, check the wiki or look in the Vbus XMl file in Resol lite.

## Examples ##

###Example 1

```
cat raw.log | ./a.out -f rrdvals -c 1 0,15,0.1 2,15,0.1 4,15,0.1 6,15,0.1 8,7,1 9,7,1 20,16,1,p 22,16,1000,p 24,16,1000000 12,16,0,t 10,1,0 10,1,1
```

will give temp of s1,s2,s3,s4 pumpspeed pump1,pump2 and total of watts, formatted system time, r1 and r2 status for a resol deltasol bs plus, put them into a file called rrdvals, and exit after decoding one frame successfully rrdvals should contain

```
45.7 24.6 49.8 29.1 100 0 2609964 14:36 1 0
```

###Example 2

```
cat raw.log | ./a.out -s 0x4221 -c 1 0,15,0.1,f 2,15,0.1 4,15,0.1 6,15,0.1 8,7,1 9,7,1 20,16,1,p 22,16,1000,p 24,16,1000000 12,16,0,t 10,1,0,y 10,1,1,l
```

will output

```
114.3 24.6 49.8 29.1 100 0 2609964 14:36 Yes False
```
