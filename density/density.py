import matplotlib.pyplot as plt
import numpy as np

def windowed_density(array_size, window_size, edge_size, edge_count):
    window_positions = np.random.randint(0, array_size - window_size, size=(edge_count, 1))
    edge_offsets = np.random.randint(0, window_size, size=(edge_count, edge_size))
    idxs = edge_offsets + window_positions
    counts = np.bincount(idxs.flatten(), minlength=array_size)
    return counts / (edge_size * edge_count)


density = 0
for _ in range(9999):
    density += windowed_density(array_size=10_000, window_size=(10_000.0)**(1/3), edge_size=3, edge_count=1_1000)

print("window_size", (10_000.0)**(1/3), )
plt.plot(density / 9999)
plt.show()

# window size 10 with 1000 -> drop at 8.8
# window size 21.5 with 1000 -> drop at 20
# 
# drop is slightly less than the window size
# according to binary fuse, opt window size is n**(1/3) which gives a drop of same size
