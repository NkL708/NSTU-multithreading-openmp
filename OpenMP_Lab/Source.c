#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// Вывод всего массива в файл
void print(int* array, int size, FILE* file) 
{
    for (int i = 0; i < size; i++)
    {
        fprintf(file, "%d ", array[i]);
        if (!(i % 10) && (i > 0))
            fprintf(file, "\n");
    }
}

// Перестановка элементов
void swap(int* first, int* second) 
{
    int temp = *first;
    *first = *second;
    *second = temp;
}

// Рекурсивная быстрая сортировка
void quickSort(int* array, int begin, int end)
{
    int i = begin;
    int j = end;
    int pivot = array[(i + j) / 2];
    while (i < j)
    {
        while (array[i] < pivot)
            i++;
        while (array[j] > pivot)
            j--;
        if (i <= j)
        {
            swap(&array[i], &array[j]);
            i++;
            j--;
        }
    }
    if (j > begin)
        quickSort(array, begin, j);
    if (i < end)
        quickSort(array, i, end);
}

int main() {
    int size, type;
    // Таймер выполнения и файлы с результатами
    clock_t begin, end;
    FILE* shuffled, * sorted;
    shuffled = fopen("shuffled.txt", "w");
    sorted = fopen("sorted.txt", "w");
    printf("1. Serial \n2. Parallel \nChoose: ");
    scanf("%d", &type);
    printf("Size of array: ");
    scanf("%d", &size);
    int* arr = (int*)malloc(size * sizeof(int));
    // Заполнение и перемешивание массива
    for (int i = 0; i < size; i++)
    {
        arr[i] = i;
    }
    for (int i = 0; i < size; i++)
    {
        swap(&arr[i], &arr[(rand() % size)]);
    }
    print(arr, size, shuffled);
    begin = clock();
    // Параллельный алгоритм (выигрыш в производительности почти в 2 раза)
    if (type == 2)
    {
#pragma omp parallel sections shared(arr)
        {
#pragma omp section
            {
                printf("%d ", omp_get_thread_num());
                quickSort(arr, 0, size / 2);
            }
#pragma omp section
            {
                printf("%d ", omp_get_thread_num());
                quickSort(arr, (size / 2) + 1, size - 1);
            }
        }
    }
    // Последовательный алгоритм
    else
        quickSort(arr, 0, size - 1);
    end = clock();
    // Вывод времени вычислений
    print(arr, size, sorted);
    printf("\nTime spent %f\n", ((double)end - begin) / (double)CLOCKS_PER_SEC);
    free(arr);
    if (shuffled) fclose(shuffled);
    if (sorted) fclose(sorted);
    return 0;
}