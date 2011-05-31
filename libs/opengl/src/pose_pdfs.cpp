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

#include <mrpt/opengl.h>  // Precompiled header

#include <mrpt/opengl/CEllipsoid.h>
#include <mrpt/opengl/CSetOfObjects.h>
#include <mrpt/opengl/CSetOfLines.h>
#include <mrpt/opengl/CPointCloud.h>
#include <mrpt/opengl/stock_objects.h>

#include <mrpt/poses.h>

using namespace mrpt;
using namespace mrpt::utils;
using namespace mrpt::opengl;
using namespace mrpt::poses;


const double POSE_TAIL_LENGTH = 0.1;
const double POSE_TAIL_WIDTH  = 3.0;
const double POSE_POINT_SIZE  = 4.0;
const double POSE_AXIS_SCALE  = 0.1;

#define POSE_COLOR		0,0,1
#define POINT_COLOR		1,0,0


namespace mrpt
{
	namespace opengl
	{

/** Returns a representation of a the PDF - this is just an auxiliary function, it's more natural to call
	*    mrpt::poses::CPosePDF::getAs3DObject     */
template <>
CSetOfObjectsPtr posePDF2opengl<CPosePDF,CSetOfObjectsPtr>(const CPosePDF &o)
{
	CSetOfObjectsPtr outObj = CSetOfObjects::Create();

	if (IS_CLASS(&o,CPosePDFSOG))
	{
		const CPosePDFSOG *p = static_cast<const CPosePDFSOG*>(&o);

		opengl::CSetOfLinesPtr lins = opengl::CSetOfLines::Create();
		lins->setColor(0,0,1,0.6);
		lins->setLineWidth(POSE_TAIL_WIDTH);

		for (CPosePDFSOG::const_iterator it=p->begin();it!=p->end();++it)
		{
			opengl::CEllipsoidPtr ellip = opengl::CEllipsoid::Create();

			ellip->setPose( CPose3D((it)->mean.x(), (it)->mean.y(), 0) );
			ellip->setCovMatrix((it)->cov, 2 /* x y */ );
			ellip->setColor(POSE_COLOR,0.6);
			ellip->setQuantiles(3);
			ellip->enableDrawSolid3D(false);

			outObj->insert(ellip);

			lins->appendLine(
				(it)->mean.x(), (it)->mean.y(), 0,
				(it)->mean.x() + POSE_TAIL_LENGTH * cos((it)->mean.phi()) , (it)->mean.y() +  POSE_TAIL_LENGTH * sin((it)->mean.phi()) , 0
				);
		}
		outObj->insert(lins);
	} else
	if (IS_CLASS(&o,CPosePDFGaussian))
	{
		const CPosePDFGaussian *p = static_cast<const CPosePDFGaussian*>(&o);

		opengl::CSetOfLinesPtr lins = opengl::CSetOfLines::Create();
		lins->setColor(POSE_COLOR,0.6);
		lins->setLineWidth(POSE_TAIL_WIDTH);

		opengl::CEllipsoidPtr ellip = opengl::CEllipsoid::Create();

		ellip->setPose( CPose3D(p->mean.x(), p->mean.y(), 0) );
		ellip->setCovMatrix(p->cov, 2 /* x y */ );
		ellip->setColor(POSE_COLOR,0.6);

		ellip->setQuantiles(3);
		ellip->enableDrawSolid3D(false);

		outObj->insert(ellip);

		lins->appendLine(
			p->mean.x(), p->mean.y(), 0,
			p->mean.x() + POSE_TAIL_LENGTH * cos(p->mean.phi()) , p->mean.y() +  POSE_TAIL_LENGTH * sin(p->mean.phi()) , 0
			);

		outObj->insert(lins);
	} else
	if (IS_CLASS(&o,CPosePDFParticles))
	{
		const CPosePDFParticles *p = static_cast<const CPosePDFParticles*>(&o);

		opengl::CPointCloudPtr  pnts = opengl::CPointCloud::Create();
		pnts->setColor(POSE_COLOR,0.6);
		pnts->setPointSize(POSE_POINT_SIZE);

		opengl::CSetOfLinesPtr lins = opengl::CSetOfLines::Create();
		lins->setColor(POSE_COLOR,0.6);
		lins->setLineWidth(POSE_TAIL_WIDTH);

		for (size_t i=0;i<p->size();++i)
		{
			pnts->insertPoint(p->m_particles[i].d->x(), p->m_particles[i].d->y(), 0);
			lins->appendLine(
				p->m_particles[i].d->x(), p->m_particles[i].d->y(), 0,
				p->m_particles[i].d->x() + POSE_TAIL_LENGTH * cos(p->m_particles[i].d->phi()), p->m_particles[i].d->y() + POSE_TAIL_LENGTH * sin(p->m_particles[i].d->phi()), 0
				);
		}
		outObj->insert(pnts);
		outObj->insert(lins);
	}


	return outObj;
}

/** Returns a representation of a the PDF - this is just an auxiliary function, it's more natural to call
	*    mrpt::poses::CPointPDF::getAs3DObject     */
template <>
CSetOfObjectsPtr posePDF2opengl<CPointPDF,CSetOfObjectsPtr>(const CPointPDF &o)
{
	CSetOfObjectsPtr outObj = CSetOfObjects::Create();

	if (IS_CLASS(&o,CPointPDFSOG))
	{
		const CPointPDFSOG *p = static_cast<const CPointPDFSOG*>(&o);

		// For each gaussian node
		for (CPointPDFSOG::CListGaussianModes::const_iterator it = p->begin(); it!= p->end();it++)
		{
			opengl::CEllipsoidPtr obj = opengl::CEllipsoid::Create();

			obj->setPose( it->val.mean);
			obj->setCovMatrix(it->val.cov,  it->val.cov(2,2)==0  ?  2:3);

			obj->setQuantiles(3);
			obj->enableDrawSolid3D(false);
			obj->setColor(POINT_COLOR);

			outObj->insert( obj );
		} // end for each gaussian node
	}
	else if (IS_CLASS(&o, CPointPDFGaussian))
	{
		const CPointPDFGaussian *p = static_cast<const CPointPDFGaussian*>(&o);

		CEllipsoidPtr obj = CEllipsoid::Create();
		obj->setLocation(p->mean);
		obj->setCovMatrix(p->cov,  p->cov(2,2)==0  ?  2:3);
		obj->setColor(POINT_COLOR);
		obj->setQuantiles(3);
		obj->enableDrawSolid3D(false);
		outObj->insert( obj );
	}
	else if (IS_CLASS(&o, CPointPDFParticles))
	{
		const CPointPDFParticles *p = static_cast<const CPointPDFParticles*>(&o);

		mrpt::opengl::CPointCloudPtr obj = mrpt::opengl::CPointCloud::Create();
		const size_t N=p->size();

		obj->resize(N);
		obj->setColor(POINT_COLOR);
		for (size_t i=0;i<N;i++)
			obj->setPoint(
				i,
				p->m_particles[i].d->x,
				p->m_particles[i].d->y,
				p->m_particles[i].d->z );
		outObj->insert( obj );
	}

	return outObj;
}

/** Returns a representation of a the PDF - this is just an auxiliary function, it's more natural to call
	*    mrpt::poses::CPose3DPDF::getAs3DObject     */
template <>
CSetOfObjectsPtr posePDF2opengl<CPose3DPDF,CSetOfObjectsPtr>(const CPose3DPDF &o)
{
	CSetOfObjectsPtr outObj = CSetOfObjects::Create();

	if (IS_CLASS(&o,CPose3DPDFSOG))
	{
		const CPose3DPDFSOG *p = static_cast<const CPose3DPDFSOG*>(&o);

		// For each gaussian node
		for (CPose3DPDFSOG::const_iterator it = p->begin(); it!= p->end();it++)
		{
			opengl::CEllipsoidPtr obj = opengl::CEllipsoid::Create();

			obj->setPose( it->val.mean);
			obj->setCovMatrix(CMatrixDouble(it->val.cov),  it->val.cov(2,2)==0  ?  2:3);

			obj->setQuantiles(3);
			obj->enableDrawSolid3D(false);
			obj->setColor(POSE_COLOR);

			outObj->insert( obj );

			opengl::CSetOfObjectsPtr axes = opengl::stock_objects::CornerXYZ();
			axes->setPose(it->val.mean);
			axes->setScale(POSE_AXIS_SCALE);
			outObj->insert(axes);
		} // end for each gaussian node
	}
	else
	if (IS_CLASS(&o,CPose3DPDFGaussian))
	{
		const CPose3DPDFGaussian *p = static_cast<const CPose3DPDFGaussian*>(&o);

		opengl::CEllipsoidPtr obj = opengl::CEllipsoid::Create();

		obj->setPose( p->mean);
		obj->setCovMatrix(CMatrixDouble(p->cov),  p->cov(2,2)==0  ?  2:3);

		obj->setQuantiles(3);
		obj->enableDrawSolid3D(false);
		obj->setColor(POSE_COLOR);

		outObj->insert( obj );

		opengl::CSetOfObjectsPtr axes = opengl::stock_objects::CornerXYZ();
		axes->setPose(p->mean);
		axes->setScale(POSE_AXIS_SCALE);
		outObj->insert(axes);
	}
	else
	if (IS_CLASS(&o,CPose3DPDFParticles))
	{
		const CPose3DPDFParticles *p = static_cast<const CPose3DPDFParticles*>(&o);

		for (size_t i=0;i<p->size();i++)
		{
			opengl::CSetOfObjectsPtr axes = opengl::stock_objects::CornerXYZ();
			axes->setPose(*p->m_particles[i].d);
			axes->setScale(POSE_AXIS_SCALE);
			outObj->insert(axes);
		}

	}

	return outObj;
}

/** Returns a representation of a the PDF - this is just an auxiliary function, it's more natural to call
	*    mrpt::poses::CPose3DQuatPDF::getAs3DObject     */
template <>
CSetOfObjectsPtr posePDF2opengl<CPose3DQuatPDF,CSetOfObjectsPtr>(const CPose3DQuatPDF &o)
{
	CSetOfObjectsPtr outObj = CSetOfObjects::Create();

	if (IS_CLASS(&o,CPose3DQuatPDFGaussian))
	{
		const CPose3DQuatPDFGaussian *p = static_cast<const CPose3DQuatPDFGaussian*>(&o);

		opengl::CEllipsoidPtr obj = opengl::CEllipsoid::Create();

		obj->setPose( p->mean);
		obj->setCovMatrix(CMatrixDouble(p->cov),  p->cov(2,2)==0  ?  2:3);

		obj->setQuantiles(3);
		obj->enableDrawSolid3D(false);
		obj->setColor(POSE_COLOR);

		outObj->insert( obj );

		opengl::CSetOfObjectsPtr axes = opengl::stock_objects::CornerXYZ();
		axes->setPose(CPose3D(p->mean));
		axes->setScale(POSE_AXIS_SCALE);
		outObj->insert(axes);
	}

	return outObj;
}


	} // end NS
} // end NS
