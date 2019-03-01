#ifndef STACK_HPP_INCLUDED
#define STACK_HPP_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

typedef double sttype; ///< type of stack items

enum errcode
{
    OK,             ///< Everything is OK
    ERR_hash,       ///< Security failure (wrong hash sum)
    ERR_canary,     ///< Security failure (a canary change)
    STACK_overflow, ///< Stack error by overflowing
    STACK_underflow ///< Stack error by underflowing
};

/////////////////////////////////////////////////
/// \brief Just stack
///
/// \param Stack()               - brief constructor (canary is not established)
/// \param Stack(int len_canary) - MAIN constructor (variable canary length)
/// \param ~Stack()              - MAIN destructor
///
/// \param errcode Push(sttype val) - push new item           | returns error code of current stack state
/// \param errcode Pop(sttype* ans) - pop stack item into ans | returns error code of current stack state
///
/// \param void Dump()       - show all stack items and current values
/// \param void SwapDbDump() - change dumping style ((default) brief <-> full)
///
/////////////////////////////////////////////////
class Stack
{
public:
    Stack();
    Stack(int len_canary);
    ~Stack();

    errcode Push(sttype val);
    errcode Pop(sttype* ans);
    void Dump();
    void SwapDbDump();

private:
    errcode Trouble();            ///< returns an error code (see enum errcode) if stack in not OK
    void StateMsg(errcode state); ///< writes to which error does given errcode-state refer to
    void Expand();                ///< makes an attempt to expand   memory by capacity
    void Contract();              ///< makes an attempt to contract memory by half-capacity

// high security:
    void Hash();   ///< recalculate hash sum
    void DbDump(); ///< debugging dump

// variables:
    int size_;              ///< current number of items
    int capacity_;          ///< maximum number of items
    const int size_canary_; ///< length of canary
    int total_capacity_;    ///< total length of array (canary + data + canary)

    sttype* canary_; ///< full memory
    sttype* data_;   ///< actual stack data

    int hash_sum_; ///< current hash sum value
    bool db_mode_; ///< debugging flag
};

#endif // STACK_HPP_INCLUDED
