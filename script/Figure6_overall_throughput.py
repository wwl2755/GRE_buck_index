import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.ticker as mticker

# Load the CSV file
file_path = 'Figure6_overall.csv'
data = pd.read_csv(file_path)

# divide the throughput by 1e6
data['throughput'] = data['throughput'] / 1e6

# Extract relevant columns
data['workload'] = (data['read_ratio'].astype(float) * 100).astype(int).astype(str) + '%'
data['dataset'] = data['key_path'].apply(lambda x: x.split('/')[-1])

# Filter data for each dataset
books_data = data[data['dataset'] == 'books']
fb_data = data[data['dataset'] == 'fb']
osm_data = data[data['dataset'] == 'osm']

# Function to change index_type names
def change_index_type_name(name):
    if name == 'buckindex':
        return 'BLI'
    elif name == 'finedex':
        return 'FINEdex'
    else:
        return name.upper()


# Function to plot on a given axis
def plot_throughput_with_markers_and_colors(ax, dataset_data, title, draw_legend=True):
    # Apply the function to the index_type column
    dataset_data['index_type'] = dataset_data['index_type'].apply(change_index_type_name)
    markers = ['o', 's', '^', '*', 'D', 'p', 'h', '>']  # Markers
    colors = plt.cm.viridis(np.linspace(0, 1, len(dataset_data['index_type'].unique())))  # Colors
    for index_type, marker, color in zip(dataset_data['index_type'].unique(), markers, colors):
        subset = dataset_data[dataset_data['index_type'] == index_type]
        ax.plot(subset['workload'], subset['throughput'], label=index_type, marker=marker, color=color, linewidth=7, markersize=36)
    ax.set_xlabel('Read Ratio', fontsize=36)
    ax.set_ylabel('Throughput (M op/s)', fontsize=36)
    ax.set_title(title, fontsize=36)
    ax.tick_params(axis='x', labelsize=36)
    ax.tick_params(axis='y', labelsize=36)
    labels = [item.get_text() for item in ax.get_xticklabels()]
    ax.set_xticklabels(labels, rotation=90)
    # Set the y-axis label size for scientific notation
    ax.yaxis.get_offset_text().set_fontsize(30)
    if draw_legend:
        ax.legend(fontsize=30, loc='best')
    ax.grid(True)


# Create a figure with three subplots
fig, axs = plt.subplots(3, 1, figsize=(28, 25))  # Adjust the size as needed

# Generate and save the charts
plot_throughput_with_markers_and_colors(axs[0], books_data, 'books', draw_legend=True)
plot_throughput_with_markers_and_colors(axs[1], fb_data, 'fb', draw_legend=False)
plot_throughput_with_markers_and_colors(axs[2], osm_data, 'osm', draw_legend=False)

plt.tight_layout()  # Adjust the layout
plt.savefig('figures/Figure6_overall_throughput.png')
