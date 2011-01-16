#include <stdio.h>

int i, j, k, GCLen, XCLen, YCLen, RCLen, GFirst, XFirst, YFirst, RFirst;
char *GBuilder[30];
volatile float GVal, XVal, YVal, RVal;
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
					 // GFirst will be 0(+1), GCLen will be 2, I want to copy 2 values, starting at 0. atof takes (const char * str)
					 //GVal = atof(GBuilder);
					 }
					 
					 if(line[i] == 'X') {
					 XFirst = i;
					 for (j = 0; line[i+j] != ' '; j++) {
					 XCLen = j;
					 }
					 }
					 
					 if(line[i] == 'Y') {
					 YFirst = i;
					 for (j = 0; line[i+j] != ' '; j++) {
					 YCLen = j;
					 }
					 }
					 
					 
					 if(line[i] == 'R') {
					 RFirst = i;
					 for (j = 0; (line[i+j] != '\n' && line[i+j] != ' ' && line[i+j]); j++) {
					 RCLen = j;
					 }
					 }
					 
					 if(line[i] != '\n') {
					 printf("%c", line[i]);
					 }
					 
						 
         
      }
      printf("\nG(%d)%d X(%d)%d Y(%d)%d R(%d)%d.\n\n", GCLen, GFirst, XCLen, XFirst, YCLen, YFirst, RCLen, RFirst);
      printf(*GBuilder);
      }
      fclose (file);
   }
   else
   {
      perror (filename); /* why didn't the file open? */
   }
   return 0;
}