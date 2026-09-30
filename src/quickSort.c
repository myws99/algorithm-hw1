/* 퀵 정렬 — 피벗을 골라 작은 값은 왼쪽, 큰 값은 오른쪽으로 나눈 뒤 양쪽을 재귀로 정렬한다.
 * 평균 O(n log n), 최악 O(n^2). 멀리 떨어진 원소끼리 교환하므로 불안정 정렬이다. */
#include "sort.h"
#include "sortctx.h"

static void quickSortRange(SortCtx *c, size_t lo, size_t hi, size_t depth) {
    /* a[lo..hi] (양끝 포함) 정렬 */
    while (lo < hi) {
        if (c->stats != NULL && depth > c->stats->maxDepth) {
            c->stats->maxDepth = depth;
        }
        /* 가운데 원소를 피벗으로 골라 맨 끝으로 보낸다 (정렬된 입력에서 최악을 피한다) */
        size_t mid = lo + (hi - lo) / 2;
        sortSwap(c, mid, hi);
        /* Lomuto 분할: 피벗보다 작은 값을 앞으로 모은다 */
        size_t store = lo;
        for (size_t i = lo; i < hi; i++) {
            if (sortCompareAt(c, i, hi) < 0) {
                if (i != store) sortSwap(c, i, store);
                store++;
            }
        }
        if (store != hi) sortSwap(c, store, hi); /* 피벗을 제자리에 */
        /* 짧은 쪽만 재귀, 긴 쪽은 반복 → 재귀 깊이 O(log n) */
        if (store - lo < hi - store) {
            if (store > lo) quickSortRange(c, lo, store - 1, depth + 1);
            lo = store + 1;
        } else {
            quickSortRange(c, store + 1, hi, depth + 1);
            if (store == 0) break;
            hi = store - 1;
        }
    }
}

void quickSort(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats) {
    SortCtx c;
    if (!sortBegin(&c, base, n, size, cmp, stats)) return;
    quickSortRange(&c, 0, n - 1, 1);
    sortEnd(&c);
}
