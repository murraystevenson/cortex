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

// This include needs to be the very first to prevent problems with warnings
// regarding redefinition of _POSIX_C_SOURCE
#include "boost/python.hpp"

#include <cassert>
#include <iostream>

#include "IECore/IndexedIO.h"
#include "IECore/FileIndexedIO.h"
#include "IECore/MemoryIndexedIO.h"
#include "IECore/VectorTypedData.h"
#include "IECore/SimpleTypedData.h"

#include "IECorePython/RefCountedBinding.h"
#include "IECorePython/IECoreBinding.h"

using namespace boost::python;
using namespace IECore;

void bindIndexedIOEntry(const char *bindName);

void bindIndexedIO(const char *bindName);
void bindFileIndexedIO(const char *bindName);
void bindMemoryIndexedIO(const char *bindName);

void bindIndexedIO()
{
	bindIndexedIOEntry("IndexedIOEntry");

	bindIndexedIO("IndexedIO");
	bindFileIndexedIO("FileIndexedIO");
	bindMemoryIndexedIO("MemoryIndexedIO");
}

struct IndexedIOHelper
{
	static void listToEntryIds( const list &path, IndexedIO::EntryIDList &entries )
	{
		int rootLen = IECorePython::len( path );
		for (int i = 0; i < rootLen; i++ )
		{
			extract< std::string > ex( path[i] );
			if ( !ex.check() )
			{
				throw InvalidArgumentException( std::string( "Invalid root! Should be a list of strings!" ) );
			}
			entries.push_back( ex() );
		}
	}

	static list entryIDsToList( const IndexedIO::EntryIDList &l )
	{
		list result;
		for( IndexedIO::EntryIDList::const_iterator it = l.begin(); it != l.end(); it++ )
		{
			result.append( *it );
		}
		return result;
	}

	template< typename T, typename P >
	static typename T::Ptr constructorAtRoot( P firstParam, IndexedIO::OpenMode mode )
	{
		return new T( firstParam, IndexedIO::rootPath, mode );
	}

	template< typename T, typename P >
	static typename T::Ptr constructor( P firstParam, list root, IndexedIO::OpenMode mode )
	{
		IndexedIO::EntryIDList rootPath;
		IndexedIOHelper::listToEntryIds( root, rootPath );
		return new T( firstParam, rootPath, mode );
	}

	static IndexedIOPtr createAtRoot( const std::string &path, IndexedIO::OpenMode mode)
	{
		return IndexedIO::create( path, IndexedIO::rootPath, mode );
	}

	static IndexedIOPtr create( const std::string &path, list root, IndexedIO::OpenMode mode)
	{
		IndexedIO::EntryIDList rootPath;
		IndexedIOHelper::listToEntryIds( root, rootPath );
		return IndexedIO::create( path, rootPath, mode );
	}

	static IndexedIO::Entry entry(IndexedIOPtr p, const IndexedIO::EntryID &name)
	{
		assert(p);

		return p->entry(name);
	}

	static IndexedIOPtr subdirectory(IndexedIOPtr p, const IndexedIO::EntryID &name)
	{
		return p->subdirectory(name);
	}

	static IndexedIOPtr directory(IndexedIOPtr p, list l, IndexedIO::MissingBehavior missingBehavior = IndexedIO::ThrowIfMissing )
	{
		IndexedIO::EntryIDList path;
		IndexedIOHelper::listToEntryIds( l, path );
		return p->directory(path, missingBehavior);
	}

	static list entryIds(IndexedIOPtr p)
	{
		assert(p);
		IndexedIO::EntryIDList l;
		p->entryIds(l);
		return IndexedIOHelper::entryIDsToList( l );
	}

	static list typedEntryIds(IndexedIOPtr p, IndexedIO::EntryType type )
	{
		assert(p);
		IndexedIO::EntryIDList l;
		p->entryIds(l, type);
		return IndexedIOHelper::entryIDsToList( l );
	}
	
	static std::string currentEntryId( IndexedIOPtr p )
	{
		return p->currentEntryId().value();
	}
	
	static list path(IndexedIOPtr p)
	{
		assert(p);
		IndexedIO::EntryIDList l;
		p->path(l);
		return IndexedIOHelper::entryIDsToList( l );
	}

	template<typename T>
	static void writeVector(IndexedIOPtr p, const IndexedIO::EntryID &name,
		const typename TypedData < T >::Ptr &x)
	{
		assert(p);

		const typename T::value_type *data = &(x->readable())[0];
		p->write( name, data, (unsigned long)x->readable().size() );
	}

	template<typename T>
	static typename TypedData<T>::Ptr readSingle(IndexedIOPtr p, const IndexedIO::EntryID &name, const IndexedIO::Entry &entry)
	{
		T data;
		p->read(name, data);
		return new TypedData<T>( data );
	}

	template<typename T>
	static typename TypedData< std::vector<T> >::Ptr readArray(IndexedIOPtr p, const IndexedIO::EntryID &name, const IndexedIO::Entry &entry)
	{
		unsigned long count = entry.arrayLength();
		typename TypedData<std::vector<T> >::Ptr x = new TypedData<std::vector<T> > ();
		x->writable().resize( entry.arrayLength() );
		T *data = &(x->writable()[0]);
		p->read(name, data, count);

		return x;
	}

	static object read(IndexedIOPtr p, const IndexedIO::EntryID &name)
	{
		assert(p);

		IndexedIO::Entry entry = p->entry(name);

		switch( entry.dataType() )
		{
			case IndexedIO::Float:
				return object( readSingle<float>(p, name, entry) );
			case IndexedIO::Double:
				return object( readSingle<double>(p, name, entry) );
			case IndexedIO::Int:
				return object( readSingle<int>(p, name, entry) );
			case IndexedIO::Long:
				return object( readSingle<int>(p, name, entry) );
			case IndexedIO::String:
				return object( readSingle<std::string>(p, name, entry) );
			case IndexedIO::StringArray:
				return object( readArray<std::string>(p, name, entry) );
			case IndexedIO::FloatArray:
				return object( readArray<float>(p, name, entry) );
			case IndexedIO::DoubleArray:
				return object( readArray<double>(p, name, entry) );
			case IndexedIO::IntArray:
				return object( readArray<int>(p, name, entry) );
			case IndexedIO::LongArray:
				return object( readArray<int>(p, name, entry) );
			case IndexedIO::UInt:
				return object( readSingle<unsigned int>(p, name, entry) );
			case IndexedIO::UIntArray:
				return object( readArray<unsigned int>(p, name, entry) );
			case IndexedIO::Char:
				return object( readSingle<char>(p, name, entry) );
			case IndexedIO::CharArray:
				return object( readArray<char>(p, name, entry) );
			case IndexedIO::UChar:
				return object( readSingle<unsigned char>(p, name, entry) );
			case IndexedIO::UCharArray:
				return object( readArray<unsigned char>(p, name, entry) );
			case IndexedIO::Short:
				return object( readSingle<short>(p, name, entry) );
			case IndexedIO::ShortArray:
				return object( readArray<short>(p, name, entry) );
			case IndexedIO::UShort:
				return object( readSingle<unsigned short>(p, name, entry) );
			case IndexedIO::UShortArray:
				return object( readArray<unsigned short>(p, name, entry) );
			case IndexedIO::SymbolicLink:
				{
					IndexedIO::EntryIDList path;
					p->read(name, path);
					return IndexedIOHelper::entryIDsToList( path );
				}
			default:
				throw IOException(name);
		}
	}

	static void writeSymlink(IndexedIOPtr p, const IndexedIO::EntryID &name, list l)
	{
		IndexedIO::EntryIDList symLink;
		IndexedIOHelper::listToEntryIds( l, symLink );
		p->write( name, symLink );
	}

	static std::string readString(IndexedIOPtr p, const IndexedIO::EntryID &name)
	{
		assert(p);

		std::string x;
		p->read(name, x);
		return x;
	}

	static list supportedExtensions()
	{
		list result;
		std::vector<std::string> e;
		IndexedIO::supportedExtensions( e );
		for( unsigned int i=0; i<e.size(); i++ )
		{
			result.append( e[i] );
		}
		return result;
	}

};

void bindIndexedIO(const char *bindName)
{
	IndexedIOPtr (IndexedIO::*nonConstParentDirectory)() = &IndexedIO::parentDirectory;
	IndexedIOPtr (IndexedIO::*nonConstSubdirectory)(const IndexedIO::EntryID &, IndexedIO::MissingBehavior) = &IndexedIO::subdirectory;
	void (IndexedIO::*writeFloat)(const IndexedIO::EntryID &, const float &) = &IndexedIO::write;
	void (IndexedIO::*writeDouble)(const IndexedIO::EntryID &, const double &) = &IndexedIO::write;
	void (IndexedIO::*writeInt)(const IndexedIO::EntryID &, const int &) = &IndexedIO::write;
	void (IndexedIO::*writeString)(const IndexedIO::EntryID &, const std::string &) = &IndexedIO::write;

#if 0
	void (IndexedIO::*writeUInt)(const IndexedIO::EntryID &, const unsigned int &) = &IndexedIO::write;
	void (IndexedIO::*writeChar)(const IndexedIO::EntryID &, const char &) = &IndexedIO::write;
	void (IndexedIO::*writeUChar)(const IndexedIO::EntryID &, const unsigned char &) = &IndexedIO::write;
	void (IndexedIO::*writeShort)(const IndexedIO::EntryID &, const short &) = &IndexedIO::write;
	void (IndexedIO::*writeUShort)(const IndexedIO::EntryID &, const unsigned short &) = &IndexedIO::write;
#endif


	enum_< IndexedIO::OpenModeFlags> ("IndexedIOOpenMode")
		.value("Read", IndexedIO::Read)
		.value("Write", IndexedIO::Write)
		.value("Append", IndexedIO::Append)
		.value("Shared", IndexedIO::Shared)
		.value("Exclusive", IndexedIO::Exclusive)
		.export_values()
	;

	enum_< IndexedIO::EntryType > ("IndexedIOEntryType")
		.value("Directory", IndexedIO::Directory)
		.value("File", IndexedIO::File)
		.export_values()
	;

	enum_< IndexedIO::DataType > ("IndexedIODataType")
		.value("Float", IndexedIO::Float)
		.value("FloatArray", IndexedIO::FloatArray)
		.value("Double", IndexedIO::Double)
		.value("DoubleArray", IndexedIO::DoubleArray)
		.value("Int", IndexedIO::Int)
		.value("IntArray", IndexedIO::IntArray)
		.value("Long", IndexedIO::Long)
		.value("LongArray", IndexedIO::LongArray)
		.value("String", IndexedIO::String)
		.value("StringArray", IndexedIO::StringArray)
		.value("UInt", IndexedIO::UInt)
		.value("UIntArray", IndexedIO::UIntArray)
		.value("Char", IndexedIO::Char)
		.value("CharArray", IndexedIO::CharArray)
		.value("UChar", IndexedIO::UChar)
		.value("UCharArray", IndexedIO::UCharArray)
		.value("Half", IndexedIO::Half)
		.value("HalfArray", IndexedIO::HalfArray)
		.value("Short", IndexedIO::Long)
		.value("ShortArray", IndexedIO::ShortArray)
		.value("UShort", IndexedIO::Long)
		.value("UShortArray", IndexedIO::UShortArray)
		.value("Int64", IndexedIO::Int64)
		.value("Int64Array", IndexedIO::Int64Array)
		.value("UInt64", IndexedIO::UInt64)
		.value("UInt64Array", IndexedIO::UInt64Array)
		.value("SymbolicLink", IndexedIO::SymbolicLink)
		.export_values()
	;

	scope varScope = IECorePython::RefCountedClass<IndexedIO, RefCounted>( bindName )
		.def("openMode", &IndexedIO::openMode)
		.def("parentDirectory", nonConstParentDirectory)
		.def("directory",  &IndexedIOHelper::directory )
		.def("subdirectory",  &IndexedIOHelper::subdirectory )
		.def("subdirectory", nonConstSubdirectory )
		.def("path", &IndexedIOHelper::path)
		.def("remove", &IndexedIO::remove)
		.def("removeAll", &IndexedIO::removeAll)
		.def("currentEntryId", &IndexedIOHelper::currentEntryId)
		.def("entryIds", &IndexedIOHelper::entryIds)
		.def("entryIds", &IndexedIOHelper::typedEntryIds)
		.def("entry", &IndexedIO::entry )
		.def("write", &IndexedIOHelper::writeVector<std::vector<float> >)
		.def("write", &IndexedIOHelper::writeVector<std::vector<double> >)
		.def("write", &IndexedIOHelper::writeVector<std::vector<int> >)
		.def("write", &IndexedIOHelper::writeVector<std::vector<std::string> >)
		.def("write", writeFloat)
		.def("write", writeDouble)
		.def("write", writeInt)
		.def("write", writeString)
		.def("write", &IndexedIOHelper::writeSymlink)
#if 0
		// We dont really want to bind these because they don't represent natural Python datatypes
		.def("write", writeUInt)
		.def("write", writeChar)
		.def("write", writeUChar)
		.def("write", writeShort)
		.def("write", writeUShort)
#endif
		.def("read", &IndexedIOHelper::read)
		.def("create", &IndexedIOHelper::create )
		.def("create", &IndexedIOHelper::createAtRoot ).staticmethod("create")
		.def("supportedExtensions", &IndexedIOHelper::supportedExtensions ).staticmethod("supportedExtensions")

	;

	enum_< IndexedIO::MissingBehavior > ("MissingBehavior")
		.value("ThrowIfMissing", IndexedIO::ThrowIfMissing)
		.value("NullIfMissing", IndexedIO::NullIfMissing)
		.value("CreateIfMissing", IndexedIO::CreateIfMissing)
		.export_values()
	;
}

void bindFileIndexedIO(const char *bindName)
{
	IECorePython::RefCountedClass<FileIndexedIO, IndexedIO>( bindName )
		.def("__init__", make_constructor( &IndexedIOHelper::constructorAtRoot<FileIndexedIO, const std::string &> ) )
		.def("__init__", make_constructor( &IndexedIOHelper::constructor<FileIndexedIO, const std::string &> ) )
	;
}

CharVectorDataPtr memoryIndexedIOBufferWrapper( MemoryIndexedIOPtr io )
{
	assert( io );
	return io->buffer()->copy();
}

void bindMemoryIndexedIO(const char *bindName)
{
	IECorePython::RefCountedClass<MemoryIndexedIO, IndexedIO>( bindName )
		.def("__init__", make_constructor( &IndexedIOHelper::constructorAtRoot<MemoryIndexedIO, ConstCharVectorDataPtr> ) )
		.def("__init__", make_constructor( &IndexedIOHelper::constructor<MemoryIndexedIO, ConstCharVectorDataPtr> ) )
		.def( "buffer", memoryIndexedIOBufferWrapper )
	;
}

void bindIndexedIOEntry(const char *bindName)
{
	class_< IndexedIO::Entry>(bindName, no_init)
		.def("id", &IndexedIO::Entry::id, return_value_policy<copy_const_reference>())
		.def("entryType", &IndexedIO::Entry::entryType)
		.def("dataType", &IndexedIO::Entry::dataType)
		.def("arrayLength", &IndexedIO::Entry::arrayLength)
		;
}
