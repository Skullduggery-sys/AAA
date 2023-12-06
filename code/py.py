import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# Загрузка данных из файла CSV
data = pd.read_csv('output.csv')

# Центроиды кластеров
centroids = [
    [204.213,54.7718,215.565],
    [67.5297,58.372,181.973],
    [82.3365,208.222,64.6316],
    [52.4887,179.298,179.471],
    [196.496,73.9446,105.323],
    [199.001,183.934,159.806],
    [115,70,18],
    [202,178,14],
    [181,241,248],
    [43,67,22]
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
