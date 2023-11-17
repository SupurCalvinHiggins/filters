import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("out.csv", header=None)
print(df.head())
idx = df[df[11] == df[11].min()].index
df.drop(idx, inplace=True)

# plt.scatter(df[10], df[11])
plt.plot([3, 4], [0.884, 0.930], color="red")
# plt.show()
print(df)
from scipy.spatial import ConvexHull, convex_hull_plot_2d
import numpy as np
points = np.array([df[10], df[11]]).T
print(points)
hull = ConvexHull(points)
print(points[hull.vertices])
x, y = points[hull.vertices].T
# plt.scatter(x, y)
# convex_hull_plot_2d(hull)
# plt.show()

tophull = hull.vertices[2:10]
print(points[tophull])

x, y = points[tophull].T

plt.scatter(x, y)
plt.show()

print(df.iloc[tophull])