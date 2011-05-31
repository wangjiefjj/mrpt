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
#ifndef CMATRIX_H
#define CMATRIX_H

#include <mrpt/utils/CSerializable.h>
#include <mrpt/math/CMatrixTemplateNumeric.h>
#include <mrpt/math/CMatrixFixedNumeric.h>
#include <mrpt/utils/CStream.h>

namespace mrpt
{
	namespace math
	{
		// This must be added to any CSerializable derived class:
		// Note: instead of the standard "DEFINE_SERIALIZABLE_PRE_CUSTOM_BASE", classes inheriting
		// from templates need special nasty handling for MSVC DLL exports...
		DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_LINKAGE2(CMatrix, mrpt::utils::CSerializable, CMatrix)
		BASE_IMPEXP ::mrpt::utils::CStream& operator>>(mrpt::utils::CStream& in, CMatrixPtr &pObj);

		/**  This class is a "CSerializable" wrapper for "CMatrixFloat".
		 * \note For a complete introduction to Matrices and vectors in MRPT, see: http://www.mrpt.org/Matrices_vectors_arrays_and_Linear_Algebra_MRPT_and_Eigen_classes
		 */
		class BASE_IMPEXP_TEMPL CMatrix : public mrpt::utils::CSerializable, public CMatrixFloat
		{
			// This must be added to any CSerializable derived class:
			//DEFINE_SERIALIZABLE( CMatrix )
			//DEFINE_MRPT_OBJECT(CMatrix)
		protected:
			static  const mrpt::utils::TRuntimeClassId* _GetBaseClass();
			static mrpt::utils::CLASSINIT _init_CMatrix;
		public:
			/*! A typedef for the associated smart pointer */
			typedef CMatrixPtr SmartPtr;
			static BASE_IMPEXP  mrpt::utils::TRuntimeClassId  classCMatrix;
			static BASE_IMPEXP  const mrpt::utils::TRuntimeClassId *classinfo;
			virtual BASE_IMPEXP  const mrpt::utils::TRuntimeClassId* GetRuntimeClass() const;
			static  BASE_IMPEXP mrpt::utils::CObject* CreateObject();
			static BASE_IMPEXP CMatrixPtr Create();
			virtual BASE_IMPEXP mrpt::utils::CObject *duplicate() const;
		protected:
			/*! @name CSerializable virtual methods */
			/*! @{ */
			BASE_IMPEXP void writeToStream(mrpt::utils::CStream &out, int *getVersion) const;
			BASE_IMPEXP void readFromStream(mrpt::utils::CStream &in, int version);
			/*! @} */

		public:
			/** Constructor  */
			CMatrix() : CMatrixFloat(1,1)
			{ }

			/** Constructor  */
			CMatrix(size_t row, size_t col) : CMatrixFloat(row,col)
			{ }

			/** Copy constructor
			  */
			CMatrix( const CMatrixFloat &m ) : CMatrixFloat(m)
			{ }

			/** Copy constructor
			  */
			CMatrix( const CMatrixTemplateNumeric<double> &m ) : CMatrixFloat(0,0)
			{
				*this = m.eval().cast<float>();
			}

			/** Constructor from a TPose2D, which generates a 3x1 matrix \f$ [x y \phi]^T \f$
			   */
			explicit CMatrix( const TPose2D &p) : CMatrixFloat(p) {}

			/** Constructor from a mrpt::poses::CPose6D, which generates a 6x1 matrix \f$ [x y z yaw pitch roll]^T \f$
			   */
			explicit CMatrix( const TPose3D &p) : CMatrixFloat(p) {}

			/** Constructor from a TPoint2D, which generates a 2x1 matrix \f$ [x y]^T \f$
			   */
			explicit CMatrix( const TPoint2D &p) : CMatrixFloat(p) {}

			/** Constructor from a TPoint3D, which generates a 3x1 matrix \f$ [x y z]^T \f$
			   */
			explicit CMatrix( const TPoint3D &p) : CMatrixFloat(p) {}

			/** Assignment operator for float matrixes
			*/
			template <class OTHERMAT>
			inline CMatrix & operator = (const OTHERMAT& m)
			{
				CMatrixFloat::operator =(m);
				return *this;
			}

			/*! Assignment operator from any other Eigen class */
			template<typename OtherDerived>
			inline CMatrix & operator= (const Eigen::MatrixBase <OtherDerived>& other) {
				CMatrixTemplateNumeric<float>::operator=(other);
				return *this;
			}
			/*! Constructor from any other Eigen class */
			template<typename OtherDerived>
			inline CMatrix(const Eigen::MatrixBase <OtherDerived>& other) : CMatrixTemplateNumeric<float>(other) { }

		}; // end of class definition

	} // End of namespace
} // End of namespace

#endif
