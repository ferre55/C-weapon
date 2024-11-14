/**
 * \file       Circular_buffer.c
 * \author     Jennifer Reynaga
 * \brief      Implementation for Circular_buffer
 */

/*----------------------------------------------------------------------------*/
/*                                 Includes                                   */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "circular_buffer.h"

/*----------------------------------------------------------------------------*/
/*                               Local defines                                */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                       Declaration of local functions                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                     Implementation of global functions                     */
/*----------------------------------------------------------------------------*/

void AppBuffer_initBuffer( AppBuffer_Buffer *hbuffer )
{
    hbuffer ->  Full = FALSE;       /* The buffer is not full */
    hbuffer ->  Empty = TRUE;       /* The buffer doesnt have any elements */
    hbuffer ->  Head = 0;           /* Head in the position 0 */
    hbuffer ->  Tail = 0;           /* Tail in the position 0 */
    hbuffer ->  Head_wrap = 0;      /* Flag when Head is wrap */
    hbuffer ->  Tail_wrap = 0;      /* Flag when Tail is wrap */
}

void AppBuffer_writeData( AppBuffer_Buffer *hbuffer, unsigned char data )
{
    /* If the buffer is full, we cannot add more elements */ 
    /* When the two pointers are equal and the wrap flags are different, then FIFO is FULL */
    if ( ((hbuffer -> Tail) == (hbuffer -> Head)) && (hbuffer -> Head_wrap != hbuffer -> Tail_wrap))
    {
        return; /* Exit from the function */
    }
    else
    {
        /* If the buffer isn't FULL, we can add more elements */
        hbuffer -> Buffer[hbuffer -> Head] = data; /* Save the data */
        
        if((hbuffer -> Head + 1) == (hbuffer -> Elements))
        {
            hbuffer -> Head_wrap = 1;
            hbuffer -> Head = 0;
        } 
        else
        {
            hbuffer -> Head = (hbuffer -> Head + 1) % hbuffer -> Elements; /* Move the Head */
        }
    }
}


unsigned char AppBuffer_readData( AppBuffer_Buffer *hbuffer )
{
    unsigned char result;

    /* Evaluate if the Buffer is empty */
    if (AppBuffer_isBufferEmpty(hbuffer)==TRUE)
    {   
        result = 0; /* The buffer is empty */
    }
    else
    {
        /* If is not empty */
        result = hbuffer -> Buffer[ hbuffer -> Tail];

        if((hbuffer -> Tail + 1) == ( hbuffer -> Elements))
        {
            hbuffer -> Tail_wrap = 1;
            hbuffer -> Tail = 0; /* Set flag Tail wrap */ 
             
        }
        else
        {
            hbuffer -> Tail = (hbuffer -> Tail + 1) % hbuffer -> Elements;
        }
    }
    return result;
}

unsigned char AppBuffer_isBufferEmpty( AppBuffer_Buffer *hbuffer )
{
    unsigned char status;

    if(((hbuffer -> Tail) == (hbuffer -> Head)) && ((hbuffer -> Head_wrap) == (hbuffer -> Tail_wrap)))
    {
        status = TRUE;  /* Buffer is empty */
    }
    else
    {
        status = FALSE; /* Buffer is NOT empty */
    }

    return status;    

}

