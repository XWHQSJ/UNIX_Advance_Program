# 习题
## 第二章

2.1 在头文件<machine/_types.h>中定义可在多个头文件中出现的基本数据类型，例如：
    #ifndef _MACHINE_TYPES_H_
    #define _MACHINE_TYPES_H_

    typedef int __int32_t;
    typedef unsigned int _uint32_t;
    ...
    typedef __uint32_t __size_t;
    ...
    #endif /* _MACHINE_TYPES_H_ */

    在每个可以定义基本数据类型size_t的头文件中，包含下面的语句序列：
    #ifndef _SIZE_T_DECLARED
    typedef __size_t size_t
    #define _SIZE_T_DECLARED
    #endif
    这样，实际上只执行一次size_t的typedef

2.2 

#include <features.h>
#include <bits/wordsize.h>

/* Convenience types.  */
typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;

/* Fixed-size types, underlying types depend on word size and compiler.  */
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;
#if __WORDSIZE == 64
typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;
#else
__extension__ typedef signed long long int __int64_t;
__extension__ typedef unsigned long long int __uint64_t;
#endif

/* quad_t is also 64 bits.  */
#if __WORDSIZE == 64
typedef long int __quad_t;
typedef unsigned long int __u_quad_t;
#else
__extension__ typedef long long int __quad_t;
__extension__ typedef unsigned long long int __u_quad_t;
#endif

2.3 见源程序 open_max2.c