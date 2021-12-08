#ifndef _JX3_H_
#define _JX3_H_

typedef int                 BOOL;
typedef unsigned char       BYTE;

#define FALSE               0
#define TRUE                1

#define KG_PROCESS_ERROR(Condition) \
    if (!(Condition))               \
    {                               \
        goto Exit0;                 \
    }

#define KG_PROCESS_SUCCESS(Condition)    \
    if ((Condition))                   \
    {                                   \
        goto Exit1;                     \
    }

#define KGLOG_PROCESS_ERROR(Condition) \
    if (!(Condition))               \
    {                               \
        printf("KG_PROCESS_ERROR(%s) at line %d in %s\n", #Condition, __LINE__, __FUNCTION__); \
        goto Exit0;                 \
    }

#define KGLOG_PRAM_PROCESS_ERROR(Condition, ...) \
    if (!(Condition))               \
    {                               \
        printf(__VA_ARGS__);        \
        printf(" at line %d in %s\n", __LINE__, __FUNCTION__); \
        goto Exit0;                 \
    }

#define KG_DELETE_POINT(Point)           \
    do{                         \
        if ((Point))            \
        {                       \
            delete((Point));    \
            (Point) = NULL;     \
        }                       \
    }while(0);

#endif
