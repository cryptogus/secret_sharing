#include "openssl_ec.h"

void interpolation(uint64_t m, BIGNUM ***point_p, BIGNUM *secret)
{
    BIGNUM *numerator = BN_new();
    BIGNUM *denominator = BN_new();
    BIGNUM *num = BN_new();
    BN_CTX *ctx = BN_CTX_new();
    BN_zero(secret);

    for (uint64_t i = 0; i < m + 1; i++)
    {

        BN_one(numerator);
        BN_one(denominator);
        for (uint64_t j = 0; j < m + 1; j++)
        {
            if (i != j)
            {
                BN_zero(num);

                BN_sub(num, num, point_p[j][0]);
                BN_mul(numerator, numerator, num, ctx);

                BN_sub(num, point_p[i][0], point_p[j][0]);
                BN_mul(denominator, denominator, num, ctx);
            }
        }
        BN_mul(num, point_p[i][1], numerator, ctx);
        BN_div(num, NULL, num, denominator, ctx);
        BN_add(secret, secret, num);
    }

    BN_free(numerator);
    BN_free(denominator);
    BN_free(num);
    BN_CTX_free(ctx);
}

int64_t recovery(uint64_t m, EC_GROUP *curve, EC_POINT *generator, BIGNUM *secret)
{
    BIGNUM *x = BN_new();
    BN_CTX *ctx = BN_CTX_new();
    EC_POINT *secret_point = EC_POINT_new(curve);
    EC_POINT *point = EC_POINT_new(curve);

    /* Change secret to secret_point */
    BN_rshift(x, secret, 256);
    BN_mask_bits(secret, 256);
    EC_POINT_set_affine_coordinates(curve, secret_point, x, secret, ctx);

    /* Create m*G */
    char b_m[20];
    sprintf(b_m, "%ld", m); // change "uint64_t m" to "BIGNUM b_m"
    BIGNUM *b_m_ = BN_new();
    BN_dec2bn(&b_m_, b_m);
    EC_POINT_mul(curve, point, NULL, generator, b_m_, ctx);

    /* Inverse of m*G */
    EC_POINT_invert(curve, point, ctx);

    /* Generate S from S + m*G using inverse of m*G*/
    EC_POINT_add(curve, secret_point, secret_point, point, ctx);

    /* Get secret S from S*/

    EC_POINT_get_affine_coordinates(curve, secret_point, x, NULL, ctx);
    int64_t buf = (int64_t)strtol(BN_bn2dec(x), NULL, 10);

    BN_free(x);
    BN_free(b_m_);

    EC_POINT_free(secret_point);
    EC_POINT_free(point);
    BN_CTX_free(ctx);

    return buf;
}
