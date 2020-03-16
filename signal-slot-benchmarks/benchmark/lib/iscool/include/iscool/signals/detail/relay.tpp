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
#ifndef ISCOOL_SIGNALS_RELAY_TPP
#define ISCOOL_SIGNALS_RELAY_TPP

#include <boost/bind.hpp>
#include <boost/preprocessor/iteration/local.hpp>

#include <cassert>

namespace iscool
{
    namespace signals
    {
        namespace detail
        {
            template< typename Signature >
            struct signal_binder;

            template< typename R, typename... T >
            struct signal_binder< R( T... ) >
            {
                typedef iscool::signals::signal< R( T... ) > signal_type;
                typedef
                typename signal_type::slot_function_type slot_function_type;
                
                static slot_function_type bind( const signal_type& signal );
            };
    
            template< std::size_t N, typename R, typename... T >
            struct signal_binder_n;

            template< typename Signature > 
            struct signal_invoker;

            template< typename R, typename... T >
            struct signal_invoker< R ( T... ) >
            {
                typedef iscool::signals::signal< R( T... ) > signal_type;
                static R invoke( const signal_type* signal, T... arguments );
            };

            template< typename ...T >
            struct signal_invoker< void( T... ) >
            {
                typedef iscool::signals::signal<void( T... )> signal_type;
                static void invoke( const signal_type* signal, T... arguments );
            };
        }
    }
}

template< typename Signature >
typename iscool::signals::signal< Signature >::slot_function_type
iscool::signals::relay( const signal< Signature >& s )
{
    return detail::signal_binder<Signature>::bind( s );
}

template< typename R, typename ...T >
typename
iscool::signals::detail::signal_binder< R( T... ) >::slot_function_type 
iscool::signals::detail::signal_binder<R( T... )>::bind
( const signal_type& signal )
{
    constexpr std::size_t size = sizeof...(T);
    return signal_binder_n< size, R, T... >::bind( signal );
}

#define SIGNAL_BINDER_PLACEHOLDER( UNUSED1, N, DATA )      \
    BOOST_PP_CAT( DATA, BOOST_PP_INC( N ) )     

#define SIGNAL_BINDER_BINDN( N ) \
    template< typename R, typename ...T > \
    struct signal_binder_n< N, R, T... > \
    { \
        typedef iscool::signals::signal<R( T... )> signal_type; \
        typedef typename signal_type::slot_function_type slot_function_type; \
        static slot_function_type bind( const signal_type& signal ) \
        { \
            return boost::bind<R>                                    \
                ( &detail::signal_invoker<R( T... )>::invoke, &signal \
                  BOOST_PP_COMMA_IF( N ) \
                  BOOST_PP_ENUM( N, SIGNAL_BINDER_PLACEHOLDER, _ ) \
            ); \
        } \
    }; 

#define BOOST_PP_LOCAL_MACRO( N )  SIGNAL_BINDER_BINDN( N )
#define BOOST_PP_LOCAL_LIMITS     (0, 8)

namespace iscool
{
    namespace signals
    {
        namespace detail
        {
#include BOOST_PP_LOCAL_ITERATE()
        }
    }
}

#undef SIGNAL_BINDER_PLACEHOLDER
#undef SIGNAL_BINDER_BINDN
#undef BOOST_PP_LOCAL_MACRO
#undef BOOST_PP_LOCAL_LIMITS

template< typename R, typename...T >
R iscool::signals::detail::signal_invoker< R( T... ) >::invoke
( const signal_type* signal, T... arguments )
{
    assert( signal );
    return *( signal->operator() ( arguments... ) );
};

template< typename ...T >
void iscool::signals::detail::signal_invoker< void( T... ) >::invoke
( const signal_type* signal, T... arguments )
{
    assert( signal );
    signal->operator() ( arguments... );
};

#endif
