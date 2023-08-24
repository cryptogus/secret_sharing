#ifndef _POLY_H_
#define _POLY_H_

void createPoly(uint64_t m, EC_GROUP *curve, EC_POINT *generator, BIGNUM *secret, BIGNUM *P[]);
void createPoint(uint64_t m, BIGNUM *P[], BIGNUM ***point_p);
#endif