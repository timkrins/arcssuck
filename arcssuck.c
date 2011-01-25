#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#define section1value (circle_y-sqrt((radius*radius)-(((float)m-circle_x)*((float)m-circle_x))))
//#define section4value (circle_x-sqrt((radius*radius)-(((float)m-circle_y)*((float)m-circle_y))))
//#define section2value (circle_y+sqrt((radius*radius)-(((float)m-circle_x)*((float)m-circle_x))))

void write_Out(float GX, float GY, FILE *fp, char gType, int l) {
fprintf(fp, "G01 X%.3f Y%.3f // G0%d at line %d.\n", GX, GY, gType, l);
}

void G02GO(float start_x, float start_y, float end_x, float end_y, float circle_x, float circle_y, FILE *fp, int l) {
//assumes G02

				float aX = (start_x - circle_x);
				float aY = (start_y - circle_y);
				float bX = (end_x - circle_x);
				float bY = (end_y - circle_y);

				float angleA = atan2(bY, bX);
				float angleB = atan2(aY, aX);

				//float	angleA = atan2(aY, aX);
				//float	angleB = atan2(bY, bX);
					
					
				if (angleB <= angleA) angleB += 2 * M_PI;
				float angle = angleB - angleA;

				float radius = sqrt(aX * aX + aY * aY);
				float length = radius * angle;
				int steps, s, step;
				steps = (int) ceil(length / 1);

				for (s = 1; s <= steps; s++) {
									step = steps - s; // Work backwards for CW
									//step = s; // Work backwards for CW
					float newPointX = circle_x + radius * cos(angleA + angle * ((float) step / steps));
					float newPointY = circle_y + radius * sin(angleA + angle * ((float) step / steps));
					write_Out(newPointX, newPointY, fp, 2, l);    //write_Out(xval, yval, l, fp)
				}
			}
			
			
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
      
      fprintf(fp, "//ArcsSuck 0.1 by Tim K. Output of file \""); fprintf(fp, argv[1]); fprintf(fp, "\"\n\n");
      
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
      G02GO(XLast,YLast,XVal,YVal,IVal,JVal,fp,l);
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