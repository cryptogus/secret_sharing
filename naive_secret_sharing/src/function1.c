#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*About P,Q not using dynamic allocate or std::vector*/
void createPoly(int32_t m, double lat, double lon, int64_t *P, int64_t *Q)
{
    /***
     * float to int
     */
    int64_t int_lat = lat * 100000000;
    int64_t int_lon = lon * 100000000;

    srand(time(NULL));
    /* Secret + ax + bx^2 ... */
    printf("* P_i \n");
    for (int j = 0; j < m; j++)
    {
        int tmp1 = 0;
        printf("polynomial %d: %ld + ", j + 1, int_lat);
        P[0] += int_lat;
        for (int i = 1; i < m; i++)
        {

            tmp1 = rand();
            printf("%d x^%d + ", tmp1, i);
            P[i] += tmp1;
        }
        tmp1 = rand();
        printf("%dx^%d", tmp1, m);
        P[m] += tmp1;
        printf("\n");
    }

    printf("\n* Q_i \n");
    for (int j = 0; j < m; j++)
    {
        int tmp1 = 0;
        printf("polynomial %d: %ld + ", j + 1, int_lon);
        Q[0] += int_lon;
        for (int i = 1; i < m; i++)
        {

            tmp1 = rand();
            printf("%d x^%d + ", tmp1, i);
            Q[i] += tmp1;
        }
        tmp1 = rand();
        printf("%dx^%d", tmp1, m);
        Q[m] += tmp1;
        printf("\n");
    }
}

void createPoint(int32_t m, int64_t *P, int64_t *Q, __int128_t point_p[][2], __int128_t point_q[][2])
{
    for (int32_t i = 1; i < m + 2; i++)
    {
        point_p[i - 1][0] = (int64_t)i;
        point_q[i - 1][0] = (int64_t)i;
        point_p[i - 1][1] += P[0];
        point_q[i - 1][1] += Q[0];
        for (int32_t j = 1; j < m + 1; j++)
        {
            point_p[i - 1][1] += (P[j] * ((__int128_t)pow(i, j)));
            point_q[i - 1][1] += (Q[j] * ((__int128_t)pow(i, j)));
        }
        // printf("(%ld, %ld) ", point_p[i - 1][0], point_p[i - 1][1]);
    }
}