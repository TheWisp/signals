///
/// Wink Signals
/// Copyright (C) 2013-2014 Miguel Martin (miguel@miguel-martin.com)
///
///
/// This software is provided 'as-is', without any express or implied warranty.
/// In no event will the authors be held liable for any damages arising from the
/// use of this software.
///
/// Permission is hereby granted, free of charge, to any person
/// obtaining a copy of this software and associated documentation files (the "Software"),
/// to deal in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// 1. The origin of this software must not be misrepresented;
///    you must not claim that you wrote the original software.
///    If you use this software in a product, an acknowledgment
///    in the product documentation would be appreciated but is not required.
///
/// 2. Altered source versions must be plainly marked as such,
///	   and must not be misrepresented as being the original software.
///
/// 3. The above copyright notice and this permission notice shall be included in
///    all copies or substantial portions of the Software.
///

#ifndef WINK_SLOT_HPP
#define WINK_SLOT_HPP

#include <utility>

//#include <wink/detail/FastDelegate.h>
#include "detail/FastDelegate.h"

namespace wink
{
	/// \brief Describes a slot that may be added to a signal, or used stand-alone for a call-back
	///
	/// This can be used as an alternative to std::function, as it is much faster.
	///
	/// \author Miguel Martin
	template <typename Signature>
	struct slot
	{
	private:
		
		typedef slot<Signature> this_type;
		
	public:
		
		/// A static function pointer with the correct signature
		typedef Signature FnPtr;

		
		/// Construct a slot with no call-back
		slot()
		{}
		
		/// Construct a slot with a static/global function call-back
		/// \param fn The static/global function
		slot(FnPtr fn)
            : _delegate(fn)
		{}
		
		/// Construct a slot with a member-function
		/// \param obj The object that the member-function belongs to
		/// \param fn The member function of the object
		template <typename T, typename MemFnPtr>
		slot(T* obj, MemFnPtr fn)
			: _delegate(obj, fn)
		{}
		
		/// Copy constructor
		slot(const this_type& slot)
			: _delegate(slot._delegate)
		{}
		
		/// Destructor
		~slot() {}
		
		/// Assignment operator
		/// \param slot The slot you wish to assign to
		/// \return *this
		this_type& operator=(const this_type& slot)
		{
			_delegate = slot._delegate;
			return *this;
		}
		
		/// Calls the slot
		/// \param args Any arguments you want to pass to the slot
		template <class ...Args>
		void operator()(Args&&... args) const
		{
			_delegate(std::forward<Args>(args)...);
		}
		
		
		// comparision operators for sorting and comparing
		
		bool operator==(const this_type& slot) const
		{ return _delegate == slot._delegate; }
		
		bool operator!=(const this_type& slot) const
		{ return !operator==(slot); }
		
		bool operator<(const this_type& slot) const
		{ return _delegate < slot._delegate; }
		
		bool operator>(const this_type& slot) const
		{ return slot._delegate > _delegate; }
		
		bool operator<=(const this_type& slot) const
		{ return !operator>(slot); }
		
		bool operator>=(const this_type& slot) const
		{ return !operator<(slot); }
		
	private:
		
		/// The implementation of the slot, as a delegate.
		typedef fastdelegate::FastDelegate<Signature> impl_delegate;
		
		impl_delegate _delegate;
	};
}


#endif // WINK_SLOT_HPP
