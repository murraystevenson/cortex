//////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2013, Image Engine Design Inc. All rights reserved.
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

#include "boost/python.hpp"

#include "OP/OP_Node.h"

#include "IECorePython/RunTimeTypedBinding.h"

#include "IECoreHoudini/ModelCacheNode.h"
#include "IECoreHoudini/OBJ_ModelCacheTransform.h"
#include "IECoreHoudini/bindings/ModelCacheNodeBinding.h"

using namespace boost::python;
using namespace IECoreHoudini;

class ModelCacheNodeHelper
{
};

void IECoreHoudini::bindModelCacheNode()
{
	scope modeCacheNodeScope = class_<ModelCacheNodeHelper>( "ModelCacheNode" );
	
	enum_<ModelCacheNode<OP_Node>::Space>( "Space" )
		.value( "World", ModelCacheNode<OP_Node>::World )
		.value( "Path", ModelCacheNode<OP_Node>::Path )
		.value( "Local", ModelCacheNode<OP_Node>::Local )
		.value( "Object", ModelCacheNode<OP_Node>::Object )
	;
	
	enum_<OBJ_ModelCacheTransform::Hierarchy>( "Hierarchy" )
		.value( "Parenting", OBJ_ModelCacheTransform::Parenting )
		.value( "SubNetworks", OBJ_ModelCacheTransform::SubNetworks )
		.value( "FlatGeometry", OBJ_ModelCacheTransform::FlatGeometry )
	;
	
	enum_<OBJ_ModelCacheTransform::Depth>( "Depth" )
		.value( "AllDescendants", OBJ_ModelCacheTransform::AllDescendants )
		.value( "Children", OBJ_ModelCacheTransform::Children )
	;
}