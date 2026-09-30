/* 힙 정렬 (수업에서 안 배운 정렬) — 배열을 최대 힙으로 만든 뒤,
 * 루트(최댓값)를 맨 뒤로 보내고 힙 크기를 줄이는 일을 반복한다.
 * 항상 O(n log n), 추가 메모리 O(1), 불안정 정렬. */
#include "sort.h"
#include "sortctx.h"

/* i번 노드를 아래로 내려 보내 a[0..n)이 다시 최대 힙이 되게 한다. */
static void siftDown(SortCtx *c, size_t i, size_t n) {
    for (;;) {
        size_t largest = i;
        size_t left = 2 * i + 1, right = 2 * i + 2;
        if (left < n && sortCompareAt(c, left, largest) > 0) largest = left;
        if (right < n && sortCompareAt(c, right, largest) > 0) largest = right;
        if (largest == i) return;
        sortSwap(c, i, largest);
        i = largest;
    }
}

void heapSort(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats) {
    SortCtx c;
    if (!sortBegin(&c, base, n, size, cmp, stats)) return;
    /* 1단계: 힙 만들기 (마지막 부모부터 거꾸로) — O(n) */
    for (size_t i = n / 2; i-- > 0;) {
        siftDown(&c, i, n);
    }
    /* 2단계: 최댓값을 뒤로 빼고 힙을 복구 — O(n log n) */
    for (size_t end = n - 1; end > 0; end--) {
        sortSwap(&c, 0, end);
        siftDown(&c, 0, end);
    }
    sortEnd(&c);
}
