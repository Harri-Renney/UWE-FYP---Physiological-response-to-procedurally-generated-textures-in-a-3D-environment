#include <float.h>

#include "QLearning.h"

using namespace QLearning;

QLearningApproxLinear::QLearningApproxLinear() : EvolutionaryStrategy(100, 10, 0.1, 0.1) {

}
QLearningApproxLinear::QLearningApproxLinear(int numberOfFeatures, float learningRate, float discountRate)
	: EvolutionaryStrategy(numberOfFeatures, 10, 0.3, 0.001)
{
	this->learningRate = learningRate;
	this->discountRate = discountRate;
	this->max = FLT_MAX;
	this->min = FLT_MIN;
	for (int i = 0; i != numberOfFeatures; ++i)
	{
		std::pair<float, float> placeHolder = std::make_pair(0.01, 0.0);
		state.push_back(placeHolder);
	}
}
QLearningApproxLinear::QLearningApproxLinear(int numberOfFeatures, float learningRate, float discountRate,
	int childrenES, float mutationRateES, float initialMutationDistributionES)
	: EvolutionaryStrategy(numberOfFeatures, childrenES, mutationRateES, initialMutationDistributionES)
{
	this->learningRate = learningRate;
	this->discountRate = discountRate;
	this->max = FLT_MAX;
	this->min = FLT_MIN;
	for (int i = 0; i != numberOfFeatures; ++i)
	{
		//std::pair<float, float> placeHolder = std::make_pair(0.01, 0.0);
		std::pair<float, float> placeHolder = std::make_pair((rand() % 100) / 100.0, 0.0);
		state.push_back(placeHolder);
	}
}

float QLearningApproxLinear::Q()
{
	float QValue = 0.0;
	for (int i = 0; i != state.size(); ++i)		// weighted_sum = f(x_0) * w_0 + ... + f(x_n) * w_n
	{
		QValue += state[i].first * state[i].second;
	}
	return QValue;
}
float QLearningApproxLinear::Q(action a)
{
	float QValue = 0.0;
	for (int i = 0; i != state.size(); ++i)		// weighted_sum = f(x_0) * w_0 + ... + f(x_n) * w_n
	{
		float stateAction = state[i].first + a[i];
		QValue += stateAction * state[i].second;
	}
	return QValue;
}

float QLearningApproxLinear::maxQ()
{
	action maxAction;
	maxAction = getGenome();
	return Q(maxAction);
}
float QLearningApproxLinear::maxQ(action a)
{
	//Unsure how to proceed.
	return 0;
}

action QLearningApproxLinear::maxAction()
{
	action maxAction;
	evolveParent();
	maxAction = getGenome();
	return maxAction;
}
//action QLearningApproxLinear::maxAction(action a)
//{
//	//Unsure how to proceed.
//	return (action)NULL;
//}

void QLearningApproxLinear::updateWeights(float reward, action a)	//Need action here??
{
	float currentQValue = Q();
	float maxQValue = maxQ();
	for (int i = 0; i != state.size(); ++i)		//w = w + lr * (r + (dr * maxQ) - currentQ) * f(s)
	{
		state[i].second += learningRate * ((reward + (discountRate * maxQValue)) - currentQValue) * a[i];	//Debatable. Should the added feature be of the state or of the action just taken??
	}
}

void QLearningApproxLinear::transition(float reward, std::vector<float> action)
{
	for (int i = 0; i != state.size(); ++i)
	{
		if(state[i].first + action[i] > max)
			state[i].first = max;
		else if (state[i].first + action[i] < min)
			state[i].first = min;
		else
			state[i].first += action[i];
	}
	updateWeights(reward, action);
}

float QLearningApproxLinear::fitnessFunction(std::vector<float> genome)
{
	float QValue = 0.0;
	for (int i = 0; i != state.size(); ++i)		// weighted_sum = f(x_0) * w_0 + ... + f(x_n) * w_n
	{
		QValue += (state[i].first + genome[i]) * state[i].second;
	}
	return QValue;
}

void QLearningApproxLinear::setMax(float m)
{
	max = m;
}
void QLearningApproxLinear::setMin(float m)
{
	min = m;
}

std::vector<feature> QLearningApproxLinear::getFeatures()
{
	std::vector<float> features;
	for (int i = 0; i != state.size(); ++i)
	{
		features.push_back(state[i].first);
	}
	return features;
}

std::vector<weight> QLearningApproxLinear::getWeights()
{
	std::vector<float> weights;
	for (int i = 0; i != state.size(); ++i)
	{
		weights.push_back(state[i].second);
	}
	return weights;
}