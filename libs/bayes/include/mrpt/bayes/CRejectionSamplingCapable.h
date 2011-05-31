/* +---------------------------------------------------------------------------+
   |          The Mobile Robot Programming Toolkit (MRPT) C++ library          |
   |                                                                           |
   |                       http://www.mrpt.org/                                |
   |                                                                           |
   |   Copyright (C) 2005-2011  University of Malaga                           |
   |                                                                           |
   |    This software was written by the Machine Perception and Intelligent    |
   |      Robotics Lab, University of Malaga (Spain).                          |
   |    Contact: Jose-Luis Blanco  <jlblanco@ctima.uma.es>                     |
   |                                                                           |
   |  This file is part of the MRPT project.                                   |
   |                                                                           |
   |     MRPT is free software: you can redistribute it and/or modify          |
   |     it under the terms of the GNU General Public License as published by  |
   |     the Free Software Foundation, either version 3 of the License, or     |
   |     (at your option) any later version.                                   |
   |                                                                           |
   |   MRPT is distributed in the hope that it will be useful,                 |
   |     but WITHOUT ANY WARRANTY; without even the implied warranty of        |
   |     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
   |     GNU General Public License for more details.                          |
   |                                                                           |
   |     You should have received a copy of the GNU General Public License     |
   |     along with MRPT.  If not, see <http://www.gnu.org/licenses/>.         |
   |                                                                           |
   +---------------------------------------------------------------------------+ */
#ifndef CRejectionSamplingCapable_H
#define CRejectionSamplingCapable_H

#include <mrpt/utils/utils_defs.h>
#include <mrpt/bayes/CProbabilityParticle.h>
#include <mrpt/random.h>

namespace mrpt
{
namespace bayes
{
	/** A base class for implementing rejection sampling in a generic state space.
	 *   See the main method CRejectionSamplingCapable::rejectionSampling
	 *  To use this class, create your own class as a child of this one and implement the desired
	 *   virtual methods, and add any required internal data.
	 */
	template <class TStateSpace>
	class CRejectionSamplingCapable
	{
	public:
		typedef CProbabilityParticle<TStateSpace> TParticle;

        /** Virtual destructor
          */
        virtual ~CRejectionSamplingCapable()
		{
		}

		/** Generates a set of N independent samples via rejection sampling.
		  * \param desiredSamples The number of desired samples to generate
		  * \param outSamples The output samples.
		  * \param timeoutTrials The maximum number of rejection trials for each generated sample (i.e. the maximum number of iterations). This can be used to set a limit to the time complexity of the algorithm for difficult probability densities.
		  *  All will have equal importance weights (a property of rejection sampling), although those samples
		  *   generated at timeout will have a different importance weights.
		  */
		void rejectionSampling(
			size_t							desiredSamples,
			std::vector<TParticle>			&outSamples,
			size_t							timeoutTrials = 1000)
		{
			MRPT_START;

			TStateSpace							x;
			typename std::vector<TParticle>::iterator	it;

			// Set output size:
			if ( outSamples.size() != desiredSamples )
			{
				// Free old memory:
				for (it = outSamples.begin();it!=outSamples.end();it++)
					delete (it->d);
				outSamples.clear();

				// Reserve new memory:
				outSamples.resize( desiredSamples );
				for (it = outSamples.begin();it!=outSamples.end();it++)
					it->d = new TStateSpace;
			}

			// Rejection sampling loop:
			double	acceptanceProb;
			for (it = outSamples.begin();it!=outSamples.end();it++)
			{
				size_t	timeoutCount = 0;
				double		bestLik = -1e250;
				TStateSpace	bestVal;
				do
				{
					RS_drawFromProposal( *it->d );
					acceptanceProb = RS_observationLikelihood( *it->d );
					ASSERT_(acceptanceProb>=0 && acceptanceProb<=1);
					if (acceptanceProb>bestLik)
					{
						bestLik = acceptanceProb;
						bestVal = *it->d;
					}
				} while (	acceptanceProb < mrpt::random::randomGenerator.drawUniform(0.0,0.999) &&
							(++timeoutCount)<timeoutTrials );

				// Save weights:
				if (timeoutCount>=timeoutTrials)
				{
					it->log_w = log(bestLik);
					*it->d    = bestVal;
				}
				else
				{
					it->log_w = 0; // log(1.0);
				}
			} // end for it

			MRPT_END;
		}

	protected:
		/** Generates one sample, drawing from some proposal distribution.
		  */
		virtual void RS_drawFromProposal( TStateSpace &outSample ) = 0;

		/** Returns the NORMALIZED observation likelihood (linear, not exponential!!!) at a given point of the state space (values in the range [0,1]).
		  */
		virtual double RS_observationLikelihood( const TStateSpace &x) = 0;

	}; // End of class def.

} // End of namespace
} // End of namespace

#endif
