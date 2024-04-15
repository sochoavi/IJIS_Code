#include "include.h"
#include "include.c"

int main(void)
{
    // Declearation
    char read, keyword[100];
    int height;
    int id[idSize] = {0};
    unsigned int i, j, k, l, m;
    unsigned int originalCount, reducedCount, max, nodesChecked;
    unsigned int lower, upper, numberOfLeaf, coverLength, start, sizeID;
    nd *tree, *mainTree;
    FILE *fp;

    // Initialization
    originalCount = reducedCount = 0;

    // Open the file
    fp = fopen("DB.txt", "r");
    // fp = fopen("input.txt", "r");
    if (fp == NULL)
    {
        printf("Failed to open file: DB.txt\n");
        return 1;
    }

    numberOfLeaf = (1 << 19);
    tree = (nd *)malloc((2 * numberOfLeaf - 1) * sizeof(nd));

    read = fgetc(fp);
    while (read != EOF)
    {
        // Read ID
        sizeID = readID(fp, read, keyword, id); // CHECKED

        DEBUG_PRINT("sizeID: %u\n", sizeID);
        originalCount += sizeID;

        // Sort the ID array
        sortArray(id, sizeID); // CHECKED
        #ifdef DEBUG_MAIN
            for (j = 0; j < sizeID; j++)
                printf("DEBUG_MAIN: id[%d]: %d\n", j, id[j]);
        #endif

        // Finding out max id present in the keyword and thus the corresponding tree height
        max = findMax(id, sizeID); // CHECKED
        height = heightOfTree(max); // CHECKED

        // numberOfLeaf = (1 << height);
        lower = (numberOfLeaf - 1);     // leaf nodes start from "lower"
        upper = (2 * numberOfLeaf - 2); // leaf nodes end at "upper"

        DEBUG_PRINT("max: %u, numberOfLeaf: %u, lower: %u, upper: %u, size of ID: %d\n", max, numberOfLeaf, lower, upper, sizeID);

        // Allocate memory for the tree

        configGenFull(tree, id, sizeID, numberOfLeaf, lower, upper);

        // printConfiguration(tree, upper, lower);
        coverGen(tree, numberOfLeaf);

        coverLength = printCover(tree, 0); // compile with -DDEBUG

        reducedCount += coverLength;

        DEBUG_PRINT("\n");

        read = fgetc(fp);
    }

    printf("Number of pairs without reduction: %u\n", originalCount);
    printf("Number of pairs after reduction: %u\n", reducedCount);

    freeTree(tree, numberOfLeaf);

    fclose(fp);

    return 0;
}








// Allocating memory for the tree
        // if(height > worikingTreeHeight)
        // {
            // DEBUG_PRINT("numberOfLeaf > worikingTreeHeight\n");

            // Memory allocation for the main tree
            // mainTree = (nd *)malloc((1 << (height - worikingTreeHeight)) * sizeof(nd));
            // memAllocate(mainTree, (1 << height));
            // lower = ((1 << (height - worikingTreeHeight)) - 1);     // leaf nodes start from "lower"
            // upper = (2 * (1 << (height - worikingTreeHeight)) - 2); // leaf nodes end at "upper"

            // // For sub-trees
            // numberOfLeaf = (1 << worikingTreeHeight);
            
            // DEBUG_PRINT("max: %u, numberOfLeaf: %u, lower: %u, upper: %u, size of ID: %d\n", max, numberOfLeaf, lower, upper, j);

            // // Allocate memory for the tree
            // tree = (nd *)malloc((2 * numberOfLeaf - 1) * sizeof(nd));
            // memAllocate(tree, numberOfLeaf);

            // l = 0;
            // for (k = 0; k < (1 << (height - worikingTreeHeight)); k++)
            // {
            //     configGen(tree, numberOfLeaf);

            //     while(id[l] < ((k + 1) * (1 << worikingTreeHeight)))
            //     {
            //         tree[id[l] - ((k * (1 << worikingTreeHeight)) +1)].val = 1;
            //         tree[id[l] - ((k * (1 << worikingTreeHeight)) +1)].countP = 1;
            //         tree[id[l] - ((k * (1 << worikingTreeHeight)) +1)].countN = 0;
            //         // DEBUG_PRINT("Node number: %5d,\t Node Value: %5d\n", (id[l] + (k * (1 << worikingTreeHeight)) +1)), tree[(id[l] + (k * (1 << worikingTreeHeight)) +1))].val);

            //         l++;
            //     }

            //     // printConfiguration(tree, upper, lower);

            //     coverGen(tree, numberOfLeaf);

            //     mainTree[lower].val = tree[0].val; 
            //     mainTree[lower].countP = tree[0].countP; 
            //     mainTree[lower].countN = tree[0].countN; 
            //     mainTree[lower].sizeCoverP = tree[0].sizeCoverP; 
            //     mainTree[lower].sizeCoverN = tree[0].sizeCoverN; 
            //     mainTree[lower].sizeCoverM = tree[0].sizeCoverM; 

            //     for (m = 0; m < tree[0].sizeCoverP; m++)
            //         mainTree[lower].coverP[m] = tree[0].coverP[m];
                
            //     for (m = 0; m < tree[0].sizeCoverN; m++)
            //         mainTree[lower].coverN[m] = tree[0].coverN[m];
                
            //     for (m = 0; m < tree[0].sizeCoverM; m++)
            //         mainTree[lower].coverM[m] = tree[0].coverM[m];
                
            //     lower++;
            // }

            // coverLength = printCover(mainTree, 0); // compile with -DDEBUG

            // reducedCount += coverLength;

            // freeTree(tree, numberOfLeaf);

            // DEBUG_PRINT("\n");
        // }