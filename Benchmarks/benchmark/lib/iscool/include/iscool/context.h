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
#ifndef ISCOOL_CONTEXT_H
#define ISCOOL_CONTEXT_H

#include "iscool/optional.h"
#include "iscool/optional.impl.tpp"

#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#define ic_detail_context_member_name( seq ) \
    BOOST_PP_CAT( _field, ic_detail_context_field_name( seq ) )

#define ic_detail_context_member_ro_declaration( seq )                \
    ic_detail_context_field_type( seq ) ic_detail_context_member_name( seq )

#define ic_detail_context_member_rw_declaration( seq )          \
    iscool::optional< ic_detail_context_field_type( seq ) >     \
    ic_detail_context_member_name( seq );

#define ic_detail_context_field_type( seq )        \
    BOOST_PP_SEQ_ELEM( 0, seq )

#define ic_detail_context_field_name( seq )        \
    BOOST_PP_SEQ_ELEM( 1, seq )

#define ic_detail_context_stringized_field_name( seq )          \
    BOOST_PP_STRINGIZE( ic_detail_context_field_name( seq ) )

#define ic_detail_context_get_method_name( seq )                \
    BOOST_PP_CAT( get_, ic_detail_context_field_name( seq ) )

#define ic_detail_context_set_method_name( seq )                \
    BOOST_PP_CAT( set_, ic_detail_context_field_name( seq ) )

#define ic_detail_context_is_method_name( seq )                 \
    BOOST_PP_CAT                                                \
    ( BOOST_PP_CAT( is_, ic_detail_context_field_name( seq ) ), \
      _set )

#define ic_detail_context_reset_method_name( seq )              \
    BOOST_PP_CAT( reset_, ic_detail_context_field_name( seq ) )

#define ic_detail_context_rw_methods( r, data, seq )       \
    ic_detail_context_get_rw_method( seq )                 \
    ic_detail_context_set_method( seq )                    \
    ic_detail_context_is_method( seq )                     \
    ic_detail_context_reset_method( seq )

#define ic_detail_context_set_method( seq )                             \
    void ic_detail_context_set_method_name( seq )                       \
        ( ic_detail_context_field_type( seq ) arg )                     \
    {                                                                   \
        ic_detail_context_member_name( seq ).emplace( arg );            \
    }

#define ic_detail_context_get_ro_method( r, data, seq )                 \
    ic_detail_context_field_type( seq )                                 \
    ic_detail_context_get_method_name( seq )() const                    \
    {                                                                   \
        return ic_detail_context_member_name( seq );                    \
    }

#define ic_detail_context_get_rw_method( seq )                  \
    ic_detail_context_field_type( seq )                         \
    ic_detail_context_get_method_name( seq )() const           \
    {                                                           \
        assert( ic_detail_context_member_name( seq ) );         \
        return *( ic_detail_context_member_name( seq ) );       \
    }
  
#define ic_detail_context_is_method( seq )                      \
    bool ic_detail_context_is_method_name( seq )() const        \
    {                                                           \
        return !!ic_detail_context_member_name( seq );          \
    }

#define ic_detail_context_reset_method( seq )           \
    void ic_detail_context_reset_method_name( seq ) ()  \
    {                                                   \
        ic_detail_context_member_name( seq ).reset();   \
    }

#define ic_detail_context_ro_method_for_each( fields )                  \
    BOOST_PP_SEQ_FOR_EACH( ic_detail_context_get_ro_method, _, fields )

#define ic_detail_context_rw_method_for_each( fields )                  \
    BOOST_PP_SEQ_FOR_EACH( ic_detail_context_rw_methods, _, fields )

#define ic_detail_context_copy_field( r, data, i, seq )          \
    BOOST_PP_COMMA_IF( i ) ic_detail_context_member_name( seq )  \
    ( data.ic_detail_context_get_method_name( seq )() )

#define ic_detail_context_copy_fields_from_other( fields, context )     \
    BOOST_PP_SEQ_FOR_EACH_I( ic_detail_context_copy_field, context, fields )

#define ic_detail_context_empty( ... )

#define ic_detail_context_function_if_list_not_empty( list, function )  \
    BOOST_PP_IF( BOOST_PP_SEQ_SIZE( list ), function, ic_detail_context_empty )

#define ic_detail_context_value_if_list_not_empty( list, value )        \
    BOOST_PP_IF( BOOST_PP_SEQ_SIZE( list ), value, ic_detail_context_empty() )

#define ic_detail_context_list_arguments( r, data, i, seq )    \
    BOOST_PP_COMMA_IF( i ) ic_detail_context_field_type( seq ) \
    BOOST_PP_CAT( arg, i )

#define ic_detail_context_set_field_from_argument( r, data, i, seq ) \
    result.ic_detail_context_set_method_name( seq )( BOOST_PP_CAT( arg, i ) );

#define ic_detail_context_set_fields_from_arguments( fields )   \
    BOOST_PP_SEQ_FOR_EACH_I                                             \
    ( ic_detail_context_set_field_from_argument, _, fields )

#define ic_detail_context_list_new_fields_arguments_with_comma( fields ) \
    BOOST_PP_SEQ_FOR_EACH_I( ic_detail_context_list_arguments, _, fields )

#define ic_detail_context_template_method_for_all( fields )                    \
    context copy_assign_properties                                             \
    ( ic_detail_context_function_if_list_not_empty                             \
      ( fields, ic_detail_context_list_new_fields_arguments_with_comma )       \
      ( fields ) ) const                                                       \
    {                                                                          \
        context result( *this );                                               \
        ic_detail_context_set_fields_from_arguments( fields )                  \
        return result;                                                         \
    }

#define ic_detail_context_set_all_properties_method( fields ) \
    ic_detail_context_function_if_list_not_empty        \
    ( fields, ic_detail_context_template_method_for_all ) ( fields )

#define ic_detail_context_default_constructor( fields ) \
    context() = default;

#define ic_detail_context_constructor( parent_fields )                  \
    BOOST_PP_IF                                                         \
    ( BOOST_PP_SEQ_SIZE( parent_fields ),                               \
      ic_detail_context_template_constructor,                           \
      ic_detail_context_default_constructor ) ( parent_fields )

#define ic_detail_context_template_constructor( parent_fields ) \
    template< typename OtherType >                              \
    context( const OtherType& other_context )                   \
        : ic_detail_context_copy_fields_from_other              \
          ( parent_fields, other_context )                      \
    {                                                           \
    }

#define ic_detail_context_ro_member( r, data, seq )        \
    ic_detail_context_member_ro_declaration( seq );  

#define ic_detail_context_ro_member_for_each( fields )                  \
    BOOST_PP_SEQ_FOR_EACH( ic_detail_context_ro_member, _, fields )

#define ic_detail_context_rw_member( r, data, seq )     \
    ic_detail_context_member_rw_declaration( seq );  

#define ic_detail_context_rw_member_for_each( fields )                  \
    BOOST_PP_SEQ_FOR_EACH( ic_detail_context_rw_member, _, fields )


#define ic_declare_context_type( parent_fields, new_fields )                \
    public:                                                                 \
        class context                                                       \
        {                                                                   \
        public:                                                             \
            ic_detail_context_constructor( parent_fields )                  \
                                                                            \
        public:                                                             \
            ic_detail_context_ro_method_for_each( parent_fields )           \
            ic_detail_context_rw_method_for_each( new_fields )              \
            ic_detail_context_set_all_properties_method( new_fields )       \
                                                                            \
        private:                                                            \
            ic_detail_context_ro_member_for_each( parent_fields )           \
            ic_detail_context_rw_member_for_each( new_fields )              \
    };                                                         


#define ic_declare_context( field_name, parent_fields, new_fields )     \
    ic_declare_context_type( parent_fields, new_fields )                \
    private:                                                            \
        context field_name

#define ic_context_declare_parent_properties( properties ) properties
#define ic_context_no_parent_properties

#define ic_context_declare_properties( properties ) properties
#define ic_context_no_properties


#endif
