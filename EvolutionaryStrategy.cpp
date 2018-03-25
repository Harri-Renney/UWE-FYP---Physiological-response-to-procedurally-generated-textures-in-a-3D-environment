#include <random>
#include <cstdlib>
#include <time.h>

#include "EvolutionaryStrategy.h"

using namespace ES;

EvolutionaryStrategy::EvolutionaryStrategy()
{
	this->numberOfChildren = 10;
	this->mutationRate = 0.1;
	for (int i = 0; i != 10; ++i)
	{
		std::pair<float, float> placeHolder = std::make_pair(0.0 / 100.0, 0.001);
		parent.push_back(placeHolder);
	}
}
EvolutionaryStrategy::EvolutionaryStrategy(int numberOfGenome, int numberOfChildren, float mutationRate, float mutationDistribution)
{
	this->numberOfChildren = numberOfChildren;
	this->mutationRate = mutationRate;
	for (int i = 0; i != numberOfGenome; ++i)
	{
		std::pair<float, float> placeHolder = std::make_pair(0.0 / 100.0, mutationDistribution);
		parent.push_back(placeHolder);
	}
}
float EvolutionaryStrategy::evolveParent()
{
	std::vector<std::vector<float>> debugging;
	std::default_random_engine generator;
	generator.seed(time(NULL));
	std::vector<float> best;
	for (int i = 0; i != parent.size(); ++i)
		best.push_back(0);
	for (int i = 0; i != numberOfChildren; ++i)
	{
		std::vector<float> child;
		for (int j = 0; j != parent.size(); ++j)
		{
			std::normal_distribution<double> distribution(0.0, parent[0].second);
			if ((rand() % 100) / 100.0 <= mutationRate)
			{
				child.push_back(distribution(generator));
			}
			else
				child.push_back(0);
		}
		if (fitnessFunction(child) >= fitnessFunction(best))
			best = child;
	}
	//if (fitnessFunction(best) >= fitnessFunction(getGenome()))
	setGenome(best);
	return fitnessFunction(getGenome());
}

///////////////////////
// Getters + Setters //
///////////////////////

void EvolutionaryStrategy::setGenome(std::vector<float> g)
{
	for (int i = 0; i != parent.size(); ++i)
	{
		std::pair<float, float> placeHolder = std::make_pair(g[i], parent[i].second);
		parent[i] = placeHolder;
	}
}

std::vector<std::pair<float, float> >& EvolutionaryStrategy::getParent()
{
	return parent;
}

std::vector<float> EvolutionaryStrategy::getGenome()
{
	std::vector<float> genome;
	for (int i = 0; i != parent.size(); ++i)
	{
		genome.push_back(parent[i].first);
	}
	return genome;
}

std::vector<float> EvolutionaryStrategy::getMutationDistribution()
{
	std::vector<float> mutationDistributions;
	for (int i = 0; i != parent.size(); ++i)
	{
		mutationDistributions.push_back(parent[i].first);
	}
	return mutationDistributions;
}