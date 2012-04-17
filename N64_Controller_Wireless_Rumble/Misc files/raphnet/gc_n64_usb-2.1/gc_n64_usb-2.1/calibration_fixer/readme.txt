The L and R sliders don't have a central resting position. This
will cause many games to think they are active, which causes
problem when performing button assignments. 

The tool provided here patches the calibration data in a way
that makes the idle position appear central to the computer.
Execute it _after_ completing the normal windows calibration
procdure. 

You need to run the patch again every time you recalibrate
using the windows tool.

Two executables are provided:

gcjoycalfix.exe			: For firmware version 1.x
gcjoycalfix_v2.exe		: For firmware version 2.x

I would gladly have recompiled the tool to detect any version
of the adapter, but I seem to have lost the source code.
Yes, really! 

So instead, I've patched the original binary which, thanks 
to my webspace, I still had. I Just had to change a few bytes 
here and there to match the new USB vendor and product IDs. 
Much easier than obtaining a development setup on a windows box..

