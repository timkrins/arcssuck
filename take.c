#include <stdio.h>
#include <stdlib.h>

//what needs to be a * str?
int i, j, k, GCLen, XCLen, YCLen, RCLen, GFirst, XFirst, YFirst, RFirst, GVal;

char GString[10], XString[16], YString[16], RString[16];

float XVal, YVal, RVal;

int main ( void )
{
   static const char filename[] = "gcodes.txt";
   FILE *file = fopen ( filename, "r" );
   if (file)
   {
      char line [ 128 ];
      while ( fgets(line, sizeof line, file)) /* read a line */
      {
				//fputs ( line, stdout ); /* write the line */
				for (i = 0; line[i]; i++) {
				//entire line is now in memory
				
					 if(line[i] == 'G') {
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
					 
					 if(line[i] == 'X') {
					 XFirst = i;
					 for (j = 0; line[i+j] != ' '; j++) {
					 XCLen = j;
					 }
					 for (k = 0; k < 16; k++) {
					 *(XString+k) = line[i+1+k];
					 }
					 XVal = atof(XString);
					 }
					 
					 if(line[i] == 'Y') {
					 YFirst = i;
					 for (j = 0; line[i+j] != ' '; j++) {
					 YCLen = j;
					 }
					 for (k = 0; k < 16; k++) {
					 *(YString+k) = line[i+1+k];
					 }
					 YVal = atof(YString);
					 }
					 
					 
					 if(line[i] == 'R') {
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
      printf("\nG(%d)%d X(%d)%d Y(%d)%d R(%d)%d. \nG=%d, X=%.3f, Y=%.3f, R=%.3f\n\n", GCLen, GFirst, XCLen, XFirst, YCLen, YFirst, RCLen, RFirst, GVal, XVal, YVal, RVal);
      }
      fclose (file);
   }
   else
   {
      perror (filename); /* why didn't the file open? */
   }
   return 0;

}