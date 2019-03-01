#include "stack.hpp"

Stack::Stack():
    size_           (0),
    capacity_       (1),
    size_canary_    (0),
    total_capacity_ (capacity_),

    canary_ ((sttype*) calloc(total_capacity_, sizeof(sttype))),
    data_   (canary_),

    hash_sum_ (0),
    db_mode_  (false)
{
    Hash();
    return;
}

Stack::Stack(int len_canary):
    size_           (0),
    capacity_       (1),
    size_canary_    (len_canary),
    total_capacity_ (size_canary_ * 2 + capacity_),

    canary_ ((sttype*) calloc(total_capacity_, sizeof(sttype))),
    data_   (canary_ + size_canary_),

    hash_sum_ (0),
    db_mode_  (false)
{
    for(int i = 0; i < size_canary_; ++i)
        canary_[i] = canary_[total_capacity_ - i - 1] = i + 1;

    Hash();
    return;
}

Stack::~Stack()
{
    free(canary_);

    return;
}

//********************************************************************************************************************************

errcode Stack::Push(sttype val)
{
    errcode state = Trouble();
    if(state != OK)
        return state;

    if(size_ == capacity_)
        Expand();

    if(size_ == capacity_)
        return STACK_overflow;

    data_[size_++] = val;

    Hash();
    return OK;
}

errcode Stack::Pop(sttype* ans)
{
    errcode state = Trouble();
    if(state != OK)
        return state;

    if(size_ == 0)
        return STACK_underflow;

    *ans = data_[--size_];

    if(size_ <= capacity_ / 4)
        Contract();

    Hash();
    return OK;
}

void Stack::Dump()
{
    if(db_mode_)
    {
        DbDump();
        return;
    }

    puts("Stack dump:\n{");

    #define echo( var ) printf("\t%s\b = %d\n", #var, var);
    echo(size_);
    echo(capacity_);
    echo(db_mode_);
    #undef echo

    int i = 0;
    puts("\tdata:\n\t{");
    if(size_ == 0)
        puts("\t\tEmpty");
    else
    {
        for( ; i < size_ - 1; ++i)
            std::cout << "\t\t * " << data_[i] << std::endl;

        std::cout     << "\t\t>* " << data_[i++] << std::endl;

        for( ; i < capacity_; ++i)
            std::cout << "\t\t   " << data_[i] << std::endl;
    }
    puts("\t}");

    StateMsg(Trouble());

    puts("}\n");
    return;
}

void Stack::SwapDbDump()
{
    char str[2] = "";

    if(db_mode_ == true)
    {
        puts("Cancel debugging-mode dump? [Y/n]");
        printf("==> ");

        scanf("%1[^\n]s", str);
        if(*str == 'Y' || *str == 'y' || *str == '\0')
        {
            puts(" Success");
            db_mode_ = false;
        }
        else
            puts(" Canceled ");
    }
    else
    {
        puts("Unlock debugging-mode dump? [Y/n]");
        printf("==> ");

        scanf("%1[^\n]s", str);
        if(*str == 'Y' || *str == 'y' || *str == '\0')
        {
            puts(" Access granted");
            db_mode_ = true;
        }
        else
            puts(" Access denied");
    }

    return;
}

//********************************************************************************************************************************

errcode Stack::Trouble()
{
    int prev_hash_sum = hash_sum_;
    Hash();
    if(prev_hash_sum != hash_sum_)
        return ERR_hash;

    for(int i = 0; i < size_canary_; ++i)
        if(canary_[i] != i + 1 || canary_[total_capacity_ - i - 1] != i + 1)
            return ERR_canary;

    return OK;
}

void Stack::StateMsg(errcode state)
{
    switch(state)
    {
    case OK:
        puts("Stack is OK");
        break;

    case ERR_hash:
        puts("ERROR: Hash-security alert");
        break;

    case ERR_canary:
        puts("ERROR: Canary-security alert");
        break;

    case STACK_overflow:
        puts("WARNING: an attempt to expand stack memory failed, MAX-limit has been reached");
        break;

    case STACK_underflow:
        puts("WARNING: an attempt to pop an item from empty stack was held, MIN-limit has been reached");
        break;

    default:
        puts("FATAL ERROR: unknown error code");
        break;
    }

    return;
}

void Stack::Expand()
{
    sttype* tmp = (sttype*) realloc(canary_, sizeof(sttype) * (total_capacity_ + capacity_));
    if(!tmp)
        return;

    total_capacity_ += capacity_;
    capacity_ *= 2;

    canary_ = tmp;
    data_ = canary_ + size_canary_;

    for(int i = size_; i < capacity_; i++)
        data_[i] = 0;

    for(int i = 0; i < size_canary_; ++i)
        canary_[total_capacity_ - i - 1] = i + 1;

    return;
}

void Stack::Contract()
{
    if(capacity_ == 1)
        return;

    sttype* tmp = (sttype*) realloc(canary_, sizeof(sttype) * (total_capacity_ - capacity_ / 2));
    if(!tmp)
        return;

    capacity_ /= 2;
    total_capacity_ -= capacity_;

    canary_ = tmp;
    data_ = canary_ + size_canary_;

    for(int i = size_; i < capacity_; i++)
        data_[i] = 0;

    for(int i = 0; i < size_canary_; ++i)
        canary_[total_capacity_ - i - 1] = i + 1;

    return;
}

//********************************************************************************************************************************

void Stack::Hash()
{
    int sum = 0;
    for(int i = 0; i < capacity_; ++i)
        sum = (sum << 5) + (sum >> 27) + (int) data_[i];

    hash_sum_ = sum;
    return;
}

void Stack::DbDump()
{
    puts("Stack dump: [DEBUGGING]\n{");

    #define echo( var ) printf("\t%s = %d\n", #var, var);
    echo(size_);
    echo(capacity_);
    echo(size_canary_);
    echo(total_capacity_);
    echo(hash_sum_);
    #undef echo

    int i = 0;
    puts("\tcanary (front):\n\t{");
    if(size_canary_ == 0)
        puts("\t\tEmpty");
    else
        for( ; i < size_canary_; ++i)
        {
            printf("\t\t   {%2d} ", i);
            std::cout << canary_[i] << std::endl;
        }
    puts("\t}\n");

    puts("\tdata:\n\t{");
    for( ; i < size_ + size_canary_ - 1; ++i)
    {
        printf("\t\t * [%2d] ", i);
        std::cout << canary_[i] << std::endl;
    }
    if(size_ >= 1)
    {
        printf("\t\t>* [%2d] ", i);
        std::cout << canary_[i++] << std::endl;
    }
    for( ; i < capacity_ + size_canary_; ++i)
    {
        printf("\t\t   (%2d) ", i);
        std::cout << canary_[i] << std::endl;
    }
    puts("\t}\n");

    puts("\tcanary (back):\n\t{");
    if(size_canary_ == 0)
        puts("\t\tEmpty");
    else
        for( ; i < total_capacity_; ++i)
        {
            printf("\t\t  {%2d} ", i);
            std::cout << canary_[i] << std::endl;
        }
    puts("\t}");

    StateMsg(Trouble());

    puts("}\n");
    return;
}

