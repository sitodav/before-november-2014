#include <string.h>
#include <dos.h>


void Stuff(char ch)
{


unsigned far *kbufbegin;
unsigned far *kbufend;
unsigned far *kbuftail;
unsigned far *kbuffer;

/* set up buffer pointers */
FP_SEG( kbufbegin ) = FP_SEG( kbufend ) = FP_SEG( kbuftail )
= FP_SEG( kbuffer ) = 0x40;
FP_OFF( kbufbegin ) = 0x80;
FP_OFF( kbufend ) = 0x82;
FP_OFF( kbuftail ) = 0x1c;
disable( );
FP_OFF( kbuffer ) = *kbuftail;
*kbuffer++ = ch;
if ( FP_OFF( kbuffer ) >= *kbufend )
FP_OFF( kbuffer ) = *kbufbegin;
*kbuftail = FP_OFF( kbuffer );
enable( );
} /*--Stuff( )---------------*/
int main( int argc, char *argv[] )
{
short i, j;



char temp[200];
if ( argc > 1 )
for ( i=1; i < argc; ++i )
{
strcpy( temp, argv[i] );
switch ( temp[0] )
{
case '0':
ch = atoi( temp );
Stuff( ch );
break;
default:
for ( j=0; temp[j] != '"' && temp[j]; ++j )
Stuff( temp[j] );
}
}
else
{
printf( "Use: STUFFKEY 027 013 a b \"hi there\"<ENTER>\n");
printf( "Parms that start with zero are ascii codes\n" );
printf("Generaly only useful called from inside a batch file\n");
}
return(0);
} /*--main( )----------*/
