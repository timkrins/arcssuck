#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int find_LinearSteps(float centre_x, float centre_y, float radius, char rez);
void write_Out(float GX, float GY, int l, FILE *fp);
char do_Determine(float start_x, float start_y, float end_x, float end_y);
void do_CalculateAndPrint(char gType, float start_x, float start_y, float end_x, float end_y, float radius, float GI, float GJ, char rez, FILE *fp, int l);

char filename[30], GString[10], XString[16], YString[16], RString[16], IString[16], JString[16];
int x_positive, y_positive, rez, amt_steps, curstep, stepcalc, y_switch, x_direction, y_direction, i, j, k, m, n, p, GCLen, XCLen, YCLen, RCLen, ICLen, JCLen, GFirst, XFirst, YFirst, RFirst, IFirst, JFirst, GVal;
int l = 1;
int stepcounter;
signed int p = 1;
float temp_x, temp_y, GX, GY, GI, GJ, yval, resolution, currentx, arc_number, arc_value, listarray[1000][2], XVal, YVal, RVal, IVal, JVal, XLast, YLast, XCen, YCen, DOPS;

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
				//GCLen, XCLen, YCLen, RCLen, ICLen, JCLen, GVal, IVal, JVal, RVal = 0;
				memset(RString, 0, 16);
				memset(IString, 0, 16);
				memset(JString, 0, 16);
				GCLen = 0;
				XCLen = 0;
				YCLen = 0;
				RCLen = 0;
				ICLen = 0;
				JCLen = 0;
				GVal = 0;
				RVal = 0;
				JVal = 0;
				IVal = 0;
				for (i = 0; line[i]; i++) {
				 if(line[i] == 'G' || line[i] == 'g') {
					 GFirst = i;
					 for (j = 0; (line[i+j] != '\n' && line[i+j] != ' ' && line[i+j]); j++) {GCLen = j;}
					 for (k = 0; k < 16; k++) {*(GString+k) = line[i+1+k];}
					 GVal = atoi(GString);
					 }
					 if(line[i] == 'X' || line[i] == 'x') {
					 XFirst = i;
					 for (j = 0; (line[i+j] != '\n' && line[i+j] != ' ' && line[i+j]); j++) {XCLen = j; }
					 for (k = 0; k < 16; k++) {*(XString+k) = line[i+1+k];}
					 XVal = atof(XString);
					 }
					 if(line[i] == 'Y' || line[i] == 'y') {
					 YFirst = i;
					 for (j = 0; (line[i+j] != '\n' && line[i+j] != ' ' && line[i+j]); j++)  {YCLen = j;}
					 for (k = 0; k < 16; k++) {*(YString+k) = line[i+1+k];}
					 YVal = atof(YString);
					 }
					 if(line[i] == 'R' || line[i] == 'r') {
					 RFirst = i;
					 for (j = 0; (line[i+j] != '\n' && line[i+j] != ' ' && line[i+j]); j++) {RCLen = j;}
					 for (k = 0; k < 16; k++) {*(RString+k) = line[i+1+k];}
					 RVal = atof(RString);
					 }
					 if(line[i] == 'I' || line[i] == 'i') {
					 IFirst = i;
					 for (j = 0; (line[i+j] != '\n' && line[i+j] != ' ' && line[i+j]); j++) {ICLen = j;}
					 for (k = 0; k < 16; k++) {*(IString+k) = line[i+1+k];}
					 IVal = atof(IString);
					 }
					 if(line[i] == 'J' || line[i] == 'j') {
					 JFirst = i;
					 for (j = 0; (line[i+j] != '\n' && line[i+j] != ' ' && line[i+j]); j++) {JCLen = j;}
					 for (k = 0; k < 16; k++) {*(JString+k) = line[i+1+k];}
					 JVal = atof(JString);
					 }
					 if(line[i] != '\n') {
					 //printf("%c", line[i]);
					 }
					// printf( "G0%d (X%.1f Y%.1f) X%.1f Y%.1f R%.1f I%.1f J%.1f \n", GVal,XLast,YLast,XVal,YVal,RVal,IVal,JVal);
      }
      printf( "G0%d (X%.1f Y%.1f) X%.1f Y%.1f R%.1f I%.1f J%.1f \n", GVal,XLast,YLast,XVal,YVal,RVal,IVal,JVal);
      if (GVal == 2 || GVal == 3 ) {
      do_CalculateAndPrint(GVal,XLast,YLast,XVal,YVal,RVal,IVal,JVal,1,fp,l);
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

void do_CalculateAndPrint(char gType, float start_x, float start_y, float end_x, float end_y, float radius, float GI, float GJ, char rez, FILE *fp, int l) {
float resolution = ((float)1/rez);

//##################### START REDO ################################
				// I am passed gType, (Start X, Start Y), (End X, End Y), radius, resolution.
				// The important aspects are StartX StartY EndX EndY Radius.
				// According to the circle, we are given 2 points on an edge, and a radius.
				// From this, we can determine the values we need.
				// First, seeing as we have no I or J coordinates, we need to determine the centre point of the circle.
				// We can do this by using 
				// q = sqrt((x2-x1)^2 + (y2-y1)^2)
				// x3 = (x1+x2)/2
				// y3 = (y1+y2)/2
				// x = [x3] ± sqrt(r^2-([q]/2)^2)*([y1]-[y2])/q
        // y = [y3] ± sqrt(r^2-([q]/2)^2)*([x2]-[x1])/q  
        // where (x,y) is the centre point. q is the distance between the two points. x1,y1 is the first point, x2,y2 is the second point. x3 and y3 are the linear distances between the start and end.
        float centre_x;
        float centre_y;
        //float centre_x_neg;
        //float centre_y_neg;
        centre_x = GI;
centre_y = GJ;
        float RDOPS = sqrt(((centre_x+start_x)-(start_x))*((centre_x+start_x)-(start_x))+((centre_y+start_y)-(start_y))*((centre_y+start_y)-(start_y)));
            //float RDOPS = sqrt(((centre_x-start_x)*(centre_x-start_x))+((centre_y-start_y)*(centre_y-start_y)));
float DOPS = sqrt(((end_x-start_x)*(end_x-start_x))+((end_y-start_y)*(end_y-start_y)));
float dist_x = (start_x+end_x)/2;
float dist_y = (start_y+end_y)/2;
printf("RDOPS-err-\n");
printf("RDOPS-%f-\n", RDOPS);
//printf("RDOPS-%f-\n", DOPS);

        // Find first point on line so I can calculate the radius.
int q;
        //for (q = centre_x; (sqrt((RDOPS*RDOPS)-((q-centre_x)*(q-centre_y)))); q--) {
       // printf("<<%d>>\n", q);
       // }
        
        float xval = sqrt((radius*radius)-((m-centre_x)*(m-centre_y))); //bottom
        

if(GI>0) {
centre_x = GI;
} else {
centre_x = dist_x + (sqrt((radius*radius)-((DOPS/2)*(DOPS/2)))*(start_y-end_y))/DOPS;
}
if(GJ>0) {
centre_y = GI;
} else {
centre_y = dist_y + (sqrt((radius*radius)-((DOPS/2)*(DOPS/2)))*(start_x-end_x))/DOPS;
}
        // we now have our centre point for our circles (clockwise and counterclockwise)
        // now to generate the linear movements.
        
int AmountSteps = find_LinearSteps(centre_x, centre_y, radius, rez);
//for (m = 0; m <= AmountSteps; m++) { 
//float centre_x = centre_x_neg;
//float centre_y = centre_y_pos;
printf("Test>%f\n",centre_y);
for(m = -1000; m < 1000; m++) { //for all values of x
float xval = sqrt((radius*radius)-((m-centre_x)*(m-centre_y))); //bottom
float otherxval = 0-sqrt((radius*radius)-((m-centre_x)*(m-centre_y)));
//printf("Test>%f\n",centre_y);
//if(m < centre_x-radius) {
//write_Out(m, line_y, l);
//}
//if((xval+centre_y < line_y)&&(m < centre_x)) {
//write_Out(m, line_y, l);
//}
if((xval+centre_y > start_y)&&(m < centre_x)) { // where bottom arc + center < start_y
write_Out(m, xval+centre_y, l, fp);
}
if (otherxval+centre_y < start_y){
write_Out(m, otherxval+centre_y, l, fp);
}
if((xval+centre_y < start_y)&&(m > centre_x)) {
write_Out(m, xval+centre_y, l, fp);
}
//if((xval+centre_y < line_y)&&(i > centre_x)) {
//write_Out(m, line_y, l);
//}
//if(m > centre_x+radius) {
//write_Out(m, line_y, l);
//}
}
}


char do_Determine(float start_x, float start_y, float end_x, float end_y) {
if (start_x < end_x) { x_positive = 1; } else { x_positive = 2; };
if (start_y < end_y) { y_positive = 4; } else { y_positive = 7; };
return (x_positive+y_positive);
}

void write_Out(float GX, float GY, int l, FILE *fp) {
fprintf(fp, "G01 X%.3f Y%.3f // G0? at line %d\n", GX, GY, l);
}

int find_LinearSteps(float centre_x, float centre_y, float radius, char rez) {
if(!rez) {rez=1;}
float resolution = ((float)1/rez);
int stepcounter = 0;
int pyy;
for(pyy = -1000; pyy < 1000; pyy++) {
   if((sqrt((radius*radius)-((pyy-centre_x)*(pyy-centre_y)))) > 0) {
   stepcounter++;
   }
   }
   //printf("%d\n",stepcounter);
return stepcounter;
}