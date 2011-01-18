#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define section1value (circle_y-sqrt((radius*radius)-(((float)m-circle_x)*((float)m-circle_x))))
#define section2value (circle_y+sqrt((radius*radius)-(((float)m-circle_x)*((float)m-circle_x))))
#define jdiscover for(j=0;(line[i+j]!='\n'&&line[i+j]!=' '&&line[i+j]);j++){

int find_LinearSteps(float centre_x, float centre_y, float radius, char rez);
void write_Out(float GX, float GY, int l, FILE *fp, char gType);
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
				memset(RString, 0, 16);memset(IString, 0, 16);memset(JString, 0, 16);
				GCLen = 0;XCLen = 0;YCLen = 0;RCLen = 0;ICLen = 0;JCLen = 0;GVal = 0;RVal = 0;JVal = 0;IVal = 0;
				
				for (i = 0; line[i]; i++) {
				 if(line[i]=='G'||line[i]=='g'){GFirst=i; jdiscover GCLen=j;}for(k=0;k<16;k++){*(GString+k)=line[i+1+k];}GVal=atoi(GString);}
				 if(line[i]=='X'||line[i]=='x'){XFirst=i; jdiscover XCLen=j;}for(k=0;k<16;k++){*(XString+k)=line[i+1+k];}XVal=atof(XString);}
				 if(line[i]=='Y'||line[i]=='y'){YFirst=i; jdiscover YCLen=j;}for(k=0;k<16;k++){*(YString+k)=line[i+1+k];}YVal=atof(YString);}
				 if(line[i]=='R'||line[i]=='r'){RFirst=i; jdiscover RCLen=j;}for(k=0;k<16;k++){*(RString+k)=line[i+1+k];}RVal=atof(RString);}
				 if(line[i]=='I'||line[i]=='i'){IFirst=i; jdiscover ICLen=j;}for(k=0;k<16;k++){*(IString+k)=line[i+1+k];}IVal=atof(IString);}
				 if(line[i]=='J'||line[i]=='j'){JFirst=i; jdiscover JCLen=j;}for(k=0;k<16;k++){*(JString+k)=line[i+1+k];}JVal=atof(JString);}
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

        float circle_x;
        float circle_y;
        float y_val;
        circle_x = GI;
        circle_y = GJ;
        printf("XCentre%f, YCentre%f, XStart%f, YStart%f\n", circle_x, circle_y, start_x, start_y);
    float RDOPS = sqrt((((circle_x)-(start_x))*((circle_x)-(start_x)))+(((circle_y)-(start_y))*((circle_y+start_y)-(start_y))));
            //float RDOPS = sqrt(((centre_x-start_x)*(centre_x-start_x))+((centre_y-start_y)*(centre_y-start_y)));
//float DOPS = sqrt(((end_x-start_x)*(end_x-start_x))+((end_y-start_y)*(end_y-start_y)));
//float dist_x = (start_x+end_x)/2;
//float dist_y = (start_y+end_y)/2;

printf("Radius <%f>\n", RDOPS);
radius = RDOPS;

int firstquadcount = 0;
int secondquadcount = 0;
int thirdquadcount = 0;
int fourthquadcount = 0;

for(m = 20; m < 50; m++) { //for all values of x

if(section1value>start_y) {
if(m < circle_x) {firstquadcount++;};
if(m > circle_x) {fourthquadcount++;};}

if(section2value>circle_y) {
if(m < circle_x) {secondquadcount++;};
if(m > circle_x) {thirdquadcount++;};}

//printf("Test2>m%d,rad%.2f,cx%.2f,cy%.2f,s1(%.2f),s2(%.2f)\n",m,radius,circle_y,circle_x, section1value,section2value);

}
// Test>7,11,11,7
printf("Test>%d,%d,%d,%d\n",firstquadcount, secondquadcount, thirdquadcount, fourthquadcount);
int fq1, fq2, fq3, fq4;
for(fq1 = 0; fq1 < firstquadcount; fq1++) {
m = (start_x-fq1);
y_val = section1value;
write_Out(m, y_val, l, fp, gType);            //write_Out(xval, yval, l, fp)
}
for(fq2 = secondquadcount; fq2 > 0; fq2--) {
m = (circle_x-fq2);
y_val = section2value;
write_Out(m, y_val, l, fp, gType);            //write_Out(xval, yval, l, fp)
}
for(fq3 = 0; fq3 <= thirdquadcount; fq3++) {
m = (circle_x+fq3);
y_val = section2value;
write_Out(m, y_val, l, fp, gType);            //write_Out(xval, yval, l, fp)
}
for(fq4 = fourthquadcount-1; fq4 > 0; fq4--) {
m = (end_x+fq4);
y_val = section1value;
write_Out(m, y_val, l, fp, gType);            //write_Out(xval, yval, l, fp)
}
}

void write_Out(float GX, float GY, int l, FILE *fp, char gType) {
fprintf(fp, "G01 X%.3f Y%.3f // G0%d at line %d\n", GX, GY, gType,l);
}