import matplotlib.pyplot as plt
import pandas as pd

# Загрузка данных из CSV файла
file_path = 'src/time.csv'
data = pd.read_csv(file_path)

# Деление значений во всех столбцах, кроме первого, на 1000
#data.iloc[:, 1:] /= 1000

# Извлечение данных из столбцов
x_values = data['len']
y_columns = ['0','1','2','4','8','16','32','64']
labels = ['Последовательный алгоритм', '1 поток', '2 потока', '4', '8', '16', '32', '64']
markers = ['o','>','s','<', 'v', '1','2','3','4','8']
# Построение графиков с логарифмической шкалой по y
for i, column in enumerate(y_columns):
    y_values = data[column]
    plt.plot(x_values, y_values, label=y_columns[i], marker=markers[i])

plt.yscale('log')  # Установка логарифмической шкалы по y

# Добавление подписей и заголовка графика
plt.xlabel('Размерность матрицы')
plt.ylabel('Время, мкс')
plt.title('Зависимость времени выполнения от количества элементов в матрице')

# Добавление легенды
plt.legend()

# Отображение графика
plt.show()
