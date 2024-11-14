/* ---- Headerswitch on (for prevention of nested includes) ------------------*/

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

/**
 * \file       Circular_buffer.h
 * \author     Jennifer Reynaga
 * \brief      Header file for the Circular_buffer.
 */

/*----------------------------------------------------------------------------*/
/*                                  Includes                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                             Defines and macros                             */
/*----------------------------------------------------------------------------*/
#define FALSE                0u
#define TRUE                 1u

/*----------------------------------------------------------------------------*/
/*                                 Data types                                 */
/*----------------------------------------------------------------------------*/
typedef struct
{
    unsigned char *Buffer;              /*!< Array of values */ 
    unsigned long Elements;             /*!< How many elements we have in the buffer */ 
    unsigned long Head;                 /*!< Write - Front of the array */
    unsigned long Tail;                 /*!< Read - Rear of the array*/ 
    unsigned char Empty;                /*!< Flag Empty  */ 
    unsigned char Full;                 /*!< Flag Full */
    unsigned char Head_wrap;            /*!< Flag when Head is wrap */
    unsigned char Tail_wrap;            /*!< Flag when Head is wrap */
} AppBuffer_Buffer;

/*----------------------------------------------------------------------------*/
/*                           Declaration of functions                         */
/*----------------------------------------------------------------------------*/


/** 
 * @brief Initializes the buffer. 
 * 
 * This function sets up the buffer structure, preparing it for use.
 *
 *  @param hbuffer Pointer to the buffer structure to initialize. 
*/
void AppBuffer_initBuffer( AppBuffer_Buffer *hbuffer );

/** 
 * @brief Writes data to the buffer.
 *
 * This function writes a single byte of data to the buffer. 
 * 
 * @param hbuffer Pointer to the buffer structure. 
 * @param data The data byte to write to the buffer. 
*/ 
void AppBuffer_writeData(AppBuffer_Buffer *hbuffer, unsigned char data); 

/** 
* @brief Reads data from the buffer. 
*
* This function reads a single byte of data from the buffer. 
* @param hbuffer Pointer to the buffer structure. 
* @return unsigned char The data byte read from the buffer. 
*/
unsigned char AppBuffer_readData( AppBuffer_Buffer *hbuffer );

/**
 * @brief Function that reports if the buffer is empty.
*/
/** 
* @brief Checks if the buffer is empty.
* 
* This function checks whether the buffer is empty.
* @param hbuffer Pointer to the buffer structure.
* @return unsigned char TRUE if the buffer is empty, FALSE otherwise. 
*/
unsigned char AppBuffer_isBufferEmpty( AppBuffer_Buffer *hbuffer );


#endif /* CIRCULAR_BUFFER_H_ */