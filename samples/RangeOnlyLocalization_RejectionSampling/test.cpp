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

#include <mrpt/slam.h>
#include <mrpt/base.h>

using namespace mrpt;
using namespace mrpt::utils;
using namespace mrpt::slam;
using namespace mrpt::system;
using namespace mrpt::random;
using namespace std;

float	SIGMA = 0.03f;

// ------------------------------------------------------
//				TestRS
// ------------------------------------------------------
void TestRS()
{
	randomGenerator.randomize();

	// Load the map:
	CMultiMetricMap									map;
	mrpt::slam::TSetOfMetricMapInitializers				mapInit;
	mapInit.loadFromConfigFile( CConfigFile("_demo_map.ini"),"MetricMap");
	map.setListOfMaps( &mapInit );

	// Create a dummy observation:
	mrpt::slam::CObservationBeaconRanges					obs;
	mrpt::slam::CObservationBeaconRanges::TMeasurement	meas;
	obs.stdError = SIGMA;

	meas.beaconID = 0;
	meas.sensedDistance = 2.1f;
	meas.sensorLocationOnRobot = CPoint3D(0,0,0);
	obs.sensedData.push_back( meas );

	meas.beaconID = 1;
	meas.sensedDistance = 3.1f;
	meas.sensorLocationOnRobot = CPoint3D(0,0,0);
	obs.sensedData.push_back( meas );

	meas.beaconID = 2;
	meas.sensedDistance = 1.1f;
	meas.sensorLocationOnRobot = CPoint3D(0,0,0);
//	obs.sensedData.push_back( meas );

	// Rejection Sampling:
	CRejectionSamplingRangeOnlyLocalization								RS;
	vector<CRejectionSamplingRangeOnlyLocalization::TParticle>		samples;
	CTicTac		tictac;

	// Set data:
	ASSERT_(map.m_landmarksMap);

	printf("Preparing...");
	tictac.Tic();
		CPose2D	dumPose(0,0,0);
		RS.setParams( *map.m_landmarksMap, obs, SIGMA, dumPose );
	printf("Ok! %fms\n",1000*tictac.Tac());

	printf("Computing...");
	tictac.Tic();
		RS.rejectionSampling( 1000,samples, 1000 );
	printf("Ok! %fms\n",1000*tictac.Tac());

	FILE	*f = os::fopen( "_out_samples.txt","wt");
	vector<CRejectionSamplingRangeOnlyLocalization::TParticle>::iterator	it;
	for (it=samples.begin();it!=samples.end();it++)
		os::fprintf(f,"%f %f %f %e\n",it->d->x(),it->d->y(),it->d->phi(),it->log_w );


	os::fclose(f);
}

// ------------------------------------------------------
//						MAIN
// ------------------------------------------------------
int main()
{
	try
	{
		 TestRS();

		return 0;
	} catch (exception &e)
	{
		cout << "EXCEPCTION: " << e.what() << endl;
		return -1;
	}
	catch (...)
	{
		printf("Untyped excepcion!!");
		return -1;
	}
}

