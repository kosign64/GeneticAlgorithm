#ifndef GENETIC_H
#define GENETIC_H

typedef double *Organism;

typedef struct
{
    int populationSize;
    int paramSize;
    int time;
    double (*fitness)(Organism, int paramSize);
    Organism *organisms;
    double *fitnesses;
    double *roulleteNumbers;
    Organism *parentsPool;
    Organism *children;
    int mutationProbability;
}Genetic;

void generateOrganisms(Genetic *genetic, int paramSize, int populationSize, int iterations,
                       double (*fitness)(Organism, int paramSize), int mutationProbability);
void calcFitnesses(Genetic *genetic);
void selection(Genetic *genetic);
void crossover(Genetic *genetic);
void mutation(Genetic *genetic);
void printPopulation(Genetic *genetic);
Organism *geneticAlgorithm(Genetic *genetic, int paramSize, int populationSize,
                           int iterations, double (*fitness)(Organism, int paramSize),
                           int mutationProbability);


#endif // GENETIC_H
