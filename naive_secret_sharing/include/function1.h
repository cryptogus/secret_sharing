#ifndef _FUNCTION1_H_
#define _FUNCTION1_H_

void createPoly(int32_t m, double lat, double lon, int64_t *P, int64_t *Q);
void createPoint(int32_t m, int64_t *P, int64_t *Q,  __int128_t point_p[][2], __int128_t point_q[][2]);

#endif