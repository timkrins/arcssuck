#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void G02_print(char casenumber, float start_x, float start_y, float radius, FILE *fp, int l);
void G03_print(char casenumber, float start_x, float start_y, float radius, FILE *fp, int l);
char do_Determine(float start_x, float start_y, float end_x, float end_y);
void do_Calculate(char typer, float start_x, float start_y, float end_x, float end_y, float radius, char rez, FILE *fp, int l);

float listarray[1000][2];
int x_positive, y_positive;
int l = 1;
int rez, amt_steps, curstep, stepcalc, y_switch, x_direction, y_direction;
//float start_x, start_y, end_x, end_y, radius
float temp_x, temp_y, GX, GY;
float yval, resolution, currentx, arc_number, arc_value;
int i, j, k, m, n, GCLen, XCLen, YCLen, RCLen, GFirst, XFirst, YFirst, RFirst, GVal;
char GString[10], XString[16], YString[16], RString[16];
float XVal, YVal, RVal, XLast, YLast = (float)0;

int main ( void )
{
   static const char filename[] = "gcodes.txt";
   FILE *file = fopen ( filename, "r" );
   if (file)
   {
   FILE *fp;
      if (fp = fopen("converted.txt", "w")) {
      char line [ 128 ];
      while ( fgets(line, sizeof line, file)) /* read a line */
      {
				//fputs ( line, stdout ); /* write the line */
				GCLen, XCLen, YCLen, RCLen, GVal = 0;
				memset (listarray, 0, 1000);
				for (i = 0; line[i]; i++) {
				//entire line is now in memory
				//reset variables
				
					 if(line[i] == 'G' || line[i] == 'g') {
					 //if a G is encountered, I want to count the amount of digits before a space.
					 GFirst = i;
					 for (j = 0; line[i+j] != ' '; j++) {
					 GCLen = j;
					 }
					 for (k = 0; k < 16; k++) {
					 *(GString+k) = line[i+1+k];
					 }
					 GVal = atoi(GString);
					 }
					 
					 if(line[i] == 'X' || line[i] == 'x') {
					 XFirst = i;
					 for (j = 0; line[i+j] != ' '; j++) {
					 XCLen = j;
					 }
					 for (k = 0; k < 16; k++) {
					 *(XString+k) = line[i+1+k];
					 }
					 XVal = atof(XString);
					 }
					 
					 if(line[i] == 'Y' || line[i] == 'y') {
					 YFirst = i;
					 for (j = 0; line[i+j] != ' '; j++) {
					 YCLen = j;
					 }
					 for (k = 0; k < 16; k++) {
					 *(YString+k) = line[i+1+k];
					 }
					 YVal = atof(YString);
					 }
					 
					 
					 if(line[i] == 'R' || line[i] == 'r') {
					 RFirst = i;
					 for (j = 0; (line[i+j] != '\n' && line[i+j] != ' ' && line[i+j]); j++) {
					 RCLen = j;
					 }
					 for (k = 0; k < 16; k++) {
					 *(RString+k) = line[i+1+k];
					 }
					 RVal = atof(RString);
					 }
					 
					 if(line[i] != '\n') {
					 printf("%c", line[i]);
					 }
					 
						 
         
      }
      printf("\nG=%d, X=%.3f, Y=%.3f, R=%.3f\n", GVal, XVal, YVal, RVal);
      //now, if it is a G02 or G03, pass it to the converter.
      //otherwise, print it straight through and keep the values.
      if (GVal == 2 || GVal == 3 ) {
      do_Calculate(GVal,XLast,YLast,XVal,YVal,RVal,1,fp,l);
      XLast = XVal;
      YLast = YVal;
      }
      else
      {
      printf("typer %d.\n\n", GVal);
      fprintf(fp, line);
      if(XCLen) {XLast = XVal;}
      if(YCLen) {YLast = YVal;}
      }
      l++;
      }
      
      }
      fclose (fp);
      fclose (file);
   }
   else
   {
      perror (filename); /* why didn't the file open? */
   }
   return 0;

}

void do_Calculate(char typer, float start_x, float start_y, float end_x, float end_y, float radius, char rez, FILE *fp, int l) {
stepcalc = 100000;

if(!rez) {rez=1;}
float resolution = ((float)1/rez);

if ((end_x-start_x) < stepcalc) {
stepcalc = (end_x-start_x);
y_switch = 0;
}

if ((end_y-start_y) < stepcalc) {
stepcalc = (end_y-start_y);
y_switch = 1;
}
amt_steps = abs(stepcalc*rez);
printf("Amount of steps: %d.\n%d %.0f %.0f %.0f %.0f %.0f %d\n", amt_steps, typer, start_x, start_y, end_x, end_y, radius, rez);
for (m = 0; m <= amt_steps; m++) {
arc_number = (m*resolution);
arc_value = radius-sqrt((radius*radius)-((arc_number*arc_number)));
listarray[m][0] = arc_number+1;
listarray[m][1] = arc_value+1;
}
printf("typer %d.\n\n", typer);
if (typer == 2) {G02_print(do_Determine(start_x, start_y, end_x, end_y), start_x, start_y, radius,fp, l);}
if (typer == 3) {G03_print(do_Determine(start_x, start_y, end_x, end_y), start_x, start_y, radius,fp, l);}
}

char do_Determine(float start_x, float start_y, float end_x, float end_y) {
if (start_x < end_x) { x_positive = 1; } else { x_positive = 2; };
if (start_y < end_y) { y_positive = 4; } else { y_positive = 7; };
return (x_positive+y_positive);
}

void G03_print(char casenumber, float start_x, float start_y, float radius, FILE *fp, int l) {
for (n = 0; listarray[n][0]; n++) {
switch(casenumber) {
  case 5: // X pos, Y pos
    GX = (listarray[n][0]-1)+start_x;
    GY = (listarray[n][1]-1)+start_y;
  break;
  case 8: // X pos, Y neg
  GX = (listarray[n][1]-1)+start_x;
  GY = (start_y-(listarray[n][0]-1));
  break;
  case 6: // X is negative, Y is positive
    GX = radius+(start_y-(listarray[n][1]-1));
    GY = start_x-(listarray[amt_steps-n][0]-1);
  break;
  case 9: // X neg, Y neg
    GX = start_x-(listarray[n][0]-1);
    GY = start_y-(listarray[n][1]-1);
  break;
  default:
    printf("screwed up\n");
  break;
  }
  fprintf(fp, "G01 X%.3f Y%.3f //G03 at line %d\n",GX ,GY, l);
}
}

void G02_print(char casenumber, float start_x, float start_y, float radius, FILE *fp, int l) {
    for (n = 0; listarray[n][0]; n++) {
switch(casenumber) {
  case 5: // X pos, Y pos
    GX = (listarray[n][1]-1)+start_x;
    GY = (listarray[n][0]-1)+start_y;
  break;
  case 8: // X pos, Y neg
    GX = (listarray[n][0]-1)+start_x;
    GY = start_y-(listarray[n][1]-1);
  break;
  case 6: // X is negative, Y is positive
    GX = listarray[amt_steps-n][0]-1;
    GY = start_y+(listarray[n][1]-1);
  break;
  case 9: // X neg, Y neg
    GX = start_x-(listarray[n][1]-1);
    GY = start_y-(listarray[n][0]-1);
  break;
  default:
    printf("screwed up\n");
  break;
  }
   fprintf(fp, "G01 X%.3f Y%.3f //G02 at line %d\n",GX ,GY, l);
  }
  }