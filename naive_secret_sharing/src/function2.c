#include <stdint.h>

void interpolation(int32_t m, __int128_t point_p[][2], __int128_t point_q[][2], int64_t *secret1, int64_t *secret2)
{
    __int128_t secret = 0;
    for (int32_t i = 0; i < m + 1; i++)
    {
        __int128_t numerator = 1, denominator = 1;
        for (int32_t j = 0; j < m + 1; j++)
        {
            if (i != j)
            {
                numerator = (numerator * (0 - point_p[j][0]));
                denominator = (denominator * (point_p[i][0] - point_p[j][0]));
            }
        }
        secret = (secret + ((point_p[i][1] * numerator) / denominator));
    }
    *secret1 = (int64_t)secret;

    secret = 0;

    for (int32_t i = 0; i < m + 1; i++)
    {
        __int128_t numerator = 1, denominator = 1;
        for (int32_t j = 0; j < m + 1; j++)
        {
            if (i != j)
            {
                numerator = (numerator * (0 - point_q[j][0]));
                denominator = (denominator * (point_q[i][0] - point_q[j][0]));
            }
        }
        secret = (secret + ((point_q[i][1] * numerator) / denominator));
    }
    *secret2 = (int64_t)secret;
}

// int64_t mod_inverse(int64_t a, int64_t b)
// {
//     int64_t inverse = 0;

//     int64_t t0 = a, t1 = b;
//     int64_t u0 = 1, v0 = 0;
//     int64_t u1 = 0, v1 = 1;

//     while(t1 != 0)
//     {
//         int64_t q = t0 / t1;
//         int64_t r = t0 % t1;
//         t0 = t1;
//         t1 = r;
//         int64_t u2 = u0 - q*u1;
//         int64_t v2 = v0 - q*v1;
//         u0 = u1;
//         v0 = v1;
//         u1 = u2;
//         v1 = v2;
//     }
//     while(u0 < 0)
//     {
//         u0 += b;
//     }
//     return u0;

// }