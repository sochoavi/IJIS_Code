#include "include.h"

void coverGen(nd *tree, unsigned int numberOfLeaf)
{
    int i, lower, upper;
    int flag = 0;

    lower = (numberOfLeaf - 1);     // leaf nodes start from "lower"
    upper = (2 * numberOfLeaf - 2); // leaf nodes end at "upper"

    /* generating cover */
    for (i = upper; i >= 0; i--)
    {
        // printf("node: %d,\t val: %d,\t countP: %d,\t countN: %d,\t leftC val: %d\n", i, tree[i].val, tree[i].countP, tree[i].countN, tree[leftC(i)].val);

        // // printf("i: %d,\t parent of i: %d\n", i, parent(i));
        // privileged leaf node
        if (tree[i].val == 1)
        {
            // printf("Leaf Priv: %d\n", i);

            // updating privileged count of parent
            if (i != 0)
            {
                tree[parent(i)].countP += tree[i].countP;
            }

            // updating cover
            tree[i].coverP[tree[i].sizeCoverP] = i;
            tree[i].sizeCoverP += 1;
        }

        // revoked leaf node
        else if (tree[i].val == -1)
        {
            // printf("Leaf Rev: %d\n", i);

            // updating revoked count of parent
            if (i != 0)
            {
                tree[parent(i)].countN += tree[i].countN;
            }

            // updating cover
            tree[i].coverN[tree[i].sizeCoverN] = i;
            tree[i].sizeCoverN += 1;
        }

        // non-leaf privileged node
        else if ((tree[i].val == 0) && (tree[i].countN == 0))
        {
            // printf("Non-Leaf Priv: %d\n", i);

            // assigning value
            tree[i].val = 1;

            // updating privileged count of parent
            if (i != 0)
            {
                tree[parent(i)].countP += tree[i].countP;
            }

            // updating cover
            tree[i].coverP[tree[i].sizeCoverP] = i;
            tree[i].sizeCoverP += 1;
        }

        // non-leaf revoked node
        else if ((tree[i].val == 0) && (tree[i].countP == 0))
        {
            // printf("Non-Leaf Rev: %d\n", i);

            // assigning value
            tree[i].val = -1;

            // updating revoked count of parent
            if (i != 0)
            {
                tree[parent(i)].countN += tree[i].countN;
            }

            // updating cover
            tree[i].coverN[tree[i].sizeCoverN] = i;
            tree[i].sizeCoverN += 1;
        }

        // non-leaf mixed node with (P == N) and both child are either privileged or revoked
        else if ((tree[i].val == 0) && (tree[i].countP == tree[i].countN) && (tree[leftChild(i)].val != 0))
        {
            // printf("P=N with both Priv/Rev: %d\n", i);

            // updating privileged and revoked count of parent
            if (i != 0)
            {
                tree[parent(i)].countP += tree[i].countP;
                tree[parent(i)].countN += tree[i].countN;
            }

            // updating cover
            if (tree[leftChild(i)].val == 1) // left node is privileged
            {
                // updating positive cover
                tree[i].coverP[tree[i].sizeCoverP] = leftChild(i);
                tree[i].sizeCoverP += 1;

                // updating negative cover
                tree[i].coverN[tree[i].sizeCoverN] = rightChild(i);
                tree[i].sizeCoverN += 1;

                // updating mixed cover
                tree[i].coverM[tree[i].sizeCoverM] = rightChild(i);
                tree[i].sizeCoverM += 1;
            }
            else // updating negative cover
            {
                // updating positive cover
                tree[i].coverP[tree[i].sizeCoverP] = rightChild(i);
                tree[i].sizeCoverP += 1;

                // updating negative cover
                tree[i].coverN[tree[i].sizeCoverN] = leftChild(i);
                tree[i].sizeCoverN += 1;

                // updating mixed cover
                tree[i].coverM[tree[i].sizeCoverM] = leftChild(i);
                tree[i].sizeCoverM += 1;
            }
        }
        // non-leaf mixed node with (P != 0) && (N!= 0) and none of it's child is either privileged or revoked
        else
        {
            // printf("P != N: %d\n", i);

            // updating privileged and revoked count of parent
            if (i != 0)
            {
                tree[parent(i)].countP += tree[i].countP;
                tree[parent(i)].countN += tree[i].countN;
            }

            if ((tree[leftChild(i)].val == 1))
            // left child is privileged
            // minimum cover for node i is either Right-N / Left-P + Right-P / Right-M (if not null) if left child is privileged
            // or the converse, in any case the cover for node i is same
            // Right-N is the neg cover of node i
            // Left-P + Right-P is the positive cover of node i
            // Right-M is the mix cover of node i
            {
                mergePositiveCover(tree, i);
                mergeNegativeCover(tree, i);

                if (tree[leftChild(i)].sizeCoverP + tree[rightChild(i)].sizeCoverN < tree[leftChild(i)].sizeCoverP + tree[rightChild(i)].sizeCoverM)
                    mergeLpRnCover(tree, i);

                else
                    mergeLpRmCover(tree, i);
            }

            else if ((tree[leftChild(i)].val == -1))
            // left child is revoked
            {
                mergePositiveCover(tree, i);
                mergeNegativeCover(tree, i);

                if (tree[leftChild(i)].sizeCoverN + tree[rightChild(i)].sizeCoverP < tree[leftChild(i)].sizeCoverN + tree[rightChild(i)].sizeCoverM)
                    mergeLnRpCover(tree, i);

                else
                    mergeLnRmCover(tree, i);
            }

            else if ((tree[rightChild(i)].val == 1))
            // right child is privileged
            {
                mergePositiveCover(tree, i);
                mergeNegativeCover(tree, i);

                if (tree[rightChild(i)].sizeCoverP + tree[leftChild(i)].sizeCoverN < tree[rightChild(i)].sizeCoverP + tree[leftChild(i)].sizeCoverM)
                    mergeLnRpCover(tree, i);

                else
                    mergeLmRpCover(tree, i);
            }

            else if ((tree[rightChild(i)].val == -1))
            // right child is revoked
            {
                mergePositiveCover(tree, i);
                mergeNegativeCover(tree, i);

                if (tree[rightChild(i)].sizeCoverN + tree[leftChild(i)].sizeCoverP < tree[rightChild(i)].sizeCoverN + tree[leftChild(i)].sizeCoverM)
                    mergeLpRnCover(tree, i);

                else
                    mergeLmRnCover(tree, i);
            }

            else
            // no child is privileged or revoked
            {
                mergePositiveCover(tree, i);
                mergeNegativeCover(tree, i);

                flag = minimumCover(tree[leftChild(i)].sizeCoverP, tree[leftChild(i)].sizeCoverN, tree[leftChild(i)].sizeCoverM, tree[rightChild(i)].sizeCoverP, tree[rightChild(i)].sizeCoverN, tree[rightChild(i)].sizeCoverM);

                switch (flag)
                {
                case 1: // flag = 1 -> lprn
                    mergeLpRnCover(tree, i);
                    break;

                case 2: // flag = 2 -> lprm
                    mergeLpRmCover(tree, i);
                    break;

                case 3: // flag = 3 -> lnrp
                    mergeLnRpCover(tree, i);
                    break;

                case 4: // flag = 4 -> lnrm
                    mergeLnRmCover(tree, i);
                    break;

                case 5: // flag = 5 -> lmrp
                    mergeLmRpCover(tree, i);
                    break;

                case 6: // flag = 6 -> lmrn
                    mergeLmRnCover(tree, i);
                    break;

                case 7: // flag = 7 -> lmrm
                    mergeLmRmCover(tree, i);
                    break;

                default:
                    break;
                }
            }
        }
    }
    /*------------------------X------------------------*/
}

void mergePositiveCover(nd *tree, int node)
{
    int i, j;

    for (j = 0; j < tree[leftChild(node)].sizeCoverP; j++)
        tree[node].coverP[j] = tree[leftChild(node)].coverP[j];

    for (i = 0; i < tree[rightChild(node)].sizeCoverP; i++)
    {
        tree[node].coverP[j] = tree[rightChild(node)].coverP[i];
        j++;
    }
    tree[node].sizeCoverP += j; // "tree[node].sizeCoverP" can be used in place of "j"; used "j" for clean code
}

void mergeNegativeCover(nd *tree, int node)
{
    int i, j;

    for (j = 0; j < tree[leftChild(node)].sizeCoverN; j++)
        tree[node].coverN[j] = tree[leftChild(node)].coverN[j];

    for (i = 0; i < tree[rightChild(node)].sizeCoverN; i++)
    {
        tree[node].coverN[j] = tree[rightChild(node)].coverN[i];
        j++;
    }
    tree[node].sizeCoverN += j; // "tree[node].sizeCoverN" can be used in place of "j"; used "j" for clean code
}

void mergeLpRnCover(nd *tree, int node)
{
    int i, j;

    for (j = 0; j < tree[leftChild(node)].sizeCoverP; j++)
        tree[node].coverM[j] = tree[leftChild(node)].coverP[j];

    for (i = 0; i < tree[rightChild(node)].sizeCoverN; i++)
    {
        tree[node].coverM[j] = tree[rightChild(node)].coverN[i];
        j++;
    }
    tree[node].sizeCoverM += j; // "tree[node].sizeCoverP" can be used in place of "j"; used "j" for clean code
}

void mergeLpRmCover(nd *tree, int node)
{
    int i, j;

    for (j = 0; j < tree[leftChild(node)].sizeCoverP; j++)
        tree[node].coverM[j] = tree[leftChild(node)].coverP[j];

    for (i = 0; i < tree[rightChild(node)].sizeCoverM; i++)
    {
        tree[node].coverM[j] = tree[rightChild(node)].coverM[i];
        j++;
    }
    tree[node].sizeCoverM += j; // "tree[node].sizeCoverP" can be used in place of "j"; used "j" for clean code
}

void mergeLnRpCover(nd *tree, int node)
{
    int i, j;

    for (j = 0; j < tree[leftChild(node)].sizeCoverN; j++)
        tree[node].coverM[j] = tree[leftChild(node)].coverN[j];

    for (i = 0; i < tree[rightChild(node)].sizeCoverP; i++)
    {
        tree[node].coverM[j] = tree[rightChild(node)].coverP[i];
        j++;
    }
    tree[node].sizeCoverM += j; // "tree[node].sizeCoverP" can be used in place of "j"; used "j" for clean code
}

void mergeLnRmCover(nd *tree, int node)
{
    int i, j;

    for (j = 0; j < tree[leftChild(node)].sizeCoverN; j++)
        tree[node].coverM[j] = tree[leftChild(node)].coverN[j];

    for (i = 0; i < tree[rightChild(node)].sizeCoverM; i++)
    {
        tree[node].coverM[j] = tree[rightChild(node)].coverM[i];
        j++;
    }
    tree[node].sizeCoverM += j; // "tree[node].sizeCoverP" can be used in place of "j"; used "j" for clean code
}

void mergeLmRpCover(nd *tree, int node)
{
    int i, j;

    for (j = 0; j < tree[leftChild(node)].sizeCoverM; j++)
        tree[node].coverM[j] = tree[leftChild(node)].coverM[j];

    for (i = 0; i < tree[rightChild(node)].sizeCoverP; i++)
    {
        tree[node].coverM[j] = tree[rightChild(node)].coverP[i];
        j++;
    }
    tree[node].sizeCoverM += j; // "tree[node].sizeCoverP" can be used in place of "j"; used "j" for clean code
}

void mergeLmRnCover(nd *tree, int node)
{
    int i, j;

    for (j = 0; j < tree[leftChild(node)].sizeCoverM; j++)
        tree[node].coverM[j] = tree[leftChild(node)].coverM[j];

    for (i = 0; i < tree[rightChild(node)].sizeCoverN; i++)
    {
        tree[node].coverM[j] = tree[rightChild(node)].coverN[i];
        j++;
    }
    tree[node].sizeCoverM += j; // "tree[node].sizeCoverP" can be used in place of "j"; used "j" for clean code
}

void mergeLmRmCover(nd *tree, int node)
{
    int i, j;

    for (j = 0; j < tree[leftChild(node)].sizeCoverM; j++)
        tree[node].coverM[j] = tree[leftChild(node)].coverM[j];

    for (i = 0; i < tree[rightChild(node)].sizeCoverM; i++)
    {
        tree[node].coverM[j] = tree[rightChild(node)].coverM[i];
        j++;
    }
    tree[node].sizeCoverM += j;
    // "tree[node].sizeCoverM" can be used in place of "j"; used "j" for clean code
}

int printCover(nd *tree, int node)
{
    int i, j;

    if (node == 0)
        DEBUG_PRINT("Cover for root is:\n");
    else
        DEBUG_PRINT("Cover for node %d is:\n", node);

    DEBUG_PRINT("Positive Cover is:\n");
    if (tree[node].sizeCoverP == 0)
    {
        DEBUG_PRINT("No positive cover.\n");
    }
    else
    {
        for (i = 0; i < tree[node].sizeCoverP; i++)
        {
            DEBUG_PRINT("(%d, 1)\n", tree[node].coverP[i]);
        }
    }

    DEBUG_PRINT("Negative Cover is:\n");
    if (tree[node].sizeCoverN == 0)
    {
        DEBUG_PRINT("No negative cover.\n");
    }
    else
    {
        for (i = 0; i < tree[node].sizeCoverN; i++)
        {
            DEBUG_PRINT("(%d, -1)\n", tree[node].coverN[i]);
        }
    }

    DEBUG_PRINT("Mixed Cover is:\n");
    if (tree[node].sizeCoverM == 0)
    {
        DEBUG_PRINT("No mixed cover.\n");
    }
    else
    {
        for (i = 0; i < tree[node].sizeCoverM; i++)
        {
            j = tree[tree[node].coverM[i]].val;
            DEBUG_PRINT("(%d, %d)\n", tree[node].coverM[i], j);
        }
    }

    if (tree[node].sizeCoverP == 0)
    {
        if (tree[node].sizeCoverN < tree[node].sizeCoverM)
        {
            DEBUG_PRINT("Minimum cover is negative cover and size of minimum cover is: %d.\n", tree[node].sizeCoverN);
            return (tree[node].sizeCoverN);
        }
        else
        {
            DEBUG_PRINT("Minimum cover is mixed cover and size of minimum cover is: %d.\n", tree[node].sizeCoverM);
            return (tree[node].sizeCoverM);
        }
    }
    else if (tree[node].sizeCoverN == 0)
    {
        if (tree[node].sizeCoverP < tree[node].sizeCoverM)
        {
            DEBUG_PRINT("Minimum cover is positive cover and size of minimum cover is: %d.\n", tree[node].sizeCoverP);
            return (tree[node].sizeCoverP);
        }
        else
        {
            DEBUG_PRINT("Minimum cover is mixed cover and size of minimum cover is: %d.\n", tree[node].sizeCoverM);
            return (tree[node].sizeCoverM);
        }
    }
    else if (tree[node].sizeCoverM == 0)
    {
        if (tree[node].sizeCoverP < tree[node].sizeCoverN)
        {
            DEBUG_PRINT("Minimum cover is positive cover and size of minimum cover is: %d.\n", tree[node].sizeCoverP);
            return (tree[node].sizeCoverP);
        }
        else
        {
            DEBUG_PRINT("Minimum cover is negative cover and size of minimum cover is: %d.\n", tree[node].sizeCoverN);
            return (tree[node].sizeCoverN);
        }
    }
    else if (tree[node].sizeCoverP < tree[node].sizeCoverN)
    {
        if (tree[node].sizeCoverN < tree[node].sizeCoverM)
        {
            DEBUG_PRINT("Minimum cover is positive cover and size of minimum cover is: %d.\n", tree[node].sizeCoverP);
            return (tree[node].sizeCoverP);
        }
        else if (tree[node].sizeCoverP < tree[node].sizeCoverM)
        {
            DEBUG_PRINT("Minimum cover is positive cover and size of minimum cover is: %d.\n", tree[node].sizeCoverP);
            return (tree[node].sizeCoverP);
        }
        else
        {
            DEBUG_PRINT("Minimum cover is mixed cover and size of minimum cover is: %d.\n", tree[node].sizeCoverM);
            return (tree[node].sizeCoverM);
        }
    }
    else if (tree[node].sizeCoverN < tree[node].sizeCoverM)
    {
        DEBUG_PRINT("Minimum cover is negative cover and size of minimum cover is: %d.\n", tree[node].sizeCoverN);
        return (tree[node].sizeCoverN);
    }
    else
    {
        DEBUG_PRINT("Minimum cover is mixed cover and size of minimum cover is: %d.\n", tree[node].sizeCoverM);
        return (tree[node].sizeCoverM);
    }
}

int printCoverLen(nd *tree, int node)
{
    int p, n, m;
    p = tree[node].sizeCoverP;
    n = tree[node].sizeCoverN;
    m = tree[node].sizeCoverM;
    if (p == 0 && n != 0 && m != 0)
        if (n < m)
            return (n);
        else
            return (m);
    else if (p != 0 && n == 0 && m != 0)
        if (p < m)
            return (p);
        else
            return (m);
    else if (p != 0 && n != 0 && m == 0)
        if (p < n)
            return (p);
        else
            return (n);
    else if (p != 0 && n == 0 && m == 0)
        return (p);
    else if (p == 0 && n != 0 && m == 0)
        return (n);
    else if (p == 0 && n == 0 && m != 0)
        return (m);
    else if (p < n)
        if (n < m)
            return (p);
        else if (p < m)
            return (p);
        else
            return (m);
    else if (n < m)
        return (n);
    else
        return (m);
}


int minimumCover(int lp, int ln, int lm, int rp, int rn, int rm)
{
    int array[7], minimum, size, c, location = 1;

    array[0] = lp + rn; // lprn
    array[1] = lp + rm; // lprm
    array[2] = ln + rp; // lnrp
    array[3] = ln + rm; // lnrm
    array[4] = lm + rp; // lmrp
    array[5] = lm + rn; // lmrn
    array[6] = lm + rm; // lmrm

    minimum = array[0];

    for (c = 1; c < 7; c++)
    {
        if (array[c] < minimum)
        {
            minimum = array[c];
            location = c + 1;
        }
    }

    return location;
}

unsigned int findMax(int *id, int size)
{
    int k;
    unsigned int max;

    k = 1;
    max = id[0];
    while (k < size)
    {
        if (id[k] > max)
            max = id[k];
        k++;
    }

    #ifdef DEBUG_MAX
        printf("Max: %u\n", max);
    #endif

    return max;
}

unsigned int heightOfTree(unsigned int max)
{
    int k;
    unsigned int numberOfLeaf;
    
    k = 0;
    numberOfLeaf = 0;
    while (numberOfLeaf < max)
    {
        ++k;
        numberOfLeaf = (1 << k);
    }

    #ifdef DEBUG_HEIGHT
        printf("Height: %d\n", k);
    #endif

    return k;
}

void freeTree(nd *tree, unsigned int numberOfLeaf)
{
    unsigned int i;

    // Free memory for coverP, coverN, and coverM arrays and the array of nodes
    for (i = 0; i < (2 * numberOfLeaf - 1); i++)
    {
        free(tree[i].coverP);
        free(tree[i].coverN);
        free(tree[i].coverM);
    }
    free(tree);
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void sortArray(int *arr, int size) 
{
    int nonZeroCount = 0;

    // Find the number of non-zero elements
    for (int i = 0; i <= size; i++) 
    {
        if (arr[i] == 0)
            break;
        nonZeroCount++;
    }

    // Sort the non-zero elements using any sorting algorithm
    for (int i = 0; i < nonZeroCount - 1; i++) 
    {
        for (int j = 0; j < nonZeroCount - i - 1; j++) 
        {
            if (arr[j] > arr[j + 1]) 
            {
                swap(&arr[j], &arr[j + 1]);
            }
        }
    }

    #ifdef DEBUG_SORT
        printf("DEBUG_SORT: Sorted array:\n");
        for (int i = 0; i < nonZeroCount - 1; i++)
        {
            printf("id[%u]: %u\n", i, arr[i]);
        }
    #endif
}


unsigned int readID(FILE *fp, char read, char *keyword, int *id)
{
    int i, j;
    // Read keyword
    i = 0;
    while ((read != ',') && (read != EOF))
    {
        if (((read >= 'a') && (read <= 'z')) || ((read >= 'A') && (read <= 'Z')) || ((read >= '0') && (read <= '9')))
        {
            keyword[i] = read;
            i++;
        }
        read = fgetc(fp);
    }
    keyword[i] = '\0';

    #ifdef DEBUG_READID
        if (keyword[0] != '\0')
        {
            printf("DEBUG_READID: Keyword: %s\n", keyword);
        }
    #endif

    // Read corresponding IDs
    j = 0;
    while ((fgetc(fp) != '\n') && (read != EOF))
    {
        fscanf(fp, "%u", &id[j]);
        
        #ifdef DEBUG_READID
            printf("DEBUG_READID: id[%d]: %d\n", j, id[j]);
        #endif
        
        j++;
        if(j > idSize){
            printf("Size of array ID is small\n");
            return 1;
        }
    }

    return j;
}


void configGen(nd *tree, unsigned int numberOfLeaf)
{
    unsigned int lower, upper;
    int k, l;

    lower = (numberOfLeaf - 1);     // leaf nodes start from "lower"
    upper = (2 * numberOfLeaf - 2); // leaf nodes end at "upper"
    
    // Initialize each nonleaf-node and allocate memory for coverP, coverN, and coverM arrays
    for (k = 0; k < lower; k++)
    {
        tree[k].val = 0;
        tree[k].countP = 0;
        tree[k].countN = 0;
        tree[k].sizeCoverP = 0;
        tree[k].sizeCoverN = 0;
        tree[k].sizeCoverM = 0;

        // Initialize coverP, coverN, and coverM arrays if needed
        for (l = 0; l < (numberOfLeaf / 2); l++)
        {
            tree[k].coverP[l] = 0;
            tree[k].coverN[l] = 0;
            tree[k].coverM[l] = 0;
        }
    }

    // Initialize each leaf-node and assigning configuration all marked as revoked initially
    for (; k <= upper; k++)
    {
        tree[k].val = -1;
        tree[k].countP = 0;
        tree[k].countN = 1;
        tree[k].sizeCoverP = 0;
        tree[k].sizeCoverN = 0;
        tree[k].sizeCoverM = 0;

        // Initialize coverP, coverN, and coverM arrays if needed
        for (l = 0; l < (numberOfLeaf / 2); l++)
        {
            tree[k].coverP[l] = 0;
            tree[k].coverN[l] = 0;
            tree[k].coverM[l] = 0;
        }
        DEBUG_PRINT("Node number: %5d,\t Node Value: %5d\n", k, tree[k].val);
    }
}
/*-----------------------------------------------------------------------*/

void configGenFull(nd *tree, int *id, int sizeOfID, int numberOfLeaf, unsigned int lower, unsigned int upper)
{
    int k, l, h;
    
    // Initialize each nonleaf-node and allocate memory for coverP, coverN, and coverM arrays
    for (k = 0; k < lower; k++)
    {
        tree[k].val = 0;
        tree[k].countP = 0;
        tree[k].countN = 0;
        tree[k].sizeCoverP = 0;
        tree[k].sizeCoverN = 0;
        tree[k].sizeCoverM = 0;
        
        h = heightOfTree(k);
        
        tree[k].coverP = malloc((numberOfLeaf / (2 * (1 << h))) * sizeof(int));
        tree[k].coverN = malloc((numberOfLeaf / (2 * (1 << h))) * sizeof(int));
        tree[k].coverM = malloc((numberOfLeaf / (2 * (1 << h))) * sizeof(int));

        // Initialize coverP, coverN, and coverM arrays if needed
        for (l = 0; l < ((numberOfLeaf / (2 * (1 << h)))/(1 << h)); l++)
        {
            tree[k].coverP[l] = 0;
            tree[k].coverN[l] = 0;
            tree[k].coverM[l] = 0;
        }
    }

    // Initialize each leaf-node and assigning configuration all marked as revoked initially
    for (; k <= upper; k++)
    {
        tree[k].val = -1;
        tree[k].countP = 0;
        tree[k].countN = 1;
        tree[k].sizeCoverP = 0;
        tree[k].sizeCoverN = 0;
        tree[k].sizeCoverM = 0;

        tree[k].coverP = malloc(1 * sizeof(int));
        tree[k].coverN = malloc(1 * sizeof(int));
        tree[k].coverM = malloc(1 * sizeof(int));

        // Initialize coverP, coverN, and coverM arrays if needed
        for (l = 0; l < 1; l++)
        {
            tree[k].coverP[l] = 0;
            tree[k].coverN[l] = 0;
            tree[k].coverM[l] = 0;
        }
        DEBUG_PRINT("Node number: %5d,\t Node Value: %5d\n", k, tree[k].val);
    }

    // Initializing present leaf nodes
    for (k = 0; k < sizeOfID; k++)
    {
        tree[id[k] + lower -1].val = 1;
        tree[id[k] + lower -1].countP = 1;
        tree[id[k] + lower -1].countN = 0;
        DEBUG_PRINT("Node number: %5d,\t Node Value: %5d\n", (id[k] + lower -1), tree[(id[k] + lower -1)].val);
    }
}
/*-----------------------------------------------------------------------*/

void memAllocate(nd *tree, unsigned int numberOfLeaf)
{
    unsigned int upper;
    int k;
    
    upper =  (2 * numberOfLeaf - 2);

    // Initialize each nonleaf-node and allocate memory for coverP, coverN, and coverM arrays
    for (k = 0; k <= upper; k++)
    {
        tree[k].coverP = malloc((numberOfLeaf / 2) * sizeof(int));
        tree[k].coverN = malloc((numberOfLeaf / 2) * sizeof(int));
        tree[k].coverM = malloc((numberOfLeaf / 2) * sizeof(int));
    }
}

void printConfiguration(nd *tree, unsigned int upper, unsigned int lower)
{
    int i;
    printf("Configuration for leaf is:\n");
    for (i = lower; i <= upper; i++)
    {
        printf("Node number: %5u,\t Node Value: %5d\n", i, tree[i].val);
    }
}

