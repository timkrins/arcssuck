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

void G02_print_list(void);
void G03_print_list(void);
void determine_direction(void);

volatile float listarray[1000][2];
int x_positive, y_positive;
int type, rez, amt_steps, curstep, stepcalc, j, i, y_switch, x_direction, y_direction;
float start_x, start_y, end_x, end_y, radius, temp_x, temp_y;
float yval, resolution, currentx, arc_number, arc_value;

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

if ((end_x-start_x) < stepcalc) {
stepcalc = (end_x-start_x);
y_switch = 0;
}

if(!rez) {rez=1;}

float resolution = ((float)1/rez);

if ((end_y-start_y) < stepcalc) {
stepcalc = (end_y-start_y);
y_switch = 1;
}

amt_steps = abs(stepcalc*rez);

printf("Amount of steps: %d.\n", amt_steps, y_switch, x_direction, y_direction);

for (i = 0; i <= amt_steps; i++) {
arc_number = (i*resolution);
arc_value = radius-sqrt((radius*radius)-((arc_number*arc_number)));
listarray[i][0] = arc_number+1;
listarray[i][1] = arc_value+1;
}

determine_direction();

if (type == 2) {
G02_print_list();
        }
if (type == 3) {
G03_print_list();
        }
}
return 0;
}

void determine_direction(void) {
if (start_x < end_x) { x_positive = 1; } else { x_positive = 2; };
if (start_y < end_y) { y_positive = 4; } else { y_positive = 7; };
}

void G03_print_list(void) {
//Print list.
switch(x_positive+y_positive) {
  case 5: // X pos, Y pos
  printf("Both are positive.\n\n");
  //03 80 0 100 20 20 1
    for (j = 0; listarray[j][0]; j++) {
    printf("G01 X%.3f Y%.3f \n", (listarray[j][0]-1)+start_x, (listarray[j][1]-1)+start_y);
    }
  break;
  case 8: // X pos, Y neg
    printf("X is positive, Y is negative.\n\n");
    //03 0 20 20 0 20 1
 for (j = 0; listarray[j][0]; j++) {
      printf("G01 X%.3f Y%.3f \n", (listarray[j][1]-1)+start_x, (start_y-(listarray[j][0]-1)));
      };
  break;
  
  case 6: // X is negative, Y is positive
    printf("X is negative, Y is positive.\n\n");
    //03 100 80 80 100 20 1
    for (j = 0; listarray[j][0]; j++) {
      printf("G01 X%.3f Y%.3f \n", (radius+(start_y-(listarray[j][1]-1))), (start_x-(listarray[amt_steps-j][0]-1)));
      };
  break;
  case 9: // X neg, Y neg
    printf("Both are negative.\n\n");
    // a 03 20 100 0 80 20 1
    for (j = 0; listarray[j][0]; j++) {
    printf("G01 X%.3f Y%.3f \n", start_x-(listarray[j][0]-1), start_y-(listarray[j][1]-1));
    }
  break;
  default:
    printf("screwed up\n");
  break;
  }
}

void G02_print_list(void) {
//Print list.
switch(x_positive+y_positive) {
  case 5: // X pos, Y pos
  printf("Both are positive.\n\n");
  // works for a 03 80 0 100 20 20 1
    for (j = 0; listarray[j][0]; j++) {
    printf("G01 X%.3f Y%.3f \n", (listarray[j][1]-1)+start_x, (listarray[j][0]-1)+start_y);
    }
  break;
  case 8: // X pos, Y neg
    printf("X is positive, Y is negative.\n\n");
//a 02 80 0 100 -20 20 1
//CONFIRMED
 for (j = 0; listarray[j][0]; j++) {
      printf("G01 X%.3f Y%.3f \n", (listarray[j][0]-1)+start_x, start_y-(listarray[j][1]-1));
      };
  break;
  case 6: // X is negative, Y is positive
    printf("X is negative, Y is positive.\n\n");
    // CONFIRMED a 02 20 -100 0 -80 20 1
    for (j = 0; listarray[j][0]; j++) {
      printf("G01 X%.3f Y%.3f \n", ((listarray[amt_steps-j][0]-1)),(start_y+(listarray[j][1]-1)));
      };
  break;
  case 9: // X neg, Y neg
    printf("Both are negative.\n\n");
    // confirmed a 02 0 -20 20 0 20 1
    for (j = 0; listarray[j][0]; j++) {
    printf("G01 X%.3f Y%.3f \n", start_x-(listarray[j][1]-1), start_y-(listarray[j][0]-1));
    }
  break;
  default:
    printf("screwed up\n");

  break;
  }

}