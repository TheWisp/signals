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
/// to deal in the Software without restriction, including without limitation the rights /// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell /// copies of the Software, and to permit persons to whom the Software is
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

#ifndef WINK_EVENT_QUEUE_HPP
#define WINK_EVENT_QUEUE_HPP

#include <queue>

#include <wink/signal.hpp>

namespace wink
{
	/// \brief A utility class to resemble a queue of events
	///
	/// A queue of events, simply resembles a queue of data you wish to
	/// emit to any slots registered to the event queue. This class is an adaption
	/// to the signal class.
	///
	/// Unlike regular queues, you can iterate through this queue. Data can optionally be cleared
	/// when emitting events.
	///
	/// \author Miguel Martin
	template <typename EventData>
	struct event_queue
	{
	private:
		
		typedef std::vector<EventData> impl_event_queue;
		typedef event_queue<EventData> this_type;
		
	public:
		
		/// The slot used for the signal, for implementatino defailts.
		typedef slot<void (const EventData&)> slot_type;
		
		/// The signal_type that is used, for implementation details.
		typedef signal<slot_type> signal_type;
		
		typedef std::vector<EventData> event_data_array;
		
		// Queue-specific typedefs
		typedef typename impl_event_queue::reference reference;
		typedef typename impl_event_queue::const_reference const_reference;
		typedef typename impl_event_queue::pointer pointer;
		typedef typename impl_event_queue::const_pointer const_pointer;
		typedef typename impl_event_queue::iterator iterator;
		typedef typename impl_event_queue::const_iterator const_iterator;
		typedef typename impl_event_queue::reverse_iterator reverse_iterator;
		typedef typename impl_event_queue::const_reverse_iterator const_reverse_iterator;
		typedef typename impl_event_queue::size_type size_type;
		typedef typename impl_event_queue::difference_type difference_type;
		
		event_queue() {}
		event_queue(const this_type& _event_queue) = default;
		event_queue& operator=(const this_type& queue) = default;
		
		// Signal-specific methods
		
		/// Connects a slot to the event queue
		/// \param args Arguments used to construct the slot to connect
		template <typename... Args>
		void connect(Args&&... args)
		{ _signal.connect(std::forward<Args>(args)...); }
		
		/// Disconnects a slot to the event queue
		/// \param args Arguments used to construct the slot to disconnect
		template <typename... Args>
		void disconnect(Args&&... args)
		{ _signal.disconnect(std::forward<Args>(args)...); }
		
		/// Emits out events, without modifying the event queue
		/// \note
		/// This will not clear out the data you pushed to the event_queue,
		/// c stands for constant.
		void cemit() const
		{
			for(typename event_queue::const_iterator i = _event_data_queue.begin(); i != _event_data_queue.end(); ++i)
			{
				_signal(*i); // send out the signal
			}
		}
		
		/// Emits out queued up events, and then clears the queued events.
		/// \see cemit If you wish for you data to not be cleared.
		void emit()
		{
			cemit(); // emit events
			clear(); // clear out data
		}
		
		/// Clears the event queue
		void clear()
		{ _event_data_queue.clear(); 	}
		
		/// operator() overload
		/// \note
		/// This contains the same implementation as emit(), it is simply an easier way to call emit().
		/// It is useful for when you need an event_queue to act as a functor.
		void operator()()
		{ emit(); }
		
		/// operator() overload, constant
		/// \note
		/// This contains the same implementation as cemit(), it is simply an easier way to call emit().
		/// It is useful for when you need an event_queue to act as a functor.
		void operator()() const
		{ cemit(); }
		
		
		
		// Queue-specific methods
		
		/// Pushses data to the event queue
		/// \param data The data you wish to add to the event queue
		void push(const EventData& data)
		{ _event_data_queue.push_back(data); }
		
		/// Pushses data to the event queue
		/// \param data The data you wish to add to the event queue
		/// \note
		/// This is useful if you wish to not copy any data.
		void push(EventData&& data)
		{ _event_data_queue.push_back(data); }
		
		/// Pushes an initializer list of data to the event queue
		/// \param data The data you wish to push
		void push(std::initializer_list<EventData>&& data)
		{ _event_data_queue.insert(_event_data_queue.end(), data.begin(), data.end());}
		
		/// Pushses an array of data to the event queue
		/// \param array The data you wish to add to the event queue
		void push(const event_data_array& array)
		{ _event_data_queue.insert(_event_data_queue.end(), array.begin(), array.end()); }
		
		/// Emplaces data to the event queue
		/// \param args The data you wish to add to the event queue
		template <typename ...Args>
		void emplace(Args&&... args)
		{ _event_data_queue.emplace_back(args...);}
		
		/// Reserves space in the event queue
		/// \note
		/// Call this method if you are about to push
		/// a lot of data on the queue.
		void reserve(size_type size)
		{ _event_data_queue.reserve(size); }
		
		/// \return The size of the queue
		size_type size() const
		{ return _event_data_queue.size(); }
		
		/// \return true if the queue is empty
		bool empty() const
		{ return _event_data_queue.empty(); }
		
		/// \param index The index that you wish to access of the queue
		/// \return A reference to data at the index
		reference operator[](size_type index)
		{ return _event_data_queue[index]; }
		
		/// \param index The index that you wish to access of the queue
		/// \return A const reference to data at the index
		const_reference operator[](size_type index) const
		{ return _event_data_queue[index]; }
		
		
		
		// iterators
		
		iterator begin()
		{ return _event_data_queue.begin(); }
		
		const_iterator begin() const
		{ return _event_data_queue.begin(); }
		
		const_iterator cbegin() const
		{ return _event_data_queue.cbegin(); }
		
		reverse_iterator rbegin() const
		{ return _event_data_queue.rbegin(); }
		
		const_reverse_iterator crbegin() const
		{ return _event_data_queue.crbegin(); }
		
		iterator end()
		{ return _event_data_queue.begin(); }
		
		const_iterator end() const
		{ return _event_data_queue.begin(); }
		
		const_iterator cend() const
		{ return _event_data_queue.cbegin(); }
		
		const_reverse_iterator crend() const
		{ return _event_data_queue.crbegin(); }
		
	private:
		
		/// The queue of data in the event queue
		impl_event_queue _event_data_queue;
		
		/// The signal that is used to send events for the event queue
		signal_type _signal;
	};
}

#endif // WINK_EVENT_QUEUE_HPP
