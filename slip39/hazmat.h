/*
 * Low level API for Daan Sprenkels' Shamir secret sharing library
 * Copyright (c) 2017 Daan Sprenkels <hello@dsprenkels.com>
 *
 * Usage of this API is hazardous and is only reserved for beings with a
 * good understanding of the Shamir secret sharing scheme and who know how
 * crypto code is implemented. If you are unsure about this, use the
 * intermediate level API. You have been warned!
 */


#ifndef sss_HAZMAT_H_
#define sss_HAZMAT_H_

#include <inttypes.h>


#define sss_KEYSHARE_LEN 33 /* 1 + 32 */


/*
 * One share of a cryptographic key which is shared using Shamir's
 * the `sss_create_keyshares` function.
 */
typedef uint8_t sss_Keyshare[sss_KEYSHARE_LEN];


void
sss_lagrange_basis(uint8_t *values,
                   uint8_t n,
                   const uint8_t *xc,
                   uint8_t x);

void
sss_interpolate(uint8_t *values,
                uint8_t n,
                const uint8_t *xc,
                const uint8_t **y,  // n sets of 32
                uint8_t x);


#endif /* sss_HAZMAT_H_ */
