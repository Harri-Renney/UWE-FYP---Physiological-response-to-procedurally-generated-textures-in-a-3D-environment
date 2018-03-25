#ifndef EVOLUTIONARY_STRATERGY_H
#define EVOLUTIONARY_STRATERGY_H

#include <vector>
#include <utility>

namespace ES {
	typedef float genome;
	typedef float mutationDistribution;

	class EvolutionaryStrategy {
	private:
		int numberOfChildren;
		float mutationRate;

		std::vector<std::pair<genome, mutationDistribution>> parent;

	public:
		//////////////////
		// Constructors //
		//////////////////
		EvolutionaryStrategy();
		EvolutionaryStrategy(int numberOfGenome, int numberOfChildren, float mutationRate, float mutationDistribution);

		float evolveParent();

		/////////////////////////////////////////////////////////////////////////////////////////
		// Fitness function. Inherit and define to decide how genome is evaluated for fitness. //
		/////////////////////////////////////////////////////////////////////////////////////////
		virtual float fitnessFunction(std::vector<float> genome) = 0;

		///////////////////////
		// Getters + Setters //
		///////////////////////

		void setGenome(std::vector<float> g);

		std::vector<std::pair<float, float>>& getParent();
		std::vector<float> getGenome();
		std::vector<float> getMutationDistribution();
	};

}

#endif