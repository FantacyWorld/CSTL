/*
**	alloc.h
**
**	����һ���ռ������ģ�����ļ�
**
**	���ߣ�hbg
**	on Sat. March 4, 2017
**
*/

#include <stddef.h> // for ptrdiff_t, size_t
#include <stdlib.h> // for malloc(), exit()�� free()
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
			exit(1); // ***����klee��ο��Դ���
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
		// ��pԤ������ڴ��ϴ���һ��T2���͵Ķ��󣬲���value����
		// c++����place newʵ�֣�new(p) T2(value)
		// c����reallocʵ��
		new(p) T2(value); // �ɹ�
		//p->T2::T2(value); // ���T2���ڽ����ͣ�ʧ��; ���T2�������ͣ�����ʧ�ܣ�˵������÷���place new���ȼ�

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
			*tmp = value; // T2���ͱ��붨���˸�ֵ������
			p = (T1*)tmp; // ����ת����T1����
		}
		*/
	}

	template <class T>
	inline void _destroy(T* p)
	{
		p->~T(); // T������һ����
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
		pointer allocate(size_t n, const void* hint = 0) // hint����û������
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

		// ȡһ�����õĵ�ַ
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
