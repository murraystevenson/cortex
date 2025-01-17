//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2007-2013, Image Engine Design Inc. All rights reserved.
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

#ifndef IECORE_SIMPLETYPEDDATA_H
#define IECORE_SIMPLETYPEDDATA_H

#include "IECore/Export.h"
#include "IECore/GeometricTypedData.h"
#include "IECore/InternedString.h"
#include "IECore/LineSegment.h"
#include "IECore/TypedData.h"

IECORE_PUSH_DEFAULT_VISIBILITY
#include "OpenEXR/OpenEXRConfig.h"
#if OPENEXR_VERSION_MAJOR < 3
#include "OpenEXR/ImathBox.h"
#include "OpenEXR/ImathColor.h"
#include "OpenEXR/ImathMatrix.h"
#include "OpenEXR/ImathQuat.h"
#include "OpenEXR/ImathVec.h"
#include "OpenEXR/half.h"
#else
#include "Imath/ImathBox.h"
#include "Imath/ImathColor.h"
#include "Imath/ImathMatrix.h"
#include "Imath/ImathQuat.h"
#include "Imath/ImathVec.h"
#include "Imath/half.h"
#endif
IECORE_POP_DEFAULT_VISIBILITY

#include <string>

namespace IECore
{

// simple typeddata types
IECORE_DECLARE_TYPEDDATA( BoolData, bool, bool, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( FloatData, float, float, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( DoubleData, double, double, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( IntData, int, int, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( UIntData, unsigned int, unsigned int, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( CharData, char, char, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( UCharData, unsigned char, unsigned char, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( ShortData, short, short, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( UShortData, unsigned short, unsigned short, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( Int64Data, int64_t, int64_t, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( UInt64Data, uint64_t, uint64_t, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( StringData, std::string, std::string, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( InternedStringData, InternedString, InternedString, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( HalfData, half, half, SimpleDataHolder )

IECORE_DECLARE_GEOMETRICTYPEDDATA( V2iData, Imath::V2i, int, SimpleDataHolder )
IECORE_DECLARE_GEOMETRICTYPEDDATA( V3iData, Imath::V3i, int, SimpleDataHolder )
IECORE_DECLARE_GEOMETRICTYPEDDATA( V2fData, Imath::V2f, float, SimpleDataHolder )
IECORE_DECLARE_GEOMETRICTYPEDDATA( V3fData, Imath::V3f, float, SimpleDataHolder )
IECORE_DECLARE_GEOMETRICTYPEDDATA( V2dData, Imath::V2d, double, SimpleDataHolder )
IECORE_DECLARE_GEOMETRICTYPEDDATA( V3dData, Imath::V3d, double, SimpleDataHolder )

IECORE_DECLARE_TYPEDDATA( Color3fData, Imath::Color3f, float, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( Color4fData, Imath::Color4f, float, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( Box2iData, Imath::Box2i, int, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( Box3iData, Imath::Box3i, int, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( Box2fData, Imath::Box2f, float, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( Box3fData, Imath::Box3f, float, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( Box2dData, Imath::Box2d, double, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( Box3dData, Imath::Box3d, double, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( M33fData, Imath::M33f, float, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( M33dData, Imath::M33d, double, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( M44fData, Imath::M44f, float, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( M44dData, Imath::M44d, double, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( QuatfData, Imath::Quatf, float, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( QuatdData, Imath::Quatd, double, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( LineSegment3fData, LineSegment3f, float, SimpleDataHolder )
IECORE_DECLARE_TYPEDDATA( LineSegment3dData, LineSegment3d, double, SimpleDataHolder )

#if !defined(IECore_EXPORTS) && !defined(_MSC_VER)

extern template class TypedData<bool>;
extern template class TypedData<float>;
extern template class TypedData<double>;
extern template class TypedData<int>;
extern template class TypedData<unsigned int>;
extern template class TypedData<char>;
extern template class TypedData<unsigned char>;
extern template class TypedData<short>;
extern template class TypedData<unsigned short>;
extern template class TypedData<int64_t>;
extern template class TypedData<uint64_t>;
extern template class TypedData<std::string>;
extern template class TypedData<InternedString>;
extern template class TypedData<half>;
extern template class TypedData<Imath::V2i>;
extern template class TypedData<Imath::V3i>;
extern template class TypedData<Imath::V2f>;
extern template class TypedData<Imath::V3f>;
extern template class TypedData<Imath::V2d>;
extern template class TypedData<Imath::V3d>;
extern template class TypedData<Imath::Color3f>;
extern template class TypedData<Imath::Color4f>;
extern template class TypedData<Imath::Box2i>;
extern template class TypedData<Imath::Box3i>;
extern template class TypedData<Imath::Box2f>;
extern template class TypedData<Imath::Box3f>;
extern template class TypedData<Imath::Box2d>;
extern template class TypedData<Imath::Box3d>;
extern template class TypedData<Imath::M33f>;
extern template class TypedData<Imath::M33d>;
extern template class TypedData<Imath::M44f>;
extern template class TypedData<Imath::M44d>;
extern template class TypedData<Imath::Quatf>;
extern template class TypedData<Imath::Quatd>;
extern template class TypedData<LineSegment3f>;
extern template class TypedData<LineSegment3d>;

#endif

} // namespace IECore

#endif // IECORE_SIMPLETYPEDDATA_H
