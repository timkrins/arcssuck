ArcsSuck 0.1 by Tim Krins

Designed to take a GCode NC file and convert all arcs into small linear movements.
Drag and drop your file onto the executable, it will write a file called "converted.txt".

This is a useful tool when using CNC machines that are controlled by microcontrollers.
Should be cross platform compatible.

At this point in time, it does not work correctly with arcs that are not at a tangent to the linear movements.
If your arc has a larger or a smaller radius than your x and y movement, it will screw up.
I intend on fixing this as soon as I can determine the formula.

The resolution is also set to 1mm in the code. I will make this an option soon.

My code may be messy... I have learned how to write C basically by trial and error.


Apart from that, its sweet!

;D