#ifndef _ECC_H_
#define _ECC_H_

EC_GROUP *create_curve(void);
EC_POINT *create_generator(EC_GROUP *curve);

#endif