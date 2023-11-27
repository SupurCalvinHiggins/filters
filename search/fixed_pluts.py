import pandas as pd
import numpy as np
from scipy.spatial import ConvexHull, convex_hull_plot_2d
import matplotlib.pyplot as plt

file_path = "fixed.csv"
out_file = "random_sample.csv"

try:
    df = pd.read_csv(file_path, header=None, usecols=[9, 10], names=['x', 'y'])
except FileNotFoundError:
    print(f"The file '{file_path}' does not exist.")
except Exception as e:
    print(f"An error occurred: {e}")

# Convert x and y columns to numeric types in the main dataframe
df['x'] = pd.to_numeric(df['x'])
df['y'] = pd.to_numeric(df['y'])

sample_size = 1000
random_samples = df.sample(n=sample_size, replace=False)

# Write the random samples to a CSV file
random_samples.to_csv(out_file, index=False, header=False, float_format='%.6f')

print(f"Random samples have been written to {out_file}")

# Compute the convex hull using the entire dataset
df = df.dropna()
points = df.to_numpy()
hull = ConvexHull(points)

top_hull = hull.vertices[2:12]
x, y = points[top_hull].T

for _x, _y in zip(x, y):
    print(f"({_x}, {_y})")


# Plotting
plt.figure()
plt.plot(points[:, 0], points[:, 1], 'o')
for simplex in hull.simplices:
    plt.plot(points[simplex, 0], points[simplex, 1], 'k-')
plt.title('Convex Hull')
plt.show()