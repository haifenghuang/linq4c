#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <regex.h>
#include <time.h>
#include "linq.h"
#include "hashmap.h"
#include "malloc.h"

static Linq *linq_where(Linq *lq, Predicate predicateFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return lq;
    }

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);

        if (predicateFn(item)) {
            arrlist_append(result, item);
        }
    }

    lq->container = result;
    return lq;
}

static Linq *linq_whereIndexed(Linq *lq, PredicateIdx predicateIdxFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return lq;
    }

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);

        if (predicateIdxFn(i, item)) {
            arrlist_append(result, item);
        }
    }

    lq->container = result;
    return lq;
}

static Linq *linq_select(Linq *lq, Selector selectFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return lq;
    }

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);

        arrlist_append(result, selectFn(item));
    }

    lq->container = result;
    return lq;
}

static Linq *linq_selectIndexed(Linq *lq, SelectorIdx selectIdxFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return lq;
    }

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);

        arrlist_append(result, selectIdxFn(i, item));
    }

    lq->container = result;
    return lq;
}

static Linq *linq_selectMany(Linq *lq, SelectorMany selectManyFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);

        Linq *newLq = selectManyFn(item);
        ArrayList newArr = (ArrayList)newLq->container;
        int newLen = arrlist_size(newArr);
        for (int j = 0; j < newLen; j++) {
            void *newItem = arrlist_get(newArr, j);
            arrlist_append(result, newItem);
        }
    }

    lq->container = result;
    return lq;
}

static Linq *linq_selectManyIndexed(Linq *lq, SelectorManyIdx selectManyIdxFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);

        Linq *newLq = selectManyIdxFn(i, item);
        ArrayList newArr = (ArrayList)newLq->container;
        int newLen = arrlist_size(newArr);
        for (int j = 0; j < newLen; j++) {
            void *newItem = arrlist_get(newArr, j);
            arrlist_append(result, newItem);
        }
    }

    lq->container = result;
    return lq;
}

static Linq *linq_selectManyBy(Linq *lq, SelectorMany selectManyFn, ResultSelector resultSelectFn){
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);

        Linq *newLq = selectManyFn(item);
        ArrayList newArr = (ArrayList)newLq->container;
        int newLen = arrlist_size(newArr);
        for (int j = 0; j < newLen; j++) {
            void *newItem = arrlist_get(newArr, j);
            arrlist_append(result, resultSelectFn(item, newItem));
        }
    }

    lq->container = result;
    return lq;
}

static void *linq_firstOrDefault(Linq *lq, Predicate predicateFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (predicateFn == NULL) {
        if (length > 0) {
            return arrlist_get(arr, 0);
        }
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);

        if (predicateFn(item)) {
            return item;
        }
    }

    return NULL;
}

static void *linq_lastOrDefault(Linq *lq, Predicate predicateFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (predicateFn == NULL) {
        if (length > 0) {
            return arrlist_get(arr, length - 1);
        }
        return NULL;
    }

    for (int i = length - 1; i >= 0; i--) {
        void *item = arrlist_get(arr, i);

        if (predicateFn(item)) {
            return item;
        }
    }

    return NULL;
}

static Linq *linq_take(Linq *lq, int number) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0 || number > length) {
        return lq;
    }

    ArrayList result = arrlist_new();

    for (int i = 0; i < number; i++) {
        void *item = arrlist_get(arr, i);
        arrlist_append(result, item);
    }

    lq->container = result;
    return lq;
}

static Linq *linq_takeWhile(Linq *lq, Predicate predicateFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return lq;
    }

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);

        if (predicateFn(item)) {
            arrlist_append(result, item);
        }
    }

    lq->container = result;
    return lq;
}

static Linq *linq_takeWhileIndexed(Linq *lq, PredicateIdx predicateIdxFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return lq;
    }

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);

        if (predicateIdxFn(i, item)) {
            arrlist_append(result, item);
        }
    }

    lq->container = result;
    return lq;
}

static Linq *linq_takeExceptLast(Linq *lq, int count) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0 || count <= 0) {
        return lq;
    }

    ArrayList result = arrlist_new();
    if (count >= length) {
        lq->container = result;
        return lq;
    }

    for (int i = 0; i < (length - count); i++) {
        void *item = arrlist_get(arr, i);
        arrlist_append(result, item);
    }

    lq->container = result;
    return lq;

}

static Linq *linq_takeFromLast(Linq *lq, int count) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList result = arrlist_new();
    if (length == 0 || count <= 0) {
        lq->container = result;
        return lq;
    }

    if (count >= length) {
        return lq;
    }

    for (int i = length - count; i < length; i++) {
        void *item = arrlist_get(arr, i);
        arrlist_append(result, item);
    }

    lq->container = result;
    return lq;
}

static Linq *linq_skip(Linq *lq, int number) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return lq;
    }

    ArrayList result = arrlist_new();

    for (int i = number; i < length; i++) {
        void *item = arrlist_get(arr, i);
        arrlist_append(result, item);
    }

    lq->container = result;
    return lq;
}

static Linq *linq_skipWhile(Linq *lq, Predicate predicateFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return lq;
    }

    bool isSkipped = false;

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        if (!isSkipped) {
            isSkipped = !predicateFn(item);
        }

        if (isSkipped) {
            arrlist_append(result, item);
        }
    }

    lq->container = result;
    return lq;
}

static Linq *linq_skipWhileIndexed(Linq *lq, PredicateIdx predicateIdxFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return lq;
    }

    bool isSkipped = false;

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        if (!isSkipped) {
            isSkipped = !predicateIdxFn(i, item);
        }

        if (isSkipped) {
            arrlist_append(result, item);
        }
    }

    lq->container = result;
    return lq;
}

static bool linq_any(Linq *lq, Predicate predicateFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return false;
    }
    if (predicateFn == NULL) {
        return true;
    }

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        if (predicateFn(item)) {
            return true;
        }
    }

    return false;

}

static bool linq_all(Linq *lq, Predicate predicateFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return false;
    }

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        if (!predicateFn(item)) {
            return false;
        }
    }
    return true;
}

static Linq *linq_reverse(Linq *lq) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList result = arrlist_new();

    for(int i = length - 1; i >= 0; i--) {
        void *item = arrlist_get(arr, i);
        arrlist_append(result, item);
    }

    lq->container = result;
    return lq;
}

static Linq *linq_distinct(Linq *lq, Equality equalityFn) {
    ArrayList arr = (ArrayList)lq->container;

    int length = arrlist_size(arr);
    if (length == 0) {
        return lq;
    }

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);

        bool found = false;
        int resultLen = arrlist_size(result);
        for (int j = 0; j < resultLen; j++) {
            void *result_item = arrlist_get(result, j);
            if (equalityFn(result_item, item)) {
                found = true;
                break;
            }
        }

        if (found) continue;
        arrlist_append(result, item);
    }

    lq->container = result;
    return lq;
}

static int linq_indexOf(Linq *lq, Predicate predicateFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        if (predicateFn(item)) {
            return i;
        }
    }
    return -1;
}

static int linq_lastIndexOf(Linq *lq, Predicate predicateFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    for (int i = length -1; i >= 0; i--) {
        void *item = arrlist_get(arr, i);
        if (predicateFn(item)) {
            return i;
        }
    }
    return -1;
}

static Linq *linq_concat(Linq *lq, Linq *otherLinq) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        arrlist_append(result, item);
    }

    if (otherLinq != NULL) {
        ArrayList otherArr = (ArrayList)otherLinq->container;
        int otherLen = arrlist_size(otherArr);

        for (int i = 0; i < otherLen; i++) {
            void *otherItem = arrlist_get(otherArr, i);
            arrlist_append(result, otherItem);
        }
    }

    lq->container = result;
    return lq;
}

static Linq *linq_except(Linq *lq, Linq *otherLinq, Equality equalityFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList other = (ArrayList)otherLinq->container;
    int other_length = arrlist_size(other);

    ArrayList result = arrlist_new();

    int found = 0;
    for (int i = 0; i < length; i++) {
        found == 0;
        void *item = arrlist_get(arr, i);

        for (int j = 0; j < other_length; j++) {
            void *otherItem = arrlist_get(other, j);
            if (equalityFn(item, otherItem)) { /* equal */
                found = 1;
                break;
            }
        }

        if (!found) {
            arrlist_append(result, item);
        }
    }

    lq->container = result;
    return lq;
}

static Linq *linq_union(Linq *lq, Linq *otherLinq, Equality equalityFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList other = (ArrayList)otherLinq->container;
    int other_length = arrlist_size(other);

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        arrlist_append(result, item);
    }

    int found = 0;
    for (int i = 0; i < other_length; i++) {
        found = 0;
        void *otherItem = arrlist_get(other, i);

        for (int j = 0; j < length; j++) {
            void *item = arrlist_get(arr, j);
            if (equalityFn(item, otherItem)) { /* equal */
                found = 1;
                break;
            }
        }

        if (found == 0) {
            arrlist_append(result, otherItem);
        }
    } /* end for */

    lq->container = result;
    return lq;
}

static Linq *linq_intersect(Linq *lq, Linq *otherLinq, Equality equalityFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList other = (ArrayList)otherLinq->container;
    int other_length = arrlist_size(other);

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);

        for (int j = 0; j < other_length; j++) {
            void *otherItem = arrlist_get(other, j);
            if (equalityFn(item, otherItem)) { /* equal */
                arrlist_append(result, item);
                break;
            }
        }
    } /* end for */

    lq->container = result;
    return lq;
}

static int linq_sumInts(Linq *lq) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return 0;
    }

    int sum = 0;
    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        int tmp = *(int *)item;
        sum += tmp;
    }

    return sum;
}

static float linq_sumFloats(Linq *lq) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return 0.0;
    }

    float sum = 0.0;
    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        float tmp = *(float *)item;
        sum += tmp;
    }

    return sum;
}

static int linq_maxInts(Linq *lq) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return 0;
    }

    int max = *(int *)arrlist_get(arr, 0);
    if (length == 1) {
        return max;
    }

    for (int i = 1; i < length; i++) {
        int tmp = *(int *)arrlist_get(arr, i);
        if (tmp > max) {
            max = tmp;
        }
    }

    return max;
}

static float linq_maxFloats(Linq *lq) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return 0.0;
    }

    float max = *(float *)arrlist_get(arr, 0);
    if (length == 1) {
        return max;
    }

    for (int i = 1; i < length; i++) {
        float tmp = *(float *)arrlist_get(arr, i);
        if (tmp > max) {
            max = tmp;
        }
    }

    return max;
}

static int linq_minInts(Linq *lq) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return 0;
    }

    int min = *(int *)arrlist_get(arr, 0);
    if (length == 1) {
        return min;
    }

    for (int i = 1; i < length; i++) {
        int tmp = *(int *)arrlist_get(arr, i);
        if (tmp < min) {
            min = tmp;
        }
    }

    return min;
}

static float linq_minFloats(Linq *lq) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return 0.0;
    }

    float min = *(float *)arrlist_get(arr, 0);
    if (length == 1) {
        return min;
    }

    for (int i = 1; i < length; i++) {
        float tmp = *(float *)arrlist_get(arr, i);
        if (tmp < min) {
            min = tmp;
        }
    }

    return min;
}

static float linq_averageInts(Linq *lq) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return 0;
    }

    float sum = 0;
    for (int i = 0; i < length; i++) {
        int tmp = *(int *)arrlist_get(arr, i);
        sum += tmp;
    }

    return sum / length;
}

static float linq_averageFloats(Linq *lq) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return 0.0;
    }

    float sum = 0.0;
    for (int i = 0; i < length; i++) {
        float tmp = *(float *)arrlist_get(arr, i);
        sum += tmp;
    }

    return sum / length;
}

static void *linq_max(Linq *lq, Comparer compareFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return NULL;
    }

    void *max = arrlist_get(arr, 0);

    for (int i = 1; i < length; i++) {
        void *item = arrlist_get(arr, i);
        if (compareFn(item, max) > 0) {
            max = item;
        }
    }

    return max;
}

static void *linq_min(Linq *lq, Comparer compareFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return NULL;
    }

    void *min = arrlist_get(arr, 0);

    for (int i = 1; i < length; i++) {
        void *item = arrlist_get(arr, i);
        if (compareFn(item, min) < 0) {
            min = item;
        }
    }

    return min;
}

static Linq *linq_orderby(Linq *lq, Comparer compareFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return lq;
    }

    arrlist_sort(arr, compareFn, 0);

    return lq;
}

static Linq *linq_orderbyDesc(Linq *lq, Comparer compareFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return lq;
    }

    arrlist_sort(arr, compareFn, 1);

    return lq;
}

static Linq *linq_groupby(Linq *lq, Selector keySelectFn, Equality keyEqualityFn, Selector elementSelectFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return lq;
    }

    HashMap set = hashmap_new();
    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);

        void *key = keySelectFn(item);
        void *val = elementSelectFn(item);

        bool found = false;
        int idx = 0;
        size_t setLen = hashmap_size(set);
        for (int j = 0; j < setLen; j++) {
            KeyValuePair *kv = hashmap_get(set, j);
            if (keyEqualityFn(kv->key, key)) { //key equal
                found = true;
                idx = j;
                break;
            }
        } /* end for */

        if (!found) {
            KeyValuePair *kv = gc_malloc(sizeof(KeyValuePair));
            kv->key = key;
            ArrayList valueArr = arrlist_new();
            arrlist_append(valueArr, val);
            kv->value = valueArr;
            hashmap_add(set, kv);
        } else {
            KeyValuePair *kv = hashmap_get(set, idx);
            ArrayList valueArr = (ArrayList)kv->value;
            arrlist_append(valueArr, val);
        }
    } /* end for */

    ArrayList result = arrlist_new();

    size_t setLen = hashmap_size(set);
    for (int j = 0; j < setLen; j++) {
        KeyValuePair *kv = hashmap_get(set, j);
        Group *gp = gc_malloc(sizeof(Group));
        gp->Key   = kv->key;
        gp->Array = kv->value;
        arrlist_append(result, gp);
    }

    lq->container = result;
    return lq;
}

static Linq *linq_zip(Linq *lq, Linq *otherLinq, ResultSelector resultSelectFn) {
    if (otherLinq == NULL) {
        return lq;
    }

    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList other = (ArrayList)otherLinq->container;
    int other_length = arrlist_size(other);

    int minLen = (length > other_length) ? other_length : length;

    ArrayList result = arrlist_new();

    for (int i = 0; i < minLen; i++) {
        void *item = arrlist_get(arr, i);
        void *other_item = arrlist_get(other, i);

        arrlist_append(result, resultSelectFn(item, other_item));
    }

    lq->container = result;
    return lq;
}

static void *linq_single(Linq *lq) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length > 1) {
        return NULL;
    }

    return arrlist_get(arr, 0);
}

static void *linq_singleWith(Linq *lq, Predicate predicateFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    void *result = NULL;
    bool found = false;
    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        if (predicateFn(item)) {
            if (found) {
                return NULL;
            }
            found = true;
            result = item;
        }
    }

    return result;
}

static bool linq_contains(Linq *lq, void *value, Equality equalityFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        if (equalityFn(item, value)) { /* equal */
            return true;
        }
    }

    return false;
}

static int linq_count(Linq *lq) {
    ArrayList arr = (ArrayList)lq->container;
    return arrlist_size(arr);
}

static int linq_countWith(Linq *lq, Predicate predicateFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    int result = 0;
    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        if (predicateFn(item)) {
            result++;
        }
    }

    return result;
}

static Linq *linq_join(Linq *lq,
                       Linq *inner, 
                       Equality keyEqualityFn, 
                       Selector outerKeySelectFn, 
                       Selector innerKeySelectFn,
                       ResultSelector resultSelectFn) {
    ArrayList outerArr= (ArrayList)lq->container;
    int outerLen = arrlist_size(outerArr);

    ArrayList innerArr= (ArrayList)inner->container;
    int innerLen = arrlist_size(innerArr);

    HashMap set = hashmap_new();
    for (int i = 0; i < innerLen; i++) {
        void *innerItem = arrlist_get(innerArr, i);

        void *innerKey = innerKeySelectFn(innerItem);
        if (innerKey == NULL) {
            continue;
        }

        bool found = false;
        int idx = 0;
        size_t setLen = hashmap_size(set);
        for (int j = 0; j < setLen; j++) {
            KeyValuePair *kv = hashmap_get(set, j);
            if (keyEqualityFn(kv->key, innerKey)) { //key equal
                found = true;
                idx = j;
                break;
            }
        } /* end for */

        if (!found) {
            KeyValuePair *kv = gc_malloc(sizeof(KeyValuePair));
            kv->key = innerKey;
            ArrayList valueArr = arrlist_new();
            arrlist_append(valueArr, innerItem);
            kv->value = valueArr;
            hashmap_add(set, kv);
        } else {
            KeyValuePair *kv = hashmap_get(set, idx);
            ArrayList valueArr = (ArrayList)kv->value;
            arrlist_append(valueArr, innerItem);
        }
    } /* end for */

    ArrayList result = arrlist_new();

    size_t setLen = hashmap_size(set);
    for (int i = 0; i < outerLen; i++) {
        void *outItem = arrlist_get(outerArr, i);
        void *outerKey = outerKeySelectFn(outItem);

        int found = 0;
        int idx = 0;
        for (int j = 0; j < setLen; j++) {
            KeyValuePair *kv = hashmap_get(set, j);
            if (keyEqualityFn(kv->key, outerKey)) { //key equal
                found = 1;
                idx = j;
                break;
            }
        }

        ArrayList innerGroup = NULL;
        if (found) {
            KeyValuePair *kv = hashmap_get(set, idx);
            innerGroup = (ArrayList)kv->value;
        } else {
            innerGroup = arrlist_new();
        }

        int groupLen = arrlist_size(innerGroup);
        for (int j = 0; j < groupLen; j++) {
            void *resultItem = resultSelectFn(outItem, arrlist_get(innerGroup, j));
            arrlist_append(result, resultItem);
        }
    } /* end for */

    lq->container = result;
    return lq;
}

static Linq *linq_groupJoin(Linq *lq,
                            Linq *inner, 
                            Equality keyEqualityFn, 
                            Selector outerKeySelectFn, 
                            Selector innerKeySelectFn,
                            ResultSelector resultSelectFn) {
    ArrayList outerArr= (ArrayList)lq->container;
    int outerLen = arrlist_size(outerArr);

    ArrayList innerArr= (ArrayList)inner->container;
    int innerLen = arrlist_size(innerArr);

    HashMap set = hashmap_new();
    for (int i = 0; i < innerLen; i++) {
        void *innerItem = arrlist_get(innerArr, i);

        void *innerKey = innerKeySelectFn(innerItem);

        bool found = false;
        int idx = 0;
        size_t setLen = hashmap_size(set);
        for (int j = 0; j < setLen; j++) {
            KeyValuePair *kv = hashmap_get(set, j);
            if (keyEqualityFn(kv->key, innerKey)) { //key equal
                found = true;
                idx = j;
                break;
            }
        } /* end for */

        if (!found) {
            KeyValuePair *kv = gc_malloc(sizeof(KeyValuePair));
            kv->key = innerKey;
            ArrayList valueArr = arrlist_new();
            arrlist_append(valueArr, innerItem);
            kv->value = valueArr;
            hashmap_add(set, kv);
        } else {
            KeyValuePair *kv = hashmap_get(set, idx);
            ArrayList valueArr = (ArrayList)kv->value;
            arrlist_append(valueArr, innerItem);
        }
    } /* end for */

    ArrayList result = arrlist_new();

    size_t setLen = hashmap_size(set);
    for (int i = 0; i < outerLen; i++) {
        void *outItem = arrlist_get(outerArr, i);

        void *outerKey = outerKeySelectFn(outItem);

        int found = 0;
        int idx = 0;
        for (int j = 0; j < setLen; j++)
        {
            KeyValuePair *kv = hashmap_get(set, j);
            if (keyEqualityFn(kv->key, outerKey)) { //key equal
                found = 1;
                idx = j;
                break;
            }
        }

        void *resultItem = NULL;
        if (found)
        {
            KeyValuePair *kv = hashmap_get(set, idx);
            resultItem = resultSelectFn(outItem, kv->value);
        } else {
            resultItem = resultSelectFn(outItem, arrlist_new());
        }
        arrlist_append(result, resultItem);
    }

    lq->container = result;
    return lq;
}

static void *linq_aggregate(Linq *lq, Accumulator accFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) return NULL;

    void *result = arrlist_get(arr, 0);
    for(int i = 1; i < length; i++) {
        void *item = arrlist_get(arr, i);
        result = accFn(result, item);
    }

    return result;
}

static void *linq_aggregateWithSeed(Linq *lq, void *seed, Accumulator accFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) return NULL;

    void *result = seed;
    for(int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        result = accFn(result, item);
    }

    return result;
}

static void *linq_aggregateWithSeedBy(Linq *lq, void *seed, Accumulator accFn, Selector selectFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) return NULL;

    void *result = seed;
    for(int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        result = accFn(result, item);
    }

    return selectFn(result);
}

static bool linq_sequenceEqual(Linq *lq, Linq *otherLinq, Equality equalityFn) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList otherArr = otherLinq->container;
    int other_length = arrlist_size(otherArr);

    if (length == 0 || other_length == 0) {
        return false;
    }

    if (length != other_length) return false;
	for(int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        void *other_item = arrlist_get(otherArr, i);

		if (!equalityFn(item, other_item)) {
			return false;
		}
	}

    return true;
}

static Linq *linq_prepend(Linq *lq, void *item) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList result = arrlist_new();

    arrlist_append(result, item);
    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        arrlist_append(result, item);
    }

    lq->container = result;
    return lq;
}

static Linq *linq_append(Linq *lq, void *item) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList result = arrlist_new();

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        arrlist_append(result, item);
    }
    arrlist_append(result, item);

    lq->container = result;
    return lq;
}

static void linq_forEach(Linq *lq, ForEachAction action) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        action(i, item);
    }
}

static Linq *linq_alternateBefore(Linq *lq, void *value) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList result = arrlist_new();
    if (length == 0) {
        arrlist_append(result, value);
        lq->container = result;
        return lq;
    }

    for (int i = 0; i < length; i++) {
        arrlist_append(result, value);
        void *item = arrlist_get(arr, i);
        arrlist_append(result, item);
    }

    lq->container = result;
    return lq;
}

static Linq *linq_alternateAfter(Linq *lq, void *value) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    ArrayList result = arrlist_new();
    if (length == 0) {
        return lq;
    }

    for (int i = 0; i < length; i++) {
        void *item = arrlist_get(arr, i);
        arrlist_append(result, item);
        arrlist_append(result, value);
    }

    lq->container = result;
    return lq;
}

static Linq *linq_shuffle(Linq *lq) {
    ArrayList arr = (ArrayList)lq->container;
    int length = arrlist_size(arr);

    if (length == 0) {
        return lq;
    }

    for (int i = length - 1; i > 0; i--) {
        static unsigned int seed = 0;

        if (seed == 0) {
            time_t t = time(NULL);
            seed = t ^ (t << 16);
        }

        seed = (seed * 58321) + 11113;
        int v = (seed >> 16);
        srand(v);
        int idx = rand() % (i + 1); // rand() % (upper - lower + 1) + lower :  lower ~ upper
        
        void *item1 = arrlist_get(arr, i);
        void *item2 = arrlist_get(arr, idx);

        //swap the two items
        arrlist_set(arr, i, item2);
        arrlist_set(arr, idx, item1);
    }

    return lq;
}

static ArrayList linq_toArray(Linq *lq) {
    return (ArrayList)lq->container;
}


Linq *From(void *container) {
    Linq *lq = gc_malloc(sizeof(Linq));
    lq->container = container;

    lq->Where               = linq_where;
    lq->WhereIndexed        = linq_whereIndexed;
    lq->Select              = linq_select;
    lq->SelectIndexed       = linq_selectIndexed;
    lq->SelectMany          = linq_selectMany;
    lq->SelectManyIndexed   = linq_selectManyIndexed;
    lq->SelectManyBy        = linq_selectManyBy;
    lq->FirstOrDefault      = linq_firstOrDefault;
    lq->LastOrDefault       = linq_lastOrDefault;
    lq->Take                = linq_take;
    lq->TakeWhile           = linq_takeWhile;
    lq->TakeWhileIndexed    = linq_takeWhileIndexed;
    lq->TakeExceptLast      = linq_takeExceptLast;
    lq->TakeFromLast        = linq_takeFromLast;
    lq->Skip                = linq_skip;
    lq->SkipWhile           = linq_skipWhile;
    lq->SkipWhileIndexed    = linq_skipWhileIndexed;
    lq->Any                 = linq_any;
    lq->All                 = linq_all;
    lq->Reverse             = linq_reverse;
    lq->IndexOf             = linq_indexOf;
    lq->LastIndexOf         = linq_lastIndexOf;
    lq->Concat              = linq_concat;
    lq->Except              = linq_except;
    lq->Union               = linq_union;
    lq->Intersect           = linq_intersect;
  
    lq->SumInts             = linq_sumInts;
    lq->SumFloats           = linq_sumFloats;
     
    lq->MaxInts             = linq_maxInts;
    lq->MaxFloats           = linq_maxFloats;
     
    lq->MinInts             = linq_minInts;
    lq->MinFloats           = linq_minFloats;
     
    lq->AverageInts         = linq_averageInts;
    lq->AverageFloats       = linq_averageFloats;
     
    lq->Max                 = linq_max;
    lq->Min                 = linq_min;
   
    lq->OrderBy             = linq_orderby;
    lq->OrderByDesc         = linq_orderbyDesc;
     
    lq->GroupBy             = linq_groupby;
    lq->Zip                 = linq_zip;
     
    lq->Single              = linq_single;
    lq->SingleWith          = linq_singleWith;
     
    lq->Contains            = linq_contains;
    lq->Count               = linq_count;
    lq->CountWith           = linq_countWith;
     
    lq->Join                = linq_join;
    lq->GroupJoin           = linq_groupJoin;

    lq->Aggregate           = linq_aggregate;
    lq->AggregateWithSeed   = linq_aggregateWithSeed;
    lq->AggregateWithSeedBy = linq_aggregateWithSeedBy;

    lq->SequenceEqual       = linq_sequenceEqual;

    lq->Prepend             = linq_prepend;
    lq->Append              = linq_append;

    lq->ForEach             = linq_forEach;

    lq->AlternateBefore     = linq_alternateBefore;
    lq->AlternateAfter      = linq_alternateAfter;

    lq->Shuffle             = linq_shuffle;

    lq->ToArray             = linq_toArray;

    return lq;
}

Linq *RangeWithStep(int start, int count, int step) {
    ArrayList result = arrlist_new();

    for (int i = 0; i < count; i++) {
        int *item = gc_malloc(sizeof(int));
        *item = start + i * step;
        arrlist_append(result, item);
    }

    return From(result);
}

Linq *Range(int start, int count) {
    return RangeWithStep(start, count, 1);
}

Linq *RangeTo(int start, int to) {
    ArrayList result = arrlist_new();
    if (start >= to) {
        for (int i = start; i >= to; i--) {
            int *item = gc_malloc(sizeof(int));
            *item = i;
            arrlist_append(result, item);
        }
    } else {
        for (int i = start; i <= to; i++) {
            int *item = gc_malloc(sizeof(int));
            *item = i;
            arrlist_append(result, item);
        }
    }

    return From(result);
}

Linq *RangeDownWithStep(int start, int count, int step) {
    ArrayList result = arrlist_new();

    for (int i = 0; i < count; i++) {
        int *item = gc_malloc(sizeof(int));
        *item = start - i * step;
        arrlist_append(result, item);
    }

    return From(result);
}

Linq *RangeDown(int start, int count) {
    return RangeDownWithStep(start, count, 1);
}

Linq *Repeat(void *item, int itemSize, int count) {
    ArrayList result = arrlist_new();

    for (int i = 0; i < count; i++) {
        void *newItem = gc_malloc(itemSize);
        memcpy(newItem, item, itemSize);
        arrlist_append(result, newItem);

    }

    return From(result);
}

Linq *Empty() {
    return From(arrlist_new());
}

Linq *Cycle(void *container, int count) {
    ArrayList arr = (ArrayList)container;
    int length = arrlist_size(arr);

    if (length == 0 || count <= 0) {
        return From(container);
    }

    ArrayList newArr = arrlist_new();
    int j = 0;
    while (j++ < count) {
        for (int i = 0; i < length; i++) {
            void *item = arrlist_get(arr, i);
            arrlist_append(newArr, item);
        }
    }

    return From(newArr);
}

Linq *Matches(bool ignoreCase, char *input, char *pattern) {
    ArrayList arr = arrlist_new();

    int status;
    regex_t regex;
    char *pstr = input;
 
    /* REG_EXTENDED: Compile modern ("extended") REs.
     * REG_NEWLINE: Compile for newline-sensitive matching.
     * */
    int cflags = REG_EXTENDED | REG_NEWLINE;

    if (ignoreCase) {
        cflags |= REG_ICASE;
    }

    status = regcomp(&regex, pattern, cflags);
    if (status == 0) {
        int numGroups = regex.re_nsub;
        regmatch_t groups[numGroups + 1];

        for (int m = 0; m < MAX_MATCHES; m++) {
            if (regexec(&regex, pstr, numGroups + 1, groups, 0)) {
                break;
            }

            for (int g = 1; g <= numGroups; g++) {
                if (groups[g].rm_so == (size_t)-1) {
                    break; /* No more groups */
                }

                /* rm_so: regex-match, start-offset */
                /* rm_eo: regex-match, end-offset */
                int length = groups[g].rm_eo - groups[g].rm_so;
                char *match = gc_malloc(length);
                strncpy(match, pstr + groups[g].rm_so, length);
                match[length] = '\0';
                arrlist_append(arr, match);

            }

            pstr += groups[0].rm_eo; /* Restart from last match */
        } /* end for */
    }

    regfree(&regex);

    return From(arr);
}

