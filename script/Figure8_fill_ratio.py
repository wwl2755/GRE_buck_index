import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('Figure8_fill_ratio.csv')

# Filter data for books dataset
books_data = df[df['key_path'] == '../datasets/books']

# Create the line chart
plt.figure(figsize=(6, 2))
plt.plot(books_data['bli_initial_filled_ratio'], 
         books_data['throughput'] / 1000000, 
         marker='o',
         linewidth=2,
         markersize=8)

# Customize the chart
plt.xlabel('Initial fill ratio')
plt.ylabel('Throughput (M op/s)')
plt.grid(True, linestyle='--', alpha=0.7)

# Format axes
plt.xticks(books_data['bli_initial_filled_ratio'])
plt.ylim(top=4.30)  # Force y-axis range from 0 to 4.50

# Add value labels on points
for x, y in zip(books_data['bli_initial_filled_ratio'], books_data['throughput'] / 1000000):
    plt.annotate(f'{y:.2f}', 
                (x, y),
                textcoords="offset points",
                xytext=(4, 8),
                ha='center')

plt.tight_layout()
plt.savefig('figures/Figure8_fill_ratio.png')