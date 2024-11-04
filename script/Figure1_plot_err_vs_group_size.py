import numpy as np
import matplotlib.pyplot as plt

datasets = ['books', 'fb', 'osm']


def compute_linear_regression(data):
    x = data
    y = np.arange(1, len(data)+1)
    A = np.vstack([x, np.ones(len(x))]).T
    m, c = np.linalg.lstsq(A, y, rcond=None)[0]
    return m, c

def compute_error(data, k):
    m, c = compute_linear_regression(data)
    errors = []
    for i in range(0, len(data), k):
        errors.append(abs(data[i] - (m*i + c)) / k)
    return np.mean(errors)

for dataset in datasets:
    # read the file
    data = np.fromfile('../datasets/' + dataset, dtype=np.uint64, count=10000)

    # sort the data
    data = np.sort(data)

    errors = []
    for k in range(1, 21):
        errors.append(compute_error(data, k)/1e12)


    plt.figure(figsize=(8, 4))
    plt.plot(range(1, 21), errors)
    plt.xlabel('Group Size', fontsize=14)
    plt.ylabel('Average Error (x 1e12)', fontsize=14)

    plt.xticks(range(2, 21, 2), fontsize=15)
    plt.yticks(fontsize=15)

    plt.title('Prediction Error vs Group Size on Books', fontsize=15, pad=20)

    plt.tight_layout()
    plt.savefig('figures/Figure1_' + dataset + '_err_vs_group_size.png')
