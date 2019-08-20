# LinQ for C

![IMG1](img/Linq4c_1.svg)
![IMG2](img/Linq4c_2.svg)

## Features

- Most of the LINQ methods(50+) are supported.
- Chaining of LINQ methods are supported.
- Lambda is supported(slow).

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
    char *str = (char *)item;
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
                     char *str = (char *)item;
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
    char *str = (char *)item;
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

## Credits

* faisalabujabal:
    For his wonderful Garbage collector for C [gc4c](https://github.com/faisalabujabal/gc4c).

## License

MIT

