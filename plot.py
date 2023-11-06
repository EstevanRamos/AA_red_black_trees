import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Read the data from the CSV files
search_data = pd.read_csv('SearchTrees/BST_tree_data.csv')
rb_data = pd.read_csv('RedBlackTrees/RB_tree_data.csv')

# Extract columns
search_entries = search_data['Number of Entries']
search_height = search_data['Tree Height']
rb_entries = rb_data['Number of Entries']
rb_height = rb_data['Tree Height']

# Calculate the optimal height (log base 2 of entries)
optimal_height = np.log2(search_entries)

# Create a plot
plt.figure(figsize=(10, 6))
plt.plot(search_entries, search_height, label='Search Tree')
plt.plot(rb_entries, rb_height, label='RB Tree')
plt.plot(search_entries, optimal_height, label='Optimal Height', linestyle='--')

# Add labels and legend
plt.xlabel('Number of Entries')
plt.ylabel('Tree Height')
plt.legend()

# Show the plot
plt.title('Tree Height vs Number of Entries')
plt.grid(True)
plt.show()
