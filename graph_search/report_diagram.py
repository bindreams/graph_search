import matplotlib.pyplot as plt
import numpy as np
from collections import defaultdict
import json

def configure_xticks(xdata):
	xint = []
	locs, labels = plt.xticks()
	for each in locs:
		xint.append(int(each))
	plt.xticks(xint)

	minimum = min(xdata[next(iter(xdata))])
	maximum = max(xdata[next(iter(xdata))])
	for key in xdata:
		minimum = min(minimum, min(xdata[key]))
		maximum = max(maximum, max(xdata[key]))

	plt.xlim(int(minimum-1), int(maximum+1))

def report1(json_data, k):
	xdata = defaultdict(list)
	ydata1 = defaultdict(list)
	ydata2 = defaultdict(list)
	
	for entry in json_data:
		if entry["puff_depth"] == k:
			xdata [str(entry["graph_ratio"])].append(int(entry["graph_size"]))
			ydata1[str(entry["graph_ratio"])].append(int(entry["puff_sectors"])**(1/k))
			ydata2[str(entry["graph_ratio"])].append(int(entry["puff_edges"])**(1/k))
		
	plt.suptitle("Max puff depth (k): {}".format(k))
		
	# Subplot 1
	plt.subplot(1, 2, 1)
	#  Add data
	for key in xdata:
		plt.plot(xdata[key], ydata1[key], label=("p = " + key))
	#  Grid and legend
	plt.grid(True)
	plt.legend()
	#  Make xticks integral and set limits
	configure_xticks(xdata)
	#  Labels
	plt.xlabel('graph size')
	plt.ylabel('k-th root of puff sectors')

	# Subplot 1
	plt.subplot(1, 2, 2)
	#  Add data
	for key in xdata:
		plt.plot(xdata[key], ydata2[key], label=("p = " + key))
	#  Grid
	plt.grid(True)
	plt.legend()
	#  Make xticks integral and set limits
	configure_xticks(xdata)
	#  Labels
	plt.xlabel('graph size')
	plt.ylabel('k-th root of puff edges')
	
f = open("report.json")
data = json.load(f);

for k in [5, 7, 10, 15]:
	plt.figure(figsize=(20, 10))
	report1(data, k);
	plt.savefig("plot-k{}.png".format(k))
	plt.show()