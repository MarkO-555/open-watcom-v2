//
//  wclist.h    Defines the WATCOM Container List Classes
//
// =========================================================================
//
//                          Open Watcom Project
//
//    Copyright (c) 2002-2010 Open Watcom Contributors. All Rights Reserved.
//    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
//
//    This file is automatically generated. Do not edit directly.
//
// =========================================================================
//
#ifndef _WCLIST_H_INCLUDED
#define _WCLIST_H_INCLUDED

#ifndef _ENABLE_AUTODEPEND
 #pragma read_only_file;
#endif

#ifndef __cplusplus
 #error This header file requires C++
#endif

#pragma warning 621 9       // I want the behaviour this warning points to

#ifndef _WCDEFS_H_INCLUDED
 #include <wcdefs.h>
#endif

#ifndef _WCEXCEPT_H_INCLUDED
 #include <wcexcept.h>
#endif

#ifndef _WCLCOM_H_INCLUDED
 #include <wclcom.h>
#endif

#ifndef _WCLBASE_H_INCLUDED
 #include <wclbase.h>
#endif

//
//  This defines a single linked list of values of type <Type>
//
//  The base class is invoked to process the copy and assignment
//  constructors.  The copies are shallow, in that only the values
//  contained in the list are copied.  If the contained values
//  themselves have defined copy constructors, they can effect a
//  deeper copy.
//

template<class Type>
class WCValSList : public WCValListBase< Type,
                   WCIsvSListBase<WCNIsvSLink<Type> >, WCNIsvSLink<Type> > {
private:
    // This virtual function defines equivalence for the base classes.
    virtual int base_equivalent( const Type &elem1, const Type &elem2 ) const {
        return( elem1 == elem2 );
    };

public:
    inline WCValSList( void * (*user_alloc)( size_t )
                , void (*user_dealloc)( void *, size_t )
                ) : WCValListBase( user_alloc, user_dealloc ) {};

    inline WCValSList() : WCValListBase( 0, 0 ) {};

    inline ~WCValSList() {};

    inline WCValSList( const WCValSList & olist ) {
        base_construct( &olist );
    };

    inline WCValSList & operator=( const WCValSList & olist ) {
        base_assign( &olist );
        return( *this );
    };
};




//
//  This defines a double linked list of values of type <Type>
//
//  The base class is invoked to process the copy and assignment
//  constructors.  The copies are shallow, in that only the values
//  contained in the list are copied.  If the contained values
//  themselves have defined copy constructors, they can effect a
//  deeper copy.
//

template<class Type>
class WCValDList : public WCValListBase<Type,
                   WCIsvDListBase<WCNIsvDLink<Type> >, WCNIsvDLink<Type> > {
private:
    // This virtual function defines equivalence for the base classes.
    virtual int base_equivalent( const Type &elem1, const Type &elem2 ) const {
        return( elem1 == elem2 );
    };

public:
    inline WCValDList( void * (*user_alloc)( size_t )
                , void (*user_dealloc)( void *, size_t )
                ) : WCValListBase( user_alloc, user_dealloc ) {};

    inline WCValDList() : WCValListBase( 0, 0 ) {};

    inline ~WCValDList() {};

    inline WCValDList( const WCValDList & olist ) {
        base_construct( &olist );
    };

    inline WCValDList & operator=( const WCValDList & olist ) {
        base_assign( &olist );
        return( *this );
    };
};




//
//  This defines a single linked list of pointers of type <Type>
//
//  The base class is invoked to process the copy and assignment
//  constructors.  The copies are shallow, in that only the pointers
//  contained in the list are copied.
//  deeper copy.
//

template<class Type>
class WCPtrSList : public WCPtrListBase<Type, WCValSList<Type *> > {
private:
    // This virtual function defines equivalence for the base classes.
    virtual int base_equivalent( Type * const &elem1
                               , Type * const &elem2 ) const {
        return( *(const Type *)elem1 == *(const Type *)elem2 );
    };

public:
    inline WCPtrSList( void * (*user_alloc)( size_t )
                , void (*user_dealloc)( void *, size_t )
                ) : WCPtrListBase( user_alloc, user_dealloc ) {};

    inline WCPtrSList() : WCPtrListBase( 0, 0 ) {};

    inline ~WCPtrSList() {};

    inline WCPtrSList( const WCPtrSList & olist ) {
        base_construct( &olist );
    };

    inline int index( const Type * ptr ) const {
        return( WCPtrListBase::index( (Type * const)ptr ) );
    };

    inline WCPtrSList & operator=( const WCPtrSList & olist ) {
        base_assign( &olist );
        return( *this );
    };
};




//
//  This defines a double linked list of pointers of type <Type>
//
//  The base class is invoked to process the copy and assignment
//  constructors.  The copies are shallow, in that only the pointers
//  contained in the list are copied.
//

template<class Type>
class WCPtrDList : public WCPtrListBase<Type, WCValDList<Type *> > {
private:
    // This virtual function defines equivalence for the base classes.
    virtual int base_equivalent( Type * const &elem1
                               , Type * const &elem2 ) const {
        return( *(const Type *)elem1 == *(const Type *)elem2 );
    };

public:
    inline WCPtrDList( void * (*user_alloc)( size_t )
                , void (*user_dealloc)( void *, size_t )
                ) : WCPtrListBase( user_alloc, user_dealloc ) {};

    inline WCPtrDList() : WCPtrListBase( 0, 0 ) {};

    inline ~WCPtrDList() {};

    inline WCPtrDList( const WCPtrDList & olist ) {
        base_construct( &olist );
    };

    inline int index( const Type * ptr ) const {
        return( WCPtrListBase::index( (Type * const)ptr ) );
    };

    inline WCPtrDList & operator=( const WCPtrDList & olist ) {
        base_assign( &olist );
        return( *this );
    };
};




//
//  This defines a single linked list of values of type <Type>,
//  stored intrusively.
//
//  The copy and assignment constructors are made private so
//  that a second copy of the list is not made (intrusive lists
//  require modification of the base links, making second copies
//  a problem).
//

template<class Type>
class WCIsvSList : public WCPtrListBase<Type, WCIsvSListBase<Type> > {
private:
    WCIsvSList( const WCIsvSList & );
    WCIsvSList & operator=( const WCIsvSList & );

protected:
    //
    // This constuctor is necessary for inherited classes such as WCStack and
    // WCQueue to be able to support user allocators and deallocators.
    // It is not intended for users of the class.
    //
    inline WCIsvSList( void * (*)( size_t ), void (*)( void *, size_t ) ) {};

public:
    inline WCIsvSList() {};
    inline ~WCIsvSList() {};

    inline void forAll( void (*fn)( Type *, void * ), void *data ) {
        WCPtrListBase::forAll( fn, data );
    };

    inline int index( int (*test_fn)( const Type *, void * )
                    , void *data ) const {
        return( WCPtrListBase::index( test_fn, data ) );
    };

    inline int index( const Type *elem ) const {
        return( WCPtrListBase::index( elem ) );
    };
};




//
//  This defines a double linked list of values of type <Type>,
//  stored intrusively.
//
//  The copy and assignment constructors are made private so
//  that a second copy of the list is not made (intrusive lists
//  require modification of the base links, making second copies
//  a problem).
//

template<class Type>
class WCIsvDList : public WCPtrListBase<Type, WCIsvDListBase<Type> > {
private:
    WCIsvDList( const WCIsvDList & );
    WCIsvDList & operator=( const WCIsvDList & );

protected:
    //
    // This constuctor is necessary for inherited classes such as WCStack and
    // WCQueue to be able to support user allocators and deallocators.
    // It is not intended for users of the class.
    //
    inline WCIsvDList( void * (*)( size_t ), void (*)( void *, size_t ) ) {};

public:
    inline WCIsvDList() {};
    inline ~WCIsvDList() {};

    inline void forAll( void (*fn)( Type *, void * ), void *data ) {
        WCPtrListBase::forAll( fn, data );
    };

    inline int index( int (*test_fn)( const Type *, void * )
                    , void *data ) const {
        return( WCPtrListBase::index( test_fn, data ) );
    };

    inline int index( const Type *elem ) const {
        return( WCPtrListBase::index( elem ) );
    };
};

#endif
