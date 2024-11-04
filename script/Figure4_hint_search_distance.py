import pandas as pd
import matplotlib.pyplot as plt

# Load the dataset
file_path = 'Figure4_hint_throughput.csv'
data = pd.read_csv(file_path)

# Unique datasets and hint choices
datasets = data['dataset'].unique()
hint_choices = data['hint choice'].unique()

# Color and hatching settings for hint choices
colors = ['#00a8e1', '#99cc00', '#e30039', '#800080']
hatches = ['-', 'x', 'o', '*']
# Set up a 2x2 plot layout
fig, axes = plt.subplots(2, 2, figsize=(16, 10), sharey=True)
axes = axes.flatten()

# Plot each dataset in a separate subplot with hatching patterns
for i, dataset in enumerate(datasets):
    ax = axes[i]
    subset = data[data['dataset'] == dataset]
    for j, hint in enumerate(hint_choices):
        # Get the subset for this hint choice
        hint_subset = subset[subset['hint choice'] == hint]
        ax.bar(hint, hint_subset['throughput(IOPS)'], color=colors[j], label=hint, hatch=hatches[j])

    ax.set_title(dataset, fontsize=20)
    ax.set_xlabel('Hint Choice', fontsize=16)
    ax.set_ylabel('Throughput (IOPS)', fontsize=14)
    ax.tick_params(axis='both', which='major', labelsize=16)
    ax.legend(fontsize=16)

# Adjust layout
plt.tight_layout()
plt.savefig('figures/Figure4_hint_throughput.png')