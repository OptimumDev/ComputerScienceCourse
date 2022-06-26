Решения находятся в папке Fixed

---

# Первая задача
Ускорить выполнение цикла for в программе, вычисляющей
покоординатную функцию от элементов массива a: a[i]=F(a[i])

Вывод программы:
```
Serial time: 80.467892
0.000000 1.000000 1.414214 1.732051 2.000000 2.236068 2.449490 2.645751 2.828427 3.000000 3.162278 3.316625 3.464102 3.605551 3.741657 3.872983 4.000000 4.123106 4.242641 4.358899 4.472136 4.582576 4.690416 4.795832 4.898979 5.000000 
Parallel time: 15.797940
0.000000 1.000000 1.189207 1.316074 1.414214 1.495349 1.565085 1.626577 1.681793 1.732051 1.778279 1.821160 1.861210 1.898829 1.934336 1.967990 2.000000 2.030543 2.059767 2.087798 2.114743 2.140695 2.165737 2.189939 2.213364 2.236068
```

 1. Время выполнения программы уменьшилось в 5,33 раза
 2. Первые 25 чисел ускоренной программы являются ожидаемым результатом (квадратный корень от результата работы однопоточной программы)

---

# Вторая задача
Элементы массива a инициализируются 0, массива b[0]…b[n-1] –
случайными числами от 0 до n-1. Распараллелить цикл
for (i=0; i<n; i++) a[b[i]]++

Вывод программы:
```
Thread count is: 8
Serial time: 0.027130
Parallel time: 0.004907
OK!
```

1. Время выполнения программы уменьшилось в 5,53 раза
2. Результат ускоренной программы совпадает с результатом однопоточной программы

---

# Третья задача
Распараллелить цикл вычисления суммы
for (i=0; i<n; i++) sum+=F(i);

Вывод программы:
```
Threads count is: 8
Serial time: 10.741860
Serial sum: 0.459698
Parallel time: 2.731727
Parallel sum: 0.459698
```

1. Время выполнения программы уменьшилось в 3,93 раза
2. Результат ускоренной программы совпадает с результатом однопоточной программы

---

# Четвертая задача
Написать программу, вычисляющую количество простых чисел в
диапазоне от 2 до N

Вывод программы:
```
Threads count is: 8
Serial prime numbers: 9592
Serial time: 10.964503
Parallel prime numbers: 9592
Parallel time: 3.662426
```

1. Время выполнения программы уменьшилось в 2,99 раза
2. Результат ускоренной программы совпадает с результатом однопоточной программы