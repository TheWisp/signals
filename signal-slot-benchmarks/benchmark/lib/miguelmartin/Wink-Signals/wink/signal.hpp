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

#ifndef WINK_SIGNAL_HPP
#define WINK_SIGNAL_HPP

#include <vector>
#include <utility>
#include <algorithm>

//#include <wink/slot.hpp>
#include "slot.hpp"

namespace wink
{
	template <class Slot>
	struct signal
	{
	private:
		
		typedef signal<Slot> this_type;
		
	public:
		
		typedef Slot slot_type;
		
		/// Connects a slot to the signal
		/// \param args The arguments you wish to construct the slot with to connect to the signal
		template <typename... Args>
		void connect(Args&&... args)
		{
			_slots.emplace_back(std::forward<Args>(args)...);
		}
		
		/// Disconnects a slot from the signal
		/// \param args The arguments you wish to construct a slot with
		template <typename... Args>
		void disconnect(Args&&... args)
		{
			_slots.erase(std::find(_slots.begin(), _slots.end(), slot_type(std::forward<Args>(args)...)));
		}
		
		/// Emits the events you wish to send to the call-backs
		/// \param args The arguments to emit to the slots connected to the signal
		template <class ...Args>
		void emit(Args&&... args) const
		{
			for(typename slot_array::const_iterator i = _slots.begin(); i != _slots.end(); ++i)
			{
				(*i)(std::forward<Args>(args)...);
			}
		}
		
		/// Emits events you wish to send to call-backs
		/// \param args The arguments to emit to the slots connected to the signal
		/// \note
		/// This is equvialent to emit.
		template <class ...Args>
		void operator()(Args&&... args) const
		{
			emit(std::forward<Args>(args)...);
		}
		
		// comparision operators for sorting and comparing
		
		bool operator==(const this_type& signal) const
		{ return _slots == signal._slots; }
		
		bool operator!=(const this_type& signal) const
		{ return !operator==(signal); }
		
	private:
		
		/// defines an array of slots
		typedef std::vector<slot_type> slot_array;
		
		/// The slots connected to the signal
		slot_array _slots;
	};
}
#endif // WINK_SIGNAL_HPP
