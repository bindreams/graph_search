import numpy as np
from mpl_toolkits.mplot3d import Axes3D # This import has side effects required for the kwarg projection='3d' in the call to fig.add_subplot
import matplotlib.pyplot as plt
import json

# Return sectors for a given graph_size and puff_depth
def puff_sectors(puff_depth, graph_size):
	f = open("report.json")
	data = json.load(f);

	for entry in data:
		if (entry['graph_size'] == graph_size and
			entry['max_puff_depth'] == puff_depth):
			return entry['puff_sectors']**(1/puff_depth)
		
	raise ValueError('did not find data in file')
  
# Build the plot
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
max_depths = np.arange(3, 9+1, 2)
sizes = np.arange(3, 25+1, 2)
X, Y = np.meshgrid(max_depths, sizes)
zs = np.array([puff_sectors(puff_depth, graph_size) for puff_depth, graph_size in zip(np.ravel(X), np.ravel(Y))])
Z = zs.reshape(X.shape)

ax.plot_wireframe(X, Y, Z)

ax.set_xlabel('maximum puff depth (k)')
ax.set_ylabel('graph size (N)')
ax.set_zlabel('k-th root of puff sectors')

plt.show()