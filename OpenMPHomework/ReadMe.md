Многоугольник генерируется автоматически с заданным количеством вершин

Количество вершин передается через командную строку

Сгенерированный прямоугольник выводится в формате, удобном для его просмотра на сайте https://www.geogebra.org/geometry

Считается площадь в цикле от 1 до 16 потоков, результат выводится в виде markdown таблицы

Для небольших многоугольников прикладываю ссылки, по которым можно посмотреть, как они выглядят и убедиться, что площадь посчитана верно

В некоторых примерах немного отличается площадь - похоже на особенность арифметики чисел с плавающей точкой

## Результаты

### 4 вершины

https://www.geogebra.org/geometry/m3etpkur

| Thread count | Time (s) | Area |
|---|---|---|
| 1 | 0.000043 | 10.281903 |
| 2 | 0.000120 | 10.281903 |
| 3 | 0.000177 | 10.281903 |
| 4 | 0.000337 | 10.281903 |
| 5 | 0.000245 | 10.281903 |
| 6 | 0.000181 | 10.281903 |
| 7 | 0.000169 | 10.281903 |
| 8 | 0.000220 | 10.281903 |
| 9 | 0.000278 | 10.281903 |
| 10 | 0.000245 | 10.281903 |
| 11 | 0.000319 | 10.281903 |
| 12 | 0.000344 | 10.281903 |
| 13 | 0.000306 | 10.281903 |
| 14 | 0.000366 | 10.281903 |
| 15 | 0.000292 | 10.281903 |
| 16 | 0.000237 | 10.281903 |

Быстрее всего - в 1 поток: слишком мало вершин - больше времени уходит на создание потоков, чем на подсчет площади

### 100 вершин

https://www.geogebra.org/geometry/dh4xsvgu

| Thread count | Time (s) | Area |
|---|---|---|
| 1 | 0.000047 | 31285.717654 |
| 2 | 0.000175 | 31285.717654 |
| 3 | 0.000297 | 31285.717654 |
| 4 | 0.000243 | 31285.717654 |
| 5 | 0.000226 | 31285.717654 |
| 6 | 0.000221 | 31285.717654 |
| 7 | 0.000387 | 31285.717654 |
| 8 | 0.000235 | 31285.717654 |
| 9 | 0.000221 | 31285.717654 |
| 10 | 0.000338 | 31285.717654 |
| 11 | 0.000233 | 31285.717654 |
| 12 | 0.000310 | 31285.717654 |
| 13 | 0.000259 | 31285.717654 |
| 14 | 0.000279 | 31285.717654 |
| 15 | 0.000271 | 31285.717654 |
| 16 | 0.000238 | 31285.717654 |

Все еще быстрее в 1 поток

### 10 000 вершин

| Thread count | Time (s) | Area |
|---|---|---|
| 1 | 0.000634 | 314159144.559855 |
| 2 | 0.000484 | 314159144.559855 |
| 3 | 0.000190 | 314159144.559856 |
| 4 | 0.000586 | 314159144.559856 |
| 5 | 0.000511 | 314159144.559856 |
| 6 | 0.000251 | 314159144.559856 |
| 7 | 0.000340 | 314159144.559856 |
| 8 | 0.000444 | 314159144.559856 |
| 9 | 0.000572 | 314159144.559856 |
| 10 | 0.000571 | 314159144.559855 |
| 11 | 0.000482 | 314159144.559856 |
| 12 | 0.000710 | 314159144.559856 |
| 13 | 0.000410 | 314159144.559856 |
| 14 | 0.000450 | 314159144.559856 |
| 15 | 0.000261 | 314159144.559856 |
| 16 | 0.000344 | 314159144.559856 |

Быстрее всего - 3 потока: Данных уже достаточно для того, чтобы использование параллельных вычислений имело смысл

### 1 000 000 вершин

| Thread count | Time (s) | Area |
|---|---|---|
| 1 | 0.010070 | 3141592653465.262695 |
| 2 | 0.005246 | 3141592653465.271484 |
| 3 | 0.003594 | 3141592653465.246094 |
| 4 | 0.003391 | 3141592653465.231445 |
| 5 | 0.002862 | 3141592653465.258789 |
| 6 | 0.002326 | 3141592653465.241699 |
| 7 | 0.002405 | 3141592653465.241211 |
| 8 | 0.002763 | 3141592653465.275391 |
| 9 | 0.002727 | 3141592653465.258789 |
| 10 | 0.002700 | 3141592653465.236816 |
| 11 | 0.002855 | 3141592653465.248535 |
| 12 | 0.002698 | 3141592653465.255859 |
| 13 | 0.002579 | 3141592653465.257812 |
| 14 | 0.002674 | 3141592653465.255859 |
| 15 | 0.002332 | 3141592653465.252930 |
| 16 | 0.002250 | 3141592653465.250488 |

Быстрее всего - 16 потоков: данных достаточно, чтобы использовать максимум потоков
