#ifndef _RECOV_H_
#define _RECOV_H_

void interpolation(uint64_t m, BIGNUM ***point_p, BIGNUM *secret);
BIGNUM *recovery(uint64_t m, EC_GROUP *curve, EC_POINT *generator, BIGNUM *secret);

#endif