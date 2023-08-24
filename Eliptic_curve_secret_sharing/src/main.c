#include <string.h>

#include "openssl_ec.h"
#include "ecc.h"
#include "poly.h"
#include "recov.h"

/*location information*/
typedef struct location
{
    double lat_;
    double lon_;
} Loca;

void usage()
{
    printf("syntax: secret-sharing-test <Latitude> <Longitude>\n");
    printf("sample: secret-sharing-test 36.9357309 127.0430941\n");
    printf("If the latitude or longitude is zero, enter 0.0\n");
}

void printBN(char *msg, BIGNUM *a)
{
    char *number_str = BN_bn2dec(a);
    printf("%s %s\n", msg, number_str);
    OPENSSL_free(number_str);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        usage();
        return -1;
    }
    if (atof(argv[1]) == 0 || atof(argv[2]) == 0)
    {
        /***
         * double atof(const char *str) - This function returns the converted floating point number as a double value. If no valid conversion could be performed, it returns zero
         * so if the latitude or longitude is zero, enter 0.0
         * */
        if ((!strcmp(argv[1], "0.0") && strcmp(argv[2], "0.0") && atof(argv[2]) != 0) || (!strcmp(argv[2], "0.0") && strcmp(argv[1], "0.0") && atof(argv[1]) != 0) || (!strcmp(argv[1], "0.0") && !strcmp(argv[2], "0.0")))
        {
            /* except */
            goto process;
        }
        fprintf(stderr, "please input floating point\n\n");
        usage();
        return -2;
    }

process:;

    Loca driver;
    driver.lat_ = atof(argv[1]);
    driver.lon_ = atof(argv[2]);
    /***
     * test
     * 36.9357309	127.0430941
     * -30.01111   127.00001111
     */
    printf("* 현재 위치\n\n");
    printf("위도: %.8lf\n", driver.lat_);
    printf("경도: %.8lf\n\n", driver.lon_);

    /**
     * float to int
     */
    int64_t lat_i = driver.lat_ * 100000000;
    int64_t lon_i = driver.lon_ * 100000000;

    /**
     * int to string
     */
    char str_lat[20];
    char str_lon[20];
    sprintf(str_lat, "%ld", lat_i);
    sprintf(str_lon, "%ld", lon_i);

    /**
     * Big number library
     */
    BN_CTX *ctx = BN_CTX_new();

    BIGNUM *lat = BN_new();
    BIGNUM *lon = BN_new();

    BN_dec2bn(&lat, str_lat);
    BN_dec2bn(&lon, str_lon);

    /*************************** Eliptic curve & secret sharing operation ******************************/
    EC_GROUP *curve = create_curve();
    EC_POINT *generator = create_generator(curve);

    srand(time(NULL));
    uint64_t m;
    m = (rand() % 50) + 3;
    printf("m is %ld\n", m);
    BIGNUM *recovery_lat = BN_new();
    BIGNUM *recovery_lon = BN_new();

    BIGNUM **poly = (BIGNUM **)malloc(sizeof(BIGNUM *) * (m + 1)); // BIGNUM *poly[m + 1]
    // secret sharing point, BIGNUM *point_p[m + 1][2]
    BIGNUM ***point_p = (BIGNUM ***)malloc(sizeof(BIGNUM **) * (m + 1));
    for (uint64_t i = 0; i < m + 1; i++)
    {
        point_p[i] = (BIGNUM **)malloc(2 * sizeof(BIGNUM *));
    }
    /* BN_new */
    for (uint64_t k = 0; k < m + 1; k++)
    {
        poly[k] = BN_new();
        point_p[k][0] = BN_new();
        point_p[k][1] = BN_new();
    }
    printf("\n");
    /***** latitude *****/
    createPoly(m, curve, generator, lat, poly);
    createPoint(m, poly, point_p);
    printf("\n%ld 개의 좌표쌍\n\n", m + 1);
    for (uint64_t k = 0; k < m + 1; k++)
    {
        printf("(%s, %s)\n", BN_bn2dec(point_p[k][0]), BN_bn2hex(point_p[k][1]));
    }
    interpolation(m, point_p, recovery_lat);
    printf("\n복구한 y절편: %s\n\n", BN_bn2hex(recovery_lat));

    double answer = recovery(m, curve, generator, recovery_lat);

    printf("복구된 위도값: %.8lf\n", answer);
    printf("\n======================================================================================================================================================================\n\n");
    /***** longitude *****/
    createPoly(m, curve, generator, lon, poly);
    createPoint(m, poly, point_p);
    printf("\n%ld 개의 좌표쌍\n\n", m + 1);
    for (uint64_t k = 0; k < m + 1; k++)
    {
        printf("(%s, %s)\n", BN_bn2dec(point_p[k][0]), BN_bn2hex(point_p[k][1]));
    }
    interpolation(m, point_p, recovery_lon);
    printf("\n복구한 y절편: %s\n\n", BN_bn2hex(recovery_lon));

    answer = recovery(m, curve, generator, recovery_lon);

    printf("복구된 경도값: %.8lf\n", answer);
    // 에러 정보 출력
    // ERR_print_errors_fp(stderr);
    EC_GROUP_free(curve); // Free curve
    EC_POINT_free(generator);

    for (uint64_t k = 0; k < m + 1; k++)
    {
        BN_free(poly[k]);
        BN_free(point_p[k][0]);
        BN_free(point_p[k][1]);
    }
    free(poly);
    for (uint64_t i = 0; i < m + 1; i++)
        free(point_p[i]);
    free(point_p);
    
    BN_CTX_free(ctx);
    
    BN_free(lat);
    BN_free(lon);
    BN_free(recovery_lat);
    BN_free(recovery_lon);

    return 0;
}