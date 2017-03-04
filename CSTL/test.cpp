/*
**	test.cpp
**
**	这是测试CSTL库的文件
**
**	作者：hbg
**	on Sat. March 4, 2017
**
*/
//#define TEST_STL
#define TEST_ALLOC_H


#ifdef TEST_STL
#include <vector>
class simple
{
public:
	int a;
};
#endif // TEST_STL

#ifdef TEST_ALLOC_H
#include "alloc.h"
#include <vector>
#include <string>
struct simple
{
public:
	int a;
};
#endif // TEST_ALLOC_H

#include <stdio.h>

int main()
{
#ifdef TEST_STL
	simple ia[5] = {0, 1, 2, 3, 4};
	std::vector<simple> iv(ia, ia + 5);
	for(unsigned int i = 0; i < iv.size();i ++)
	{
		printf("%d ", iv[i].a);
	}
    putchar('\n');
#endif // TEST_STL

#ifdef TEST_ALLOC_H
	//#define ARBITRARY_TYPE_STACK_MEMORY
	#ifdef ARBITRARY_TYPE_STACK_MEMORY
	int ia[5] = {0, 1, 2, 3, 4};
	std::vector< int, msvl::allocator<int> > iv(ia, ia + 5);
	for(unsigned int i = 0; i < iv.size();i ++)
	{
		printf("%d ", iv[i]);
	}
	putchar('\n');
	#endif // ARBITRARY_TYPE_STACK_MEMORY

	#define CLASS_TYPE
	#ifdef CLASS_TYPE
	// 容器元素类型为类或结构体类型时，测试失败。还未找到原因
	simple sa[3];
	for(int i = 0;i < 3;i++)
	{
		sa[i].a = i;
	}
	std::vector< simple, msvl::allocator<int> > sv(sa, sa + 3);
	for(unsigned int i = 0; i < sv.size();i ++)
	{
		printf("%d ", sv[i].a);
	}
	putchar('\n');
	#endif // CLASS_TYPE

	//#define STACK_MEMORY
	#ifdef STACK_MEMORY
	int *sia = (int*)malloc(5 * sizeof(int));
	for(int i = 0;i < 5;i ++)
	{
		sia[i] = i;
	}
	std::vector< int, msvl::allocator<int> > siv(sia, sia + 5);
	for(unsigned int i = 0; i < siv.size();i ++)
	{
		printf("%d ", siv[i]);
	}
	putchar('\n');
	#endif // STACK_MEMORY
#endif // TEST_ALLOC_H
	return 0;
}
