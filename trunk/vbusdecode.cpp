//sample cat raw.log | ./a.out  0,15,0.1 2,15,0.1 4,15,0.1 6,15,0.1 8,7,1 9,7,1 10,8,1
#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;

int model;
unsigned char frame[4];
unsigned char allframes[256];

void giveresults(char parray[])
{
	int offset = atoi(strtok (parray,","));
// line below causes segfault
	int length = atoi(strtok (NULL, ","));
	float multiplier =  atof(strtok (NULL, ","));
	float f = (allframes[offset] + (allframes[offset+1]*0x100)) * multiplier;
	printf ("%.2f ",f);
}

int decodeheader()
{
	char  buffer[10];
	unsigned char a;
	unsigned char b;
	if ( fgets(buffer, 10, stdin) != NULL)
	{
		a = buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6] + buffer[7];
		b = ~ a;
		a = buffer[8];
	}
	if (  a == b )
	{
		model = buffer[3] * 0x100  + buffer[2];
		a = buffer[7];
		return a;
	}
	else
	{
		return 0;
	}
}

unsigned char decodeframe(int x)
{
	char  buffer[7];
	unsigned char a;
	unsigned char b;
	if ( fgets(buffer, 7, stdin) != NULL)
	{
		a = buffer[0]  + buffer[1]  + buffer[2] + buffer[3] + buffer[4] ;
		b =  (( ~a | 0x80) - 0x80 ) ;
		a = buffer[5];
		if (  a == b )
		{
			frame[1] =  buffer[1] + (( buffer[4] >> 1 & 0x01 ) * 0x80 ) ;
			frame[0] =  buffer[0] + (( buffer[4] & 0x01 ) * 0x80 ) ; 
			frame[3] =  buffer[3] + (( buffer[4] >> 3 & 0x01 ) * 0x80 ) ; 
			frame[2] =  buffer[2] + (( buffer[4] >> 2 & 0x01 ) * 0x80 )  ;
		}
		else
		{
			frame[0] = 0;
			frame[1] = 0;
			frame[2] = 0;
			frame[3] = 0;
		}


	}

}

int main(int argc, char* argv[])
{
	char  buffer[2];
	unsigned char a;	
	int framecount;
	while ( fgets(buffer, 2, stdin) != NULL)
	{
		a = buffer[0];
		if ( a ==  0xAA) {
			framecount  =    decodeheader();
			if (framecount !=0 ) 
			{
				for ( int x = 0; x < framecount ; x++ ) 
				{
					decodeframe(x);
					allframes[4*x] =frame[0];
					allframes[(4*x)+1] =frame[1];
					allframes[(4*x)+2] =frame[2];
					allframes[(4*x)+3] =frame[3];
				}
				for ( int y = 0; y < framecount ; y++ )
				{ 
					printf("%02x",allframes[(4*y)+0]);
					printf("%02x ",allframes[(4*y)+1]);
					printf("%02x",allframes[(4*y)+2]);
					printf("%02x ",allframes[(4*y)+3]);
				}

				printf("\n");
				for(int i = 1; i < argc; i++)
				{
					giveresults(argv[i]);
				}
				printf("\n");
			}
		}
	}
	
	return 0; 
}

