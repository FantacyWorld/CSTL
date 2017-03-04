/*
**	alloc.h
**
**	这是一个空间分配器模版类文件
**
**	作者：hbg
**	on Sat. March 4, 2017
**
*/

#include <stddef.h> // for ptrdiff_t, size_t
#include <stdlib.h> // for malloc(), exit()， free()
#include <limits.h> // for UINT_MAX
#include <stdio.h>  // for printf
#ifndef ALLOC_H_INCLUDED
#define ALLOC_H_INCLUDED

namespace msvl
{
	template <class T>
	inline T* _allocate(ptrdiff_t size, T*)
	{
		T* tmp = (T*)malloc((size_t)(size * sizeof(T)));
		if(!tmp)
		{
			printf("malloc error: out of memory\n");
			exit(1); // ***考虑klee如何可以处理
		}
		return tmp;
	}

	template <class T>
	inline void _deallocate(T* buffer)
	{
		free(buffer);
		buffer = NULL;
	}

	template <class T1, class T2>
	inline void _construct(T1* p, const T2& value) // something need done
	{
		// 在p预分配的内存上创建一个T2类型的对象，并用value覆盖
		// c++采用place new实现：new(p) T2(value)
		// c采用realloc实现
		new(p) T2(value); // 成功
		//p->T2::T2(value); // 如果T2是内建内型，失败; 如果T2是类类型，依旧失败，说明这个用法与place new不等价

		/*
		T2* tmp =(T2*)realloc(p, sizeof(T2));
		if(!tmp)
		{
			printf("realloc error: out of memory\n");
			free(p);p = NULL;
			exit(1);
		}
		else
		{
			*tmp = value; // T2类型必须定义了赋值操作符
			p = (T1*)tmp; // 必须转换成T1类型
		}
		*/
	}

	template <class T>
	inline void _destroy(T* p)
	{
		p->~T(); // T必须是一个类
	}

	template <class T>
	class allocator
	{
	public:
		// type
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		// rebind allocator of type U
		template <class U>
		struct rebind
		{
			typedef allocator<U> other;
		};

		// some wrapper
		pointer allocate(size_t n, const void* hint = 0) // hint参数没有意义
		{
			return _allocate((difference_type)n, (pointer)0);
		}

		void deallocate(pointer buffer, size_type n)
		{
			_deallocate(buffer);
		}

		void construct(pointer p, const T& value)
		{
			_construct(p, value);
		}

		void destroy(pointer p)
		{
			_destroy(p);
		}

		// 取一个引用的地址
		pointer address(reference x)
		{
			return (pointer)&x;
		}

		const_pointer const_address(const_reference x)
		{
			return (const_pointer)&x;
		}

		size_type max_size()const
		{
			return size_type((UINT_MAX)/sizeof(value_type));
		}
	};
}


#endif // ALLOC_H_INCLUDED
