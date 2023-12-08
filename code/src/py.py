import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Загрузка данных из файла CSV
data = pd.read_csv('output.csv')

# Центроиды кластеров
centroids = [
    [169,128,107],
    [151.898,128.862,112.022],
    [91.6751,127.614,136.351]
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
