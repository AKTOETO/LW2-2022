#define _USE_MATH_DEFINES	// для PI

#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <chrono>
//#include <algorithm>	// для sort()

using namespace std;

// изменение размера динамического массива
void resize_arr(int*& arr, int& size, int inc = 1)
{
	int* temp = new int[size + inc];

	for (int i = 0; i < size; i++)
	{
		temp[i] = arr[i];
	}

	for (int i = 0; i < inc; i++)
	{
		temp[size + i] = 0;
	}

	arr = temp;
	size += inc;
}

// возвращение случайного значения из 
// интервала (min, max)
int rand_int_num(int min, int max)
{
	return rand() % (max - min) + min;
}

// рандомизация значений массива
void randomize_array(int* arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		arr[i] = rand_int_num(0, 50);
	}
}

// печать массива в поток
void print_arr(int* arr, int size, ostream& stream = cout)
{
	// вывод элементов массива
	for (int i = 0; i < size; i++)
	{
		stream << arr[i] << '\t';
	}
	cout << '\n';
}

// сортировка по не убыванию
void insertionSort(
	int* arr,			// исходный массив
	int size			// размер массива
)
{
	for (int i = 1; i < size; i++)
	{
		int j = i - 1;
		while (j >= 0 && arr[j] > arr[j + 1])
		{
			swap(arr[j], arr[j + 1]);
			j--;
		}
	}
}

// алгоритм Better Linear Search
int BLS(
	int* arr,			// исходный массив
	int size,			// размер массива
	int target,			// цель поиска
	int& num_of_comp	// количество сравнений
)
{
	for (int i = 0; i < size; i++)
	{
		num_of_comp++;
		if (arr[i] == target)
		{
			return i;
		}
	}
	return -1;
}

// алгоритм Sentinel Linear Search
int SLS(
	int* arr,			// исходный массив
	int size,			// размер массива
	int target,			// цель поиска
	int& num_of_comp	// количество сравнений
)
{
	// Last element of the array
	int last = arr[size - 1];

	// Element to be searched is
	// placed at the last index
	arr[size - 1] = target;
	int i = 0;

	while (arr[i] != target)
	{
		num_of_comp++;
		i++;
	}
	// сравнение, которое было при 
	// target > arr[i] = false
	num_of_comp++;

	// Put the last element back
	arr[size - 1] = last;

	num_of_comp++;
	if ((i < size - 1) || (arr[size - 1] == target))
	{
		return i;
	}
	return -1;
}

// алгоритм T
int T(
	int* arr,			// исходный массив
	int size,			// размер массива
	int target,			// цель поиска
	int& num_of_comp	// количество сравнений
)
{
	// добавление в конец массва фиктивной записи
	arr[size - 1] = INT_MAX;

	int i = 0;
	while (target > arr[i])
	{
		num_of_comp++;
		i++;
	}
	// сравнение, которое было при 
	// target > arr[i] = false
	num_of_comp++;

	if (target == arr[i])
	{
		num_of_comp++;
		return i;
	}

	return -1;
}

int prepare_for_T(
	int* arr,			// исходный массив
	int size,			// размер массива
	int target,			// цель поиска
	int& num_of_comp	// количество сравнений
)
{
	insertionSort(arr, size);
	resize_arr(arr, size);
	print_arr(arr, size);
	return T(arr, size, target, num_of_comp);
}


int main()
{
	setlocale(LC_ALL, "ru");

	srand(time(NULL));

	int size = 10;
	int* arr1 = new int[size];
	int num_of_comp = 0;

	int target = 60;
	int index_of_target = size / 2;

	randomize_array(arr1, size);

	// в ячейку под индексом index_of_target 
	// ставим значение target
	arr1[index_of_target] = target;

	print_arr(arr1, size);

	cout << "индекс элемента " << target << " в массиве = "
		<< BLS(arr1, size, target, num_of_comp)
		<< " | количество сравнений: " << num_of_comp << '\n';
	num_of_comp = 0;

	cout << "индекс элемента " << target << " в массиве = "
		<< SLS(arr1, size, target, num_of_comp)
		<< " | количество сравнений: " << num_of_comp << '\n';
	num_of_comp = 0;

	cout << "индекс элемента " << target << " в массиве = "
		<< prepare_for_T(arr1, size, target, num_of_comp)
		<< " | количество сравнений: " << num_of_comp << '\n';
	num_of_comp = 0;


	return 0;
}