import math
import time

height = 4

cover = [(2,1)]

frist_leaf = (1 << height) - 1
last_leaf = (1 << (height + 1)) - 2

# time measure
elapsed_time = 0
for k in range(1000):
    configuration = []

    start_time = time.time()

    for element in cover:
        node = node_left = node_right = element[0]
        if node > frist_leaf:
            configuration.append(element[0])
        
        else:
            while node_left < frist_leaf:
                node_left = 2 * node_left + 1
                node_right = 2 * node_right + 2
            
            for i in range(node_left, node_right + 1):
                configuration.append(i)
    
    end_time = time.time()

    elapsed_time += end_time - start_time

avg_time = elapsed_time/1000
print("Time elapsed:", avg_time, "seconds")

