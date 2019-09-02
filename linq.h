#ifndef LINQ_H
#define LINQ_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "ArrayList.h"

/* Used in 'Matches' function */
#ifndef MAX_MATCHES
    #define MAX_MATCHES 50
#endif

#define lambda(ret, body) ({ ret __fn__ body __fn__; })

/* Convert (void *) to an int */
#define TOINT(item)   (*(int *)(item))
/* Convert (void *) to a float */
#define TOFLOAT(item) (*(float *)(item))

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

typedef void *(*Accumulator)(void *, void *);

typedef void (*ForEachAction)(int, void *);

typedef char *(*Stringizor)(int, void *);

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
#define TAKE_EVERY(step)                                TakeEvery(LINQ_PTR, step)
#define TAKE_WHILE(predicateFn)                         TakeWhile(LINQ_PTR, predicateFn)
#define TAKE_WHILE_INDEXED(predicateIdxFn)              TakeWhileIndexed(LINQ_PTR, predicateIdxFn)
#define TAKE_EXCEPT_LAST(count)                         TakeExceptLast(LINQ_PTR, count)
#define TAKE_FROM_LAST(count)                           TakeFromLast(LINQ_PTR, count)
#define SKIP(num)                                       Skip(LINQ_PTR, num)
#define SKIP_WHILE(predicateFn)                         Skipwhile(LINQ_PTR, predicateFn)
#define SKIP_WHILE_INDEXED(predicateIdxFn)              SkipWhileIndexed(LINQ_PTR, predicateIdxFn)
#define ANY(predicateFn)                                Any(LINQ_PTR, predicateFn)
#define ALL(predicateFn)                                All(LINQ_PTR, predicateFn)
#define REVERSE()                                       Reverse(LINQ_PTR)
#define DISTINCT(equalityFn)                            Distinct(LINQ_PTR, equalityFn)
#define INDEXOF(predicateFn)                            IndexOf(LINQ_PTR, predicateFn)
#define LAST_INDEXOF(predicateFn)                       LastIndexOf(LINQ_PTR, predicateFn)
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

#define JOIN_STR(separator)                             JoinStr(LINQ_PTR, separator)
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

#define FOREACH(action)                                 ForEach(LINQ_PTR, action)

#define ALTERNATE_BEFORE(value)                         AlternateBefore(LINQ_PTR, value)
#define ALTERNATE_AFTER(value)                          AlternateAfter(LINQ_PTR, value)

#define SHUFFLE()                                       Shuffle(LINQ_PTR)
#define SLICE(startIndex, count)                        Slice(LINQ_PTR, startIndex, count)

#define PAD(width, pad_value)                           Pad(LINQ_PTR, width, pad_value)
#define PAD_BY(width, selectFn)                         PadBy(LINQ_PTR, width, selectFn)

#define PRINT(separator, stringizorFn)                  Print(LINQ_PTR, separator, stringizorFn)
#define PRINTLN(stringizorFn)                           Println(LINQ_PTR, stringizorFn)

#define TO_STRING(separator, stringizorFn)              ToString(LINQ_PTR, separator, stringizorFn)
#define TO_ARRAY()                                      ToArray(LINQ_PTR)

/* Used to store `GroupBy` result. */
typedef struct Group {
    void *Key;
    ArrayList Array;
}Group;

struct tagLinq {
    void *container;

    /* Filters a sequence of values based on a predicate. */
    Linq *(*Where)(Linq *lq, Predicate predicateFn);
    Linq *(*WhereIndexed)(Linq *lq, PredicateIdx predicateIdxFn);

    /* Projects each element of a sequence into a new form. */
    Linq *(*Select)(Linq *lq, Selector selectFn);
    Linq *(*SelectIndexed)(Linq *lq, SelectorIdx selectIdxFn);
    /* Projects each element of a sequence and flattens the resulting sequences into one sequence. */
    Linq *(*SelectMany)(Linq *lq, SelectorMany selectManyFn);
    Linq *(*SelectManyIndexed)(Linq *lq, SelectorManyIdx selectManyIdxFn);
    Linq *(*SelectManyBy)(Linq *lq, SelectorMany selectManyFn, ResultSelector resultSelectFn);

    /* Returns the first element of the sequence that satisfies a condition or a default value if no such element is found. */
    void *(*FirstOrDefault)(Linq *lq, Predicate predicateFn);

    /* Returns the last element of a sequence that satisfies a condition or a default value if no such element is found. */
    void *(*LastOrDefault)(Linq *lq, Predicate predicateFn);

    Linq *(*Take)(Linq *lq, int number);
    Linq *(*TakeEvery)(Linq *lq, int step);
    Linq *(*TakeWhile)(Linq *lq, Predicate predicateFn);
    Linq *(*TakeWhileIndexed)(Linq *lq, PredicateIdx predicateIdxFn);
    Linq *(*TakeExceptLast)(Linq *lq, int count);
    Linq *(*TakeFromLast)(Linq *lq, int count);

    /* Bypasses a specified number of elements in a sequence and then returns the remaining elements. */
    Linq *(*Skip)(Linq *lq, int number);
    /* Bypasses elements in a sequence as long as a specified condition is true and then returns the remaining elements. */
    Linq *(*SkipWhile)(Linq *lq, Predicate predicateFn);
    /* Bypasses elements in a sequence as long as a specified condition is true and then returns the remaining elements with element index. */
    Linq *(*SkipWhileIndexed)(Linq *lq, PredicateIdx predicateIdxFn);

    /* Determines whether a sequence contains any elements using predicate. */
    bool (*Any)(Linq *lq, Predicate predicateFn);

    /* Determines whether all elements of a sequence satisfy a condition. */
    bool (*All)(Linq *lq, Predicate predicateFn);

    /* Inverts the order of the elements in a sequence. */
    Linq *(*Reverse)(Linq *lq);

    /* Returns distinct elements from a sequence by using the equality function.*/
    void *(*Distinct)(Linq *lq, Equality equalityFn);

    /* Returns the zero-based index of the first occurrence within a sequence. */
    int (*IndexOf)(Linq *lq, Predicate predicateFn);
    /* Returns the last index of the first occurrence within a sequence. */
    int (*LastIndexOf)(Linq *lq, Predicate predicateFn);

    /* Concatenates two sequences. */
    Linq *(*Concat)(Linq *lq, Linq *otherLinq);

    /* Produces the set difference of two sequences by using the equality function to compare values. */
    Linq *(*Except)(Linq *lq, Linq *otherLinq, Equality equalityFn);

    /* Produces the set union of two sequences by using a specified equality function. */
    Linq *(*Union)(Linq *lq, Linq *otherLinq, Equality equalityFn);

    /* Produces the set intersection of two sequences by using the equality function to compare values. */
    Linq *(*Intersect)(Linq *lq, Linq *otherLinq, Equality equalityFn);

    /* Computes the sum of a sequence.*/
    int (*SumInts)(Linq *lq);
    float (*SumFloats)(Linq *lq);

    /* Computes the maximum number of a sequence. */
    int (*MaxInts)(Linq *lq);
    float (*MaxFloats)(Linq *lq);

    /* Computes the minimum number of a sequence. */
    int (*MinInts)(Linq *lq);
    float (*MinFloats)(Linq *lq);

    /* Computes the average of a sequence of number(integer/float) values. */
    float (*AverageInts)(Linq *lq);
    float (*AverageFloats)(Linq *lq);

    /* Returns the maximum/Minimum value in a sequence of Decimal values. */
    void *(*Max)(Linq *lq, Comparer compareFn);
    void *(*Min)(Linq *lq, Comparer compareFn);

    /* Sorts the elements of a sequence in ascending/descending order according to a key. */
    Linq *(*OrderBy)(Linq *lq, Comparer compareFn);
    Linq *(*OrderByDesc)(Linq *lq, Comparer compareFn);

    /* Groups the elements of a sequence according to a specified key selector function. */
    Linq *(*GroupBy)(Linq *lq, Selector keySelectFn, Equality keyEqualityFn, Selector elementSelectFn);

    /* Applies a specified function to the corresponding elements of two sequences, producing a sequence of the results. */
    Linq *(*Zip)(Linq *lq, Linq *otherLinq, ResultSelector resultSelectFn);

    /* Returns the only element of a sequence. */
    void *(*Single)(Linq *lq);
    /* Returns the only element of a sequence that satisfies a specified condition. */
    void *(*SingleWith)(Linq *lq, Predicate predicateFn);

    /* Determines whether a sequence contains a specified `value` by using the equality function. */
    bool (*Contains)(Linq *lq, void *value, Equality equalityFn);

    /* Returns the number of elements in a sequence, optionally using predicate function. */
    int (*Count)(Linq *lq);
    int (*CountWith)(Linq *lq, Predicate predicateFn);

    /* Join string using separator */
    char *(*JoinStr)(Linq *lq, char *separator);
    /* Correlates the elements of two sequences based on matching keys. The equality function is used to compare keys. */
    Linq *(*Join)(Linq *lq,
                  Linq *inner,
                  Equality keyEqualityFn,
                  Selector outerKeySelectFn,
                  Selector innerKeySelectFn,
                  ResultSelector resultSelectFn);

    /* Correlates the elements of two sequences based on equality of keys and groups the results. */
    Linq *(*GroupJoin)(Linq *lq,
                       Linq *inner,
                       Equality keyEqualityFn,
                       Selector outerKeySelectFn,
                       Selector innerKeySelectFn,
                       ResultSelector resultSelectFn);

    /* Applies an accumulator function over a sequence. */
    void *(*Aggregate)(Linq *lq, Accumulator accFn);
    void *(*AggregateWithSeed)(Linq *lq, void *seed, Accumulator accFn);
    void *(*AggregateWithSeedBy)(Linq *lq, void *seed, Accumulator accFn, Selector selectFn);

    /* Determines whether two sequences are equal by comparing the elements by using the equality function.*/
    bool (*SequenceEqual)(Linq *lq, Linq *otherLinq, Equality equalityFn);

    /* Prepend/Append an item to a sequence. */
    Linq *(*Prepend)(Linq *lq, void *item);
    Linq *(*Append)(Linq *lq, void *item);

    /* Iterate a sequence. */
    void (*ForEach)(Linq *lq, ForEachAction action);

    Linq *(*AlternateBefore)(Linq *lq, void *value);
    Linq *(*AlternateAfter)(Linq *lq, void *value);

    /* Inline shuffle */
    Linq *(*Shuffle)(Linq *lq);

    /* Extracts a contiguous count of elements from a sequence at a particular zero-based starting index */
    Linq *(*Slice)(Linq *lq, int startIndex, int count);

    /* Pads a sequence with a given filler value if it is narrower (shorter in length) than a given width. */
    Linq *(*Pad)(Linq *lq, int width, void *pad_value);
    Linq *(*PadBy)(Linq *lq, int width, Selector selectFn);

    /* Print the sequence using separator */
    void (*Print)(Linq *lq, char *separator, Stringizor stringizorFn);
    /* Print each sequence item with a new line */
    void (*Println)(Linq *lq, Stringizor stringizorFn);

    /* convert the sequence into a string. */
    char *(*ToString)(Linq *lq, char *separator, Stringizor stringizorFn);

    /* Returns the containing array of the linq sequence. */
    ArrayList (*ToArray)(Linq *lq);
};

/* Linq Generation */
extern Linq *From(void *container);

/* Generates a sequence of integral numbers within a specified range. */
extern Linq *Range(int start, int count);
extern Linq *RangeTo(int start, int to);
extern Linq *RangeWithStep(int start, int count, int step);
extern Linq *RangeDown(int start, int count);
extern Linq *RangeDownWithStep(int start, int count, int step);

/* Generates a sequence that contains one repeated value.*/
extern Linq *Repeat(void *item, int itemSize, int count);

/* Generates an empty sequence.*/
extern Linq *Empty();

/* Cycle a sequence 'count' times */
extern Linq *Cycle(void *container, int count);

/* Generate a sequence that matches the 'pattern' from source 'input'. */
extern Linq *Matches(bool ignoreCase, char *input, char *pattern);

/** Other utility function **/
extern void *newInt(int value);
extern void *newFloat(float value);
extern void *newStr(char *fmt, ...);

#endif

