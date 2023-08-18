#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "function1.h"
#include "function2.h"

// 랜덤 위치 생성
double generateRandomCoordinate()
{
    return (double)rand() / RAND_MAX * 180.0 - 90.0;
}

int main(void)
{
    /*test 횟수*/
    int count = 10000;

    while (count--)
    {
        srand(count); // 시드 값 설정
        int m = (rand() % 8) + 3;
        int64_t P[11] = {
            0,
        };
        int64_t Q[11] = {
            0,
        };

        double latitude = generateRandomCoordinate();
        double longitude = generateRandomCoordinate();
        printf("위도: %lf 경도: %lf\n\n", latitude, longitude);
        createPoly(m, latitude, longitude, P, Q);

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
        if (latitude == (double)secret1 / (m * 100000000))
            printf("Latitude is true\n");
        else
            printf("경도 오차: %g\n", latitude - (double)secret1 / (m * 100000000));
        if (longitude == (double)secret2 / (m * 100000000))
            printf("Longitude is true\n\n");
        else
            printf("위도 오차: %g\n\n", longitude - (double)secret2 / (m * 100000000));

        printf("===============================================================================================\n");
    }
}