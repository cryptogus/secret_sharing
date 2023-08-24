#include <string.h>

#include "openssl_ec.h"
#include "ecc.h"
#include "poly.h"
#include "recov.h"

void usage()
{
    printf("syntax: Eliptic-secret-sharing <Secret number>\n");
    printf("sample: Eliptic-secret-sharing 486\n");
}

void printBN(char *msg, BIGNUM *a)
{
    char *number_str = BN_bn2dec(a);
    printf("%s %s\n", msg, number_str);
    OPENSSL_free(number_str);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        usage();
        return -1;
    }
    
    printf("* 입력 정보\n\n");
    printf("Secret is %s\n", argv[1]);


    /**
     * Big number library
     */
    BN_CTX *ctx = BN_CTX_new();
    BIGNUM *sec2 = BN_new();
    

    BN_dec2bn(&sec2, argv[1]);

    /*************************** Eliptic curve & secret sharing operation ******************************/
    EC_GROUP *curve = create_curve();
    EC_POINT *generator = create_generator(curve);

    // m 은 다항식의 차수, m + 1 개의 shares를 생성하는데 사용됩니다.
    srand(time(NULL));
    uint64_t m;
    m = (rand() % 20) + 3;
    printf("m is %ld\n", m);
    BIGNUM *recovery_sec = BN_new();

    // BIGNUM *poly[m + 1] 와 같다
    BIGNUM **poly = (BIGNUM **)malloc(sizeof(BIGNUM *) * (m + 1));
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

    createPoly(m, curve, generator, sec2, poly);
    createPoint(m, poly, point_p);
    printf("\n%ld 개의 좌표쌍\n\n", m + 1);
    for (uint64_t k = 0; k < m + 1; k++)
    {
        printf("(%s, %s)\n", BN_bn2dec(point_p[k][0]), BN_bn2hex(point_p[k][1]));
    }
    interpolation(m, point_p, recovery_sec);
    printf("\n복구한 y절편: %s\n\n", BN_bn2hex(recovery_sec));

    BIGNUM *answer = recovery(m, curve, generator, recovery_sec);

    printf("복구된 비밀 값: %s\n", BN_bn2dec(answer));
    
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
    
    BN_free(sec2);
    BN_free(recovery_sec);
    BN_free(answer);
    

    return 0;
}