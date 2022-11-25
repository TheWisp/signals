/*===================================================================
*	Copyright (c) Vadim Karkhin. All rights reserved.
*	Use, modification, and distribution is subject to license terms.
*	You are welcome to contact the author at: vdksoft@gmail.com
===================================================================*/

#ifndef VDK_SIGNALS_H
#define VDK_SIGNALS_H

#define VDK_NS_BEGIN		namespace vdk {
#define VDK_NS_END			}

#include <cstddef>		// std::size_t, std::nullptr_t
#include <utility>		// std::move(), std::forward<>()
#include <algorithm>	// std::copy()
#include <new>			// ::new()
#include <atomic>		// std::atomic<>, std::atomic_flag
#include <memory>		// std::shared_ptr<>, std::weak_ptr<>
#include <thread>		// std::this_thread

VDK_NS_BEGIN

// Macros for dynamic memory allocation
#define VDK_NEW(arg) ::operator new(arg)
#define VDK_DELETE(arg) ::operator delete(arg)

// TEMPLATE CLASS signal
template<typename _ResT, typename ... _ArgTs>
class signal;

#ifndef VDK_SIGNALS_LITE

template<typename _ResT, typename ... _ArgTs>
class signal<_ResT(_ArgTs...)> final
{
	// Unified function pointer wrapper. Instances of slot_functor
	// can store, copy, and invoke any callable target (slot).
	class slot_functor;

	// Node in linked list of connected slots. Contains information
	// related to a particular connection between a signal and a slot.
	class connection;

	// Memory manager for signal object. Keeps memory blocks to store
	// connection objects. Provides memory allocations / deallocations.
	class storage;

	// Synchronization primitive that protects shared data from being
	// simultaneously modified by multiple threads. Optimized for speed
	// and occupies very little memory. Meets Lockable requirements.
	class slim_lock;

	// Class auto_lock is a slim_lock ownership wrapper that provides a
	// convenient RAII-style mechanism for automatic locking / unlocking.
	class auto_lock;

	// Class read_guard is a stage counter ownership wrapper. Provides
	// a convenient RAII-style mechanism for automatic increm / decrem.
	class read_guard;

	// Synchronization stage. Specifies current access stage.
	enum class sync_stage : char { sync_stage_1 = 1, sync_stage_2 = 2 };

	using byte				= unsigned char;
	using size_type			= ::std::size_t;
	using bool_type			= ::std::atomic<bool>;
	using counter_type		= ::std::atomic<unsigned short>;
	using access_stage		= ::std::atomic<sync_stage>;
	using connection_ptr	= ::std::atomic<connection*>;
	using track_ptr			= ::std::weak_ptr<void>;

	class slot_functor final
	{
		// Structure to store target pointers
		template<typename _Class, typename _Signature>
		struct target_slot
		{
			using slot_function = _Signature;
			using slot_instance = _Class*;
			slot_function mp_function;
			slot_instance mp_instance;
		};

		// Unknown default class (undefined)
		class default_class;

		// Unknown default function (undefined)
		using default_function = void(default_class::*)(void);

		// Default target_slot type
		using default_type = target_slot<default_class, default_function>;

		// Size of default target data
		static const size_type target_size = sizeof(default_type);

		// Storage for target data
		using slot_storage = byte[target_size];

		// Type of invoker-function
		using invoker_type = _ResT(*)(const byte * const, _ArgTs&...);

		// Invoke target slot (static method / free function)
		template<::std::nullptr_t, typename _Signature>
		static _ResT invoke(const byte * const data, _ArgTs& ... args)
		{
			return
			(*reinterpret_cast<const target_slot<::std::nullptr_t, _Signature>*>
			(data)->mp_function)(args...);
		}

		// Invoke target slot (method)
		template<typename _Class, typename _Signature>
		static _ResT invoke(const byte * const data, _ArgTs& ... args)
		{
			return
			(reinterpret_cast<const target_slot<_Class, _Signature>*>
			(data)->mp_instance->*
			reinterpret_cast<const target_slot<_Class, _Signature>*>
			(data)->mp_function)(args...);
		}

		// Invoke target slot (functor)
		template<typename _Class, ::std::nullptr_t>
		static _ResT invoke(const byte * const data, _ArgTs& ... args)
		{
			return
			(*reinterpret_cast<const target_slot<_Class, ::std::nullptr_t>*>
			(data)->mp_instance)(args...);
		}

	public:

		// Construct slot_functor (static method / free function)
		explicit slot_functor(_ResT(*function)(_ArgTs...)) noexcept
		: m_target{}, mp_invoker{ nullptr }
		{
			using _Signature = decltype(function);
			auto storage = reinterpret_cast<target_slot
				<::std::nullptr_t, _Signature>*>(&m_target[0]);
			storage->mp_function = function;
			storage->mp_instance = nullptr;
			mp_invoker = &slot_functor::invoke<nullptr, _Signature>;
		}

		// Construct slot_functor (method)
		template<typename _Class, typename _Signature>
		slot_functor(_Class * object, _Signature method) noexcept
		: m_target{}, mp_invoker{ nullptr }
		{
			auto storage = reinterpret_cast<target_slot
				<_Class, _Signature>*>(&m_target[0]);
			storage->mp_function = method;
			storage->mp_instance = object;
			mp_invoker = &slot_functor::invoke<_Class, _Signature>;
		}

		// Construct slot_functor (functor)
		template<typename _Class>
		explicit slot_functor(_Class * functor) noexcept
		: m_target{}, mp_invoker{ nullptr }
		{
			auto storage = reinterpret_cast<target_slot
				<_Class, ::std::nullptr_t>*>(&m_target[0]);
			storage->mp_function = nullptr;
			storage->mp_instance = functor;
			mp_invoker = &slot_functor::invoke<_Class, nullptr>;
		}

		// Deleted constructor (null pointer)
		slot_functor(::std::nullptr_t)noexcept = delete;

		// Copy-construct slot_functor
		slot_functor(const slot_functor & other) noexcept
		{
			::std::copy(other.m_target,
				other.m_target + target_size, m_target);
			mp_invoker = other.mp_invoker;
		}

		// Destroy slot_functor
		~slot_functor() noexcept
		{}

		// Copy-assign slot_functor
		slot_functor & operator=(const slot_functor & other) noexcept
		{
			if (this != &other)
			{
				::std::copy(other.m_target,
					other.m_target + target_size, m_target);
				mp_invoker = other.mp_invoker;
			}
			return *this;
		}

		// Invoke target slot
		_ResT operator()(_ArgTs& ... args) const
		{
			return (*mp_invoker)(&m_target[0], args...);
		}

		// Compare slot_functors (equal)
		bool operator==(const slot_functor & other) const noexcept
		{
			for (size_type index = 0; index < target_size; ++index)
			{
				if (m_target[index] != other.m_target[index])
				{
					return false;
				}
			}
			return true;
		}

		// Compare slot_functors (not equal)
		bool operator!=(const slot_functor & other) const noexcept
		{
			for (size_type index = 0; index < target_size; ++index)
			{
				if (m_target[index] != other.m_target[index])
				{
					return true;
				}
			}
			return false;
		}

	private:

		alignas(default_type)slot_storage m_target;
		alignas(invoker_type)invoker_type mp_invoker;

	};

	class connection final
	{
	public:

		// Construct connection
		connection( const slot_functor & slot,
					const track_ptr & t_ptr,
					bool trackable ) noexcept
		:	m_slot(slot),
			m_track_ptr(t_ptr),
			mp_next(nullptr),
			mp_deleted(nullptr),
			m_trackable(trackable)
		{}

		// Deleted copy-constructor
		connection(const connection&)noexcept = delete;

		// Destroy connection
		~connection() noexcept
		{}

		// Deleted copy-assignment operator
		connection & operator=(const connection&)noexcept = delete;

	public:

		slot_functor	m_slot;
		track_ptr		m_track_ptr;
		connection_ptr	mp_next;
		connection *	mp_deleted;
		const bool		m_trackable;

	};

	class storage final
	{
		// Initialize requested memory block
		void init(connection * address) noexcept
		{
			mp_store = address;
			connection ** current = reinterpret_cast<connection**>(address);

			for (size_type index = 1; index < m_capacity; ++index)
			{
				(*current) = ++address;
				current = reinterpret_cast<connection**>(address);
			}

			(*current) = nullptr;
		}

		// Expand memory by allocating new memory block
		// May throw exception if memory allocation fails
		void expand()
		{
			connection * new_block = reinterpret_cast<connection*>
				(VDK_NEW(sizeof(connection) * m_capacity +
					sizeof(connection*)));
			connection ** next_block = reinterpret_cast<connection**>
				(mp_block + m_capacity);

			while (*next_block)
			{
				next_block = reinterpret_cast<connection**>
					((*next_block) + m_capacity);
			}

			(*next_block) = new_block;
			init(new_block);
			(*reinterpret_cast<connection**>
				(new_block + m_capacity)) = nullptr;
		}

		// Free all allocated memory blocks
		void clear() noexcept
		{
			if (mp_block)
			{
				connection * to_delete = (*reinterpret_cast<connection**>
					(mp_block + m_capacity));

				while (to_delete)
				{
					connection * block = (*reinterpret_cast<connection**>
						(to_delete + m_capacity));
					VDK_DELETE(to_delete);
					to_delete = block;
				}

				VDK_DELETE(mp_block);
			}
		}

	public:

		// Construct storage
		// May throw exception if memory allocation fails
		storage(size_type capacity)
		:	mp_block(nullptr),
			mp_store(nullptr),
			m_capacity(capacity >= 1 ? capacity : 1)
		{
			mp_block = reinterpret_cast<connection*>(VDK_NEW(m_capacity
				* sizeof(connection) + sizeof(connection*)));
			init(mp_block);
			(*reinterpret_cast<connection**>
				(mp_block + m_capacity)) = nullptr;
		}

		// Move-construct storage
		storage(storage && other) noexcept
		:	mp_block(other.mp_block),
			mp_store(other.mp_store),
			m_capacity(other.m_capacity)
		{
			other.mp_block = nullptr;
			other.mp_store = nullptr;
			other.m_capacity = 0;
		}

		// Destroy storage
		~storage() noexcept
		{
			clear();
		}

		// Move-assign storage
		storage & operator=(storage && other) noexcept
		{
			clear();

			mp_block = other.mp_block;
			mp_store = other.mp_store;
			m_capacity = other.m_capacity;

			other.mp_block = nullptr;
			other.mp_store = nullptr;
			other.m_capacity = 0;

			return *this;
		}

		// Allocate memory from storage
		// May throw exception if memory allocation fails
		connection * allocate()
		{
			if (!mp_store)
			{
				expand(); // May throw
			}

			connection * current = mp_store;
			mp_store = (*reinterpret_cast<connection**>(mp_store));
			return current;
		}

		// Deallocate previously allocated memory
		void deallocate(connection * address) noexcept
		{
			(*reinterpret_cast<connection**>(address)) = mp_store;
			mp_store = address;
		}

	private:

		connection *	mp_block;
		connection *	mp_store;
		size_type		m_capacity;

	};

	class slim_lock final
	{
		using lock_type = ::std::atomic_flag;

	public:

		// Construct slim_lock
		slim_lock() noexcept
		{}

		// Deleted copy-constructor
		slim_lock(const slim_lock&)noexcept = delete;

		// Destroy slim_lock
		~slim_lock() noexcept
		{}

		// Deleted copy-assignment operator
		slim_lock & operator=(const slim_lock&)noexcept = delete;

		// Lock slim_lock. If another thread has already locked this
		// slim_lock, blocks execution until the lock is acquired.
		void lock() noexcept
		{
			while (m_lock.test_and_set(::std::memory_order_acquire))
			{
				::std::this_thread::yield();
			}
		}

		// Unlock slim_lock
		void unlock() noexcept
		{
			m_lock.clear(::std::memory_order_release);
		}

		// Try to lock slim_lock. Returns immediately. On successful
		// lock acquisition returns true, otherwise returns false.
		bool try_lock() noexcept
		{
			return m_lock.test_and_set(::std::memory_order_acquire)
				? false : true;
		}

	private:

		lock_type m_lock = ATOMIC_FLAG_INIT;

	};

	class auto_lock
	{
	public:

		// Construct auto_lock
		explicit auto_lock(slim_lock & s_lock) noexcept
		: mp_lock(&s_lock)
		{
			mp_lock->lock();
		}

		// Move-construct auto_lock
		auto_lock(auto_lock && other) noexcept
		: mp_lock(other.mp_lock)
		{
			other.mp_lock = nullptr;
		}

		// Destroy auto_lock
		~auto_lock() noexcept
		{
			if (mp_lock)
			{
				mp_lock->unlock();
			}
		}

		// Move-assign auto_lock
		auto_lock & operator=(auto_lock && other) noexcept
		{
			mp_lock->unlock();
			mp_lock = other.mp_lock;
			other.mp_lock = nullptr;
			return *this;
		}

	private:

		slim_lock * mp_lock;

	};

	class read_guard final
	{
	public:

		// Construct read_guard
		explicit read_guard(counter_type & counter) noexcept
		: mp_counter(&counter)
		{
			++(*mp_counter);
		}

		// Move-construct read_guard
		read_guard(read_guard && other) noexcept
		: mp_counter(other.mp_counter)
		{
			other.mp_counter = nullptr;
		}

		// Destroy read_guard
		~read_guard() noexcept
		{
			if (mp_counter)
			{
				--(*mp_counter);
			}
		}

		// Move-assign read_guard
		read_guard & operator=(read_guard && other) noexcept
		{
			--(*mp_counter);
			mp_counter = other.mp_counter;
			other.mp_counter = nullptr;
			return *this;
		}

	private:

		counter_type * mp_counter;

	};

	// Access signal's internal structure for reading
	read_guard read_access() const noexcept
	{
		return read_guard(m_sync_stage.load() ==
			sync_stage::sync_stage_1 ? m_access_s1 : m_access_s2);
	}

	// Access signal's internal structure for writing
	auto_lock write_access() const noexcept
	{
		return auto_lock(m_write_lock);
	}

	// Synchronize internal signal's state - delete logically removed
	// elements according to synchronized stage (if possible).
	// Must be called under write_access() protection.
	void synchronize() noexcept
	{
		connection ** first = nullptr;
		connection * to_delete = nullptr;

		sync_stage current_stage = m_sync_stage.load();

		switch (current_stage)
		{
		case sync_stage::sync_stage_1:
			if (!m_access_s2.load())
			{
				to_delete = mp_deleted_s2;
				first = &mp_deleted_s2;
				current_stage = sync_stage::sync_stage_2;
				break;
			}
			else
			{
				return;
			}
		case sync_stage::sync_stage_2:
			if (!m_access_s1.load())
			{
				to_delete = mp_deleted_s1;
				first = &mp_deleted_s1;
				current_stage = sync_stage::sync_stage_1;
				break;
			}
			else
			{
				return;
			}
		}

		while (to_delete)
		{
			(*first) = to_delete->mp_deleted;
			to_delete->~connection();
			m_storage.deallocate(to_delete);
			to_delete = (*first);
		}

		m_sync_stage.store(current_stage);
	}

	// Fix pointers of removed elements to point after this element.
	// Must be called under write_access() protection.
	void fix_pointers(connection * node, connection * removed) noexcept
	{
		while (removed)
		{
			if (removed->mp_next.load() == node)
			{
				removed->mp_next.store(node->mp_next.load());
			}
			removed = removed->mp_deleted;
		}
	}

	// Logically remove element from connection list.
	// Must be called under write_access() protection.
	void remove(connection * node) noexcept
	{
		connection ** previous = nullptr;
		connection * current = nullptr;

		switch (m_sync_stage.load())
		{
		case sync_stage::sync_stage_1:
			previous = &mp_deleted_s1;
			current = mp_deleted_s1;
			break;
		case sync_stage::sync_stage_2:
			previous = &mp_deleted_s2;
			current = mp_deleted_s2;
			break;
		}

		fix_pointers(node, mp_deleted_s1);
		fix_pointers(node, mp_deleted_s2);

		while (current)
		{
			previous = &current->mp_deleted;
			current = current->mp_deleted;
		}

		(*previous) = node;
	}

	// Logically remove all elements from connection list.
	// Must be called under write_access() protection.
	void remove_all() noexcept
	{
		connection * to_delete = mp_first_slot.load();

		if (to_delete)
		{
			while (to_delete)
			{
				to_delete->mp_deleted = to_delete->mp_next.load();
				to_delete->mp_next.store(nullptr);
				to_delete = to_delete->mp_deleted;
			}

			remove(mp_first_slot.load());
			mp_first_slot.store(nullptr);
		}
	}

	// Delete logically removed elements regardless of synchronization.
	// Must be called under write_access() protection.
	void clear(connection * removed) noexcept
	{
		while (removed)
		{
			connection * to_delete = removed;
			removed = removed->mp_deleted;
			to_delete->~connection();
			m_storage.deallocate(to_delete);
		}
	}

	// Connect new slot to the signal
	// May throw exception if memory allocation fails
	// Must be called under write_access() protection
	bool connect(const slot_functor & slot,
				 const track_ptr & t_ptr,
				 bool trackable)
	{
		synchronize();

		connection * current = mp_first_slot.load();
		connection_ptr * previous = &mp_first_slot;

		while (current)
		{
			if (current->m_slot == slot)
			{
				return false;
			}
			else
			{
				previous = &current->mp_next;
				current = current->mp_next.load();
			}
		}

		connection * new_connection = m_storage.allocate();
		::new(new_connection) connection(slot, t_ptr, trackable);
		previous->store(new_connection);
		return true;
	}

	// Disconnect slot from the signal
	// Must be called under write_access() protection
	bool disconnect(const slot_functor & slot) noexcept
	{
		synchronize();

		connection * current = mp_first_slot.load();
		connection_ptr * previous = &mp_first_slot;

		while (current)
		{
			if (current->m_slot == slot)
			{
				previous->store(current->mp_next.load());
				remove(current);
				return true;
			}
			else
			{
				previous = &current->mp_next;
				current = current->mp_next.load();
			}
		}

		return false;
	}

	// Check whether slot is connected to the signal
	// Must be called under read_access() protection
	bool connected(const slot_functor & slot) const noexcept
	{
		connection * current = mp_first_slot.load();

		while (current)
		{
			if (current->m_slot == slot)
			{
				return true;
			}
			else
			{
				current = current->mp_next.load();
			}
		}

		return false;
	}

	// Activate signal
	// May throw exception if some slot does
	// Must be called under read_access() protection
	void activate(_ArgTs& ... args)
	{
		connection * current = mp_first_slot.load();

		while (current)
		{
			if (!current->m_trackable)
			{
				current->m_slot(args...);
				current = current->mp_next.load();
			}
			else
			{
				auto ptr = current->m_track_ptr.lock();

				if (ptr)
				{
					current->m_slot(args...);
					current = current->mp_next.load();
				}
				else
				{
					connection * to_delete = current;
					current = current->mp_next.load();
					auto writer = write_access();
					disconnect(to_delete->m_slot);
				}
			}
		}
	}

public:

	// Construct signal with provided / default capacity
	// May throw exception if memory allocation fails
	explicit signal(size_type capacity = 5)
	:	m_storage(capacity),
		mp_first_slot(nullptr),
		mp_deleted_s1(nullptr),
		mp_deleted_s2(nullptr),
		m_access_s1(0),
		m_access_s2(0),
		m_write_lock(),
		m_sync_stage(sync_stage::sync_stage_1),
		m_blocked(false)
	{}

	// Deleted copy-constructor
	signal(const signal &) noexcept = delete;

	// Destroy signal
	~signal() noexcept
	{
		auto writer = write_access();
		remove_all();
		clear(mp_deleted_s1);
		clear(mp_deleted_s2);
	}

	// Deleted copy-assignment operator
	signal & operator=(const signal &) noexcept = delete;

	// Connect signal to slot (static method / free function)
	// May throw exception if memory allocation fails
	bool connect(_ResT(*function)(_ArgTs...))
	{
		auto writer = write_access();
		return connect(slot_functor(function), track_ptr(), false);
	}

	// Connect signal to slot (method)
	// May throw exception if memory allocation fails
	template<typename _Class, typename _Signature>
	bool connect(_Class * object, _Signature method)
	{
		auto writer = write_access();
		return connect(slot_functor(object, method), track_ptr(), false);
	}

	// Connect signal to trackable slot (method)
	// May throw exception if memory allocation fails
	template<typename _Class, typename _Signature>
	bool connect(::std::shared_ptr<_Class> object, _Signature method)
	{
		auto writer = write_access();
		return connect(slot_functor(object.get(), method), track_ptr(object), true);
	}

	// Connect signal to slot (functor)
	// May throw exception if memory allocation fails
	template<typename _Class>
	bool connect(_Class * functor)
	{
		auto writer = write_access();
		return connect(slot_functor(functor), track_ptr(), false);
	}

	// Connect signal to trackable slot (functor)
	// May throw exception if memory allocation fails
	template<typename _Class>
	bool connect(::std::shared_ptr<_Class> functor)
	{
		auto writer = write_access();
		return connect(slot_functor(functor.get()), track_ptr(functor), true);
	}

	// Disconnect signal from slot (static method / free function)
	bool disconnect(_ResT(*function)(_ArgTs...)) noexcept
	{
		auto writer = write_access();
		return disconnect(slot_functor(function));
	}

	// Disconnect signal from slot (method)
	template<typename _Class, typename _Signature>
	bool disconnect(_Class * object, _Signature method) noexcept
	{
		auto writer = write_access();
		return disconnect(slot_functor(object, method));
	}

	// Disconnect signal from trackable slot (method)
	template<typename _Class, typename _Signature>
	bool disconnect(::std::shared_ptr<_Class> object, _Signature method) noexcept
	{
		auto writer = write_access();
		return disconnect(slot_functor(object.get(), method));
	}

	// Disconnect signal from slot (functor)
	template<typename _Class>
	bool disconnect(_Class * functor) noexcept
	{
		auto writer = write_access();
		return disconnect(slot_functor(functor));
	}

	// Disconnect signal from trackable slot (functor)
	template<typename _Class>
	bool disconnect(::std::shared_ptr<_Class> functor) noexcept
	{
		auto writer = write_access();
		return disconnect(slot_functor(functor.get()));
	}

	// Disconnect signal from all slots
	void disconnect_all() noexcept
	{
		auto writer = write_access();
		remove_all();
	}

	// Check whether slot is connected (static method / free function)
	bool connected(_ResT(*function)(_ArgTs...)) const noexcept
	{
		auto reader = read_access();
		return connected(slot_functor(function));
	}

	// Check whether slot is connected (method)
	template<typename _Class, typename _Signature>
	bool connected(_Class * object, _Signature method) const noexcept
	{
		auto reader = read_access();
		return connected(slot_functor(object, method));
	}

	// Check whether trackable slot is connected (method)
	template<typename _Class, typename _Signature>
	bool connected(::std::shared_ptr<_Class> object, _Signature method) const noexcept
	{
		auto reader = read_access();
		return connected(slot_functor(object.get(), method));
	}

	// Check whether slot is connected (functor)
	template<typename _Class>
	bool connected(_Class * functor) const noexcept
	{
		auto reader = read_access();
		return connected(slot_functor(functor));
	}

	// Check whether trackable slot is connected (functor)
	template<typename _Class>
	bool connected(::std::shared_ptr<_Class> functor) const noexcept
	{
		auto reader = read_access();
		return connected(slot_functor(functor.get()));
	}

	// Block signal
	void block(bool block = true) noexcept
	{
		m_blocked.store(block);
	}

	// Check whether signal is blocked
	bool blocked() const noexcept
	{
		return m_blocked.load();
	}

	// Emit signal
	// May throw exception if some slot does
	void emit(_ArgTs ... args)
	{
		auto reader = read_access();
		if (!m_blocked.load())
		{
			activate(args...);
		}
	}

	// Emit signal
	// May throw exception if some slot does
	void operator()(_ArgTs ... args)
	{
		auto reader = read_access();
		if (!m_blocked.load())
		{
			activate(args...);
		}
	}

	// Get number of connected slots
	size_type size() const noexcept
	{
		auto writer = write_access();
		connection * current = mp_first_slot.load();
		size_type size = 0;

		while (current)
		{
			++size;
			current = current->mp_next.load();
		}

		return size;
	}

	// Check whether list of connected slots is empty
	bool empty() const noexcept
	{
		return !mp_first_slot.load();
	}

private:

	storage					m_storage;
	connection_ptr			mp_first_slot;
	connection *			mp_deleted_s1;
	connection *			mp_deleted_s2;
	mutable counter_type	m_access_s1;
	mutable counter_type	m_access_s2;
	mutable slim_lock		m_write_lock;
	access_stage			m_sync_stage;
	bool_type				m_blocked;

};

#endif
#ifdef VDK_SIGNALS_LITE

template<typename _ResT, typename ... _ArgTs>
class signal<_ResT(_ArgTs...)> final
{
	// Unified function pointer wrapper. Instances of slot_functor
	// can store, copy, and invoke any callable target (slot).
	class slot_functor;

	// Node in linked list of connected slots. Contains information
	// related to a particular connection between a signal and a slot.
	class connection;

	using byte		= unsigned char;
	using size_type	= unsigned short;
	using track_ptr	= ::std::weak_ptr<void>;

	static const size_type null_index = ((size_type)-1);

	class slot_functor final
	{
		// Structure to store target pointers
		template<typename _Class, typename _Signature>
		struct target_slot
		{
			using slot_function = _Signature;
			using slot_instance = _Class*;
			slot_function mp_function;
			slot_instance mp_instance;
		};

		// Unknown default class (undefined)
		class default_class;

		// Unknown default function (undefined)
		using default_function = void(default_class::*)(void);

		// Default target_slot type
		using default_type = target_slot<default_class, default_function>;

		// Size of default target data
		static const size_type target_size = sizeof(default_type);

		// Storage for target data
		using slot_storage = byte[target_size];

		// Type of invoker-function
		using invoker_type = _ResT(*)(const byte * const, _ArgTs&...);

		// Invoke target slot (static method / free function)
		template<::std::nullptr_t, typename _Signature>
		static _ResT invoke(const byte * const data, _ArgTs& ... args)
		{
			return
			(*reinterpret_cast<const target_slot<::std::nullptr_t, _Signature>*>
			(data)->mp_function)(args...);
		}

		// Invoke target slot (method)
		template<typename _Class, typename _Signature>
		static _ResT invoke(const byte * const data, _ArgTs& ... args)
		{
			return
			(reinterpret_cast<const target_slot<_Class, _Signature>*>
			(data)->mp_instance->*
			reinterpret_cast<const target_slot<_Class, _Signature>*>
			(data)->mp_function)(args...);
		}

		// Invoke target slot (functor)
		template<typename _Class, ::std::nullptr_t>
		static _ResT invoke(const byte * const data, _ArgTs& ... args)
		{
			return
			(*reinterpret_cast<const target_slot<_Class, ::std::nullptr_t>*>
			(data)->mp_instance)(args...);
		}

	public:

		// Construct slot_functor (static method / free function)
		explicit slot_functor(_ResT(*function)(_ArgTs...)) noexcept
		: m_target{}, mp_invoker{ nullptr }
		{
			using _Signature = decltype(function);
			auto storage = reinterpret_cast<target_slot
				<::std::nullptr_t, _Signature>*>(&m_target[0]);
			storage->mp_function = function;
			storage->mp_instance = nullptr;
			mp_invoker = &slot_functor::invoke<nullptr, _Signature>;
		}

		// Construct slot_functor (method)
		template<typename _Class, typename _Signature>
		slot_functor(_Class * object, _Signature method) noexcept
		: m_target{}, mp_invoker{ nullptr }
		{
			auto storage = reinterpret_cast<target_slot
				<_Class, _Signature>*>(&m_target[0]);
			storage->mp_function = method;
			storage->mp_instance = object;
			mp_invoker = &slot_functor::invoke<_Class, _Signature>;
		}

		// Construct slot_functor (functor)
		template<typename _Class>
		explicit slot_functor(_Class * functor) noexcept
		: m_target{}, mp_invoker{ nullptr }
		{
			auto storage = reinterpret_cast<target_slot
				<_Class, ::std::nullptr_t>*>(&m_target[0]);
			storage->mp_function = nullptr;
			storage->mp_instance = functor;
			mp_invoker = &slot_functor::invoke<_Class, nullptr>;
		}

		// Deleted constructor (null pointer)
		slot_functor(::std::nullptr_t)noexcept = delete;

		// Copy-construct slot_functor
		slot_functor(const slot_functor & other) noexcept
		{
			::std::copy(other.m_target,
				other.m_target + target_size, m_target);
			mp_invoker = other.mp_invoker;
		}

		// Destroy slot_functor
		~slot_functor() noexcept
		{}

		// Copy-assign slot_functor
		slot_functor & operator=(const slot_functor & other) noexcept
		{
			if (this != &other)
			{
				::std::copy(other.m_target,
					other.m_target + target_size, m_target);
				mp_invoker = other.mp_invoker;
			}
			return *this;
		}

		// Invoke target slot
		_ResT operator()(_ArgTs& ... args) const
		{
			return (*mp_invoker)(&m_target[0], args...);
		}

		// Compare slot_functors (equal)
		bool operator==(const slot_functor & other) const noexcept
		{
			for (size_type index = 0; index < target_size; ++index)
			{
				if (m_target[index] != other.m_target[index])
				{
					return false;
				}
			}
			return true;
		}

		// Compare slot_functors (not equal)
		bool operator!=(const slot_functor & other) const noexcept
		{
			for (size_type index = 0; index < target_size; ++index)
			{
				if (m_target[index] != other.m_target[index])
				{
					return true;
				}
			}
			return false;
		}

	private:

		alignas(default_type)slot_storage m_target;
		alignas(invoker_type)invoker_type mp_invoker;

	};

	class connection final
	{
	public:

		// Construct connection
		connection( const slot_functor & slot,
					const track_ptr & t_ptr,
					bool trackable ) noexcept
		:	m_slot(slot),
			m_track_ptr(t_ptr),
			m_next(null_index),
			m_trackable(trackable)
		{}

		// Copy-construct connection
		connection(const connection & other) noexcept
		:	m_slot(other.m_slot),
			m_track_ptr(other.m_track_ptr),
			m_next(other.m_next),
			m_trackable(other.m_trackable)
		{}

		// Destroy connection
		~connection() noexcept
		{}

		// Copy-assign connection
		connection & operator=(const connection & other) noexcept
		{
			if (this != &other)
			{
				m_slot = other.m_slot;
				m_track_ptr = other.m_track_ptr;
				m_next = other.m_next;
				m_trackable = other.m_trackable;
			}
			return *this;
		}

	public:

		slot_functor	m_slot;
		track_ptr		m_track_ptr;
		size_type		m_next;
		const bool		m_trackable;

	};

	// Initialize storage
	void init_storage(connection * address, size_type capacity) noexcept
	{
		for (size_type index = 1; index < capacity; ++index)
		{
			(*reinterpret_cast<size_type*>(address)) = index;
			++address;
		}
		(*reinterpret_cast<size_type*>(address)) = null_index;
	}

	// Expand memory by allocating new memory block
	// May throw exception if memory allocation fails
	void expand_storage()
	{
		size_type new_capacity = m_capacity * 2;
		connection * new_block = reinterpret_cast<connection*>
			(VDK_NEW(new_capacity * sizeof(connection)));

		init_storage(new_block, new_capacity);
		m_store = m_capacity;

		for (size_type index = 0; index < m_capacity; ++index)
		{
			::new (new_block + index) connection(*(mp_block + index));
			(mp_block + index)->~connection();
		}

		VDK_DELETE(mp_block);
		m_capacity = new_capacity;
		mp_block = new_block;
	}

	// Allocate memory from storage
	// May throw exception if memory allocation fails
	size_type allocate()
	{
		if (m_store == null_index)
		{
			expand_storage(); // May throw
		}
		size_type current = m_store;
		m_store = *(reinterpret_cast<size_type*>(mp_block + m_store));
		return current;
	}

	// Deallocate previously allocated memory
	void deallocate(size_type index) noexcept
	{
		(*reinterpret_cast<size_type*>(mp_block + index)) = m_store;
		m_store = index;
	}

	// Connect new slot to the signal
	// May throw exception if memory allocation fails
	bool connect(const slot_functor & slot,
				 const track_ptr & t_ptr,
				 bool trackable )
	{
		if (m_first_slot != null_index)
		{
			size_type current = m_first_slot;
			size_type previous = null_index;

			while (current != null_index)
			{
				if (mp_block[current].m_slot == slot)
				{
					return false;
				}
				else
				{
					previous = current;
					current = mp_block[current].m_next;
				}
			}

			size_type index = allocate();
			::new (&mp_block[index]) connection(slot, t_ptr, trackable);
			mp_block[previous].m_next = index;
			return true;
		}
		else
		{
			size_type index = allocate();
			::new (&mp_block[index]) connection(slot, t_ptr, trackable);
			m_first_slot = index;
			return true;
		}
	}

	// Disconnect slot from the signal
	bool disconnect(const slot_functor & slot) noexcept
	{
		if (m_first_slot != null_index)
		{
			size_type current = m_first_slot;

			if (mp_block[current].m_slot == slot)
			{
				m_first_slot = mp_block[current].m_next;
				mp_block[current].~connection();
				deallocate(current);
				return true;
			}
			else
			{
				size_type previous = current;
				current = mp_block[current].m_next;

				while (current != null_index)
				{
					if (mp_block[current].m_slot == slot)
					{
						mp_block[previous].m_next = mp_block[current].m_next;
						mp_block[current].~connection();
						deallocate(current);
						return true;
					}
					else
					{
						previous = current;
						current = mp_block[current].m_next;
					}
				}
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	// Check whether slot is connected to the signal
	bool connected(const slot_functor & slot) const noexcept
	{
		size_type current = m_first_slot;

		while (current != null_index)
		{
			if (mp_block[current].m_slot == slot)
			{
				return true;
			}
			else
			{
				current = mp_block[current].m_next;
			}
		}

		return false;
	}

	// Activate signal
	// May throw exception if some slot does	
	void activate(_ArgTs ... args)
	{
		if (!m_blocked)
		{
			size_type current = m_first_slot;

			while (current != null_index)
			{
				if (!mp_block[current].m_trackable)
				{
					mp_block[current].m_slot(args...);
					current = mp_block[current].m_next;
				}
				else
				{
					auto ptr = mp_block[current].m_track_ptr.lock();

					if (ptr)
					{
						mp_block[current].m_slot(args...);
						current = mp_block[current].m_next;
					}
					else
					{
						size_type to_delete = current;
						current = mp_block[current].m_next;
						disconnect(mp_block[to_delete].m_slot);
					}
				}
			}
		}
	}

public:

	// Construct signal with provided / default capacity
	// May throw exception if memory allocation fails
	explicit signal(size_type capacity = 5)
	:	mp_block(nullptr),
		m_first_slot(null_index),
		m_capacity(capacity >= 1 ? capacity : 1),
		m_store(0),
		m_blocked(false)
	{
		mp_block = reinterpret_cast<connection*>
			(VDK_NEW(m_capacity * sizeof(connection)));
		init_storage(mp_block, m_capacity);
	}

	// Deleted copy-constructor
	signal(const signal &)noexcept = delete;

	// Destroy signal
	~signal() noexcept
	{
		disconnect_all();
		VDK_DELETE(mp_block);
	}

	// Deleted copy-assignment operator
	signal & operator=(const signal &)noexcept = delete;

	// Connect signal to slot (function)
	// May throw exception if memory allocation fails
	bool connect(_ResT(*function)(_ArgTs...))
	{
		return connect(slot_functor(function), track_ptr(), false);
	}

	// Connect signal to slot (method)
	// May throw exception if memory allocation fails
	template<typename _Class, typename _Signature>
	bool connect(_Class * object, _Signature method)
	{
		return connect(slot_functor(object, method), track_ptr(), false);
	}

	// Connect signal to trackable slot (method)
	// May throw exception if memory allocation fails
	template<typename _Class, typename _Signature>
	bool connect(::std::shared_ptr<_Class> object, _Signature method)
	{
		return connect(slot_functor(object.get(), method), track_ptr(object), true);
	}

	// Connect signal to slot (functor)
	// May throw exception if memory allocation fails
	template<typename _Class>
	bool connect(_Class * functor)
	{
		return connect(slot_functor(functor), track_ptr(), false);
	}

	// Connect signal to trackable slot (functor)
	// May throw exception if memory allocation fails
	template<typename _Class>
	bool connect(::std::shared_ptr<_Class> functor)
	{
		return connect(slot_functor(functor.get()), track_ptr(functor), true);
	}

	// Disconnect signal from slot (function)
	bool disconnect(_ResT(*function)(_ArgTs...)) noexcept
	{
		return disconnect(slot_functor(function));
	}

	// Disconnect signal from slot (method)
	template<typename _Class, typename _Signature>
	bool disconnect(_Class * object, _Signature method) noexcept
	{
		return disconnect(slot_functor(object, method));
	}

	// Disconnect signal from trackable slot (method)
	template<typename _Class, typename _Signature>
	bool disconnect(::std::shared_ptr<_Class> object, _Signature method) noexcept
	{
		return disconnect(slot_functor(object.get(), method));
	}

	// Disconnect signal from slot (functor)
	template<typename _Class>
	bool disconnect(_Class * functor) noexcept
	{
		return disconnect(slot_functor(functor));
	}

	// Disconnect signal from trackable slot (functor)
	template<typename _Class>
	bool disconnect(::std::shared_ptr<_Class> functor) noexcept
	{
		return disconnect(slot_functor(functor.get()));
	}

	// Disconnect signal from all slots
	void disconnect_all() noexcept
	{
		size_type to_delete = m_first_slot;

		while (to_delete != null_index)
		{
			size_type current = mp_block[to_delete].m_next;
			mp_block[to_delete].~connection();
			deallocate(to_delete);
			to_delete = current;
		}

		m_first_slot = null_index;
	}

	// Check whether slot is connected (function)
	bool connected(_ResT(*function)(_ArgTs...)) const noexcept
	{
		return connected(slot_functor(function));
	}

	// Check whether slot is connected (method)
	template<typename _Class, typename _Signature>
	bool connected(_Class * object, _Signature method) const noexcept
	{
		return connected(slot_functor(object, method));
	}

	// Check whether trackable slot is connected (method)
	template<typename _Class, typename _Signature>
	bool connected(::std::shared_ptr<_Class> object, _Signature method) const noexcept
	{
		return connected(slot_functor(object.get(), method));
	}

	// Check whether slot is connected (functor)
	template<typename _Class>
	bool connected(_Class * functor) const noexcept
	{
		return connected(slot_functor(functor));
	}

	// Check whether trackable slot is connected (functor)
	template<typename _Class>
	bool connected(::std::shared_ptr<_Class> functor) const noexcept
	{
		return connected(slot_functor(functor.get()));
	}

	// Block signal
	void block(bool block = true) noexcept
	{
		m_blocked = block;
	}

	// Check whether signal is blocked
	bool blocked() const noexcept
	{
		return m_blocked;
	}

	// Emit signal
	void emit(_ArgTs ... args)
	{
		activate(args...);
	}

	// Emit signal
	void operator()(_ArgTs ... args)
	{
		activate(args...);
	}

	// Get number of connected slots
	size_type size() const noexcept
	{
		size_type current = m_first_slot;
		size_type size = 0;

		while (current != null_index)
		{
			++size;
			current = mp_block[current].m_next;
		}

		return size;
	}

	// Check whether list of connected slots is empty
	bool empty() const noexcept
	{
		return m_first_slot == null_index;
	}

private:

	connection *	mp_block;
	size_type		m_first_slot;
	size_type		m_capacity;
	size_type		m_store;
	bool			m_blocked;

};

#endif

// Construct an object of type _Ty, passing std::shared_ptr<_Ty> and
// args as arguments to the constructor of _Ty.
// Allocates memory using ::operator new.
// May throw std::bad_alloc exception if memory allocation fails.
// _Ty's constructor must not throw.
template<typename _Ty, typename ... _ArgTs>
::std::shared_ptr<_Ty> create_shared_object(_ArgTs&& ... args)
{
	_Ty * address = static_cast<_Ty*>(VDK_NEW(sizeof(_Ty)));
	::std::shared_ptr<_Ty> sh_ptr(address,
	[](_Ty * ptr) { ptr->~_Ty(); VDK_DELETE(ptr); });
	::new (address) _Ty(sh_ptr, ::std::forward<_ArgTs>(args)...);
	return ::std::move(sh_ptr);
}

// Construct an object of type _Ty, passing std::shared_ptr<_Ty> and
// args as arguments to the constructor of _Ty.
// Allocates memory using provided allocator alloc.
// alloc must satisfy C++ Allocator concept requirements.
// May throw std::bad_alloc exception if memory allocation fails.
// _Ty's constructor must not throw.
template<typename _Ty, typename _Alloc, typename ... _ArgTs>
::std::shared_ptr<_Ty> allocate_shared_object(const _Alloc & alloc, _ArgTs&& ... args)
{
	_Alloc _allocator(alloc);
	_Ty * address = _allocator.allocate(1);
	::std::shared_ptr<_Ty> sh_ptr(address,
	[_allocator](_Ty * ptr) mutable
	{ _allocator.destroy(ptr); _allocator.deallocate(ptr, 1); }, _allocator);
	_allocator.construct(address, sh_ptr, ::std::forward<_ArgTs>(args)...);
	return ::std::move(sh_ptr);
}

VDK_NS_END

#endif