import matplotlib.pyplot as plt
import numpy as np

def are_edges_unique(array_size, window_size, edge_size, edge_count):
    window_positions = np.random.randint(0, array_size - window_size, size=(edge_count, 1))
    edge_offsets = np.random.randint(0, window_size, size=(edge_count, edge_size))
    edges = window_positions + edge_offsets
    return len(set(map(tuple, edges))) == edge_count

def are_edges_unique_no_replacement(array_size, window_size, edge_size, edge_count):
    window_positions = np.random.choice(array_size - window_size, size=(edge_count, 1), replace=False)
    edge_offsets = np.array([np.random.choice(window_size, size=edge_size, replace=False) for _ in range(edge_count)])
    edges = window_positions + edge_offsets
    return len(set(map(tuple, edges))) == edge_count

good = 0
good_nr = 0
print("Probability that all edges are unique:")
for _ in range(999):
    #good += are_edges_unique(array_size=302, window_size=6, edge_size=3, edge_count=216)
    #good_nr += are_edges_unique_no_replacement(array_size=302, window_size=6, edge_size=3, edge_count=216)
    good += are_edges_unique(array_size=1.23*(10**3+32), window_size=10, edge_size=3, edge_count=(10**3 + 32))
    good_nr += are_edges_unique_no_replacement(array_size=int(1.23*(10**3+32)), window_size=10, edge_size=3, edge_count=(10**3 + 32))
    print(f"replacement {good / (_+1)}, no_replacement={good_nr / (_+1)}")

# window size 10 with 1000 -> drop at 8.8
# window size 21.5 with 1000 -> drop at 20
# 
# drop is slightly less than the window size
# according to binary fuse, opt window size is n**(1/3) which gives a drop of same size
