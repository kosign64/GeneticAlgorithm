#include "genetic.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double maxElement(const double elems[], int size)
{
    double max = 0;
    for(int i = 0; i < size; i++)
    {
        if(elems[i] > max)
        {
            max = elems[i];
        }
    }
    return max;
}

static int minElement(const double elems[], int size)
{
    double min = 0;
    int num = 0;
    for(int i = 0; i < size; i++)
    {
        if(elems[i] < min)
        {
            num = i;
            min = elems[i];
        }
    }
    return num;
}

static int maxElementNum(const double elems[], int size)
{
    double max = 0;
    int num = 0;
    for(int i = 0; i < size; i++)
    {
        if(elems[i] > max)
        {
            num = i;
            max = elems[i];
        }
    }
    return num;
}

static int secondMaxElement(double elems[], int size, int maxNum)
{
    double max = 0;
    int num = 0;
    const double maxVal = elems[maxNum];
    for(int i = 0; i < size; i++)
    {
        if(elems[i] == maxVal) continue;
        if(elems[i] > max)
        {
            num = i;
            max = elems[i];
        }
    }
    return num;
}

static void copyOrganism(Genetic *genetic, Organism *src, Organism *dst)
{
    for(int i = 0; i < genetic->paramSize; i++)
    {
        (*dst)[i] = (*src)[i];
    }
}

void generateOrganisms(Genetic *genetic, int paramSize, int populationSize,
                       int iterations, double (*fitness)(Organism, int), int mutationProbability)
{
    srand(time(NULL));
    genetic->fitness = fitness;
    genetic->populationSize = populationSize;
    genetic->paramSize = paramSize;
    genetic->time = iterations;
    genetic->mutationProbability = mutationProbability;
    genetic->organisms = (Organism*)malloc(populationSize * sizeof(Organism));
    genetic->fitnesses = (double*)malloc(populationSize * sizeof(double));
    genetic->roulleteNumbers = (double*)malloc(populationSize * sizeof(double));
    genetic->parentsPool = (Organism*)malloc(populationSize * sizeof(Organism));
    genetic->children = (Organism*)malloc(populationSize * sizeof(Organism));
    for(int i = 0; i < populationSize; i++)
    {
        genetic->organisms[i] = (double*)malloc(paramSize * sizeof(double));
    }
    for(int i = 0; i < populationSize; i++)
    {
        genetic->parentsPool[i] = (double*)malloc(paramSize * sizeof(double));
    }
    for(int i = 0; i < populationSize; i++)
    {
        genetic->children[i] = (double*)malloc(paramSize * sizeof(double));
    }
    for(int i = 0; i < populationSize; i++)
    {
        for(int j = 0; j < paramSize; j++)
        {
            Organism *organism = &genetic->organisms[i];
            (*organism)[j] = (rand() % 20001) / 100. - 100;
        }
    }
}

void calcFitnesses(Genetic *genetic)
{
    for(int i = 0; i < genetic->populationSize; i++)
    {
        genetic->fitnesses[i] = genetic->fitness(genetic->organisms[i],
                                                 genetic->paramSize);
    }
}

void selection(Genetic *genetic)
{
    double maxFit = maxElement(genetic->fitnesses, genetic->populationSize);
    for(int i = 0; i < genetic->populationSize; i++)
    {
        genetic->fitnesses[i] = 1 / genetic->fitnesses[i];
    }
    double sum = 0;
    for(int i = 0; i < genetic->populationSize; i++)
    {
        sum += genetic->fitnesses[i];
    }
    double roulleteSum = 0;
    for(int i = 0; i < genetic->populationSize - 1; i++)
    {
        genetic->roulleteNumbers[i] = roulleteSum + genetic->fitnesses[i] / sum;
        roulleteSum = genetic->roulleteNumbers[i];
    }
    genetic->roulleteNumbers[genetic->populationSize - 1] = sum;
    for(int i = 0; i < genetic->populationSize; i++)
    {
        double randRoulletValue = (rand() % (int)(roulleteSum * 100)) / 100.;
        for(int j = 0; j < genetic->populationSize; j++)
        {
            if(randRoulletValue < genetic->roulleteNumbers[j])
            {
                copyOrganism(genetic, &genetic->organisms[j], &genetic->parentsPool[i]);
                break;
            }
        }
    }
}

void crossover(Genetic *genetic)
{
    int best = maxElementNum(genetic->fitnesses, genetic->populationSize);
    int second = secondMaxElement(genetic->fitnesses, genetic->populationSize, best);
    printf("best: %d, second: %d\n", best, second);
    copyOrganism(genetic, &genetic->organisms[best], &genetic->children[0]);
    copyOrganism(genetic, &genetic->organisms[second], &genetic->children[1]);
    for(int i = 2; i < (genetic->populationSize); i++)
    {
        int p1 = rand() % genetic->populationSize;
        int p2 = rand() % genetic->populationSize;
        Organism *parent1 = &genetic->parentsPool[p1];
        Organism *parent2 = &genetic->parentsPool[p2];
        Organism *org = &genetic->children[i];
        for(int j = 0; j < genetic->paramSize; j++)
        {
            double alpha = (rand() % 2000) / 100. - 10;
            (*org)[j] = (*parent1)[j] + alpha * ((*parent2)[j] - (*parent1)[j]);
        }
    }
    for(int i = 0; i < genetic->populationSize; i++)
    {
        copyOrganism(genetic, &genetic->children[i], &genetic->organisms[i]);
    }
}

void mutation(Genetic *genetic)
{
    for(int i = 2; i < genetic->populationSize; i++)
    {
        int mut = rand() % 100;
        if(mut > genetic->mutationProbability) return;
        int paramNum = rand() % genetic->paramSize;
        double mutation = (rand() % 20) / 10. - 10;
        genetic->organisms[i][paramNum] += mutation;
    }
}

void printPopulation(Genetic *genetic)
{
    printf("Population:\n");
    for(int i = 0; i < genetic->populationSize; i++)
    {
        for(int j = 0; j < genetic->paramSize; j++)
        {
            Organism organism = genetic->organisms[i];
            printf("%lf ", organism[j]);
        }
        printf("   %lf", genetic->fitnesses[i]);
        printf("\n");
    }
}

void printOrganism(Organism *org, int paramSize)
{
    printf("Organism:\n");
    for(int j = 0; j < paramSize; j++)
    {
        printf("%lf ", (*org)[j]);
    }
    printf("\n");
}

Organism *geneticAlgorithm(Genetic *genetic, int paramSize, int populationSize,
                           int iterations, double (*fitness)(Organism, int), int mutationProbability)
{
    generateOrganisms(genetic, paramSize, populationSize, iterations,
                      fitness, mutationProbability);
    for(int i = 0; i < iterations; i++)
    {
        calcFitnesses(genetic);
        printPopulation(genetic);
        selection(genetic);
        crossover(genetic);
        mutation(genetic);
    }
    calcFitnesses(genetic);
    int best = minElement(genetic->fitnesses, genetic->populationSize);
    return &genetic->organisms[best];
}
