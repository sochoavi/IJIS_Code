#ifndef INCLUDE_H
#define INCLUDE_H

#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) printf("DEBUG: " fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define mask 0x0001
#define idSize (1 << 18)
#define worikingTreeHeight 12

#define parent(X) (X % 2 == 0) ? (X / 2 - 1) : (X / 2)
#define leftChild(X) (2 * X + 1)
#define rightChild(X) (2 * X + 2)

typedef struct treeNode
{
    int val; // val = 1/0/-1 => privileged/mixed/revoked
    int countP, countN;
    int sizeCoverP, sizeCoverN, sizeCoverM;
    int *coverP, *coverN, *coverM;
} nd;

void configGen(nd *tree, unsigned int numberOfLeaf);
void configGenFull(nd *, int *, int, int, unsigned int, unsigned int);
void memAllocate(nd *, unsigned int);
void coverGen(nd *, unsigned int);
void printConfiguration(nd *, unsigned int, unsigned int);
void mergePositiveCover(nd *, int);
void mergeNegativeCover(nd *, int);
void mergeLpRnCover(nd *, int);
void mergeLpRmCover(nd *, int);
void mergeLnRpCover(nd *, int);
void mergeLnRmCover(nd *, int);
void mergeLmRpCover(nd *, int);
void mergeLmRnCover(nd *, int);
void mergeLmRmCover(nd *, int);
void freeTree(nd *, unsigned int);
unsigned int findMax(int *, int);
unsigned int heightOfTree(unsigned int);
unsigned int readID(FILE *fp, char read, char *keyword, int *id);
int printCover(nd *, int);
int printCoverLen(nd *, int);
int minimumCover(int, int, int, int, int, int);
void swap(int *, int *);
void sortArray(int *, int );

#endif
