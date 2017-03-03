#include <stdio.h>
#include "genetic.h"
#include <math.h>

double func(double params[], int size)
{
    return fabs(params[0] + 2 * params[1] + 3 * params[2] +
            4 * params[3] + 5 * params[4] - 50);
}

int main()
{
    Genetic genetic;
    Organism *result = geneticAlgorithm(&genetic, 5, 100, 1000, &func, 5);
    printf("best = %lf %lf %lf %lf %lf, result = %lf", (*result)[0], (*result)[1],
            (*result)[2], (*result)[3], (*result)[4], func(*result, 5));

    return 0;
}
