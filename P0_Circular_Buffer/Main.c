/*----------------------------------------------------------------------------*/
/*                                 Includes                                   */
/*----------------------------------------------------------------------------*/

#include "circular_buffer.h"
#include <stdio.h>

int main( void )
{
    unsigned char data_i;       /* Input Data */
    unsigned char data_o;       /* Output Data */
    unsigned char array[6];     /* The array is defined to hold 6 unsigned char elements. */
    AppBuffer_Buffer  CircBuffer;

    /* Initialization */
    CircBuffer.Buffer = array;
    CircBuffer.Elements = 6u; /* Specifies that the circular buffer will manage 6 elements. */
    AppBuffer_initBuffer( &CircBuffer );
    AppBuffer_isBufferEmpty( &CircBuffer);

    /* Writing to the buffer */
    data_i = 100;
    AppBuffer_writeData( &CircBuffer, data_i);
    data_i = 120;
    AppBuffer_writeData( &CircBuffer, data_i );
    data_i = 200;
    AppBuffer_writeData( &CircBuffer, data_i);
    data_i = 20;
    AppBuffer_writeData( &CircBuffer, data_i);
    data_i = 50;
    AppBuffer_writeData( &CircBuffer, data_i);
    data_i = 15;
    AppBuffer_writeData( &CircBuffer, data_i);

    /* Print buffer contents */
    for (int i = 0; i < CircBuffer.Elements; i++) {
        printf("Buffer[%d]: value = %d \n", i, CircBuffer.Buffer[i]);
    }

    /* Read all the messages */
    while( AppBuffer_isBufferEmpty( &CircBuffer ) == 0u )
    {
        data_o = AppBuffer_readData( &CircBuffer );
        printf( "data read from the queue %d\n", data_o);
    }

    AppBuffer_isBufferEmpty( &CircBuffer);

    return 0
}