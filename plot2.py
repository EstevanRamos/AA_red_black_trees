import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

search_data = pd.read_csv('SearchTrees/BST_tree_data_removal.csv')
rb_data = pd.read_csv('RedBlackTrees/RB_tree_data_removal.csv')

search_data = search_data.iloc[::-1]
rb_data = rb_data.iloc[::-1]

plt.figure(figsize=(10, 5))
plt.plot(search_data['Number of Entries'], search_data['Tree Height'], label='BST', color='blue', marker='o')

plt.plot(rb_data['Number of Entries'], rb_data['Tree Height'], label='Red-Black Tree', color='red', marker='x')
plt.plot(search_data['Number of Entries'], np.log2(search_data['Number of Entries']), label='log2(entries)', linestyle='--', color='green')

plt.title('Height vs. Entries for BST and Red-Black Tree (Removal)')
plt.xlabel('Number of Entries')
plt.ylabel('Tree Height')
plt.gca().invert_xaxis()
plt.grid(True)
plt.legend()
plt.show()
