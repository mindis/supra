// ================================================================================================
// 
// If not explicitly stated: Copyright (C) 2016, all rights reserved,
//      Rüdiger Göbl 
//		Email r.goebl@tum.de
//      Chair for Computer Aided Medical Procedures
//      Technische Universität München
//      Boltzmannstr. 3, 85748 Garching b. München, Germany
// 
// ================================================================================================

#ifndef __IQDEMODULATOR_H__
#define __IQDEMODULATOR_H__

#include "Container.h"

#include <memory>

namespace supra
{
	class IQDemodulator
	{
	public:
		typedef float WorkType;

		IQDemodulator(
			double samplingFrequency,
			double referenceFrequency,
			double cutoffFrequency,
			size_t lowpassFilterLength,
			size_t bandpassFilterLength);
		~IQDemodulator();

		template<typename InputType, typename OutputType>
		std::shared_ptr<Container<OutputType> >
			demodulateMagnitudeFrequencyCompounding(
				const std::shared_ptr<const Container<InputType> >& inImageData,
				int numScanlines, int numSamples, uint32_t decimation,
				const std::vector<double>& referenceFrequencies,
				const std::vector<double>& bandwidths,
				const std::vector<double>& weights);
		int decimatedSignalLength(int numSamples, uint32_t decimation);

	private:
		void prepareFilter();

		double m_samplingFrequency;
		double m_cutoffFrequency;
		size_t m_decimationLowpassFilterLength;
		size_t m_frequencyCompoundingBandpassFilterLength;

		std::shared_ptr<Container<WorkType> > m_decimationLowpassFilter;
		std::vector<std::shared_ptr<Container<WorkType> > > m_frequencyCompoundingBandpassFilters;
		std::shared_ptr<Container<WorkType> > m_frequencyCompoundingBandpassFilterBank;

		std::vector<double> m_frequencyCompoundingReferenceFrequencies;
		std::vector<double> m_frequencyCompoundingBandwidths;
		std::vector<double> m_frequencyCompoundingWeights;
		std::shared_ptr<Container<WorkType> > m_frequencyCompoundingReferenceFrequenciesOverSamplingFrequencies;
		std::shared_ptr<Container<WorkType> > m_frequencyCompoundingWeightsGpu;
	};
}

#endif //!__IQDEMODULATOR_H__
