#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "function1.h"
#include "function2.h"

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
        if (( !strcmp(argv[1],"0.0") && strcmp(argv[2],"0.0") && atof(argv[2]) != 0) || (!strcmp(argv[2],"0.0") && strcmp(argv[1],"0.0") && atof(argv[1]) != 0) || (!strcmp(argv[1],"0.0") && !strcmp(argv[2],"0.0")))
        {
            /* except */
            goto process;
        }
        fprintf(stderr, "please input floating point\n\n");
        usage();
        return -2;
    }
// https://www.educative.io/answers/resolving-the-a-label-can-only-be-part-of-a-statement-error
process: ;

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

    /***
     * Create the degree of a polynomial
     */
    srand(time(NULL));
    int m = (rand() % 8) + 3;
    printf("다항식의 갯수/차수: %d\n\n", m);

    /***
     * polynomial not using dynamic allocation
     */
    int64_t P[11] = {
        0,
    };
    int64_t Q[11] = {
        0,
    };
    createPoly(m, driver.lat_, driver.lon_, P, Q);
    // int64_t prime = 25190923363;

    /***
     * m + 1 coordinates
     */
    __int128_t point_p[11][2] = {
        0,
    };
    __int128_t point_q[11][2] = {
        0,
    };
    createPoint(m, P, Q, point_p, point_q);

    int64_t secret1 = 0;
    int64_t secret2 = 0;
    interpolation(m, point_p, point_q, &secret1, &secret2);

    printf("\n");
    printf("복구된 경도 값: %.8lf\n", (double)secret1 / (m * 100000000));
    printf("복구된 위도 값: %.8lf\n", (double)secret2 / (m * 100000000));
}
