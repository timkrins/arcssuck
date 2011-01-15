/*
Loose Copyright 2011 Tim Krins.
If you use this, say hi :)
If you modify it, make it better. just don't sell it.
Leave something in that links back to me please.
Cheers,
Tim.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define clear_screen system("cls")

void makelist(void);
void printlist(void);

int type, rez, amt_steps, curstep, stepcalc, j, i, ysmallest, x_direction, y_direction;
float start_x, start_y, end_x, end_y, radius, listarray[1000][2], temp_x, temp_y;
float yval, resolution, currentx;

int main(int argc, char *argv[] ) {

printf( "\n\n Arcs Suck, v0.1 by timkrins@gmail.com \n\n" );
//clear_screen;

if ( argc != 8 )
    {
        printf( "\n\n Usage: %s [Curve Type] [start_x] [start_y] [end_x] [end_y] [radius] [resolution divisor] \n\n", argv[0] );
    }
    else 
    {
        type = atoi(argv[1]);
        start_x = atof(argv[2]);
        start_y = atof(argv[3]);
        end_x = atof(argv[4]);
        end_y = atof(argv[5]);
        radius = atof(argv[6]);
        rez = atoi(argv[7]);
        
printf( "\n\n G0%d (X%.1f Y%.1f) X%.1f Y%.1f R%.1f (1/%d mm resolution) \n\n", type, start_x, start_y, end_x, end_y, radius, rez);

stepcalc = 100000;

if (type == 2) {
printf( "\n\n Sorry, G02 has not yet been implemented. \n\n", argv[0] );
return 0;
        }

if ((end_x-start_x) < stepcalc) {
stepcalc = (end_x-start_x);
ysmallest = 0;
}

if(!rez) {rez=1;}

float resolution = ((float)1/rez);

if ((end_y-start_y) < stepcalc) {
stepcalc = (end_y-start_y);
ysmallest = 1;
}

if ((end_x-start_x) < 0) {
x_direction = 1;
temp_x = start_x;
start_x = end_x;
end_x = temp_x;
}

if ((end_y-start_y) < 0) {
y_direction = 1;
temp_y = start_y;
start_y = end_y;
end_y = temp_y;
}

amt_steps = abs(stepcalc*rez);

printf("Amount of steps: %d. Yswitch: %d. Xdir: %d. Ydir: %d.\n\n", amt_steps, ysmallest, x_direction, y_direction);

for (i = 0; i <= amt_steps; i++) {
currentx = ((float)i*resolution)+start_x;
yval = radius-sqrt((radius*radius)-((currentx - end_x)*(currentx - end_x)));
listarray[i][0] = (float)currentx;
listarray[i][1] = (float)yval;
}

printlistnorm();
}
return 0;
}


void printlistnorm(void) {
for (j=0; listarray[j][0]; j++) {
printf("G01 X%.3f Y%.3f \n", listarray[j][0], listarray[amt_steps-j][1]);
}
}