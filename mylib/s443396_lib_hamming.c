/**
 **************************************************************
 * @file mylib/s4433963_lib_hamming.c
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
 * INTERNAL FUNCTIONS
 ***************************************************************
 * hamming_hbyte_encode() - Internal function to Encode a half byte.
 ***************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "s443396_lib_hamming.h"
#include "board.h"
#include "debug_printf.h"
#include "processor_hal.h"
/* ---------------------------------------------------------------------------*/

uint8_t hamming_hbyte_encoder(unsigned char value);

/*
 * @brief Return the 16bit encoded value of a byte.
 */
uint16_t s443396_lib_hamming_byte_encode(unsigned char value)
{
    uint16_t out;

    /* first encode D0..D3 (first 4 bits),
	 * then D4..D7 (second 4 bits).
	 */

    out = hamming_hbyte_encoder(value & 0xF) | (hamming_hbyte_encoder(value >> 4) << 8);

    return out;
}

/*
 * @brief Internal function to Encode a half byte.
 */
uint8_t hamming_hbyte_encoder(unsigned char value)
{
    uint8_t d0, d1, d2, d3;
    uint8_t p0 = 0, h0, h1, h2;
    uint8_t z;
    uint8_t out;

    /* extract bits */
    d0 = !!(value & 0x1);
    d1 = !!(value & 0x2);
    d2 = !!(value & 0x4);
    d3 = !!(value & 0x8);

    /* calculate hamming parity bits */
    h0 = d0 ^ d1 ^ d2;
    h1 = d0 ^ d1 ^ d3;
    h2 = d0 ^ d2 ^ d3;

    /* generate out byte without parity bit P0 */
    out = (h2 << 7) | (h1 << 6) | (d3 << 5) | (h0 << 4) | (d2 << 3) | (d1 << 2) | (d0 << 1);

    /* calculate even parity bit */
    for (z = 1; z < 8; z++)
        p0 = p0 ^ !!(out & (1 << z));

    out |= p0;

    return out;
}

/*
 * @brief Return the decoded half byte from a byte. Errors must be detected and corrected.
 */
uint8_t s443396_lib_hamming_byte_decode(uint16_t value)
{  
    uint8_t d0, d1, d2, d3;
    uint8_t p0, h0, h1, h2;
    uint8_t out;

    p0 = !!(value & (1 << 8));

    d0 = !!(value & (1 << 1));
    d1 = !!(value & (1 << 2));
    d2 = !!(value & (1 << 3));
    d3 = !!(value & (1 << 5));
    h0 = !!(value & (1 << 4));
    h1 = !!(value & (1 << 6));
    h2 = !!(value & (1 << 7));

    int c1, c2, c3;
    c1 = h2 ^ d3 ^ d2 ^ d0;
    c2 = h1 ^ d3 ^ d1 ^ d0;
    c3 = h0 ^ d2 ^ d1 ^ d0;
    int c = c3 * 4 + c2 * 2 + c1;

    if (c == 1)
    {
        d0 = !!(value & (1 << 1));
        d1 = !!(value & (1 << 2));
        d2 = !!(value & (1 << 3));
        d3 = !!(value & (1 << 5));
        h0 = !!(value & (1 << 4));
        h1 = !!(value & (1 << 6));
        h2 = !!(value & (1 << 7));

        switch (c)
        {
        case 0:
            if (d0 == 0) {
                d0 = 1;
            } else {
                d0 = 0;
            }
            break;
        case 1:
            if (d1 == 0) {
                d1 = 1;
            } else {
                d1 = 0;
            }
            break;
        case 2:
            if (d2 == 0) {
                d2 = 1;
            } else {
                d2 = 0;
            }
            break;
        case 3:
            if (h0 == 0) {
                h0 = 1;
            } else {
                h0 = 0;
            }
            break;
        case 4:
            if (d3 == 0) {
                d3 = 1;
            } else {
                d3 = 0;
            }
            break;
        case 5:
            if (h1 == 0) {
                h1 = 1;
            } else {
                h1 = 0;
            }
            break;
        case 6:
            if (h2 == 0) {
                h2 = 1;
            } else {
                h2 = 0;
            }
            break;
        }
    }

    out = (d3 << 3) | (d2 << 2) | (d1 << 1) | (d0 << 0);

    return out;
}

/*
 * @brief Return 1 if a parity error has occurred, else 0.
 */
int s443396_lib_hamming_parity_error(unsigned char value)
{
    int error = value == hamming_hbyte_encoder(value);

    if (error)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
