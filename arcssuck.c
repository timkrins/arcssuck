#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int do_AmountSteps(float start_x, float start_y, float end_x, float end_y, char rez);
char do_Determine(float start_x, float start_y, float end_x, float end_y);
void do_CalculateAndPrint(char gType, float start_x, float start_y, float end_x, float end_y, float radius, char rez, FILE *fp, int l);

char filename[30], GString[10], XString[16], YString[16], RString[16];
int x_positive, y_positive, rez, amt_steps, curstep, stepcalc, y_switch, x_direction, y_direction, i, j, k, m, n, GCLen, XCLen, YCLen, RCLen, GFirst, XFirst, YFirst, RFirst, GVal;
int l = 1;
float temp_x, temp_y, GX, GY, yval, resolution, currentx, arc_number, arc_value, listarray[1000][2], XVal, YVal, RVal, XLast, YLast, XCen, YCen, DOPS;

int main ( int argc, char *argv[] )
{
if (!argv[1]) {
argv[1] = "gcodes.txt";
}
FILE *file = fopen ( argv[1], "r" );
   if (file)
   {
   FILE *fp;
      if (fp = fopen("converted.txt", "w")) {
      char line [ 128 ];
      
      fprintf(fp, "//ArcsSuck 0.1 by Tim K. Output of file \"");
      fprintf(fp, argv[1]);
      fprintf(fp, "\"\n\n");
      
      while ( fgets(line, sizeof line, file)) {
				GCLen, XCLen, YCLen, RCLen, GVal = 0;
				for (i = 0; line[i]; i++) {
				 if(line[i] == 'G' || line[i] == 'g') {
					 GFirst = i;
					 for (j = 0; line[i+j] != ' '; j++) {GCLen = j;}
					 for (k = 0; k < 16; k++) {*(GString+k) = line[i+1+k];}
					 GVal = atoi(GString);
					 }
					 if(line[i] == 'X' || line[i] == 'x') {
					 XFirst = i;
					 for (j = 0; line[i+j] != ' '; j++) {XCLen = j; }
					 for (k = 0; k < 16; k++) {*(XString+k) = line[i+1+k];}
					 XVal = atof(XString);
					 }
					 if(line[i] == 'Y' || line[i] == 'y') {
					 YFirst = i;
					 for (j = 0; line[i+j] != ' '; j++) {YCLen = j;}
					 for (k = 0; k < 16; k++) {*(YString+k) = line[i+1+k];}
					 YVal = atof(YString);
					 }
					 if(line[i] == 'R' || line[i] == 'r') {
					 RFirst = i;
					 for (j = 0; (line[i+j] != '\n' && line[i+j] != ' ' && line[i+j]); j++) {RCLen = j;}
					 for (k = 0; k < 16; k++) {*(RString+k) = line[i+1+k];}
					 RVal = atof(RString);
					 }
					 if(line[i] != '\n') {
					 //printf("%c", line[i]);
					 }
      }
      if (GVal == 2 || GVal == 3 ) {
      do_CalculateAndPrint(GVal,XLast,YLast,XVal,YVal,RVal,1,fp,l);
      XLast = XVal;
      YLast = YVal;
      }
      else
      {
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
      printf("No file could be loaded. Drag or specify a file to process.");
      getchar();
   }
   //getchar();
   return 0;
}

void do_CalculateAndPrint(char gType, float start_x, float start_y, float end_x, float end_y, float radius, char rez, FILE *fp, int l) {
int AmountSteps = do_AmountSteps(start_x, start_y, end_x, end_y, rez);
printf("\n%d\n",AmountSteps);
float resolution = ((float)1/rez);


//##################### START REDO ################################
				// I am passed gType, (Start X, Start Y), (End X, End Y), radius, resolution.
				// The important aspects are StartX StartY EndX EndY Radius.
				// According to the circle, we are given 2 points on an edge, and a radius.
				// From this, we can determine the values we need.
				// First, seeing as we have no I or J coordinates, we need to determine the center point of the circle.
				// We can do this by using 
				// q = sqrt((x2-x1)^2 + (y2-y1)^2)
				// x3 = (x1+x2)/2
				// y3 = (y1+y2)/2
				// x = [x3] ± sqrt(r^2-([q]/2)^2)*([y1]-[y2])/q
        // y = [y3] ± sqrt(r^2-([q]/2)^2)*([x2]-[x1])/q  
        // where (x,y) is the center point. q is the distance between the two points. x1,y1 is the first point, x2,y2 is the second point. x3 and y3 are the linear distances between the start and end.
        
float DOPS = sqrt(((end_x-start_x)*(end_x-start_x))+((end_y-start_y)*(end_y-start_y)));
float dist_x = (start_x+end_x)/2;
float dist_y = (start_y+end_y)/2;
float center_x_pos = dist_x + (sqrt((radius*radius)-((DOPS/2)*(DOPS/2)))*(start_y-end_y))/DOPS;
float center_x_neg = dist_x - (sqrt((radius*radius)-((DOPS/2)*(DOPS/2)))*(start_y-end_y))/DOPS;
float center_y_pos = dist_y + (sqrt((radius*radius)-((DOPS/2)*(DOPS/2)))*(start_x-end_x))/DOPS;
float center_y_neg = dist_y - (sqrt((radius*radius)-((DOPS/2)*(DOPS/2)))*(start_x-end_x))/DOPS;

        // we now have our center point for our circles (clockwise and counterclockwise)
        // now to generate the linear movements.

for (m = 0; m <= AmountSteps; m++) { 

        // by studyng circular theory, we can determine the points on the circle.
        // (x*x) + (y*y) = (r*r)
        // there are a max of four quadrants for a circle.
        // it is likely our arc will only cross a few, and the ones which we do not cross will be imaginary.
        // so, for starters, our step will be 1.
        // Lets assume that we will be stepping with set values in the X direction, and generating the Y values.
        // To do this, we will be using four different formulas, of which two will have a value (we will later choose which one we want)
        
        // the y value when x is one and R is 10, and the center point is (10,10)
        // transpose formula to give y=
        // ((x-10)*(x-10)) + ((y-10)*(y-10)) = (r*r)
        // (-9*-9) + ((y-10)*(y-10) = (10*10)
        // (81) + (y-10)*(y-10) = 100
        // (y-10)*(y-10) = 100 - 81
        // (y-10)*(y-10) = 19
        // sqrt(19) = (y-10)
        // ± sqrt(19) + 10 = y [positive and negative values]
        // so y ~= 10 ±4.358898943540674.
        // so the coords would be (1, 14.358898943540674) and (1, 5.641101056459326)
        
        // so its basically
        // (x-cx)^2 + (y-cy)^2 = r^2
        // r^2 - (x-cx)^2 = (y-cy)^2
        // sqrt(r^2 - (x-cx)^2) = (y-cy)
        // sqrt(r^2 - (x-cx)^2) + cy = y
        
float resolved_x;
if(start_x<end_x) {resolved_x = start_x+(m*resolution);}
if(start_x>end_x) {resolved_x = start_x-(m*resolution);}

//float next_value_pos = center_y_pos + sqrt((radius*radius)-((resolved_x-center_x_pos)*(resolved_x-center_x_pos)));
//float next_value_neg = center_y_pos - sqrt((radius*radius)-((resolved_x-center_x_pos)*(resolved_x-center_x_pos)));
float next_value_pos2 = center_y_neg + sqrt((radius*radius)-((resolved_x-center_x_pos)*(resolved_x-center_x_pos)));
float next_value_neg2 = center_y_neg - sqrt((radius*radius)-((resolved_x-center_x_pos)*(resolved_x-center_x_pos)));
float next_value_posx = center_y_pos + sqrt((radius*radius)-((resolved_x-center_x_neg)*(resolved_x-center_x_neg)));
float next_value_negx = center_y_pos - sqrt((radius*radius)-((resolved_x-center_x_neg)*(resolved_x-center_x_neg)));
//float next_value_pos2x = center_y_neg + sqrt((radius*radius)-((resolved_x-center_x_neg)*(resolved_x-center_x_neg)));
//float next_value_neg2x = center_y_neg - sqrt((radius*radius)-((resolved_x-center_x_neg)*(resolved_x-center_x_neg)));

        // now we have the new values ready to be put out.
        
if(gType == 2) {if(start_x<end_x) {fprintf(fp, "G01 X%.3f Y%.3f // G02 at line %d\n", resolved_x, next_value_posx, AmountSteps, l);};}
if(gType == 2) {if(start_x>end_x) {fprintf(fp, "G01 X%.3f Y%.3f // G02 at line %d\n", resolved_x, next_value_negx, AmountSteps, l);};}
if(gType == 3) {if(start_x>end_x) {fprintf(fp, "G01 X%.3f Y%.3f // G03 at line %d\n", resolved_x, next_value_pos2, AmountSteps, l);};}
if(gType == 3) {if(start_x<end_x) {fprintf(fp, "G01 X%.3f Y%.3f // G03 at line %d\n", resolved_x, next_value_neg2, AmountSteps, l);};}
}
}


char do_Determine(float start_x, float start_y, float end_x, float end_y) {
if (start_x < end_x) { x_positive = 1; } else { x_positive = 2; };
if (start_y < end_y) { y_positive = 4; } else { y_positive = 7; };
return (x_positive+y_positive);
}

int do_AmountSteps(float start_x, float start_y, float end_x, float end_y, char rez) {
int minsteps = 0;
if(!rez) {rez=1;}
if ((end_x-start_x) > minsteps) {minsteps = (end_x-start_x);}
if ((start_x-end_x) > minsteps) {minsteps = (start_x-end_x);}
if ((end_y-start_y) > minsteps) {minsteps = (end_y-start_y);}
if ((start_y-end_y) > minsteps) {minsteps = (start_y-end_y);}
return minsteps*rez;
}