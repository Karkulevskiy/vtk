# VTK laboratory

## Сборка

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

```sh
cmake -S . -B build -DVTK_DIR=/path/to/lib/cmake/vtk-9.x
```

## Практическая часть: изоповерхность

Программа `ContourToFile` читает структурированные воксельные данные из `data/head.vti`, вычисляет диапазон скалярного поля, строит изоповерхность на среднем значении диапазона фильтром `vtkContourFilter` и записывает полигональную сетку в `head_contour.vtp`:

```sh
./build/ContourToFile data/head.vti head_contour.vtp
```

В результате программа выводит уровень изоповерхности, число точек и ячеек сетки.

Примеры с интерактивным окном запускаются из каталога `lab`, например:

```sh
./build/ContourFilter data/head.vti
```

## Задача 2: выделение плотных структур

Программа `HeadBone` отбирает в объёме узлы со значениями скаляра выше порога, затем извлекает поверхность отобранных ячеек и показывает её в окне VTK. Порог по умолчанию равен 2500; его можно задать третьим аргументом:

```sh
./build/HeadBone data/head.vti
./build/HeadBone data/head.vti 2200
```