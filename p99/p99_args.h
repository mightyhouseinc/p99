/*
** orwl_macro.h
** 
** Made by Jens Gustedt
** Login   <gustedt@damogran.loria.fr>
** 
** Started on  Sun Feb 28 18:48:25 2010 Jens Gustedt
** Last update Sun Feb 28 18:48:25 2010 Jens Gustedt
*/

#ifndef   	P99_ARGS_H_
# define   	P99_ARGS_H_

/**
 ** @file
 ** @brief Providing macros that handle variadic macro argument lists.
 **/

#include "p99_paste.h"

/**
 ** @addtogroup basic_list_operations
 **
 ** @{
 **/

/**
 ** @brief Return the length of the variate argument list.
 **
 ** This supposes that the length is less than 64.
 **
 ** Idea is due to Laurent Deniau, CERN, and taken from a discussion
 ** in comp.lang.c.
 **
 ** @warning this also counts an empty argument list as having one (=
 ** the empty) argument
 ** @see P99_NARG for a macro that returns 0 if the list is empty
 **/
#define P99__NARG(...) P99__NARG_1(__VA_ARGS__)

#define P99__IS__EQ__(...) ,

/**
 ** @brief Test if the argument list is empty.
 **
 ** This expands to token 1 if the list was empty and to token 0 if
 ** there was anything different from a comment in the list.
 **
 ** The implementation of this macro is kind of tricky and heavily
 ** uses the fact that a function macro (@c P99__IS__EQ__ in this case) is
 ** left untouched if it is not followed by a parenthesis. See
 ** http://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
 **
 ** @return tokens 0 or 1
 **/
#define P99_IS_EMPTY(...)                                               \
P99__ISEMPTY(                                                           \
             /* test if there is just one argument, eventually an empty \
                one */                                                  \
             P99_HAS_COMMA(__VA_ARGS__),                                \
             /* test if P99_IS__EQ__ together with the argument         \
                adds a comma */                                         \
             P99_HAS_COMMA(P99__IS__EQ__ __VA_ARGS__),                  \
             /* test if the argument together with a parenthesis        \
                adds a comma */                                         \
             P99_HAS_COMMA(__VA_ARGS__ (~)),                            \
             /* test if placing it between P99_IS__EQ__ and the         \
                parenthesis adds a comma */                             \
             P99_HAS_COMMA(P99__IS__EQ__ __VA_ARGS__ (~))               \
             )

#define P99__ISEMPTY(_0, _1, _2, _3) P99_HAS_COMMA(P99_PASTE5(P99__IS_EMPTY_CASE_, _0, _1, _2, _3))
#define P99__IS_EMPTY_CASE_0001 ,

#define P99__NARG_EMPTY_1(VAL) 0
#define P99__NARG_EMPTY_0(VAL) VAL

/**
 ** @brief Return the length of the variate argument list, an empty
 ** argument list accounting for 0 arguments.
 **
 ** This supposes that the length of the list is less than ::P99_MAX_NUMBER.
 **
 ** @see P99__NARG for a macro that accounts an empty list to be 1
 **/
#define P99_NARG(...) P99__NARG__1(P99_IS_EMPTY(__VA_ARGS__), P99__NARG(__VA_ARGS__))
#define P99__NARG__1(B, VAL) P99__NARG__2(P99_PASTE2(P99__NARG_EMPTY_, B), VAL)
#define P99__NARG__2(B, VAL) B(VAL)


/**
 ** @}
 **/


/**
 ** @addtogroup preprocessor_text
 **
 ** @{
 **/

/**
 ** @brief Detect if two tokens are equal.
 **
 ** These must be alphanumerical tokens composed of [_a-zA-Z_] and to
 ** be able to test for equality of token @c X the macro @c
 ** P99__IS_X_EQ_X(...) must be defined to expand to a comma.
 **
 ** For a list of predefined tokens for which this is implemented see @ref preprocessor_text
 **/
#define P99_TOK_EQ(TOK, ...)  P99__TOK_EQ_(P99_PASTE3(P99__IS_, TOK, _EQ_), __VA_ARGS__)
#define P99__TOK_EQ_(MAC, ...)  P99__TOK_EQ__(MAC, __VA_ARGS__)
#define P99__TOK_EQ__(MAC, ...) P99_HAS_COMMA(P99_PASTE2(P99__TOK_EQ_, P99__NARG(MAC ## __VA_ARGS__ (~) MAC ## __VA_ARGS__))(~))

#define P99__TOK_EQ_0(...) ~
#define P99__TOK_EQ_1(...) ~
#define P99__TOK_EQ_2(...) ,
#define P99__TOK_EQ_3(...) ~
#define P99__TOK_EQ_4(...) ~


/**
 ** @}
 **/

/**
 ** @addtogroup variadic Macros to ease programming of variadic functions
 **
 ** @{
 **/

/**
 ** @brief Helper macro to declare a variable length parameter list.
 **
 ** Inside the declared function @a X will of @c size_t and should
 ** hold the actual length of the list. It can be used as the argument
 ** to @c va_start.
 **
 ** Wrap your function into a macro that uses P99_LENGTH_VA_ARG. If used through
 ** that macro, the correct value for @a X will always be provided at
 ** compile time. Declare such a function as this:
 ** @code
 ** unsigned P99_FSYMB(toto)(unsigned a, P99_VA_ARGS(number));
 ** #define toto(A, ...) P99_FSYMB(toto)(A, P99_LENGTH_VA_ARG(__VA_ARGS__))
 ** @endcode
 **
 ** In the definition of the function you then may use the @c va_start
 ** etc from stdarg.h to tread the argument list.
 ** @code
 ** unsigned P99_FSYMB(toto)(unsigned a, P99_VA_ARGS(number)) {
 **     unsigned ret = 0;
 **     va_list ap;
 **     va_start(ap, number);
 **     for (size_t i = 0; i < number; ++i) {
 **       ret += va_arg(ap, unsigned);
 **     }
 **     va_end(ap);
 **     return ret % a;
 ** }
 ** @endcode
 ** In this toy example @c toto can be used as
 ** @code
 ** unsigned magic = toto(3, 1, 3, 5, 7);
 ** @endcode
 ** which will result in converting 1, 3, 5, 7 (the variable
 ** arguments) to @c unsigned, computing their sum, i.e 16u, and
 ** compute that value mod 3u (the fixed argument @a a). So @a magic
 ** should hold the value 1u thereafter. (But beware of implicit
 ** integer promotion rules for integers of small width.)
 **
 ** @param X is the name of the `length' parameter that you want to
 ** use in the definition of the function. As in the example above it
 ** should be then used as the second argument to @c va_start and as a
 ** loop boudary when you actual handle the argument list. @a X is
 ** implicitly declared to have type @c size_t.
 **
 ** @see P99_LENGTH_ARR_ARG for a way that is generally more efficient
 **      than using @c va_list
 ** @see P99_LENGTH_VA_ARG
 ** @see P99_FSYMB
 **/
#define P99_VA_ARGS(X) size_t X /*!< the number of arguments that follow */, ...

#define P99__FSYMB(NAME) P99_PASTE5(NAME, _f, sy, mb, _)

/**
 ** @brief Mangle @a NAME
 **
 ** This should only be used in declaration and definition of the
 ** function that is hidden behind the macro @a NAME.
 **/
#define P99_FSYMB(NAME) P99__FSYMB(NAME)

/**
 ** @brief Meta-macro to generate calls to functions with variable
 ** argument list.
 **
 ** This supposes that the length is less than 64. It prefixes the
 ** list of arguments by an integer constant containing the length of
 ** the list.
 ** @see P99_VA_ARGS
 **/
#define P99_LENGTH_VA_ARG(...) ((size_t)P99_NARG(__VA_ARGS__)), __VA_ARGS__


/**
 ** @brief Helper macro to declare a variable length parameter list.
 **
 ** Inside the declared function @a X will of @c size_t and should
 ** hold the actual length of the list. It can be used as the argument
 ** to @c va_start.
 **
 ** Wrap your function into a macro that uses P99_LENGTH_ARR_ARG. If used through
 ** that macro, the correct value for @a X will always be provided at
 ** compile time. Declare such a function as this:
 ** @code
 ** unsigned P99_FSYMB(tutu)(unsigned a, size_t number, unsigned *arr);
 ** #define tutu(A, ...) P99_FSYMB(tutu)(A, P99_LENGTH_ARR_ARG(unsigned, __VA_ARGS__))
 ** @endcode
 **
 ** In the definition of the function you then may use an array of the
 ** arguments in the obvious way.
 ** @code
 ** unsigned P99_FSYMB(tutu)(unsigned a, size_t number, unsigned *arr) {
 **   unsigned ret = 0;
 **   for (size_t i = 0; i < number; ++i) {
 **     ret += arr[i];
 **   }
 **   return ret % a;
 ** }
 ** @endcode
 ** In this toy example @c tutu can be used as
 ** @code
 ** unsigned magic = tutu(3, 1, 3, 5, 7);
 ** @endcode
 ** which will result in converting 1, 3, 5, 7 (the variable
 ** arguments) to @c unsigned, computing their sum, i.e 16u, and
 ** compute that value mod 3u (the fixed argument @a a). So @a magic
 ** should hold the value 1u thereafter.
 **
 ** In the example @c number is the name of the `length' parameter
 ** that you want to use in the definition of the function.
 **
 ** This method here is generally more efficient than using
 ** ::P99_VA_ARGS since it results in code that can easier be inlined
 ** by the compiler. In  particular, if a function as @c tutu above is
 ** called with all parameters being compile time constants, the call
 ** may completely be optimized away.
 **
 ** @see P99_VA_ARGS
 ** @see P99_FSYMB
 **/
#define P99_LENGTH_ARR_ARG(T, ...) ((size_t)P99_NARG(__VA_ARGS__)), (T[]){ __VA_ARGS__ }


/**
 ** @}
 **/



#endif 	    /* !P99_ARGS_H_ */