/*
  Copyright 2018-present IsCool Entertainment

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/
#ifndef ISCOOL_OPTIONAL_IMPL_TPP
#define ISCOOL_OPTIONAL_IMPL_TPP

#include <cassert>
#include <new>
#include <utility>

template< typename T >
iscool::optional< T >::optional()
    : _initialized( false )
{

}
  
template< typename T >
iscool::optional< T >::optional( T&& that )
    : _initialized( true )
{
    new( &_storage ) T( std::move( that ) );
}

template< typename T >
iscool::optional< T >::optional( const T& that )
    : _initialized( true )
{
    new( &_storage ) T( that );
}

template< typename T >
iscool::optional< T >::optional( const optional< T >& that )
    : _initialized( that._initialized )
{
    if ( _initialized )
        new ( &_storage ) T( *that );
}

template< typename T >
template< typename U >
iscool::optional< T >::optional( const optional< U >& that )
    : _initialized( that )
{
    if ( _initialized )
        new ( &_storage ) T( *that );
}

template< typename T >
iscool::optional< T >::optional( optional< T >&& that )
    : _initialized( that._initialized )
{
    if ( _initialized )
        new ( &_storage ) T( std::move( *that ) );
}

template< typename T >
iscool::optional< T >::optional( const none_t& )
    : optional()
{

}

template< typename T >
iscool::optional< T >::~optional()
{
    reset();
}
  
template< typename T >
iscool::optional< T >::operator bool() const
{
    return _initialized;
}

template< typename T >
T& iscool::optional< T >::operator*()
{
    return *get();
}
  
template< typename T >
const T& iscool::optional< T >::operator*() const
{
    return *get();
}

template< typename T >
T* iscool::optional< T >::operator->()
{
    return get();
}

template< typename T >
const T* iscool::optional< T >::operator->() const
{
    return get();
}

template< typename T >
iscool::optional< T >& iscool::optional< T >::operator=( const T& that )
{
    if ( _initialized && ( get() == &that ) )
        return *this;

    emplace( that );

    return *this;
}
  
template< typename T >
iscool::optional< T >&
iscool::optional< T >::operator=( const optional< T >& that )
{
    if ( this == &that )
        return *this;

    if ( that )
        emplace( *that );
    else
        reset();

    return *this;
}

template< typename T >
iscool::optional< T >& iscool::optional< T >::operator=( optional< T >&& that )
{
    if ( this == &that )
        return *this;

    if ( that )
        emplace( std::move( *that ) );
    else
        reset();

    return *this;
}

template< typename T >
iscool::optional< T >& iscool::optional< T >::operator=( const none_t& )
{
    reset();
    return *this;
}

template< typename T >
bool iscool::optional< T >::operator==( const none_t& ) const
{
    return !_initialized;
}

template< typename T >
bool iscool::optional< T >::operator!=( const none_t& ) const
{
    return _initialized;
}

template< typename T >
template< typename... Args >
void iscool::optional< T >::emplace( Args&&... args )
{
    reset();

    new ( &_storage ) T( std::forward< Args >( args )... );
    _initialized = true;
}
   
template< typename T >
void iscool::optional< T >::reset()
{
    if ( _initialized )
    {
        get()->~T();
        _initialized = false;
    }
}

template< typename T >
void iscool::optional< T >::swap( optional< T >& that )
{
    std::swap( *this, that );
}

template< typename T >
T* iscool::optional< T >::get()
{
    assert( _initialized );
    return reinterpret_cast< T* >( &_storage );
}

template< typename T >
const T* iscool::optional< T >::get() const
{
    assert( _initialized );
    return reinterpret_cast< const T* >( &_storage );
}

template< typename T >
iscool::optional< T& >::optional()
    : _value( nullptr )
{

}
  
template< typename T >
iscool::optional< T& >::optional( T& that )
    : _value( &that )
{

}

template< typename T >
iscool::optional< T& >::optional( const optional< T& >& that )
    : _value( that._value )
{

}

template< typename T >
template< typename U >
iscool::optional< T& >::optional( const optional< U& >& that )
    : _value( that._value )
{

}

template< typename T >
iscool::optional< T& >::optional( optional< T& >&& that )
    : _value( that._value )
{

}

template< typename T >
iscool::optional< T& >::optional( const none_t& )
    : optional()
{

}

template< typename T >
iscool::optional< T& >::~optional()
{

}
  
template< typename T >
iscool::optional< T& >::operator bool() const
{
    return _value != nullptr;
}

template< typename T >
T& iscool::optional< T& >::operator*()
{
    return *operator->();
}
  
template< typename T >
const T& iscool::optional< T& >::operator*() const
{
    return *operator->();
}

template< typename T >
T* iscool::optional< T& >::operator->()
{
    assert( _value != nullptr );
    return _value;
}

template< typename T >
const T* iscool::optional< T& >::operator->() const
{
    assert( _value != nullptr );
    return _value;
}

template< typename T >
iscool::optional< T& >& iscool::optional< T& >::operator=( T& that )
{
    emplace( that );
    return *this;
}
  
template< typename T >
iscool::optional< T& >&
iscool::optional< T& >::operator=( const optional< T& >& that )
{
    _value = that._value;
    return *this;
}

template< typename T >
iscool::optional< T& >&
iscool::optional< T& >::operator=( optional< T& >&& that )
{
    _value = that._value;
    return *this;
}

template< typename T >
iscool::optional< T& >& iscool::optional< T& >::operator=( const none_t& )
{
    reset();
    return *this;
}

template< typename T >
bool iscool::optional< T& >::operator==( const none_t& ) const
{
    return _value == nullptr;
}

template< typename T >
bool iscool::optional< T& >::operator!=( const none_t& ) const
{
    return _value != nullptr;
}

template< typename T >
void iscool::optional< T& >::emplace( T& that )
{
    _value = &that;
}

template< typename T >
void iscool::optional< T& >::reset()
{
    _value = nullptr;
}

template< typename T >
void iscool::optional< T& >::swap( optional< T& >& that )
{
    std::swap( _value, that._value );
}

template< typename T >
bool iscool::operator==( const none_t&, const optional< T >& value )
{
    return !value;
}

template< typename T >
bool iscool::operator!=( const none_t&, const optional< T >& value )
{
    return !!value;
}

template< typename T >
iscool::optional< T > iscool::make_optional( T value )
{
    return iscool::optional< T >( value );
}

#endif
