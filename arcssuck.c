#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void write_Out(float GX, float GY, FILE *fp, char gType, int l) {
fprintf(fp, "G01 X%.3f Y%.3f // G0%d at line %d.\n", GX, GY, gType, l);
}

void GCIRCLE(int directy, float start_x, float start_y, float end_x, float end_y, float circle_x, float circle_y, FILE *fp, int l) {
float aA, aB;
				float dsX = (start_x - circle_x);
				float dsY = (start_y - circle_y);
				float deX = (end_x - circle_x);
				float deY = (end_y - circle_y);
        if(directy == 2) {
				aA = atan2(deY, deX);
				aB = atan2(dsY, dsX);
				} else {
				aA = atan2(dsY, dsX);
        aB = atan2(deY, deX);
				}
				if (aB <= aA) {
				aB += 3.141592653589793238*2;
				}
				float radius = sqrt((dsX*dsX) + (dsY*dsY));
				float a = aB - aA;
				float length = radius*a;
				int steps, s, step;
				steps = (int) ceil(length / 1);
				for (s = 1; s <= steps; s++) {
				int step;
				if(directy == 2) {
				step = steps - s;
				} else {
								step = s;
				}
					float nextX = circle_x + radius * cos(aA+a*((float) step / steps));
					float nextY = circle_y + radius * sin(aA+a*((float) step / steps));
					write_Out(nextX, nextY, fp, 2, l);
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
     }
      if (GVal == 2 || GVal == 3 ) {
      fprintf(fp, "//G0%d (X%.1f Y%.1f) X%.1f Y%.1f R%.1f I%.1f J%.1f \n", GVal,XLast,YLast,XVal,YVal,RVal,IVal,JVal);
      float RDOPS = sqrt(((XVal-XLast)*(XVal-XLast))+((YVal-YLast)*(YVal-YLast)));
      if(RVal) {
      float x2 = XVal;
      float x1 = XLast;
      float y2 = YVal;
      float y1 = YLast;
      float x_cen_pos = 0;
      float x_cen_neg = 0;
      float y_cen_pos = 0;
      float y_cen_neg = 0;
      float r1 = RVal;
      float r2 = RVal;
      float d = sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
       
           x_cen_pos = ((x2+x1)/2) + (((x2-x1)*((r1*r1)-(r2*r2))/(2*(d*d))) + (((y2-y1)/(2*(d*d))*(sqrt((((r1+r2)*(r1+r2))-(d*d))*((d*d)-((r2-r1)*(r2-r1))))))));
           x_cen_neg = ((x2+x1)/2) - (((x2-x1)*((r1*r1)-(r2*r2))/(2*(d*d))) + (((y2-y1)/(2*(d*d))*(sqrt((((r1+r2)*(r1+r2))-(d*d))*((d*d)-((r2-r1)*(r2-r1))))))));
           y_cen_pos = ((y2+y1)/2) + (((y2-y1)*((r1*r1)-(r2*r2))/(2*(d*d))) + (((x2-x1)/(2*(d*d))*(sqrt((((r1+r2)*(r1+r2))-(d*d))*((d*d)-((r2-r1)*(r2-r1))))))));
           y_cen_neg = ((y2+y1)/2) - (((y2-y1)*((r1*r1)-(r2*r2))/(2*(d*d))) + (((x2-x1)/(2*(d*d))*(sqrt((((r1+r2)*(r1+r2))-(d*d))*((d*d)-((r2-r1)*(r2-r1))))))));
           
       if(GVal == 2) {
       IVal = x_cen_pos;
       JVal = y_cen_neg;
       } else {
       IVal = x_cen_neg;
       JVal = y_cen_pos;
       }
      }      
      GCIRCLE(GVal,XLast,YLast,XVal,YVal,IVal,JVal,fp,l);
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
   return 0;
}