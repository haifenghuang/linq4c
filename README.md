# Linq for C

![IMG1](img/Linq4c_1.png)
![IMG2](img/Linq4c_2.png)

## Features

- Most of the LINQ methods(50+) are supported.
- Chaining of LINQ methods are supported.
- Pseudo lambda is supported(slow).

## Examples

### Example using callbacks:

```c
#include "ArrayList.h"
#include "linq.h"
#include "malloc.h"

bool WhereCallback(void *item) {
    char *str= (char *)item;
    return str[0] == 'h';
}

void *SelectCallback(void *item) {
    char *newStr = gc_malloc(64);
    sprintf(newStr, "%s_1", (char *)item);
    return newStr;
}

ArrayList testLinq(ArrayList array) {
    Linq *lq = From(array);

    ArrayList result = 
        lq
        ->Where(lq, WhereCallback)
        ->Select(lq, SelectCallback)
        ->ToArray(lq);

    return result;
}

int main() {
    gc_init();

    char *str1 = "huang", *str2 = "hai", *str3 = "feng";

    ArrayList array = arrlist_new();
    arrlist_append(array, str1);
    arrlist_append(array, str2);
    arrlist_append(array, str3);

    ArrayList result = testLinq(array);
    for(int i = 0; i < arrlist_size(result); i++) {
        printf("%s\n", arrlist_get(result, i));
    }

    gc_destroy();
    return 0;
}
```

### Example with lambdas:

```c
ArrayList testLinq(ArrayList array) {
    Linq *lq = From(array);

    ArrayList result = 
        lq
        ->Where(lq, 
                lambda(bool, (void *item) {
                    char *str= (char *)item;
                    return str[0] == 'h';
                }))
        ->Select(lq,
                 lambda(void *, (void *item) {
                     char *newStr = gc_malloc(64);
                     sprintf(newStr, "%s_1", (char *)item);
                     return newStr;
                 }))
        ->ToArray(lq);

    return result;
}

int main() {
    gc_init();

    char *str1 = "huang", *str2 = "hai", *str3 = "feng";

    ArrayList array = arrlist_new();
    arrlist_append(array, str1);
    arrlist_append(array, str2);
    arrlist_append(array, str3);

    ArrayList result = testLinq(array);
    for(int i = 0; i < arrlist_size(result); i++) {
        printf("%s\n", arrlist_get(result, i));
    }

    gc_destroy();
    return 0;
}
```

### Example with macros:

```c
#define LINQ_PTR lq

bool WhereCallback(void *item) {
    char *str= (char *)item;
    return str[0] == 'h';
}

void *SelectCallback(void *item) {
    char *newStr = gc_malloc(64);
    sprintf(newStr, "%s_1", (char *)item);
    return newStr;
}

ArrayList testLinq(ArrayList array) {
    Linq *lq = From(array);

    ArrayList result = 
        lq
        ->WHERE(WhereCallback)
        ->SELECT(SelectCallback)
        ->TO_ARRAY();

    return result;
}

int main(int argc, char **argv) {
    gc_init();

    char *str1 = "huang", *str2 = "hai", *str3 = "feng";

    ArrayList array = arrlist_new();
    arrlist_append(array, str1);
    arrlist_append(array, str2);
    arrlist_append(array, str3);

    ArrayList result = testLinq(array);
    for(int i = 0; i < arrlist_size(result); i++) {
        printf("%s\n", arrlist_get(result, i));
    }

    gc_destroy();
    return 0;
}

```

## What's implemented

-----

- [x] Where
- [x] WhereIndexed
- [x] Select
- [x] SelectIndexed
- [x] SelectMany
- [x] SelectManyIndexed
- [x] SelectManyBy
- [x] FirstOrDefault
- [x] LastOrDefault
- [x] Take
- [x] TakeWhile
- [x] TakeWhileIndexed
- [x] TakeExceptLast
- [x] TakeFromLast
- [x] Skip
- [x] SkipWhile
- [x] SkipWhileIndexed
- [x] Any
- [x] All
- [x] Reverse
- [x] IndexOf
- [x] LastIndexOf
- [x] Concat
- [x] Except
- [x] Union
- [x] Intersect
- [x] SumInts
- [x] SumFloats
- [x] MaxInts
- [x] MaxFloats
- [x] MinInts
- [x] MinFloats
- [x] AverageInts
- [x] AverageFloats
- [x] Max
- [x] Min
- [x] OrderBy
- [x] OrderByDesc
- [x] GroupBy
- [x] Zip
- [x] Single
- [x] SingleWith
- [x] Contains
- [x] Count
- [x] CountWith
- [x] Join
- [x] GroupJoin
- [x] Aggregate
- [x] AggregateWithSeed
- [x] AggregateWithSeedBy
- [x] SequenceEqual
- [x] Prepend
- [x] Append
- [x] ForEach
- [x] AlternateBefore
- [x] AlternateAfter
- [x] Range
- [x] RangeTo
- [x] RangeWithStep
- [x] RangeDown
- [x] RangeDownWithStep
- [x] Repeat
- [x] Empty
- [x] Cycle
- [x] Matches

## Credits

* faisalabujabal:
    For his wonderful Garbage collector for C [gc4c](https://github.com/faisalabujabal/gc4c).

## License

MIT

