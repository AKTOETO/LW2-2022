/***********************************************************************\
*                    кафедра № 304 2 курс 3 семестр информатика			*
*-----------------------------------------------------------------------*
*	Project type : solution												*
*	Project name : LW2													*
*	File name    : LW2.cpp												*
*	Language     : c/c++												*
*	Programmers  : Плоцкий Б.А. Раужев Ю. М.							*
*	Created      :  4/10/22												*
*	Last revision:  7/10/22												*
*	Comment(s)   : 														*
*																		*
*	Для алгоритмов BLS и SLS в качестве входного массива использовать  	*
*	одну и ту же последовательность значений (функция rand( )).			*
*																		*
*	Для алгоритмов Т и В – значения массива должны быть отсортированы	*
*	по не убыванию, одна и та же последовательность чисел (можно		*
*	использовать соответствующую функцию из первой лабораторной работы).*
*																		*
*	Оценить длительность поиска для различных значений размеров			*
*	последовательностей (начиная с 10000 до 200000 элементов массива,	*
*	провести измерения не менее, чем для 10 разных размерностей).		*
*	Для каждой размерности рассматриваются случаи нахождения ключа		*
*	поиска в начале, в середине и в конце массива.						*
*																		*
*	Для алгоритмов BLS и SLS кроме подсчета времени, необходимого для	*
*	поиска, требуется определить сколько раз выполняются операции 		*
*	сравнения (сравнение ключа с элементом массива, а также в одном		*
*	из этих двух алгоритмов добавляется подсчет сравнений при анализе 	*
*	индекса элемента массива в цикле… ).								*
*																		*
*	Все результаты оформить в виде таблиц и графиков. На графиках -		*
*	только временные характеристики поиска. 							*
*	По результатам сделать выводы об эффективности того или иного		*
*	алгоритма поиска.													*
*																		*
\***********************************************************************/

#include <iostream>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <algorithm>	// для функции sort

using namespace std;

// тип данных NS
typedef chrono::nanoseconds NS;

// минимальное и максимальное значение
// для чисел в массиве
#define MIN_VALUE 0
#define MAX_VALUE 500

// минимальный и максимальный
// размер массива
#define MAX_ARR_SIZE 1000
#define MIN_ARR_SIZE 1

/****************************************************************
*              П Р О Т О Т И П Ы   Ф У Н К Ц И Й                *
****************************************************************/

// изменение размера динамического массива
void resize_arr(int*& arr, int& size, int inc = 1);

// возвращение случайного значения из 
// интервала (min, max)
int rand_int_num(int min, int max);

// рандомизация значений массива
void randomize_array(int* arr, int size);

// печать массива в поток
void print_arr(int* arr, int size, ostream& stream = cout);

// сортировка по не убыванию
void insertionSort(
	int* arr,			// исходный массив
	int size			// размер массива
);

// алгоритм Better Linear Search
int BLS(
	int* arr,			// исходный массив
	int size,			// размер массива
	int target,			// цель поиска
	int& num_of_comp	// количество сравнений
);

// алгоритм Sentinel Linear Search
int SLS(
	int* arr,			// исходный массив
	int size,			// размер массива
	int target,			// цель поиска
	int& num_of_comp	// количество сравнений
);

// функция, которая сортирует массив и 
// добавляет фиктивный элемент в конец 
void support_T(
	int*& arr,			// исходный массив
	int& size			// размер массива
);

// алгоритм T
int T(
	int* arr,			// исходный массив
	int size,			// размер массива
	int target,			// цель поиска
	int& num_of_comp	// количество сравнений
);

// функция, которая сортирует массив и 
// убирает с конца фиктивный элемент
void support_B(
	int*& arr,			// исходный массив
	int& size			// размер массива
);

// алгоритм бинарного поиска
int B(
	int* arr,			// исходный массив
	int size,			// размер массива
	int target,			// цель поиска
	int& num_of_comp	// количество сравнений
);

// функция измерения времени выполнения функций
int measure_time(
	int* arr,				// исходный массив
	int size,				// размер массива
	int target,				// цель поиска
	int& num_of_comp,		// количество сравнений
	NS& time,				// время работы функции
	int (*search_func)(		// функция поиска
		int* arr,			// исходный массив
		int size,			// размер массива
		int target,			// цель поиска
		int& num_of_comp	// количество сравнений
		),
	void (*support_func)(	// вспомогательная функция
							// нужна для T и B алгоритмов
		int*& arr,			// исходный массив
		int& size			// размер массива		
		)
);

// ввод и проверка значений
template<typename T>
T input_and_check(T _min, T _max,
	const char* welcome_str, const char* err_str);

/****************************************************************
*                Г Л А В Н А Я   Ф У Н К Ц И Я                  *
****************************************************************/

int main()
{
	setlocale(LC_ALL, "ru");

	srand(time(NULL));

	// запрашиваем у пользователя размер массива
	int size = input_and_check(MIN_ARR_SIZE, MAX_ARR_SIZE,
		"Введите размер массива ", "Интервал");

	int* arr = new int[size];	// массив
	int num_of_comp = 0;		// количество сравнений	
	NS elapsed_time;			// время выполнения функции
	int target = 0;				// элемент поиска

	// рандомизация массива
	randomize_array(arr, size);

	// массив с вспомогательными функциями
	void (*sup_funcs[])(int*&, int&) =
	{
		[](int*&, int&) {},
		[](int*&, int&) {},
		support_T,
		support_B
	};

	// массив с функциями поиска
	int (*search_funcs[])(int*, int, int, int&) =
	{
		BLS,
		SLS,
		T,
		B
	};

	// названия алгоритмов
	const char* algorithms[] =
	{
		"BLS",
		"SLS",
		"T",
		"B"
	};

	// вывод массива
	print_arr(arr, size);

	// запрос у пользователя элемета для поиска
	target = input_and_check(MIN_VALUE, MAX_VALUE,
		"Введите элемент для поиска ", "Интервал ");

	cout << "| Алгоритм | ключ | индекс ключа| количество сравнений | время выполнения(нс) |\n";
	// вызов всех функций поиска
	for (int i = 0; i < 4; i++)
	{
		// получение индекса элемента в массиве
		int index = measure_time(arr, size, target, num_of_comp,
			elapsed_time, search_funcs[i], sup_funcs[i]);
		// вывод строки таблицы
		cout << "| " << fixed << setfill(' ') << setw(8) << algorithms[i]
			<< " | " << fixed << setfill(' ') << setw(4) << target
			<< " | " << fixed << setfill(' ') << setw(11) << index
			<< " | " << fixed << setfill(' ') << setw(20) << num_of_comp
			<< " | " << fixed << setfill(' ') << setw(20) << elapsed_time.count() << " |\n";
	}

	// удаление массива
	delete[] arr;

	return 0;
}

/****************************************************************
*             Р Е А Л И З А Ц И Я   Ф У Н К Ц И Й               *
****************************************************************/

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
		arr[i] = rand_int_num(MIN_VALUE, MAX_VALUE);
	}
}

// печать массива в поток
void print_arr(int* arr, int size, ostream& stream = cout)
{
	// вывод элементов массива
	for (int i = 0; i < size; i++)
	{
		stream << arr[i] << ' ';
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
		num_of_comp += 2;
		if (arr[i] == target)
		{
			return i;
		}
	}
	num_of_comp++;
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

// функция, которая сортирует массив и 
// добавляет фиктивный элемент в конец 
void support_T(
	int*& arr,			// исходный массив
	int& size			// размер массива
)
{
	// сортировка массива 
	//insertionSort(arr, size);
	sort(arr, arr + size);

	// добавление фиктивного значения в конец массива
	// если еще нет фиктивного элемента
	if (arr[size - 1] != INT_MAX)
	{
		resize_arr(arr, size);
	}
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

	num_of_comp++;
	if (target == arr[i])
	{
		return i;
	}

	return -1;
}

// функция, которая сортирует массив и 
// убирает с конца фиктивный элемент
void support_B(
	int*& arr,			// исходный массив
	int& size			// размер массива
)
{
	// сортировка массива
	insertionSort(arr, size);

	// убирание последнего элемента из массива,
	// если он является фиктивным
	if (arr[size - 1] == INT_MAX)
	{
		resize_arr(arr, size, -1);
	}
}

// алгоритм бинарного поиска
int B(
	int* arr,			// исходный массив
	int size,			// размер массива
	int target,			// цель поиска
	int& num_of_comp	// количество сравнений
)
{
	// левая граница
	int left = 0;
	// правая граница
	int right = size - 1;
	// индекс среднего элемента
	int mid;

	while (left <= right)
	{
		num_of_comp++;

		mid = (left + right) / 2;
		num_of_comp++;
		if (arr[mid] == target)
		{
			return mid;
		}
		else
		{
			num_of_comp++;
			if (arr[mid] > target)
			{
				right = mid - 1;
			}
			else
			{
				left = mid + 1;
			}
		}
	}
	num_of_comp++;
	return -1;
}

// функция измерения времени выполнения функций
int measure_time(
	int* arr,				// исходный массив
	int size,				// размер массива
	int target,				// цель поиска
	int& num_of_comp,		// количество сравнений
	NS& time,				// время работы функции
	int (*search_func)(		// функция поиска
		int* arr,			// исходный массив
		int size,			// размер массива
		int target,			// цель поиска
		int& num_of_comp	// количество сравнений
		),
	void (*support_func)(	// вспомогательная функция
							// нужна для T и B алгоритмов
		int*& arr,			// исходный массив
		int& size			// размер массива		
		)
)
{
	// обнуление количества сравнений
	num_of_comp = 0;

	// вызов вспомогательной функции
	support_func(arr, size);

	// начало отсчета времени
	auto begin = chrono::steady_clock::now();

	// вызов функции поиска
	int res = search_func(arr, size, target, num_of_comp);

	// конец отсчета времени
	auto end = chrono::steady_clock::now();

	// вывод в консоль времени работы программы
	time = chrono::duration_cast<chrono::nanoseconds>(end - begin);

	return res;
}

// ввод и проверка значений
template<typename T>
T input_and_check(T _min, T _max,
	const char* welcome_str, const char* err_str)
{
	// размер массива
	T num;

	// вывод сообщения
	cout << welcome_str
		<< " (" << _min << ";" << _max << ")\n";
	cin >> num;

	// если было введено не то
	if (num > _max || num < _min) {
		// если была введена не цифра
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}

		// отчистка консоли
		system("cls");
		cout << err_str
			<< " (" << _min << ";" << _max << ")\n";

		// рекурсивное обращение
		num = input_and_check(_min, _max, welcome_str, err_str);
	}
	return num;
}

/**************** End Of LW2.cpp File ***************/