//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2012, John Haddon. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//     * Neither the name of Image Engine Design nor the names of any
//       other contributors to this software may be used to endorse or
//       promote products derived from this software without specific prior
//       written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//////////////////////////////////////////////////////////////////////////

#ifndef IECOREALEMBIC_ALEMBICINPUT_H
#define IECOREALEMBIC_ALEMBICINPUT_H

#include "boost/shared_ptr.hpp"

#include "IECore/RefCounted.h"
#include "IECore/CompoundData.h"
#include "IECore/VectorTypedData.h"

namespace IECoreAlembic
{

IE_CORE_FORWARDDECLARE( AlembicInput )

/// This class provides very high level access to the contents
/// of an Alembic cache. It deliberately hides all Alembic
/// data types and provides an interface tailored to the
/// reading of 3D scene data only. Finer control and lower level
/// access can be obtained by using a combination of the Alembic
/// APIs and the FromAlembicConverters.
class AlembicInput : public IECore::RefCounted
{

	public :

		IE_CORE_DECLAREMEMBERPTR( AlembicInput );

		AlembicInput( const std::string &fileName );
		virtual ~AlembicInput();
		
		//! @name Metadata
		////////////////////////////////////////////////////////////
		//@{
		const std::string &name() const;
		const std::string &fullName() const;
		IECore::CompoundDataPtr metaData() const;
		//@}
		
		//! @name Sampling and time
		/// Each level of the hierarchy may be sampled at differing
		/// points in time. These functions provide queries mapping
		/// from time to sample indices and back. The indices may
		/// then be used in the conversion functions below. Note that
		/// the values returned for the top level input (constructed
		/// via fileName) are only valid in the case of the whole
		/// cache using the same sampling. 
		////////////////////////////////////////////////////////////
		//@{
		/// Returns the number of samples.
		size_t numSamples() const;
		/// Returns the time associated with the specified sample.
		double sampleTime( size_t sampleIndex ) const;
		/// Computes a sample interval suitable for use in producing interpolated
		/// values, returning the appropriate lerp factor between the two samples.
		/// In the case of time falling outside of the sample range, or coinciding
		/// nearly exactly with a single sample, 0 is returned and floorIndex==ceilIndex
		/// will hold.
		double sampleInterval( double time, size_t &floorIndex, size_t &ceilIndex ) const;
		//@}
		
		//! @name Conversion
		////////////////////////////////////////////////////////////
		//@{
		/// Returns the bounding box of everything below this point in the
		/// hierarchy, without the transform() applied.
		Imath::Box3d bound() const;
		/// Returns the transformation matrix of this node if it has one,
		/// and the identity otherwise.
		Imath::M44d transform() const;
		/// Converts the alembic object into Cortex form, preferring conversions
		/// yielding the specified result type.
		IECore::ObjectPtr convert( IECore::TypeId resultType = IECore::ObjectTypeId ) const;
		//@}
		
		//! @name Child access
		////////////////////////////////////////////////////////////
		//@{
		size_t numChildren() const;
		AlembicInputPtr child( size_t index ) const;
		IECore::StringVectorDataPtr childNames() const;
		AlembicInputPtr child( const std::string &name ) const;
		//@}

	private :
	
		AlembicInput();
	
		void ensureTimeSampling() const;
	
		struct DataMembers;
		
		boost::shared_ptr<DataMembers> m_data;

};

} // namespace IECoreAlembic

#endif // IECOREALEMBIC_ALEMBICINPUT_H