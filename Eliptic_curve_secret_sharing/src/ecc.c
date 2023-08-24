#include "openssl_ec.h"

EC_GROUP *create_curve(void)
{
    EC_GROUP *curve;

    /* https://wiki.openssl.org/index.php/Elliptic_Curve_Cryptography */

    /* Create the curve */
    if (NULL == (curve = EC_GROUP_new_by_curve_name(NID_secp256k1)))
    {
        fprintf(stderr, "curve is not created!\n");
        return NULL;
    }

    return curve;
}

EC_POINT *create_generator(EC_GROUP *curve)
{
    /* Create generator from curve*/
    EC_POINT *generator = EC_POINT_new(curve); // 그냥 이대로  사용하면 point at infinity로 초기화

    if (generator == NULL)
    {
        fprintf(stderr, "Error creating generator!\n");
        EC_GROUP_free(curve); // Cleanup
        return NULL;
    }

    BIGNUM *x = BN_new();
    BIGNUM *y = BN_new();
    BN_CTX *ctx = BN_CTX_new();

    if (BN_hex2bn(&x, "79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798") &&
        BN_hex2bn(&y, "483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8"))
    {

        if (EC_POINT_set_affine_coordinates(curve, generator, x, y, NULL) == 1)
        {
            printf("Generator is set.\n");
        }
        else
        {
            fprintf(stderr, "Error setting generator coordinates.\n");
        }
    }
    else
    {
        fprintf(stderr, "Error setting BN values for Gx, Gy.\n");
    }

    if (EC_POINT_is_at_infinity(curve, generator) == 1)
    {
        printf("Generator is point at infinity.\n");
    }
    BN_free(x);
    BN_free(y);
    BN_CTX_free(ctx);

    return generator;
}