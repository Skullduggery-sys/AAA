import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Загрузка данных из файла CSV
data = pd.read_csv('output.csv')

# Центроиды кластеров
centroids = [
    [227.588,173.423,222.612],
    [200.323,216.936,28.3991],
    [38.4931,96.0597,208.854],
    [97.2524,107.106,213.51],
    [84.6451,20.9024,41.8317],
    [116.397,30.4445,176.458],
    [231.539,163.893,123.383],
    [16.9664,144.022,138.469],
    [184.02,76.892,76.7036],
    [179.898,228.08,222.7],
    [122.847,155.941,102.535],
    [32.6688,230.378,229.046],
    [201.556,20.6295,232.519],
    [179.06,46.9029,153.439],
    [180.06,224.463,150.669],
    [32.6806,140.022,47.3314],
    [48.9485,204.597,83.2199],
    [16.006,30.7612,150.037],
    [213.893,110.18,212.428],
    [213.519,24.4814,17.7521]
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
