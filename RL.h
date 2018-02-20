#ifndef RL_H
#define RL_H

#include <vector>
#include <numeric>

class QLearningApprox {
	

	float learningRate;
	float discountRate;

public:
	std::vector<float> state;
	std::vector<float> weights;
	QLearningApprox();
	QLearningApprox(int numberOfFeatures, float learningRate, float discountRate);
	float QFunction(std::vector<float> action);
	float currentQFunction();
	float maxQFunction();
	float maxQFunction(std::vector<float> &action);
	float sumFeatures();
	void transitionNewState(float reward, std::vector<float> action);

	//Is this equation right because we only discover reward after making transition? I think so but hard understanding maths.
	void updateWeights(float reward);
};

class EvolutionaryStratergy {
	QLearningApprox &qLearningApprox;
	std::vector<float> genome;
	std::vector<float> mutationRange;
	float mutationRate;
	int numberOfChildren;

public:
	EvolutionaryStratergy(QLearningApprox &qLearningApprox, int numberChildren, int numberOfGenome, float mutationRange, float mutationRate);
	void evolveParent();
	std::vector<float> getParent();
};

#endif