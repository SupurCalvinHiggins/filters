import matplotlib.pyplot as plt
import numpy as np

def are_edges_unique(array_size, window_size, edge_size, edge_count):
    window_positions = np.random.randint(0, (array_size // window_size), size=(edge_count, 1))
    edge_offsets = np.zeros((edge_count, edge_size))
    for i in range(edge_size):
        edge_offsets[:, i] = np.random.randint(window_size * i, window_size * (i + 1), size=edge_count)
    edges = window_positions + edge_offsets
    # print(edges)
    return len(set(map(tuple, edges))) == edge_count

good = 0
print("Probability that all edges are unique:")
for _ in range(999):
    # good += are_edges_unique(array_size=302, window_size=6, edge_size=3, edge_count=216)
    #good_nr += are_edges_unique_no_replacement(array_size=302, window_size=6, edge_size=3, edge_count=216)
    good += are_edges_unique(array_size=int(1.23*(10**7+32)), window_size=231, edge_size=3, edge_count=(10**7 + 32))
    print(f"{good / (_+1)}")


# window size 10 with 1000 -> drop at 8.8
# window size 21.5 with 1000 -> drop at 20
# 
# drop is slightly less than the window size
# according to binary fuse, opt window size is n**(1/3) which gives a drop of same size
