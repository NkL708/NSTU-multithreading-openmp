#include <iostream>
#include <omp.h>

// Вывод всего массива в консоль
void print(int*& array, int size) {
    std::cout << std::endl;
    for (int i = 0; i < size; i++)
    {
        std::cout << array[i] << " ";
        if (!(i % 10) && (i > 0)) std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Ф-я для перемешивания массива
void swap(int*& array, int first, int second) {
    int temp = array[first];
    array[first] = array[second];
    array[second] = temp;
}

// Рекурсивная быстрая сортировка
void quickSort(int* array, int begin, int end)
{
    int i = begin;
    int j = end;
    int pivot = array[(i + j) / 2];
    int temp;
    while (i < j)
    {
        while (array[i] < pivot)
            i++;
        while (array[j] > pivot)
            j--;
        if (i <= j)
        {
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            i++;
            j--;
        }
    }
    if (j > begin)
        quickSort(array, begin, j);
    if (i < end)
        quickSort(array, i, end);
}

enum EstType { linear = 1, parallel = 2 };

int main() {
    setlocale(LC_ALL, "Russian");
    //srand(time(0));
    int size, type;
    // Таймер выполнения
    clock_t begin, end;
    std::cout << "1. Линейный \n2. Параллельный \nВыберите: ";
    std::cin >> type;
    std::cout << "Размер массива: ";
    std::cin >> size;
    // 250 000 000 самое показательное кол-во э-ов
    int* arr = new int[size];
    // Заполнение и перемешивание массива
    for (int i = 0; i < size; i++)
    {
        arr[i] = i;
    }
    for (int i = 0; i < size; i++)
    {
        swap(arr, i, (rand() % size));
    }
    begin = clock();
    // Параллельный алгоритм (выигрыш в производительности почти в 2 раза)
    if (type == EstType::parallel)
    {
        int count;
        int beginI, endI, prevI = -1;
#pragma omp parallel
        {
            beginI = prevI + 1;
            endI = (size - 1) / (omp_get_num_threads() - omp_get_thread_num());
            prevI = endI;
            quickSort(arr, beginI, endI);
        }
    }
    // Последовательный алгоритм
    else
        quickSort(arr, 0, size - 1);
    end = clock();
    // Вывод времени вычислений
    std::cout << std::endl << "Сортировка заняла " << ((double)end - begin) / (double)CLOCKS_PER_SEC << " секунд(ы)" << std::endl;
    system("Pause");
    return 0;
}