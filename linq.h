#ifndef LINQ_H
#define LINQ_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "ArrayList.h"

#define lambda(ret, body) ({ ret __fn__ body __fn__; })

#define COMPARE_NUM(a, b) do{ \
    if ((a) > (b)) return 1; \
    if ((b) > (a)) return -1; \
    return 0; \
} while (0)

typedef struct tagLinq Linq;

typedef bool (*Predicate)(void *);
typedef bool (*PredicateIdx)(int, void *);

typedef int (*Comparer)(void *, void *);
typedef bool (*Equality)(void *, void *);

typedef void *(*Selector)(void *);
typedef void *(*SelectorIdx)(int, void *);

typedef Linq *(*SelectorMany)(void *);
typedef Linq *(*SelectorManyIdx)(int, void *);

typedef void *(*ResultSelector)(void *, void *);

typedef void *(Accumulator)(void *, void *);

#define WHERE(predicateFn)                              Where(LINQ_PTR, predicateFn)
#define WHERE_INDEXED(predicateIdxFn)                   WhereIndexed(LINQ_PTR, predicateIdxFn)
#define SELECT(selectFn)                                Select(LINQ_PTR, selectFn)
#define SELECT_INDEXED(selectIdxFn)                     SelectIndexed(LINQ_PTR, selectIdxFn)
#define SELECT_MANY(selectManyFn)                       SelectMany(LINQ_PTR, selectManyFn)
#define SELECT_MANY_INDEXED(selectManyIdxFn)            SelectManyIndexed(LINQ_PTR, selectManyIdxFn)
#define SELECT_MANYBY(selectManyFn, resultSelectFn)     SelectManyBy(LINQ_PTR, selectManyFn, resultSelectFn)
#define FIRST_OR_DEFAULT(predicateFn)                   FirstOrDefault(LINQ_PTR, predicateFn)
#define LAST_OR_DEFAULT(predicateFn)                    LastOrDefault(LINQ_PTR, predicateFn)
#define TAKE(num)                                       Take(LINQ_PTR, num)
#define TAKE_WHILE(predicateFn)                         TakeWhile(LINQ_PTR, predicateFn)
#define TAKE_WHILE_INDEXED(predicateIdxFn)              TakeWhileIndexed(LINQ_PTR, predicateIdxFn)
#define SKIP(num)                                       Skip(LINQ_PTR, num)
#define SKIP_WHILE(predicateFn)                         Skipwhile(LINQ_PTR, predicateFn)
#define SKIP_WHILE_INDEXED(predicateIdxFn)              SkipWhileIndexed(LINQ_PTR, predicateIdxFn)
#define ANY(predicateFn)                                Any(LINQ_PTR, predicateFn)
#define ALL(predicateFn)                                All(LINQ_PTR, predicateFn)
#define REVERSE()                                       Reverse(LINQ_PTR)
#define DISTINCT(equalityFn)                            Distinct(LINQ_PTR, equalityFn)
#define INDEXOF(predicateFn)                            IndexOf(LINQ_PTR, predicateFn)
#define CONCAT(otherLinq)                               Concat(LINQ_PTR, otherLinq)
#define EXCEPT(otherLinq, equalityFn)                   Except(LINQ_PTR, otherLinq, equalityFn)
#define UNION(otherLinq, equalityFn)                    Union(LINQ_PTR, otherLinq, equalityFn)
#define INTERSECT(otherLinq, equalityFn)                Intersect(LINQ_PTR, otherLinq, equalityFn)
#define ORDERBY(compareFn)                              OrderBy(LINQ_PTR, compareFn)
#define ORDERBY_DESC(compareFn)                         OrderByDesc(LINQ_PTR, compareFn)
#define GROUPBY(keySelectFn, keyEqualityFn, elementSelectFn) GroupBy(LINQ_PTR, keySelectFn, keyEqualityFn, elementSelectFn)
#define ZIP(otherLinq, resultSelectFn)                  Zip(LINQ_PTR, otherLinq, resultSelectFn)

#define SUM_INTS()                                      SumInts(LINQ_PTR)
#define SUM_FLOATS()                                    SumFloats(LINQ_PTR)

#define MAX_INTS()                                      MaxInts(LINQ_PTR)
#define MAX_FLOATS()                                    MaxFloats(LINQ_PTR)

#define MIN_INTS()                                      MinInts(LINQ_PTR)
#define MIN_FLOATS()                                    MinFloats(LINQ_PTR)

#define AVERAGE_INTS()                                  AverageInts(LINQ_PTR)
#define AVERAGE_FLOATS()                                AverageFloats(LINQ_PTR)

#define MAX(compareFn)                                  Max(LINQ_PTR, compareFn)
#define MIN(compareFn)                                  Min(LINQ_PTR, compareFn)

#define SINGLE()                                        Single(LINQ_PTR)
#define SINGLE_With(predicateFn)                        SingleWith(LINQ_PTR, predicateFn)

#define CONTAINS(value, equalityFn)                     Contains(LINQ_PTR, value, equalityFn)
#define COUNT()                                         Count(LINQ_PTR)
#define COUNT_With(predicateFn)                         CountWith(LINQ_PTR, predicateFn)

#define JOIN(innerLq, keyEqualityFn, outerKeySelectFn, innerKeySelectFn, resultSelectFn)  \
    Join(LINQ_PTR, innerLq, keyEqualityFn, outerKeySelectFn, innerKeySelectFn, resultSelectFn)

#define GROUP_JOIN(innerLq, keyEqualityFn, outerKeySelectFn, innerKeySelectFn, resultSelectFn)  \
    GroupJoin(LINQ_PTR, innerLq, keyEqualityFn, outerKeySelectFn, innerKeySelectFn, resultSelectFn)

#define AGGREGATE(accFn)                                   Aggregate(LINQ_PTR, accFn)
#define AGGREGATE_WITH_SEED(seed, accFn)                   AggregateWithSeed(LINQ_PTR, seed, accFn)
#define AGGREGATE_WITH_SEEDBY(seed, accFn, resultSelectFn) AggregateWithSeedBy(LINQ_PTR, seed, accFn, resultSelectFn)

#define SEQUENCE_EQUAL(otherLinq, equalityFn)           SequenceEqual(LINQ_PTR, otherLinq, equalityFn)

#define PREPEND(value)                                  Prepend(LINQ_PTR, value)
#define APPEND(value)                                   Append(LINQ_PTR, value)

#define TO_ARRAY()                                      ToArray(LINQ_PTR)

/* Used to store `GroupBy` result. */
typedef struct Group {
    void *Key;
    ArrayList Array;
}Group;

struct tagLinq {
    void *container;

    Linq *(*Where)(Linq *lq, Predicate predicateFn);
    Linq *(*WhereIndexed)(Linq *lq, PredicateIdx predicateIdxFn);
    Linq *(*Select)(Linq *lq, Selector selectFn);
    Linq *(*SelectIndexed)(Linq *lq, SelectorIdx selectIdxFn);
    Linq *(*SelectMany)(Linq *lq, SelectorMany selectManyFn);
    Linq *(*SelectManyIndexed)(Linq *lq, SelectorManyIdx selectManyIdxFn);
    Linq *(*SelectManyBy)(Linq *lq, SelectorMany selectManyFn, ResultSelector resultSelectFn);
    void *(*FirstOrDefault)(Linq *lq, Predicate predicateFn);
    void *(*LastOrDefault)(Linq *lq, Predicate predicateFn);

    Linq *(*Take)(Linq *lq, int number);
    Linq *(*TakeWhile)(Linq *lq, Predicate predicateFn);
    Linq *(*TakeWhileIndexed)(Linq *lq, PredicateIdx predicateIdxFn);
    Linq *(*Skip)(Linq *lq, int number);
    Linq *(*SkipWhile)(Linq *lq, Predicate predicateFn);
    Linq *(*SkipWhileIndexed)(Linq *lq, PredicateIdx predicateIdxFn);
    bool (*Any)(Linq *lq, Predicate predicateFn);
    bool (*All)(Linq *lq, Predicate predicateFn);
    Linq *(*Reverse)(Linq *lq);
    void *(*Distinct)(Linq *lq, Equality equalityFn);
    int (*IndexOf)(Linq *lq, Predicate predicateFn);
    Linq *(*Concat)(Linq *lq, Linq *otherLinq);
    Linq *(*Except)(Linq *lq, Linq *otherLinq, Equality equalityFn);
    Linq *(*Union)(Linq *lq, Linq *otherLinq, Equality equalityFn);
    Linq *(*Intersect)(Linq *lq, Linq *otherLinq, Equality equalityFn);

    int (*SumInts)(Linq *lq);
    float (*SumFloats)(Linq *lq);

    int (*MaxInts)(Linq *lq);
    float (*MaxFloats)(Linq *lq);

    int (*MinInts)(Linq *lq);
    float (*MinFloats)(Linq *lq);

    float (*AverageInts)(Linq *lq);
    float (*AverageFloats)(Linq *lq);

    void *(*Max)(Linq *lq, Comparer compareFn);
    void *(*Min)(Linq *lq, Comparer compareFn);

    Linq *(*OrderBy)(Linq *lq, Comparer compareFn);
    Linq *(*OrderByDesc)(Linq *lq, Comparer compareFn);

    Linq *(*GroupBy)(Linq *lq, Selector keySelectFn, Equality keyEqualityFn, Selector elementSelectFn);
    Linq *(*Zip)(Linq *lq, Linq *otherLinq, ResultSelector resultSelectFn);

    void *(*Single)(Linq *lq);
    void *(*SingleWith)(Linq *lq, Predicate predicateFn);

    bool (*Contains)(Linq *lq, void *value, Equality equalityFn);
    int (*Count)(Linq *lq);
    int (*CountWith)(Linq *lq, Predicate predicateFn);

    Linq *(*Join)(Linq *lq,
                  Linq *inner,
                  Equality keyEqualityFn,
                  Selector outerKeySelectFn,
                  Selector innerKeySelectFn,
                  ResultSelector resultSelectFn);
    Linq *(*GroupJoin)(Linq *lq,
                       Linq *inner,
                       Equality keyEqualityFn,
                       Selector outerKeySelectFn,
                       Selector innerKeySelectFn,
                       ResultSelector resultSelectFn);

    void *(*Aggregate)(Linq *lq, Accumulator accFn);
    void *(*AggregateWithSeed)(Linq *lq, void *seed, Accumulator accFn);
    void *(*AggregateWithSeedBy)(Linq *lq, void *seed, Accumulator accFn, Selector selectFn);

    bool (*SequenceEqual)(Linq *lq, Linq *otherLinq, Equality equalityFn);

    Linq *(*Prepend)(Linq *lq, void *item);
    Linq *(*Append)(Linq *lq, void *item);

    ArrayList (*ToArray)(Linq *lq);
};

/* Linq Generation */
extern Linq *From(void *container);
extern Linq *Range(int start, int count);
extern Linq *Repeat(void *item, int itemSize, int count);
extern Linq *Empty();

#endif
