import matplotlib.pyplot as plt
import pandas as pd
from scipy.spatial import ConvexHull, convex_hull_plot_2d
import numpy as np

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Read data from CSV
df = pd.read_csv("out.csv", header=None)

# Find the center of the data
center_x = df[10].mean()
center_y = df[11].mean()

# Calculate the slope and intercept for the line passing through the center of the data
slope = (0.938 - 0.883) / (df[10].max() - df[10].min())
intercept = center_y - slope * center_x

# Calculate y values for the red line based on the x values
x_values = np.array([df[10].min(), df[10].max()])
y_values = slope * x_values + intercept

# Plot the scatter plot
plt.scatter(df[10], df[11])

# Plot the red line passing through the center of the data
plt.plot(x_values, y_values, color="red")

# Save the plot as an image
plt.savefig("plot.png")

# Close the plot
plt.close()
 # Close the plot to avoid displaying it in the notebook

print(df)

points = np.array([df[10], df[11]]).T
print(points)

hull = ConvexHull(points)
print(points[hull.vertices])

x, y = points[hull.vertices].T
plt.scatter(x, y)
convex_hull_plot_2d(hull)
plt.savefig("convex_hull.png")  # Save the convex hull plot as an image
plt.close()  # Close the plot to avoid displaying it in the notebook

tophull = hull.vertices[2:10]

# Scatter plot with the same color for all top_haul points
x, y = points[tophull].T
plt.scatter(x, y, color='blue')  # You can choose any color you prefer

# Save the plot as an image
plt.savefig("tophull_points.png")

# Close the plot to avoid displaying it in the notebook
plt.close()

# Assuming you have a DataFrame 'df' with your data
print(df.iloc[tophull])