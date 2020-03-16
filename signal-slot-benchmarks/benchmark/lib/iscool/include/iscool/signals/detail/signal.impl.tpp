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
#ifndef ISCOOL_SIGNALS_DETAIL_SIGNAL_IMPL_TPP
#define ISCOOL_SIGNALS_DETAIL_SIGNAL_IMPL_TPP

#include <cassert>

template< typename Signature >
iscool::signals::detail::signal< Signature >::signal()
    : _storage_kind( storage_kind::none )
{

}

template< typename Signature >
iscool::signals::detail::signal< Signature >::~signal()
{
    switch( _storage_kind )
    {
    case storage_kind::pointer:
        get_storage_as_pointer().~shared_slot_ptr();
        break;
    case storage_kind::vector:
        get_storage_as_vector().~shared_slot_ptr_vector();
        break;
    case storage_kind::none:
        break;
    }
}

template< typename Signature >
iscool::signals::connection
iscool::signals::detail::signal< Signature >::connect
( boost::function< Signature > f )
{
    if ( _storage_kind == storage_kind::none )
    {
        _storage_kind = storage_kind::pointer;
        new ( &_slot_storage )
            shared_slot_ptr
            ( std::make_shared< internal_slot >( std::move( f ) ) );
        return connection( get_storage_as_pointer() );
    }

    if ( _storage_kind == storage_kind::pointer )
    {
        shared_slot_ptr& pointer( get_storage_as_pointer() );

        if ( pointer == nullptr )
        {
            pointer = std::make_shared< internal_slot >( std::move( f ) );
            return connection( pointer );
        }

        shared_slot_ptr old_pointer( std::move( pointer ) );
        pointer.~shared_slot_ptr();

        _storage_kind = storage_kind::vector;
        new ( &_slot_storage ) shared_slot_ptr_vector( 2 );

        shared_slot_ptr_vector& vector( get_storage_as_vector() );
        vector[ 0 ] = std::move( old_pointer );
        vector[ 1 ] = std::make_shared< internal_slot >( std::move( f ) );

        return connection( vector[ 1 ] );
    }

    assert( _storage_kind == storage_kind::vector );
    shared_slot_ptr_vector& vector( get_storage_as_vector() );
    vector.emplace_back( std::make_shared< internal_slot >( std::move( f ) ) );

    return connection( vector.back() );
}

template< typename Signature >
template< typename... Arg >
void
iscool::signals::detail::signal< Signature >::operator()( Arg&&... arg ) const
{
    if ( _storage_kind == storage_kind::pointer )
    {
        const shared_slot_ptr& slot( get_storage_as_pointer() );
        
        if ( ( slot != nullptr ) && slot->connected() )
            slot->callback( std::forward< Arg >( arg )... );
    }
    else if ( _storage_kind == storage_kind::vector )
    {
        const shared_slot_ptr_vector slots( get_storage_as_vector() );

        for( const auto& s : slots )
            if ( s->connected() )
                s->callback( std::forward< Arg >( arg )... );
    }
}

template< typename Signature >
bool iscool::signals::detail::signal< Signature >::empty() const
{
    if ( _storage_kind == storage_kind::none )
        return true;
    
    if ( _storage_kind == storage_kind::pointer )
    {
        const shared_slot_ptr& slot( get_storage_as_pointer() );
        return ( slot == nullptr ) || !slot->connected();
    }
    
    for ( const auto& s : get_storage_as_vector() )
        if ( s->connected() )
            return false;

    return true;
}

template< typename Signature >
void iscool::signals::detail::signal< Signature >::disconnect_all_slots()
{
    switch( _storage_kind )
    {
    case storage_kind::pointer:
        get_storage_as_pointer().reset();
        break;
    case storage_kind::vector:
        get_storage_as_vector().clear();
        break;
    case storage_kind::none:
        break;
    }
}

template< typename Signature >
std::size_t iscool::signals::detail::signal< Signature >::num_slots() const
{
    if ( _storage_kind == storage_kind::none )
        return 0;
    
    if ( _storage_kind == storage_kind::pointer )
        return get_storage_as_pointer() != nullptr;
    
    std::size_t result( 0 );

    for ( const auto& s : get_storage_as_vector() )
        if ( s->connected() )
            ++result;

    return result;
}

template< typename Signature >
void iscool::signals::detail::signal< Signature >::swap
( signal< Signature >& that ) noexcept
{
    if ( _storage_kind == storage_kind::none )
    {
        if ( that._storage_kind == storage_kind::pointer )
            that.swap_pointer_none( *this );
        else if ( that._storage_kind == storage_kind::vector )
            that.swap_vector_none( *this );
    }
    else if ( _storage_kind == storage_kind::pointer )
    {
        if ( that._storage_kind == storage_kind::none )
            swap_pointer_none( that );
        else if ( that._storage_kind == storage_kind::vector )
            swap_pointer_vector( that );
        else
            get_storage_as_pointer().swap( that.get_storage_as_pointer() );
    }
    else
    {
        if ( that._storage_kind == storage_kind::pointer )
            that.swap_pointer_vector( *this );
        else if ( that._storage_kind == storage_kind::none )
            swap_vector_none( *this );
        else
            get_storage_as_vector().swap( that.get_storage_as_vector() );
    }
}
    
template< typename Signature >
typename iscool::signals::detail::signal< Signature >::shared_slot_ptr&
iscool::signals::detail::signal< Signature >::get_storage_as_pointer()
{
    assert( _storage_kind == storage_kind::pointer );
    return *reinterpret_cast< shared_slot_ptr* >( &_slot_storage );
}

template< typename Signature >
const typename iscool::signals::detail::signal< Signature >::shared_slot_ptr&
iscool::signals::detail::signal< Signature >::get_storage_as_pointer() const
{
    assert( _storage_kind == storage_kind::pointer );
    return *reinterpret_cast< const shared_slot_ptr* >( &_slot_storage );
}

template< typename Signature >
typename iscool::signals::detail::signal< Signature >::shared_slot_ptr_vector&
iscool::signals::detail::signal< Signature >::get_storage_as_vector()
{
    assert( _storage_kind == storage_kind::vector );
    return *reinterpret_cast< shared_slot_ptr_vector* >( &_slot_storage );
}

template< typename Signature >
const typename
iscool::signals::detail::signal< Signature >::shared_slot_ptr_vector&
iscool::signals::detail::signal< Signature >::get_storage_as_vector() const
{
    assert( _storage_kind == storage_kind::vector );
    return *reinterpret_cast< const shared_slot_ptr_vector* >( &_slot_storage );
}
template< typename Signature >
void iscool::signals::detail::signal< Signature >::swap_pointer_vector
( signal< Signature >& that ) noexcept
{
    assert( _storage_kind == storage_kind::pointer );
    assert( that._storage_kind == storage_kind::vector );

    shared_slot_ptr& pointer( get_storage_as_pointer() );
    shared_slot_ptr old_pointer( std::move( pointer ) );
    pointer.~shared_slot_ptr();

    shared_slot_ptr_vector& vector( that.get_storage_as_vector() );
    new ( &_slot_storage ) shared_slot_ptr_vector( std::move( vector ) );
    vector.~shared_slot_ptr_vector();
            
    new ( &that._slot_storage ) shared_slot_ptr( std::move( old_pointer ) );

    _storage_kind = storage_kind::vector;
    that._storage_kind = storage_kind::pointer;
}

template< typename Signature >
void iscool::signals::detail::signal< Signature >::swap_pointer_none
( signal< Signature >& that ) noexcept
{
    assert( _storage_kind == storage_kind::pointer );
    assert( that._storage_kind == storage_kind::none );

    shared_slot_ptr& pointer( get_storage_as_pointer() );
    new ( &that._slot_storage ) shared_slot_ptr( std::move( pointer ) );
    pointer.~shared_slot_ptr();

    _storage_kind = storage_kind::none;
    that._storage_kind = storage_kind::pointer;
}

template< typename Signature >
void iscool::signals::detail::signal< Signature >::swap_vector_none
( signal< Signature >& that ) noexcept
{
    assert( _storage_kind == storage_kind::vector );
    assert( that._storage_kind == storage_kind::none );

    shared_slot_ptr_vector& vector( get_storage_as_vector() );
    new ( &that._slot_storage ) shared_slot_ptr_vector( std::move( vector ) );
    vector.~shared_slot_ptr_vector();

    _storage_kind = storage_kind::none;
    that._storage_kind = storage_kind::vector;
}

template< typename Signature >
iscool::signals::detail::signal< Signature >::internal_slot::internal_slot
( boost::function< Signature > f )
    : callback( std::move( f ) )
{

}

template< typename Signature >
iscool::signals::detail::signal< Signature >::internal_slot::~internal_slot() =
  default;

#endif
