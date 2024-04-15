## first run python3 cover_gen.py > cover.txt
## then run this code
## note mixed cover likely to be generated when 
## number of id is close to half of the total leaf nodes

import math
import time

def find_ancestors(node):
    # Calculate the parent of a node
    def calculate_parent(node):
        return math.ceil(node / 2) - 1

    ancestors = []
    while node >= 0:
        ancestors.append(node)
        parent = calculate_parent(node)

        # Ensure termination by checking if the parent is the same as the current node
        if parent == node:
            break

        node = parent

    return ancestors

def build_list_with_ancestors(existing_list, node):
    # Find the ancestors of the given node
    ancestors = find_ancestors(node)

    # Append the node and its list of ancestors to the existing list
    existing_list.append([node, ancestors])


def main():
    file_path = 'cover.txt'
    ittr = 0
    elapsed_time = 0

    with open(file_path, 'r') as file:
        for line in file:

            # Split the line into parts
            parts = line.strip().split(', ')

            # Extract the first value (1, 0, or -1)
            cover_type = int(parts[0])
            # print(cover_type)

            # Extract the rest of the line as a single string
            rest_of_line = ', '.join(parts[1:])

            # Use eval to convert the string to a Python object (list)
            nested_list = eval(rest_of_line)

            # Flatten the nested list to a single-level list
            cover = [item for sublist in nested_list for item in sublist]
            # print(cover)

            # test input
            # cover = [(10, 0), (17, 0), (23,1)]
            # cover = [(31,1),(62,0)]

            if cover_type == 0: # 0 =  mixed cover
                ittr += 1
                node_ancestors_list = []
                node_supernode_list = {}

                start_time = time.time()

                for i, (node_i, sign) in enumerate(cover):
                    # Append ancestors for node 1
                    build_list_with_ancestors(node_ancestors_list, node_i)
                    node_supernode_list[node_i] = 0

                    while len(node_ancestors_list) >= 2:
                        first_node = node_ancestors_list[0]
                        node1_value = first_node[0]
                        ancestor1_list = first_node[1]

                        for i in range (1, len(node_ancestors_list)):
                            second_node = node_ancestors_list[1]
                            node2_value = second_node[0]
                            ancestor2_list = second_node[1]
                            
                            flag = 0
                            for i in range(len(ancestor1_list)):
                                for j in range(len(ancestor2_list)):
                                    if ancestor1_list[i] == ancestor2_list[j]:
                                        flag = 1
                                        break
                                if flag == 1:
                                    break
                            
                            if node_supernode_list[node1_value] < ancestor1_list[i-1]:
                                node_supernode_list[node1_value] = ancestor1_list[i-1]
                            
                            if node_supernode_list[node2_value] < ancestor2_list[j-1]:
                                node_supernode_list[node2_value] = ancestor2_list[j-1]
                        
                        del node_ancestors_list[0]

                end_time = time.time()
                elapsed_time += end_time - start_time
                print("Node supernode list:", node_supernode_list)


    print(f"ittr: {ittr}")
    elapsed_time = elapsed_time / ittr
    # print("Node ancestor list:", node_ancestors_list)

    print("Time elapsed:", elapsed_time, "seconds")


if __name__ == "__main__":
    main()