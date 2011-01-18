#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define section1value (circle_y-sqrt((radius*radius)-(((float)m-circle_x)*((float)m-circle_x))))
#define section4value (circle_x-sqrt((radius*radius)-(((float)m-circle_y)*((float)m-circle_y))))
#define section2value (circle_y+sqrt((radius*radius)-(((float)m-circle_x)*((float)m-circle_x))))

int find_LinearSteps(float centre_x, float centre_y, float radius, char rez);
void write_Out(float GX, float GY, int l, FILE *fp, char gType, char section);
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
				memset(RString, 0, 16);memset(IString, 0, 16);memset(JString, 0, 16);memset(GString, 0, 10);memset(XString, 0, 16);memset(YString, 0, 16);
				GCLen = 0;XCLen = 0;YCLen = 0;RCLen = 0;ICLen = 0;JCLen = 0;GVal = 0;RVal = 0;JVal = 0;IVal = 0;
				
				for (i = 0; line[i]; i++) {
				 if(line[i]=='G'||line[i]=='g'){GFirst=i; for(j=0;(line[i+j]!='\n'&&line[i+j]!=' '&&line[i+j]);j++){ GCLen=j;}for(k=0;k<16;k++){*(GString+k)=line[i+1+k];}GVal=atoi(GString);}
				 if(line[i]=='X'||line[i]=='x'){XFirst=i; for(j=0;(line[i+j]!='\n'&&line[i+j]!=' '&&line[i+j]);j++){ XCLen=j;}for(k=0;k<16;k++){*(XString+k)=line[i+1+k];}XVal=atof(XString);}
				 if(line[i]=='Y'||line[i]=='y'){YFirst=i; for(j=0;(line[i+j]!='\n'&&line[i+j]!=' '&&line[i+j]);j++){ YCLen=j;}for(k=0;k<16;k++){*(YString+k)=line[i+1+k];}YVal=atof(YString);}
				 if(line[i]=='R'||line[i]=='r'){RFirst=i; for(j=0;(line[i+j]!='\n'&&line[i+j]!=' '&&line[i+j]);j++){ RCLen=j;}for(k=0;k<16;k++){*(RString+k)=line[i+1+k];}RVal=atof(RString);}
				 if(line[i]=='I'||line[i]=='i'){IFirst=i; for(j=0;(line[i+j]!='\n'&&line[i+j]!=' '&&line[i+j]);j++){ ICLen=j;}for(k=0;k<16;k++){*(IString+k)=line[i+1+k];}IVal=atof(IString);}
				 if(line[i]=='J'||line[i]=='j'){JFirst=i; for(j=0;(line[i+j]!='\n'&&line[i+j]!=' '&&line[i+j]);j++){ JCLen=j;}for(k=0;k<16;k++){*(JString+k)=line[i+1+k];}JVal=atof(JString);}
				// printf( "G0%d (X%.1f Y%.1f) X%.1f Y%.1f R%.1f I%.1f J%.1f \n", GVal,XLast,YLast,XVal,YVal,RVal,IVal,JVal);
				
      }
      //printf( "G0%d (X%.1f Y%.1f) X%.1f Y%.1f R%.1f I%.1f J%.1f \n", GVal,XLast,YLast,XVal,YVal,RVal,IVal,JVal);
      if (GVal == 2 || GVal == 3 ) {
      fprintf(fp, "//G0%d (X%.1f Y%.1f) X%.1f Y%.1f R%.1f I%.1f J%.1f \n", GVal,XLast,YLast,XVal,YVal,RVal,IVal,JVal);
      do_CalculateAndPrint(GVal,XLast,YLast,XVal,YVal,RVal,IVal,JVal,1,fp,l);
      fprintf(fp, "G01 X%.1f Y%.1f // End of line\n", XVal,YVal);
      XLast = XVal;
      YLast = YVal;
      }
      else
      {
      fprintf(fp, line);
      XLast = XVal;
      YLast = YVal;
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

//fprintf(fp, "//Test3>rad%.2f\n",radius);
//printf("Radius <%f>\n", RDOPS);
if(radius > 0) {
//jval shouldnt be zero
if(start_x>end_x) {circle_x = end_x;fprintf(fp, "//startx(%.2f)endx(%.2f). circlex has been set to endx\n",start_x,end_x);};
if(start_x<end_x) {circle_x = start_x;fprintf(fp, "//startx(%.2f)endx(%.2f). circlex has been set to startx\n",start_x,end_x);};
if(start_y<end_y) {circle_y = start_y;fprintf(fp, "//starty(%.2f)endy(%.2f). circley has been set to starty\n",start_y,end_y);};
//if(start_y>end_y) {circle_y = end_y;fprintf(fp, "//starty(%.2f)endy(%.2f). circley has been set to endy\n",start_y,end_y);};
if(start_y>end_y) {circle_y = end_y+radius;fprintf(fp, "//starty(%.2f)endy(%.2f). circley has been set to endy+rad\n",start_y,end_y);};

//circle_y = end_y;
} else {
radius = RDOPS;
};

fprintf(fp, "//Test3>rad%.2f\n",radius);
int firstquadcount = 0;
int secondquadcount = 0;
int thirdquadcount = 0;
int fourthquadcount = 0;



int fq1=0, fq2=0, fq3=0, fq4=0;

for(fq1 = circle_x; fq1 > (circle_x-radius); fq1--) {
m = fq1;
if((fq1 < circle_x)&&(start_x<circle_x)&&((fq1<start_x)||(fq1>end_x))) {
y_val = section1value;
fprintf(fp, "//start(%.2f)circ(%.2f)sectionvalue(%.2f)jval(%.2f)\n",start_x, circle_x, section1value, circle_y);
write_Out(m, y_val, l, fp, gType, 1);    //write_Out(xval, yval, l, fp)
}
}

for(fq2 = circle_x-radius+1; fq2 < circle_x; fq2++) { // seems to be counting in the wrong direction.
m = fq2;
if((fq2 < circle_x)&&(start_x<circle_x)&&((fq2<end_x)||(fq2>start_x))) {
y_val = section2value;
fprintf(fp, "//start(%.2f)circ(%.2f)sectionvalue(%.2f)jval(%.2f)\n",start_x, circle_x, section2value, circle_y);
write_Out(m, y_val, l, fp, gType, 2);    //write_Out(xval, yval, l, fp)
}
}

for(fq3 = circle_x; fq3 < circle_x+radius; fq3++) {
m = fq3;
if((fq3 > circle_x)&&((fq3<end_x)||(fq3>start_x))) {
y_val = section2value;
fprintf(fp, "//start(%.2f)circ(%.2f)sectionvalue(%.2f)jval(%.2f)\n",start_x, circle_x, section2value, circle_y);
write_Out(m, y_val, l, fp, gType, 3);    //write_Out(xval, yval, l, fp)
}
}

for(fq4 = circle_x+radius; fq4 > circle_x; fq4--) {
m = fq4;
if((fq4 > circle_x)&&((fq4<start_x)||(fq4>end_x))) {
y_val = section1value;
fprintf(fp, "//start(%.2f)circ(%.2f)sectionvalue(%.2f)jval(%.2f)\n",start_x, circle_x, section1value, circle_y);
write_Out(m, y_val, l, fp, gType, 4);    //write_Out(xval, yval, l, fp)
}
}
}

void write_Out(float GX, float GY, int l, FILE *fp, char gType, char section) {
fprintf(fp, "G01 X%.3f Y%.3f // G0%d at line %d, Section%d.\n", GX, GY, gType,l, section);
}