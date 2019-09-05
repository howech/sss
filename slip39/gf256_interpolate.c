#include "gf256_interpolate.h"

#ifdef HAZMAT_QQQQ

// Lagrange Polynomials:
//                  n
//                 ---
// l_n_m(x,[x]) =  | |   ( x - x_i ) / ( x_m - x_i )
//               i != m
//
// note that x_i != x_j for i != j

int16_t lagrange(
    uint8_t n,   // number of points to interpolate
    uint8_t m,   // index of this point
    const uint8_t *xi, // x coordinates of all points (array of size n)
    uint8_t x    // x coordinate to evaluate
) {
    uint8_t result = 1;
    uint8_t i;

    for(i=0;i<n;++i) {
        if(i!=m) {

            if(xi[m] == xi[i]) {
                return -1;
            }

            result = gf256_mult( result, gf256_div(x ^ xi[i], xi[m] ^ xi[i]));
        }
    }

    return result;
}

// Interpolate
// given a polynomial that goes through n points { (x_i, y_i) }
// calculate the value for y at a given x
//
// This is readily accomplished by computing the lagrange polynomials
// for [x] and using them to generate the appropriate interpolation
// to match [y].
//                 n
//                ---
// P([x],[y],x) = >   pn_i(x,[x]) * y_i
//                ---
//                 i
//
// On success, this returns the length of the recovered secret. On failure,
// it returns -1

int16_t interpolate(
    uint8_t n,           // number of points to interpolate
    const uint8_t* xi,   // x coordinates for points (array of length n)
    uint32_t yl,         // length of y coordinate array
    const uint8_t **yij, // n arrays of yl bytes representing y values
    uint8_t x,           // x coordinate to interpolate
    uint8_t* result      // space for yl bytes of results
) {
    uint8_t lags[n];
      uint8_t i;
      uint32_t j;

    // Calculate lagrange polynomials for the target value.
    // the lagrange polynomial function will return -1 if there is a
    // duplicate in one of the x-coordinates.
      for(i=0; i<n; ++i) {
          int l = lagrange(n,i,xi,x);
          if(l==-1) {
              return -1;
          }
          lags[i] = l;
      }

    for(j=0; j<yl; ++j) {
        result[j] = 0;
        for(i=0;i<n;++i) {
            if(lags[i] != 0 && yij[i][j] != 0) {
                result[j] ^= gf256_mult(lags[i], yij[i][j]);
            }
        }
    }

    return yl;
}
#else
#include "hazmat.h"
int16_t lagrange(
    uint8_t n,   // number of points to interpolate
    uint8_t m,   // index of this point
    const uint8_t *xi, // x coordinates of all points (array of size n)
    uint8_t x    // x coordinate to evaluate
) {
    uint8_t values[n];

    sss_lagrange_basis(values, n, xi, x);

    return values[m];
}



int16_t interpolate(
    uint8_t n,           // number of points to interpolate
    const uint8_t* xi,   // x coordinates for points (array of length n)
    uint32_t yl,         // length of y coordinate array
    const uint8_t **yij, // n arrays of yl bytes representing y values
    uint8_t x,           // x coordinate to interpolate
    uint8_t* result      // space for yl bytes of results
) {
    uint8_t *y[n];
    uint8_t yv[32*n];
    uint8_t r[32];

    for(uint8_t i=0; i<n; i++) {
        y[i] = &yv[32*i];

        for(uint8_t j=0; j<32; ++j) {
            if(j<yl) {
                y[i][j] = yij[i][j];
            } else {
                y[i][j] = 0;
            }
        }
    }

    sss_interpolate(r, n, xi, y, x);

    for(uint8_t i=0; i<yl; i++) {
        result[i] = r[i];
    }

    return yl;
}
#endif