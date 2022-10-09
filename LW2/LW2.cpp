/***********************************************************************\
*                    кафедра № 304 2 курс 3 семестр информатика			*
*-----------------------------------------------------------------------*
*	Project type : solution												*
*	Project name : LW2													*
*	File name    : LW2.cpp												*
*	Language     : c/c++												*
*	Programmers  : Плоцкий Б.А. Раужев Ю. М.							*
*	Created      :  4/10/22												*
*	Last revision:  8/10/22												*
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

using namespace std;

// тип данных NS
typedef chrono::nanoseconds NS;

// нужна ли печать массива в консоль
#define NEED_PRINT false

// минимальный и максимальный
// размер массива
#define MIN_ARR_SIZE 10000
#define MAX_ARR_SIZE 200000

// количество таблиц в консоли
#define NUMB_OF_TABLES 10

// минимальное и максимальное значение
// для чисел в массиве
#define MIN_VALUE MIN_ARR_SIZE * 2
#define MAX_VALUE MAX_ARR_SIZE * 2

// заполнение len элементов элементом symb
#define OUT_W(symb, len) fixed << setfill(symb) << setw(len)

/****************************************************************
*              П Р О Т О Т И П Ы   Ф У Н К Ц И Й                *
****************************************************************/

// упорядочная функция по возрастанию
template<typename T>
void f1(T* arr, int size, int max = MAX_VALUE, int step = 1);

// изменение размера динамического массива
template<typename T>
void resize_arr(T*& arr, int& size, int inc = 1);

// возвращение случайного значения из 
// интервала (min, max)
template<typename T>
T rand_num(T min, T max);

// рандомизация значений массива
template<typename T>
void randomize_array(T* arr, int size);

// печать массива в поток
template<typename T>
void print_arr(T* arr, int size, ostream& stream = cout);

// ввод и проверка значений
template<typename T>
T input_and_check(T _min, T _max,
	const char* welcome_str, const char* err_str);

// алгоритм Better Linear Search
template<typename T>
T BLS(
	T* arr,			// исходный массив
	int size,			// размер массива
	T target,			// цель поиска
	int& num_of_comp	// количество сравнений
);

// алгоритм Sentinel Linear Search
template<typename T>
T SLS(
	T* arr,			// исходный массив
	int size,			// размер массива
	T target,			// цель поиска
	int& num_of_comp	// количество сравнений
);

// функция, которая сортирует массив и 
// добавляет фиктивный элемент в конец 
template<typename T>
T support_T(
	T*& arr,			// исходный массив
	int& size,			// размер массива
	int target_pos		// позиция цели
);

// алгоритм T
template<typename Tt>
Tt T(
	Tt* arr,			// исходный массив
	int size,			// размер массива
	Tt target,			// цель поиска
	int& num_of_comp	// количество сравнений
);

// функция, которая сортирует массив и 
// убирает с конца фиктивный элемент
template<typename T>
T support_B(
	T*& arr,			// исходный массив
	int& size,			// размер массива
	int target_pos		// позиция цели
);

// алгоритм бинарного поиска
template<typename T>
T B(
	T* arr,			// исходный массив
	int size,			// размер массива
	T target,			// цель поиска
	int& num_of_comp	// количество сравнений
);

// стандартная вспомогательная функция
template<typename T>
T support_default(T*& arr, int&, int target_pos);

// функция измерения времени выполнения функций
template<typename T>
int measure_time(
	T*& arr,				// исходный массив
	int size,				// размер массива
	T& target,			// элемент поиска
	int& num_of_comp,		// количество сравнений
	NS& time,				// время работы функции
	T(*search_func)(		// функция поиска
		T* arr,			// исходный массив
		int size,			// размер массива
		T target,			// цель поиска
		int& num_of_comp	// количество сравнений
		)
);

// вызов всех функций поиска и вывод результатов в таблицу
void gen_arr_draw_table(
	int size,	//размер массива 
	int target	// элемент поиска
);

/****************************************************************
*                      К О Н С Т А Н Т Ы                        *
****************************************************************/

// массив с вспомогательными функциями
template<typename T>
T(*sup_funcs[])(T*& arr, int&, int target_pos) =
{
	support_default,
	support_default,
	support_T,
	support_B
};

// массив с функциями поиска
template<typename Tt>
Tt(*search_funcs[])(Tt*, int, Tt, int&) =
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


/****************************************************************
*                Г Л А В Н А Я   Ф У Н К Ц И Я                  *
****************************************************************/

int main()
{
	setlocale(LC_ALL, "ru");

	srand(time(NULL));

	// запрашиваем у пользователя расположение элемента
	int choose = input_and_check(1, 3, "Введите расположение элемента,который надо найти:\n\
\t1.В начале\n\t2.В середине\n\t3.В конце\n", "Строго целые числа 1,2 или 3\n");

	// позиция элемента поиска
	int target_pos = 0;

	// 10 раз просчитываем работу поиска
	for (
		int i = MIN_ARR_SIZE; i <= MAX_ARR_SIZE;
		i += (MAX_ARR_SIZE - MIN_ARR_SIZE) / NUMB_OF_TABLES
		)
	{
		switch (choose)
		{
		case 1:
			target_pos = 0;
			break;
		case 2:
			target_pos = i / 2;
			break;
		case 3:
			target_pos = i - 1;
			break;
		}
		gen_arr_draw_table(i, target_pos);
	}

	return 0;
}

/****************************************************************
*             Р Е А Л И З А Ц И Я   Ф У Н К Ц И Й               *
****************************************************************/

//#include <string>
// упорядочная функция по возрастанию
template<typename T>
void f1(T* arr, int size, int max, int step)
{
	//k, b - коэффициенты прямой
	T k = max / (step * size);
	T b = MIN_VALUE;
	int x = 0.0; // координата x

	// заполнение массива 
	for (int i = 0; i < size; i++, x += step)
	{
		arr[i] = k * x + b;
	}
}

// изменение размера динамического массива
template<typename T>
void resize_arr(T*& arr, int& size, int inc)
{

	// увеличение длины массива
	size += inc;

	// создание нового массива
	T* new_arr = new T[size];

	if (size - 1 != 0)
	{
		// копирование данных в новый массив из старого
		for (int i = 0; i < size - 1; i++)
		{
			new_arr[i] = arr[i];
		}// end for i

		// удаление старого массива
		delete[] arr;
	}

	// присваивание старому массиву адресс памяти нового
	arr = new_arr;
}

// возвращение случайного значения из 
// интервала (min, max)
template<typename T>
T rand_num(T min, T max)
{
	return rand() % (max - min) + min;
}

// рандомизация значений массива
template<typename T>
void randomize_array(T* arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		arr[i] = rand_num(MIN_VALUE, MAX_VALUE);
	}
}

// печать массива в поток
template<typename T>
void print_arr(T* arr, int size, ostream& stream)
{
	// вывод элементов массива
	for (int i = 0; i < size; i++)
	{
		stream << arr[i] << ' ';
	}
	cout << '\n';
}

// ввод и проверка значений
template<typename T>
T input_and_check(T _min, T _max,
	const char* welcome_str, const char* err_str)
{
	// размер массива
	T num;

	// вывод сообщения
	cout << welcome_str << "\n";
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
		cout << err_str << "\n";

		// рекурсивное обращение
		num = input_and_check(_min, _max, welcome_str, err_str);
	}
	return num;
}

// алгоритм Better Linear Search
template<typename T>
T BLS(
	T* arr,			// исходный массив
	int size,			// размер массива
	T target,			// цель поиска
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
	num_of_comp++;
	return -1;
}

// алгоритм Sentinel Linear Search
template<typename T>
T SLS(
	T* arr,			// исходный массив
	int size,			// размер массива
	T target,			// цель поиска
	int& num_of_comp	// количество сравнений
)
{
	// последний элемент массива
	int last = arr[size - 1];

	// элемент, который нужно найти
	// располагаем в последеней ячейке массива
	arr[size - 1] = target;

	// счетчик элементов массива
	int i = 0;

	while (arr[i] != target)
	{
		num_of_comp++;
		i++;
	}
	num_of_comp++;

	// Возвращаем последний элемент в массив
	arr[size - 1] = last;

	// если элемент был найден где-то в массиве
	// или на его конце, то возвращаем индекс
	num_of_comp++;
	if ((i < size - 1) || (arr[size - 1] == target))
	{
		return i;
	}
	return -1;
}

// функция, которая сортирует массив и 
// добавляет фиктивный элемент в конец 
template<typename T>
T support_T(
	T*& arr,			// исходный массив
	int& size,			// размер массива
	int target_pos		// позиция цели
)
{
	// формирование неубывающей последовательности 
	f1(arr, size);

	// добавление фиктивного значения в конец массива
	// если еще нет фиктивного элемента
	if (arr[size - 1] != INT_MAX)
	{
		resize_arr(arr, size);
	}
	return arr[target_pos];
}

// алгоритм T
template<typename Tt>
Tt T(
	Tt* arr,			// исходный массив
	int size,			// размер массива
	Tt target,			// цель поиска
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
template<typename T>
T support_B(
	T*& arr,			// исходный массив
	int& size,			// размер массива
	int target_pos		// позиция цели
)
{
	// убирание последнего элемента из массива,
	// если он является фиктивным
	if (arr[size - 1] == INT_MAX)
	{
		resize_arr(arr, size, -1);
	}

	// формирование неубывающей последовательности 
	f1(arr, size);

	//print_arr(arr, size);
	return arr[target_pos];
}

// алгоритм бинарного поиска
template<typename T>
T B(
	T* arr,			// исходный массив
	int size,			// размер массива
	T target,			// цель поиска
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

// стандартная вспомогательная функция
template<typename T>
T support_default(T*& arr, int&, int target_pos)
{
	// необзодимо для отметки элемента
	// который надо найти
	if (arr[target_pos] > 0)
	{
		arr[target_pos] *= -1;
	}

	return arr[target_pos];
}

// функция измерения времени выполнения функций
template<typename T>
int measure_time(
	T*& arr,				// исходный массив
	int size,				// размер массива
	T& target,			// элемент поиска
	int& num_of_comp,		// количество сравнений
	NS& time,				// время работы функции
	T(*search_func)(		// функция поиска
		T* arr,			// исходный массив
		int size,			// размер массива
		T target,			// цель поиска
		int& num_of_comp	// количество сравнений
		)
)
{
	// обнуление количества сравнений
	num_of_comp = 0;

	// начало отсчета времени
	auto begin = chrono::steady_clock::now();

	// вызов функции поиска
	int res = search_func(arr, size, target, num_of_comp);

	// конец отсчета времени
	auto end = chrono::steady_clock::now();

	// если необходимое число было
	// помечено для SLS И BLS алгоритмов
	// то убираем пометку
	if (target < 0)
	{
		target *= -1;
	}

	// вывод в консоль времени работы программы
	time = chrono::duration_cast<NS>(end - begin);

	return res;
}

// вызов всех функций поиска и вывод результатов в таблицу
void gen_arr_draw_table(int size, int target_pos)
{
	long* arr = new long[size];	// массив
	int num_of_comp = 0;		// количество сравнений	
	NS elapsed_time;			// время выполнения функции
	long target;					// цель поиска

	// рандомизация массива
	randomize_array(arr, size);

	// вывод таблицы
	cout << OUT_W('_', 83) << "\n";
	cout << "|_Размер_массива:___|_" << OUT_W('_', 58) << size << "_|\n";
	cout << "| Алгоритм |  Ключ  | Индекс ключа | Количество сравнений | Время выполнения(нс) |\n";

	// вызов всех функций поиска
	for (int i = 0; i < 4; i++)
	{
		// поиск элемента на позиции target_pos
		// + подготовка массивов для работы алгоритмов поиска
		target = sup_funcs<long>[i](arr, size, target_pos);

		// получение индекса элемента в массиве
		int index = measure_time<long>(arr, size, target, num_of_comp,
			elapsed_time, search_funcs<long>[i]);

		// вывод строки таблицы
		cout << "| " << OUT_W(' ', 8) << algorithms[i]
			<< " | " << OUT_W(' ', 6) << target
			<< " | " << OUT_W(' ', 12) << index
			<< " | " << OUT_W(' ', 20) << num_of_comp
			<< " | " << OUT_W(' ', 20) << elapsed_time.count() << " |\n";

		// печать массива, если это необходимо
		if (NEED_PRINT)
		{
			cout << "|МАССИВ ДЛЯ " << OUT_W(' ', 8) << algorithms[i] << "|";
			print_arr(arr, size);
		}
	}
	// конец таблицы
	cout << OUT_W('-', 83) << "\n";

	// удаление массива
	delete[] arr;

}

/**************** End Of LW2.cpp File ***************/