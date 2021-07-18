/** 
 **************************************************************
 * @file mylib/s4433963_lib_hamming.h
 * @author Joseph Twin - 44339638
 * @date 10042021
 * @brief Hamming Mylib LIB Driver
 ***************************************************************
 * EXTERNAL FUNCTIONS 
 ***************************************************************
 * s443396_lib_hamming_byte_encode() - Return the 16bit encoded value of a byte .
 * s443396_lib_hamming_byte_decode() - Return the decoded half byte from a byte.
 * s443396_lib_hamming_parity_error() - Return 1 if a parity error has occurred, else 0.
 *************************************************************** 
 */

#ifndef S443396_LIB_HAMMING_H
#define S443396_LIB_HAMMING_H

#include "board.h"
#include "processor_hal.h"

extern uint16_t s443396_lib_hamming_byte_encode(unsigned char value);
extern uint8_t s443396_lib_hamming_byte_decode(uint16_t value);
extern int s443396_lib_hamming_parity_error(unsigned char value);

#endif
