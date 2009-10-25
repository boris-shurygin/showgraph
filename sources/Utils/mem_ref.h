/**
 * @file: mem_ref.h 
 * Implementation of memory reference template
 */
#pragma once

#ifndef MEM_H
#	error
#endif

#ifndef MEM_REF_H
#define MEM_REF_H

namespace Mem
{
   /**
     * Memory reference template
     * @ingroup Mem
     */
    template < class RefObj> class Ref
    {
#ifdef USE_REF_COUNTERS

#endif
	private:
		RefObj *ptr;
	public:
		/** Default constructor */
		Ref(): ptr( NULL){};
		/** Copy constructor !!! NO uTEST !!! */
		Ref( const Ref< RefObj>& orig): ptr( orig.ptr)
		{
#ifdef USE_REF_COUNTERS
			ptr->incRefCount();
#endif
		}
		/** Assignement operator overloading */
		Ref< RefObj> & operator=( const Ref< RefObj>& orig)
		{
			ptr = orig.ptr;
#ifdef USE_REF_COUNTERS
			ptr->incRefCount();
#endif
			return *this;
		}
	    /**
		 * Constructor from pointer
		 * Used in Ref ref = new Obj(...) initialization
		 * !!! No uTEST !!!
		 */
		Ref( RefObj* p): ptr( p)
		{
#ifdef USE_REF_COUNTERS
			if ( ptr != 0)
				ptr->incRefCount();
#endif	
		}
		/**
		 * Assignement of pointer
		 * Used in Ref ref; ref = new Obj(...) expression
		 */
		Ref< RefObj> & operator=( const RefObj* p)
		{
			ptr = p;
#ifdef USE_REF_COUNTERS
			if ( ptr != 0)
				ptr->incRefCount();
#endif	
		}
		~Ref()
		{
#ifdef USE_REF_COUNTERS
			ptr->decRefCount();
#endif
		}
		inline RefObj* operator->()
		{
			return ptr;
		}
		inline bool operator == ( Ref< RefObj> &r)
		{
			return ptr == r.ptr;
		}
		inline operator bool()
		{
			return ptr != NULL;
		}
    };
};
#endif /* MEM_REF_H */