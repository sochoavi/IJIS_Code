## run this scode as python3 cover_gen.py > cover.txt
## then run the cover reconstruct.py
## note mixed cover likely to be generated when 
## number of id is close to half of the total leaf nodes

import random
import time

class TreeNode:
    def __init__(self, val):
        self.val = val
        self.cover_p = []
        self.cover_n = []
        self.cover_m = []

def generate_file_ids(n, m):
    if n > m:
        raise ValueError("n must be less than or equal to m")

    return random.sample(range(0, m), n)

def create_tree(height):
    return [TreeNode(-1) for _ in range(1 << height)]

def print_tree_node(node):
    # print(f"Status of the node is:")
    # print(f"  Value: {node.val}")
    p =  len(node.cover_p)
    n =  len(node.cover_n)
    m =  len(node.cover_m)

    # print(f"P: {p}, N: {n}, M: {m}")

    sorted_numbers = sorted([p, n, m])

    if sorted_numbers[0] != 0:
        if sorted_numbers[0] == p:
            flag = 1
        elif sorted_numbers[0] == n:
            flag = 2
        else:
            flag = 3
    elif sorted_numbers[1] != 0:
        if sorted_numbers[1] == p:
            flag = 1
        elif sorted_numbers[1] == n:
            flag = 2
        else:
            flag = 3
    else:
        if sorted_numbers[2] == p:
            flag = 1
        elif sorted_numbers[2] == n:
            flag = 2
        else:
            flag = 3
    
    if flag == 1:
        print(f"1, {node.cover_p},")
        # print(f"  Cover_p: {node.cover_p}, Length: {len(node.cover_p)}")
    
    if flag == 2:
        print(f"-1, {node.cover_n},")

    if flag == 3:
        print(f"0, {node.cover_m},")

def print_tree(tree):
    for i, node in enumerate(tree):
        # print(f"Node {i}:")
        print_tree_node(node)

def update_tree_values(tree, file_ids, height):
    for file_id in file_ids:
        tree[file_id].val = 1

    for i in range(len(tree)):
        if tree[i].val == 1:
            tree[i].cover_p.append(((1 << height) + i - 1, tree[i].val))
        else:
            tree[i].cover_n.append(((1 << height) + i - 1, tree[i].val))

def update_new_tree_values(main_tree, new_tree, new_height):
    for i in range(1 << new_height):
        if main_tree[2 * i].val == 1 and main_tree[2 * i + 1].val == 1:
            new_tree[i].val = 1
            new_tree[i].cover_p.append(((1 << new_height) + i - 1, new_tree[i].val))
        elif main_tree[2 * i].val == -1 and main_tree[2 * i + 1].val == -1:
            new_tree[i].val = -1
            new_tree[i].cover_n.append(((1 << new_height) + i - 1, new_tree[i].val))
        else:
            new_tree[i].val = 0

            new_tree[i].cover_p = list(set(main_tree[2*i].cover_p + main_tree[2*i+1].cover_p))
            new_tree[i].cover_n = list(set(main_tree[2*i].cover_n + main_tree[2*i+1].cover_n))

            a = [0] * 7
            if len(main_tree[2 * i].cover_p) > 0 and len(main_tree[2 * i + 1].cover_n) > 0:
                a[0] = len(main_tree[2 * i].cover_p) + len(main_tree[2 * i + 1].cover_n)

            if len(main_tree[2 * i].cover_p) > 0 and len(main_tree[2 * i + 1].cover_m) > 0:
                a[1] = len(main_tree[2 * i].cover_p) + len(main_tree[2 * i + 1].cover_m)

            if len(main_tree[2 * i].cover_n) > 0 and len(main_tree[2 * i + 1].cover_p) > 0:
                a[2] = len(main_tree[2 * i].cover_n) + len(main_tree[2 * i + 1].cover_p)

            if len(main_tree[2 * i].cover_n) > 0 and len(main_tree[2 * i + 1].cover_m) > 0:
                a[3] = len(main_tree[2 * i].cover_n) + len(main_tree[2 * i + 1].cover_m)

            if len(main_tree[2 * i].cover_m) > 0 and len(main_tree[2 * i + 1].cover_p) > 0:
                a[4] = len(main_tree[2 * i].cover_m) + len(main_tree[2 * i + 1].cover_p)
            
            if len(main_tree[2 * i].cover_m) > 0 and len(main_tree[2 * i + 1].cover_n) > 0:
                a[5] = len(main_tree[2 * i].cover_m) + len(main_tree[2 * i + 1].cover_n)
            
            if len(main_tree[2 * i].cover_m) > 0 and len(main_tree[2 * i + 1].cover_m) > 0:
                a[6] = len(main_tree[2 * i].cover_m) + len(main_tree[2 * i + 1].cover_m)

            min = -1
            for k in range(7):
                if a[k] > 0:
                    min = a[k]
                    break
            
            flag = 0

            for k in range(7):
                if a[k] > 0 and a[k] <= min:
                    min = a[k]
                    flag = k
            
            if flag == 0 and a[0] < len(new_tree[i].cover_p) and a[0] < len(new_tree[i].cover_n):
                new_tree[i].cover_m = list(set(main_tree[2*i].cover_p + main_tree[2*i+1].cover_n))
            
            elif flag == 1 and a[1] < len(new_tree[i].cover_p) and a[1] < len(new_tree[i].cover_n):
                new_tree[i].cover_m = list(set(main_tree[2*i].cover_p + main_tree[2*i+1].cover_m))
            
            elif flag == 2 and a[2] < len(new_tree[i].cover_p) and a[2] < len(new_tree[i].cover_n):
                new_tree[i].cover_m = list(set(main_tree[2*i].cover_n + main_tree[2*i+1].cover_p))
            
            elif flag == 3 and a[3] < len(new_tree[i].cover_p) and a[3] < len(new_tree[i].cover_n):
                new_tree[i].cover_m = list(set(main_tree[2*i].cover_n + main_tree[2*i+1].cover_m))
            
            elif flag == 4 and a[4] < len(new_tree[i].cover_p) and a[4] < len(new_tree[i].cover_n):
                new_tree[i].cover_m = list(set(main_tree[2*i].cover_m + main_tree[2*i+1].cover_p))
            
            elif flag == 5 and a[5] < len(new_tree[i].cover_p) and a[5] < len(new_tree[i].cover_n):
                new_tree[i].cover_m = list(set(main_tree[2*i].cover_m + main_tree[2*i+1].cover_n))
            
            elif flag == 6 and a[6] < len(new_tree[i].cover_p) and a[6] < len(new_tree[i].cover_n):
                new_tree[i].cover_m = list(set(main_tree[2*i].cover_m + main_tree[2*i+1].cover_m))


def main():
    initial_height = 17
    number_of_id = 1 << 16
    
    elapsed_time = 0
    
    for z in range(1):
        # Generate file IDs only once
        file_ids = generate_file_ids(number_of_id, (1 << initial_height))
        # print(file_ids)

        # Create the main tree
        main_tree = create_tree(initial_height)

        # Update the values of the main tree nodes with the distinct numbers
        update_tree_values(main_tree, file_ids, initial_height)
        
        start_time = time.time()

        ## CHECKED ##

        # Set the initial height
        height = initial_height

        # while height > initial_height -1:
        while height > 0:
            # Create a new tree with height (height - 1)
            new_height = height - 1
            new_tree = create_tree(new_height)

            # Update the values of the new tree
            update_new_tree_values(main_tree, new_tree, new_height)

            # print(f"\nNew Tree (Height {new_height}):")
            # # Print the initial values of the new tree
            # print_tree(new_tree)

            # Free memory occupied by main_tree
            main_tree = None

            # Set new_tree as the main_tree for the next iteration
            main_tree = new_tree

            # Reduce the height by 1 for the next iteration
            height = new_height
        
        end_time = time.time()

        elapsed_time += end_time - start_time
        print_tree(new_tree)
    
    elapsed_time = elapsed_time / 1

    # print("Time elapsed:", elapsed_time, "seconds")

    # print("\nAll iterations completed.")
    # print_tree(new_tree)
    # print(f"No of files: {number_of_id}")
    # print(f"Length of Cover_p: {len(new_tree[0].cover_p)}")
    # print(f"Length of Cover_n: {len(new_tree[0].cover_n)}")
    # print(f"Length of Cover_m: {len(new_tree[0].cover_m)}")
    # print(f"Length of Cover_m: {new_tree[0].cover_m}")
    # print(f"Length of Cover_m: {new_tree[0].cover_p}")

if __name__ == "__main__":
    main()
