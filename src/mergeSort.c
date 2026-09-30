/* 병합 정렬 — 반으로 나눠 각각 정렬하고, 정렬된 두 구간을 합친다.
 * 항상 O(n log n), 안정 정렬. 대신 합칠 때 보조 배열 O(n)이 필요하다. */
#include "sort.h"
#include "sortctx.h"
#include <stdlib.h>

static void mergeSortRange(SortCtx *c, char *buf, size_t lo, size_t hi, size_t depth) {
    /* a[lo..hi) 정렬 */
    if (c->stats != NULL && depth > c->stats->maxDepth) {
        c->stats->maxDepth = depth;
    }
    if (hi - lo < 2) return;
    size_t mid = lo + (hi - lo) / 2;
    mergeSortRange(c, buf, lo, mid, depth + 1);
    mergeSortRange(c, buf, mid, hi, depth + 1);
    /* 이미 이어져 있으면 합칠 필요 없음 */
    if (sortCompareAt(c, mid - 1, mid) <= 0) return;
    /* 왼쪽 구간을 buf로 복사해 두고 앞에서부터 채운다 */
    size_t leftLen = mid - lo;
    for (size_t k = 0; k < leftLen; k++) {
        sortMove(c, buf + k * c->size, sortElemAt(c, lo + k));
    }
    size_t i = 0, j = mid, out = lo;
    while (i < leftLen && j < hi) {
        if (c->stats != NULL) c->stats->compares++;
        /* '<='로 왼쪽을 먼저 → 같은 값의 원래 순서 유지 (안정) */
        if (c->cmp(buf + i * c->size, sortElemAt(c, j)) <= 0) {
            sortMove(c, sortElemAt(c, out++), buf + (i++) * c->size);
        } else {
            sortMove(c, sortElemAt(c, out++), sortElemAt(c, j++));
        }
    }
    while (i < leftLen) {
        sortMove(c, sortElemAt(c, out++), buf + (i++) * c->size);
    }
}

void mergeSort(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats) {
    SortCtx c;
    if (!sortBegin(&c, base, n, size, cmp, stats)) return;
    size_t bufBytes = (n / 2 + 1) * size;          /* 왼쪽 절반만 담으면 된다 */
    char *buf = (char *)malloc(bufBytes);
    if (buf != NULL) {
        if (stats != NULL) stats->extraBytes += bufBytes;
        mergeSortRange(&c, buf, 0, n, 1);
        free(buf);
    }
    sortEnd(&c);
}
