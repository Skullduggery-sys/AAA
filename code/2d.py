import pandas as pd
import matplotlib.pyplot as plt

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

# Создание scatter plot
plt.figure(figsize=(10, 8))

# Визуализация точек данных
for i in range(len(data)):
    color = (data['r'][i] / 255, data['g'][i] / 255, data['b'][i] / 255)  # Нормализация до диапазона [0, 1]
    plt.scatter(data['r'][i], data['g'][i], color=color)

# Визуализация центроидов
for i, centroid in enumerate(centroids):
    normalized_color = (centroid[0] / 255, centroid[1] / 255, centroid[2] / 255)  # Нормализация до диапазона [0, 1]
    plt.scatter(centroid[0], centroid[1], marker='X', color=normalized_color, edgecolors='black', s=200)

# Настройка осей и меток
plt.xlabel('Red')
plt.ylabel('Green')
plt.title('Fuzzy C-means Clustering')

# Отображение графика
plt.show()
