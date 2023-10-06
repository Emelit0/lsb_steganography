
#ifndef TYPES_H
#define TYPES_H

typedef unsigned int uint;

//status will be used in main
typedef enum
{
    e_success,
    e_failure
} Status;

//operation types
typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;

#endif //TYPES_H
