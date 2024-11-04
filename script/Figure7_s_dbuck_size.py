import pandas as pd
import matplotlib.pyplot as plt
import os

# Load the updated data
file_path = 'Figure7_s_dbuck_size.csv'  # Replace with the path to your CSV file
data = pd.read_csv(file_path)

# Extract dataset name from key_path
data['dataset'] = data['key_path'].apply(lambda x: os.path.basename(x))

# Define font sizes
title_fontsize = 20
label_fontsize = 20
tick_fontsize = 18
legend_fontsize = 14

# Group data by dataset
dataset_grouped = data.groupby('dataset')

for dataset, dataset_group in dataset_grouped:
    # Group data by read_ratio and insert_ratio within each dataset
    sub_grouped = dataset_group.groupby(['read_ratio', 'insert_ratio'])

    # Determine the maximum throughput across all subgroups for consistent y-axis scaling
    max_throughput = dataset_group['throughput'].max()

    # Create subplots for each read_ratio and insert_ratio combination
    plt.figure(figsize=(8 * len(sub_grouped), 5))
    
    for i, ((read_ratio, insert_ratio), group) in enumerate(sub_grouped, 1):
        ax = plt.subplot(1, len(sub_grouped), i)
        
        # Plot for each bli_sbuck_size in the group
        markers = ['o', 's', '^', 'D', 'v', '<', '>', 'p', 'h', '8']  # Different marker styles
        unique_sbuck_sizes = group['bli_sbuck_size'].unique()
        for idx, sbuck_size in enumerate(unique_sbuck_sizes):
            subset = group[group['bli_sbuck_size'] == sbuck_size]
            x_ticks = range(len(subset))
            marker = markers[idx % len(markers)]  # Cycle through markers if more S-bucket sizes than markers
            ax.plot(x_ticks, subset['throughput'], marker=marker, linestyle='-', 
                   label=f'S-bucket size = {sbuck_size}')
            
            # Custom x-axis tick labels
            custom_labels = [f"{x//1024}K" if x < 1024**2 else f"{x//1024**2}M" for x in subset['bli_dbuck_size']]
            ax.set_xticks(x_ticks)
            ax.set_xticklabels(custom_labels, rotation='vertical', fontsize=tick_fontsize)

        ax.set_ylim(0, max_throughput)  # Set the y-axis limit
        ax.tick_params(axis='y', labelsize=tick_fontsize)
        ax.yaxis.get_offset_text().set_fontsize(tick_fontsize)
        ax.set_xlabel('D-bucket size', fontsize=label_fontsize)
        ax.set_ylabel('Throughput (QPS)', fontsize=label_fontsize)
        ax.set_title(f'{dataset} (Read: {read_ratio}, Insert: {insert_ratio})', fontsize=title_fontsize)
        # Only show legend for the first subplot
        if i == 1:
            ax.legend(fontsize=legend_fontsize)
        ax.grid(True)

    # Add this line before saving to adjust the layout
    plt.subplots_adjust(bottom=0.4)  # Adjust the bottom margin
    
    # Save the figure with a descriptive name for the dataset
    figure_name = f'figures/Figure7_s_dbuck_size_{dataset}.png'
    plt.savefig(figure_name)
    plt.close()
