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

#include <mrpt/base.h>  // Precompiled headers 


#include <mrpt/synch.h>
#include <mrpt/utils/CStream.h>

#include <iostream>

using namespace mrpt::utils;
using namespace mrpt::synch;
using namespace std;

#define CS_LOCKER_VERBOSE  0

/*---------------------------------------------------------------
				CCriticalSectionLocker
---------------------------------------------------------------*/
CCriticalSectionLocker::CCriticalSectionLocker( const CCriticalSection * cs)
	: m_cs(cs)
{
#if CS_LOCKER_VERBOSE
	cout << "[CCriticalSectionLocker] Locking " << static_cast<const void*>(m_cs) << ": " << m_cs->getName() << endl;
#endif
	m_cs->enter();
}

/*---------------------------------------------------------------
				~CCriticalSectionLocker
---------------------------------------------------------------*/
CCriticalSectionLocker::~CCriticalSectionLocker()
{
#if CS_LOCKER_VERBOSE
	cout << "[CCriticalSectionLocker] Unlocking " << static_cast<const void*>(m_cs) << ": " << m_cs->getName() << endl;
#endif
	m_cs->leave();
}
