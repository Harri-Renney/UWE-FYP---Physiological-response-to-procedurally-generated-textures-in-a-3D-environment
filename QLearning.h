#ifndef Q_LEARNING_H
#define Q_LEARNING_H

#include <utility>
#include <vector>

#include "EvolutionaryStrategy.h"

namespace QLearning {
	typedef std::vector<float> action;
	typedef float feature;
	typedef float weight;

	class QLearningDiscrete {

	};

	class QLearningApproxLinear : ES::EvolutionaryStrategy {
	private:
		std::vector<std::pair<feature, weight>> state;
		float learningRate;
		float discountRate;

		float max;
		float min;

		void updateWeights(float reward, action a);
	public:
		//////////////////
		// Constructors //
		//////////////////
		QLearningApproxLinear();
		QLearningApproxLinear(int numberOfFeatures, float learningRate, float discountRate);
		QLearningApproxLinear(int numberOfFeatures, float learningRate, float discountRate, int childrenES, float mutationRateES, float initialMutationDistributionES);

		float Q();
		float Q(action a);
		float maxQ();
		float maxQ(action a);
		action maxAction();
		action maxAction(action a);

		void transition(float reward, std::vector<float> action);

		/////////////////////////////////////////////////////////
		// Pure virtual function for deciding fitness override //
		/////////////////////////////////////////////////////////
		float fitnessFunction(std::vector<float> genome) override;

		void setMax(float m);
		void setMin(float m);

		std::vector<feature> getFeatures();
		std::vector<feature> getWeights();
	};

	class QLearningApproxANN {

	};
}

#endif