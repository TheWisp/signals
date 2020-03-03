// memory.hpp v2.4.1
/*
 *  Copyright (c) 2007 Leigh Johnston.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 *     * Neither the name of Leigh Johnston nor the names of any
 *       other contributors to this software may be used to endorse or
 *       promote products derived from this software without specific prior
 *       written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* WARNING: The classes present here are not a substitute for any equivalent std:: 
 * classes available on your platform which you should be using instead.  They exist here
 * either for technical reasons or for when there is no standard library available.
  */

#pragma once

#include "neolib.hpp"
#include <new>
#include <stdexcept>
#include <memory>
#include <iostream>
#include "detail_memory.hpp"

namespace neolib
{
	template <typename InIter, typename OutIter> inline
	OutIter uninitialized_copy(InIter first, InIter last, OutIter result)
	{
		return detail::uninitialized_copy(first, last, result, *result);
	}

	template <typename T, std::size_t ChunkSize = 4096, bool Omega = false, std::size_t Instance = 0>
	class pool_allocator
	{
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef const T* const_pointer;
		typedef const T& const_reference;
		typedef std::size_t size_type;
		typedef ptrdiff_t difference_type;
	private:
		typedef std::allocator<T> backup_allocator_t;

	// implementation
	private:
		struct link { link* iNext; };

		static constexpr std::size_t chunk_size()
		{
			if constexpr (ChunkSize > sizeof(T))
				return ChunkSize;
			else
				return sizeof(T);
		}

		static constexpr std::size_t element_size()
		{
			return sizeof(value_type) < sizeof(link) ? sizeof(link) : sizeof(value_type);
		}

		struct chunk
		{
			alignas(T) char iMem[chunk_size()];
			chunk* iNext;
		};
		class pool
		{
		public:
			pool() : iChunks(nullptr), iHead(nullptr) 
			{
			}
			~pool()
			{
				chunk* n = iChunks;
				while (n)
				{
					chunk* p = n;
					n = n->iNext;
					delete p;
				}
			}
		public:
			void* allocate()
			{
				if (iHead == nullptr)
					grow();
				link* p = iHead;
				if constexpr (!Omega)
					iHead = p->iNext;
				else if (reinterpret_cast<intptr_t>(iHead->iNext) == intptr_t{ -1 })
					iHead = reinterpret_cast<link*>(reinterpret_cast<char*>(p) + element_size());
				else
					iHead = p->iNext;
				return p;
			}
			void deallocate(void* aObject)
			{
				if constexpr (!Omega)
				{
					link* p = reinterpret_cast<link*>(aObject);
					p->iNext = iHead;
					iHead = p;
				}
			}
		public:
			void omega_recycle()
			{
				if constexpr (Omega)
				{
					for (chunk* n = iChunks; n != nullptr; n = n->iNext)
					{
						constexpr std::size_t nelem = chunk_size() / element_size();
						char* start = n->iMem;
						char* last = start + nelem * element_size();
						std::memset(start, 0xFF, last - start);
						reinterpret_cast<link*>(last - element_size())->iNext = (n->iNext != nullptr ? reinterpret_cast<link*>(n->iNext->iMem) : nullptr);
					}
					iHead = reinterpret_cast<link*>(iChunks->iMem);
				}
			}
			void info()
			{
				uint32_t total = 0;
				uint32_t pct = 0;
				for (chunk* n = iChunks; n != nullptr; n = n->iNext)
				{
					++total;
					char* start = n->iMem;
					char* last = start + chunk_size();
					if (reinterpret_cast<char*>(iHead) >= start && reinterpret_cast<char*>(iHead) < last)
						pct = (reinterpret_cast<char*>(iHead) - start) * 100 / (last - start);
				}
				std::cout << "Number of chunks: " << total << std::endl;
				if constexpr (Omega)
					std::cout << "% utilization of last used chunk: " << pct << "%" << std::endl;
			}
		private:
			void grow()
			{
				chunk* n = new chunk;
				n->iNext = iChunks;
				iChunks = n;

				constexpr std::size_t nelem = chunk_size() / element_size();
				char* start = n->iMem;
				char* last = start + nelem * element_size();
				if constexpr (!Omega)
					for (char* p = start; p < last; p += element_size())
						reinterpret_cast<link*>(p)->iNext = reinterpret_cast<link*>(p + element_size());
				else
					std::memset(start, 0xFF, last - start);

				reinterpret_cast<link*>(last - element_size())->iNext = nullptr;
				iHead = reinterpret_cast<link*>(start);
			}
		private:
			chunk * iChunks;
			link* iHead;
		};

	// construction
	public:
		pool_allocator()
		{
		}

		pool_allocator(const pool_allocator&) 
		{
		}

		template <typename U>
		pool_allocator(const pool_allocator<U, ChunkSize, Omega, Instance>& /*rhs*/)
		{
		}

		~pool_allocator() 
		{
		}

	// operations
	public:
		pointer allocate(size_type aCount = 1)
		{
			if (aCount == 1)
				return reinterpret_cast<pointer>(sPool.allocate());
			else
				return backup_allocator().allocate(aCount);
		}

		void deallocate(pointer aObject, size_type aCount = 1)
		{
			if constexpr (!Omega)
			{
				if (aCount == 1)
					sPool.deallocate(aObject);
				else
					backup_allocator().deallocate(aObject, aCount);
			}
		}

		void construct(pointer aObject, const_reference val)
		{
			new (aObject) T(val);
		}

		template <typename... Args>
		void construct(pointer aObject, Args&&... aArguments)
		{
			new (aObject) T(std::forward<Args>(aArguments)...);
		}

		void destroy(pointer aObject)
		{
			if constexpr (!Omega)
				aObject->~T();
		}

		void omega_recycle()
		{
			sPool.omega_recycle();
		}

		void info()
		{
			sPool.info();
		}
			
		template <typename U>
		struct rebind
		{
			typedef pool_allocator<U, ChunkSize, Omega, Instance> other;
		};

		// this should really return 1 but popular implementations assume otherwise
		size_type max_size() const { return std::allocator<T>().max_size(); }

		bool operator==(const pool_allocator&) const { return true; }
		bool operator!=(const pool_allocator&) const { return false; }

	private:
		static backup_allocator_t& backup_allocator()
		{
			static backup_allocator_t sBackupAllocator;
			return sBackupAllocator;
		}

	// attributes
	private:
		static pool sPool;
	};

	template <typename T, std::size_t ChunkSize, bool Omega, std::size_t Instance>
	typename pool_allocator<T, ChunkSize, Omega, Instance>::pool pool_allocator<T, ChunkSize, Omega, Instance>::sPool;

	template <typename T, std::size_t N, std::size_t Instance = 0>
	class reserve_allocator
	{
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef const T* const_pointer;
		typedef const T& const_reference;
		typedef std::size_t size_type;
		typedef ptrdiff_t difference_type;

	// implementation
	private:
		struct link { link* iNext; };
		struct block
		{
			size_type iElementSize;
			union
			{
				union
				{
					alignas(T) char a[sizeof(T)];
					alignas(link) char b[sizeof(link)];
				} iBuffer[N];
			} iBuffer;
			char* iMem;
			link* iHead;
			block() : iElementSize(sizeof(T) < sizeof(link) ? sizeof(link) : sizeof(T)), iMem(reinterpret_cast<char*>(iBuffer.iBuffer)), iHead(reinterpret_cast<link*>(iMem)) { init(); }
			void init()
			{
				char* start = iMem;
				char* last = &start[(N-1) * iElementSize];
				for (char* p = start; p < last; p+= iElementSize)
					reinterpret_cast<link*>(p)->iNext = reinterpret_cast<link*>(p+iElementSize);
				reinterpret_cast<link*>(last)->iNext = nullptr;
			}
			void* allocate()
			{
				if (iHead == nullptr)
					throw std::bad_alloc("neolib::reserve_allocator::allocate() when full");
				link* p = iHead;
				iHead = p->iNext;
				return p;
			}
			void deallocate(void* aObject)
			{
				link* p = reinterpret_cast<link*>(aObject);
				p->iNext = iHead;
				iHead = p;
			}
		};

	// construction
	public:
		reserve_allocator() {}
		reserve_allocator(const reserve_allocator& rhs) {}
		template <typename U>
		reserve_allocator(const reserve_allocator<U, N, Instance>& rhs) {}
		~reserve_allocator() {}

	// operations
	public:
		T* allocate(size_type aCount = 1)
		{
			if (aCount != 1)
				throw std::bad_alloc();
			return reinterpret_cast<T*>(sBlock.allocate());
		}

		void deallocate(T* aObject, size_type aCount = 1)
		{
			if (aCount != 1)
				throw std::logic_error("neolib::reserve_allocator::deallocate");
			sBlock.deallocate(aObject);
		}

		void construct(pointer aObject, const_reference val)
		{
			new (aObject) T(val);
		}

		void destroy(pointer aObject)
		{
			aObject->~T();
		}

		template <typename U>
		struct rebind
		{
			typedef reserve_allocator<U, N, Instance> other;
		};

		// this should really return 1 but popular implementations assume otherwise
		size_type max_size() const { return std::allocator<T>().max_size(); }

		bool operator==(const reserve_allocator&) const { return true; }
		bool operator!=(const reserve_allocator&) const { return false; }
		
		// attributes
	private:
		static block sBlock;
	};

	template <typename T, std::size_t N, std::size_t Instance>
	typename reserve_allocator<T, N, Instance>::block reserve_allocator<T, N, Instance>::sBlock;
}
