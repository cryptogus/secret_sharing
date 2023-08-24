#include "openssl_ec.h"

void createPoly(uint64_t m, EC_GROUP *curve, EC_POINT *generator, BIGNUM *secret, BIGNUM *P[])
{
    BIGNUM *x = BN_new();
    BIGNUM *y = BN_new();
    BN_CTX *ctx = BN_CTX_new();

    /* Chang secret value to point (secret, 1) */
    EC_POINT *secret_point = EC_POINT_new(curve);
    EC_POINT_set_affine_coordinates(curve, secret_point, secret, BN_value_one(), ctx); // The function will return -1 because secret point is not on curve, But The coordinates are set.

    /* m* generator (m*G) */
    EC_POINT *point = EC_POINT_new(curve);

    char b_m[20];
    sprintf(b_m, "%ld", m); // change "uint64_t m" to "BIGNUM b_m"
    BIGNUM *b_m_ = BN_new();
    BN_dec2bn(&b_m_, b_m);
    EC_POINT_mul(curve, point, NULL, generator, b_m_, ctx);

    /* S(secret, 1) + m*G */
    EC_POINT_add(curve, point, point, secret_point, ctx);

    /* Create an m-th degree polynomial*/
    BIGNUM *range = BN_new();
    BN_hex2bn(&range, "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

    EC_POINT_get_affine_coordinates(curve, point, x, y, ctx); // 다항식의 point인 y 절편
    printf("(%s, %s) +\n", BN_bn2hex(x), BN_bn2hex(y));
    // (x, y) -> x||y and input P[0] of polynomial
    BN_lshift(P[0], x, 256);
    BN_add(P[0], P[0], y);

    /* ax + bx^2 ... */
    for (uint64_t j = 1; j < m; j++)
    {
        BN_rand_range(P[j], range);
        printf("%s x^%ld + ", BN_bn2hex(P[j]), j);
    }

    BN_rand_range(P[m], range);
    printf("%s x^%ld\n", BN_bn2hex(P[m]), m);

    BN_free(x);
    BN_free(y);
    BN_free(b_m_);
    BN_free(range);
    EC_POINT_free(secret_point);
    EC_POINT_free(point);
    BN_CTX_free(ctx);
}

void createPoint(uint64_t m, BIGNUM *P[], BIGNUM ***point_p)
{

    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *num = BN_new();

    // inituialization
    for (uint64_t i = 1; i < m + 2; i++)
    {
        BN_zero(point_p[i - 1][1]);
    }

    for (uint64_t i = 1; i < m + 2; i++)
    {
        char buffer[20];
        sprintf(buffer, "%ld", i);
        BN_dec2bn(&point_p[i - 1][0], buffer);
        BN_add(point_p[i - 1][1], point_p[i - 1][1], P[0]);

        for (uint64_t j = 1; j < m + 1; j++)
        {
            char buffer2[20];
            sprintf(buffer2, "%ld", j);
            BN_dec2bn(&num, buffer2);
            BN_exp(num, point_p[i - 1][0], num, ctx);
            BN_mul(num, P[j], num, ctx);
            BN_add(point_p[i - 1][1], point_p[i - 1][1], num);
        }
    }

    BN_free(num);
    BN_CTX_free(ctx);
}