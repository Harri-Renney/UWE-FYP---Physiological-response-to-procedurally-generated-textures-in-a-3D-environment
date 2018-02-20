#include "RL.h"

float QFunction(std::vector<float> state, std::vector<float> weights, std::vector<float> action)
{
	float QValueEstimate = 0.0;
	std::vector<float>::iterator a = action.begin();
	std::vector<float>::iterator w = weights.begin();
	for (float &s : state)
	{
		QValueEstimate += (s+(*a)) * (*w);
		++w;
		++a;
	}
	return QValueEstimate;
}

EvolutionaryStratergy::EvolutionaryStratergy(QLearningApprox &qLearningApprox, int numberChildren, int numberOfGenome, float mutationRange, float mutationRate) : qLearningApprox(qLearningApprox)
{
	this->numberOfChildren = numberChildren;
	this->mutationRate = mutationRate;
	for (int i = 0; i != numberOfGenome; ++i)
	{
		genome.push_back((rand() % 100) / 100.0);
		this->mutationRange.push_back(mutationRange);
	}
}
void EvolutionaryStratergy::evolveParent()
{
	std::vector<float> child;
	for (int i = 0; i != numberOfChildren; ++i)
	{
		for (float g : genome)
		{
			if ((rand() % 100) / 100.0 < mutationRate)
			{
				if ((rand() % 2) == 0)
					child.push_back(g + mutationRange[0]);
				else
					child.push_back(g - mutationRange[0]);
			}
			else
				child.push_back(g);
		}
		//Should fitness be QFunction or try that state and fitness is reward(GSR)
		if (QFunction(qLearningApprox.state, qLearningApprox.weights, child) > QFunction(qLearningApprox.state, qLearningApprox.weights, genome))
			genome = child;
	}
}
std::vector<float> EvolutionaryStratergy::getParent()
{
	return genome;
}

QLearningApprox::QLearningApprox()
{
	this->learningRate = 0;
	this->discountRate = 0;
	for (int i = 0; i != 0; ++i)
	{
		weights.push_back(0.0);
		state.push_back(0.0);
	}
}

QLearningApprox::QLearningApprox(int numberOfFeatures, float learningRate, float discountRate)
{
	this->learningRate = learningRate;
	this->discountRate = discountRate;
	for (int i = 0; i != numberOfFeatures; ++i)
	{
		weights.push_back(0.0);
		state.push_back(0.0);
	}
}

float QLearningApprox::QFunction(std::vector<float> action)
{
	float QValueEstimate = 0.0;
	std::vector<float>::iterator a = action.begin();
	std::vector<float>::iterator w = weights.begin();
	for (float &s : state)
	{
		QValueEstimate += (s + (*a)) * (*w);
		++a; ++w;
	}
	return QValueEstimate;
}


float QLearningApprox::currentQFunction()
{
	float QValueEstimate = 0.0;
	std::vector<float>::iterator w = weights.begin();
	for (float &s : state)
	{
		QValueEstimate += s * (*w);
		++w;
	}
	return QValueEstimate;
}

//Possible implmentation?
float QLearningApprox::maxQFunction()
{
	EvolutionaryStratergy es((*this), 1000, state.size(), 0.1, 0.1);
	std::vector<float> bestAction;
	es.evolveParent();
	bestAction = es.getParent();
	return QLearningApprox::QFunction(bestAction);
}
float QLearningApprox::maxQFunction(std::vector<float> &action)
{
	EvolutionaryStratergy es((*this), 1000, state.size(), 0.1, 0.1);
	std::vector<float> bestAction;
	es.evolveParent();
	bestAction = action = es.getParent();
	return QLearningApprox::QFunction(action);
}

float QLearningApprox::sumFeatures()
{
	float sumOfFeatures = std::accumulate(state.begin(), state.end(), 0);
	return sumOfFeatures;
}

void QLearningApprox::transitionNewState(float reward, std::vector<float> action)
{
	std::vector<float>::iterator a = action.begin();
	for (float &s : state)
	{
		s += (*a);
		++a;
	}
	updateWeights(reward);
}

//Is this equation right because we only discover reward after making transition? I think so but hard understanding maths.
void QLearningApprox::updateWeights(float reward)
{
	std::vector<float>::iterator s = state.begin();
	for (float &w : weights)
	{
		w = w + learningRate * (reward + (discountRate * maxQFunction()) - currentQFunction()) * (*s);
		++s;
	}
}