ArcsSuck 0.1 by Tim Krins

Designed to take a GCode NC file and convert all arcs into small linear movements.
Drag and drop your file onto the executable, it will write a file called "converted.txt".

This is a useful tool when using CNC machines that are controlled by microcontrollers.
Should be cross platform compatible.

If your arc has a smaller radius than your x or y movement, it will screw up.
I intend on fixing this as soon as I can determine a (more complex) formula.
If your radius is larger, it will work. 

The resolution is also set to 1mm in the code. I will make this an option soon.

Its also much, much cleaner than it was :)

;D