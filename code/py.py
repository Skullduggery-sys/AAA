import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Загрузка данных из файла CSV
data = pd.read_csv('output.csv')

# Центроиды кластеров
centroids = [
    [94,154,67],
    [111,151,96],
    [95,162,92],
    [98,157,54],
    [98,157,54],
    [146,127,142],
    [121,130,148],
    [116,151,86],
    [106,120,132],
    [107,149,103],
    [126.476,131.967,135.769],
    [122.247,133.629,126.473],
    [123.265,131.671,124.135],
    [119.67,131.636,120.851],
    [125.215,126.697,124.792],
    [127.779,129.491,126.828],
    [124.994,130.915,124.977],
    [123.177,131.435,120.716],
    [123.5,127.882,128.235],
    [132.641,126.069,131.837]
]

# Создание 3D scatter plot
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# Визуализация точек данных
for i in range(len(data)):
    color = (data['r'][i] / 255, data['g'][i] / 255, data['b'][i] / 255)  # Нормализация до диапазона [0, 1]
    ax.scatter(data['r'][i], data['g'][i], data['b'][i], c=[color], marker='o')

# Визуализация центроидов
for i, centroid in enumerate(centroids):
    normalized_color = (centroid[0] / 255, centroid[1] / 255, centroid[2] / 255)  # Нормализация до диапазона [0, 1]
    ax.scatter(centroid[0], centroid[1], centroid[2], s=200, marker='X', c=[normalized_color], edgecolors='black')

# Настройка осей и меток
ax.set_xlabel('Red')
ax.set_ylabel('Green')
ax.set_zlabel('Blue')
ax.set_title('Fuzzy C-means Clustering')

# Отображение графика
plt.show()
