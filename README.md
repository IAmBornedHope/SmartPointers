# SmartPointers
Выполнил: Беляцкий Сергей (Б25-511)
Домашняя работа №1 - учебная реализация умных указателей (UniquePtr, SharedPtr)
Для сборки проекта используется CMake
Тесты написаны на GoogleTest

Время работы:

========== N = 1000 ==========

Operation                 Raw      std::         My
------------------------------------------------
unique create           0.159      0.402      0.226
unique move                 -      0.278      0.194
unique reset                -      0.170      0.063
shared create               -      0.427      0.333
shared copy                 -      0.105      0.083
shared move                 -      0.382      0.217
shared reset                -      0.264      0.088

========== N = 1000000 ==========

Operation                 Raw      std::         My
------------------------------------------------
unique create          73.400    171.423    129.797
unique move                 -    110.221     62.293
unique reset                -     47.706     15.487
shared create               -    174.401    136.921
shared copy                 -     47.185     32.657
shared move                 -    109.470     98.024
shared reset                -     85.473     35.648

Отчет Valgrind:
![alt text](valgrind_review.png)