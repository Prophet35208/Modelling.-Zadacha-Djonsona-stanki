
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
// Перестановка в массиве a значений с индексами i и j
void swap(int* a, int i, int j)
{
    int s = a[i];
    a[i] = a[j];
    a[j] = s;
}
bool NextSet(int* a, int n)
{
    int j = n - 2;
    // Ищем такую a[j], где она меньше следующего в массиве
    while (j != -1 && a[j] >= a[j + 1]) j--;
    if (j == -1)
        return false; // больше перестановок нет 
    int k = n - 1;
    // Ищем такую a[j], где она больше a[j]
    while (a[j] >= a[k]) k--;
    swap(a, j, k);
    int l = j + 1;
    int r = n - 1;
    while (l < r)
        swap(a, l++, r--);// Сортируем оставшуюся часть последовательности (т.е. инвертируем её)
    return true;
}
// Инициализация последовательности 12345...
void InitializeOrder(int num_detal, int* order_mas) {
    for (int i = 0; i < num_detal; i++)
        order_mas[i] = i + 1;
}
// Нахождение простоя для двух станков
int FindProstoiForTwo(int detalsMatrix[10][3], int* currentOrder, int num_detal) {
    int last_value = detalsMatrix[currentOrder[0] - 1][0];
    int max = last_value;

    for (size_t i = 1; i < num_detal; i++)
    {
        last_value += detalsMatrix[currentOrder[i] - 1][0];
        last_value -= detalsMatrix[currentOrder[i - 1] - 1][1];

        if (max < last_value)
            max = last_value;
    }
    return max;
}
// Нахождение простоя для трёх станков
int FindProstoiForThree(int detalsMatrix[10][3], int* currentOrder, int num_detal) {
    int buf;
    int last_valueK = detalsMatrix[currentOrder[0] - 1][0];
    int last_valueH = detalsMatrix[currentOrder[0] - 1][1];
    int max = last_valueK + last_valueH;

    for (size_t i = 1; i < num_detal; i++)
    {
        last_valueK += detalsMatrix[currentOrder[i] - 1][0];
        last_valueK -= detalsMatrix[currentOrder[i - 1] - 1][1];

        last_valueH += detalsMatrix[currentOrder[i] - 1][1];
        last_valueH -= detalsMatrix[currentOrder[i - 1] - 1][2];

        buf = last_valueK + last_valueH;
        if (max < buf)
            max = buf;
    }
    return max;

}
// Превращает начальный порядок в оптимальный методом Джонсона
void JonsonForTwo(int* start_order_mas, int* end_order_mas, int detals_matrix[10][3], int num_detal) {

    int n1 = 0; // Счётчик заполнения массива с начала
    int n2 = num_detal - 1; // Счётчик заполнения массива с конца
    int place_row;
    int place_column;
    int min;
    int next;

    do
    {
        min = -1;
        // Находим минимальное значение
        for (size_t i = 0; i < num_detal; i++)
        {         
            for (size_t j = 0; j < 2; j++)
            {

                if (start_order_mas[i] == -1)
                    break;

                next = detals_matrix[start_order_mas[i] - 1][j];
                if (min > next || min == -1) {
                    min = next;
                    place_row = i;
                    place_column = j;
                }
            }
        }

        if (min != -1) 
        {
            if (place_column == 0)
            {
                end_order_mas[n1] = place_row+1;
                n1++;
                start_order_mas[place_row] = -1;
            }

            if (place_column == 1)
            {
                end_order_mas[n2] = place_row+1;
                n2--;
                start_order_mas[place_row] = -1;
            }
        }

    } while (min != -1);
}
// Функция перебора для 2 и 3 станков
int Perebor(int* start_order_mas, int detals_matrix[10][3], int num_detal, int* min_mas, int numOfStancov) {
    int min;
    int max;
    min = -1;
    do {
        if (numOfStancov == 2)
            max = FindProstoiForTwo(detals_matrix, start_order_mas, num_detal);
        else
            max = FindProstoiForThree(detals_matrix, start_order_mas, num_detal);

        // Если общий простой меньше чем для предыдущей цепочки или он первый, то запомним его и цепочку 
        if (max < min || min == -1) {
            for (size_t i = 0; i < num_detal; i++)
            {
                min_mas[i] = start_order_mas[i];
            }
            min = max;
        }

        // Получаем следующую цепочку
    } while ((NextSet(start_order_mas, num_detal)));
    // Возвращяем изменённый массив min_mas и min
    return min;
}
// Функция проверки условия перехода от 3 к 2 станкам. Если условие выполняется, то возвращется true, если нет, то false
bool CanThreeToTwo(int detals_matrix[10][3], int num_detal) {
    int minA;
    int maxB;
    int minC;

    minA = detals_matrix[0][0];
    for (size_t i = 1; i < num_detal; i++)
    {
        if (detals_matrix[i][0] < minA)
            minA = detals_matrix[i][0];
    }

    maxB = detals_matrix[0][1];
    for (size_t i = 1; i < num_detal; i++)
    {
        if (detals_matrix[i][1] > maxB)
            maxB = detals_matrix[i][0];
    }

    if (minA > maxB)
        return true;

    minC = detals_matrix[0][2];
    for (size_t i = 1; i < num_detal; i++)
    {
        if (detals_matrix[i][2] < minC)
            minC = detals_matrix[i][2];
    }
    if (minC > maxB)
        return true;
    else
        return false;
}
// Конвертирует массив путём суммирования среднего столбца двум другим
void ConvertFromThreeToTwo(int detals_matrix[10][3], int detalsMatrixNew[][3], int num_detal) {
    for (size_t i = 0; i < num_detal; i++)
    {
        detalsMatrixNew[i][0] = detals_matrix[i][0] + detals_matrix[i][1];
        detalsMatrixNew[i][1] = detals_matrix[i][1] + detals_matrix[i][2];

    }
}
void PrintMatrix(int detalsMatrix[][3], int num_stank, int num_detal, int* mas_order) {
    cout << setw(10) << "a";
    cout << setw(5) << "b";
    if (num_stank == 3) cout << setw(5) << "c" << endl; else cout << endl;
    for (size_t i = 0; i < num_detal; i++)
    {
        cout << setw(5) << mas_order[i];
        for (size_t j = 0; j < num_stank; j++)
        {
            cout << setw(5) << detalsMatrix[mas_order[i]-1][j];
        }
        cout << endl;
    }
}

// int Prostoi(int* detals_matrix, int* order_mas) {}
int main()
{
        
    ifstream in;      
    in.open("In.txt"); 
    std::ofstream out;    
    int num_stank;
    int num_detal;
    int detals_matrix[10][3]; // Массив для матрицы деталей. Рассчитан максимум на 10 деталей и 3 станка
    int detals_matrix_new[10][3];
    int mas_prost[10];

    int min;

    in >> num_stank;
    in >> num_detal;

    // Считываем матрицу деталей (строки со временем для каждой детали) из файла
    if (in.is_open())
    {
        for (size_t i = 0; i < num_detal; i++)
        {
            for (size_t j = 0; j < num_stank; j++)
            {
                in >> detals_matrix[i][j];
            }
        }
    }


    int start_order_mas[10]; // Массив на макс 10 деталей. Изменяется по мере анализа
    int end_order_mas[10];

    InitializeOrder(num_detal, start_order_mas);
    //int JMas[10];
    //JonsonForTwo(buf_mas, JMas, detals_matrix, 5);
    //int a;
    //a = Perebor(buf_mas, detals_matrix, num_detal, min_mas, num_stank);
    //int new_mas[10][3];
    //ConvertFromThreeToTwo(detals_matrix, new_mas, 4);

    setlocale(LC_ALL, "Rus");
    cout << "Дана следующая матрица деталей. Кол-во станков: " <<num_stank <<" . Кол-во деталей: " <<num_detal<< endl;
    PrintMatrix(detals_matrix, num_stank, num_detal, start_order_mas);
    cout << endl;

    if (num_stank == 2) {
        min = FindProstoiForTwo(detals_matrix, start_order_mas, num_detal);
        cout << "Текущий простой (X): " << min<< endl;
        cout << endl;

        cout << "Воспользуемся алгоритмом Джонсона для поиска оптимальной последовательности."<< endl;
        JonsonForTwo(start_order_mas, end_order_mas, detals_matrix, num_detal);
        cout << "Оптимальная последовательность: "<< endl;
        PrintMatrix(detals_matrix, num_stank, num_detal, end_order_mas);
        cout << endl;
        min =  FindProstoiForTwo(detals_matrix, end_order_mas, num_detal);
        cout << "Общий простой (X): " << min;
        cout << endl; cout << endl;

        cout << "Сделаем проверку. Посмотрим какой резултат даст перебор."<< endl;
        InitializeOrder(num_detal, start_order_mas);
        min = Perebor(start_order_mas, detals_matrix, num_detal, end_order_mas, num_stank);
        cout << "Оптимальная последовательность: " << endl;
        PrintMatrix(detals_matrix, num_stank, num_detal, end_order_mas);
        cout << "Общий простой (X): " << min<< endl;


        // Составляем вывод для создания графа
        out.open("Out_optimal.txt");
        out << num_stank << endl;
        out << num_detal << endl;

        int start_point = 0;
        int mas_of_ends[10]; // Массив окончаний работ на первом станке для каждой детали
        // Первый станок
        for (size_t i = 0; i < num_detal; i++)
        {
            out << start_point << " " << detals_matrix[end_order_mas[i]-1][0] << " ";
            start_point += detals_matrix[end_order_mas[i] - 1][0];
            mas_of_ends[i] = start_point;

            if (i == num_detal - 1) {
                out << endl<< start_point << endl;
            }
        }
        // Второй станок
        start_point = detals_matrix[end_order_mas[0] - 1][0];
        for (size_t i = 0; i < num_detal; i++)
        {
            out << start_point << " " << detals_matrix[end_order_mas[i] - 1][1] << " ";
            if (i != num_detal - 1)
                start_point = max(mas_of_ends[i + 1], start_point + detals_matrix[end_order_mas[i] - 1][1]);
            else
                out << endl << start_point+ detals_matrix[end_order_mas[i] - 1][1] << endl;
        }
        out.close();

        out.open("Out_start.txt");

        out << num_stank << endl;
        out << num_detal << endl;

        start_point = 0;
        // Первый станок
        for (size_t i = 0; i < num_detal; i++)
        {
            out << start_point << " " << detals_matrix[i][0] << " ";
            start_point += detals_matrix[i][0];
            mas_of_ends[i] = start_point;

            if (i == num_detal - 1) {
                out << endl << start_point << endl;
            }
        }
        // Второй станок
        start_point = detals_matrix[0][0];
        int end;
        for (size_t i = 0; i < num_detal; i++)
        {
            out << start_point << " " << detals_matrix[i][1] << " ";
            if (i != num_detal - 1)
                start_point = max(mas_of_ends[i + 1], start_point + detals_matrix[i][1]);
            else
                out << endl << start_point + detals_matrix[i][1] << endl;
        }
        out.close();


    }
    else
    {
        cout << "Можно ли привести к задаче о 2 станках: ";
        if (CanThreeToTwo(detals_matrix, num_detal)) 
        {
            cout << "да." << endl;
            cout << "Перейдём к задаче с 2 станками. Получим следующую матрицу.";
            ConvertFromThreeToTwo(detals_matrix, detals_matrix_new, num_detal);
            PrintMatrix(detals_matrix_new, 2, num_detal, start_order_mas);
            cout << endl;

            min = FindProstoiForTwo(detals_matrix, start_order_mas, num_detal);
            cout << "Текущий простой (X): " << min << endl;
            cout << endl;

            cout << "Воспользуемся алгоритмом Джонсона для поиска оптимальной последовательности." << endl;
            JonsonForTwo(start_order_mas, end_order_mas, detals_matrix, num_detal);
            cout << "Оптимальная последовательность: " << endl;
            PrintMatrix(detals_matrix_new, 2, num_detal, end_order_mas);
            cout << endl;
            min = FindProstoiForTwo(detals_matrix_new, end_order_mas, num_detal);
            cout << "Общий простой (X): " << min;
            cout << endl; cout << endl;
            cout << "Сделаем проверку. Посмотрим какой резултат даст перебор." << endl;
            InitializeOrder(num_detal, start_order_mas);
            min = Perebor(start_order_mas, detals_matrix_new, num_detal, end_order_mas, 2);
            cout << "Оптимальная последовательность: " << endl;
            PrintMatrix(detals_matrix_new, 2, 2, end_order_mas);
            cout << "Общий простой (X): " << min << endl;

            // Составляем вывод для создания графа
            out.open("Out_optimal.txt");

            out << num_stank << endl;
            out << num_detal << endl;

            int start_point = 0;
            int mas_of_ends[10]; // Массив окончаний работ на первом станке для каждой детали
            // Первый станок
            for (size_t i = 0; i < num_detal; i++)
            {
                out << start_point << " " << detals_matrix_new[end_order_mas[i] - 1][0] << " ";
                start_point += detals_matrix_new[end_order_mas[i] - 1][0];
                mas_of_ends[i] = start_point;

                if (i == num_detal - 1) {
                    out << endl << start_point << endl;
                }
            }
            // Второй станок
            start_point = detals_matrix_new[end_order_mas[0] - 1][0];
            for (size_t i = 0; i < num_detal; i++)
            {
                out << start_point << " " << detals_matrix_new[end_order_mas[i] - 1][1] << " ";
                if (i != num_detal - 1)
                    start_point = max(mas_of_ends[i + 1], start_point + detals_matrix_new[end_order_mas[i] - 1][1]);
                else
                    out << endl << start_point + detals_matrix_new[end_order_mas[i] - 1][1] << endl;
            }
            out.close();

            out.open("Out_start.txt");

            out << num_stank << endl;
            out << num_detal << endl;

            start_point = 0;
            // Первый станок
            for (size_t i = 0; i < num_detal; i++)
            {
                out << start_point << " " << detals_matrix_new[i][0] << " ";
                start_point += detals_matrix_new[i][0];
                mas_of_ends[i] = start_point;

                if (i == num_detal - 1) {
                    out << endl << start_point << endl;
                }
            }
            // Второй станок
            start_point = detals_matrix_new[0][0];
            int end;
            for (size_t i = 0; i < num_detal; i++)
            {
                out << start_point << " " << detals_matrix_new[i][1] << " ";
                if (i != num_detal - 1)
                    start_point = max(mas_of_ends[i + 1], start_point + detals_matrix_new[i][1]);
                else
                    out << endl << start_point + detals_matrix_new[i][1] << endl;
            }
            out.close();
        }
        else
        {
            cout << "нет." << endl;
            cout << "Тогда решим перебором." << endl;

            InitializeOrder(num_detal, start_order_mas);
            min = Perebor(start_order_mas, detals_matrix, num_detal, end_order_mas, num_stank);
            cout << "Оптимальная последовательность: " << endl;
            PrintMatrix(detals_matrix, num_stank, num_detal, end_order_mas);
            cout << "Общий простой (X): " << min << endl;

            // Составляем вывод для создания графа
            out.open("Out_start.txt");

            out << num_stank << endl;
            out << num_detal << endl;

            int start_point = 0;
            int mas_of_ends[10]; // Массив окончаний работ на первом станке для каждой детали
            // Первый станок
            for (size_t i = 0; i < num_detal; i++)
            {
                out << start_point << " " << detals_matrix[end_order_mas[i] - 1][0] << " ";
                start_point += detals_matrix[end_order_mas[i] - 1][0];
                mas_of_ends[i] = start_point;

                if (i == num_detal - 1) {
                    out << endl << start_point << endl;
                }
            }
            // Второй станок
            start_point = detals_matrix[end_order_mas[0] - 1][0];
            for (size_t i = 0; i < num_detal; i++)
            {
                out << start_point << " " << detals_matrix[end_order_mas[i] - 1][1] << " ";
                if (i != num_detal - 1)
                    start_point = max(mas_of_ends[i + 1], start_point + detals_matrix[end_order_mas[i] - 1][1]);
                else
                    out << endl << start_point + detals_matrix[end_order_mas[i] - 1][1] << endl;
            }
            // Третий станок
            start_point = detals_matrix[end_order_mas[0] - 1][0];
            for (size_t i = 0; i < num_detal; i++)
            {
                out << start_point << " " << detals_matrix[end_order_mas[i] - 1][1] << " ";
                if (i != num_detal - 1)
                    start_point = max(mas_of_ends[i + 1], start_point + detals_matrix[end_order_mas[i] - 1][2]);
                else
                    out << endl << start_point + detals_matrix[end_order_mas[i] - 1][2] << endl;
            }
            out.close();

            out.open("Out_optimal.txt");

            out << num_stank << endl;
            out << num_detal << endl;

            start_point = 0;
            // Первый станок
            for (size_t i = 0; i < num_detal; i++)
            {
                out << start_point << " " << detals_matrix[i][0] << " ";
                start_point += detals_matrix[i][0];
                mas_of_ends[i] = start_point;

                if (i == num_detal - 1) {
                    out << endl << start_point << endl;
                }
            }
            // Второй станок
            start_point = detals_matrix[0][0];
            int end;
            for (size_t i = 0; i < num_detal; i++)
            {
                out << start_point << " " << detals_matrix[i][1] << " ";
                if (i != num_detal - 1)
                    start_point = max(mas_of_ends[i + 1], start_point + detals_matrix[i][1]);
                else
                    out << endl << start_point + detals_matrix[i][1] << endl;
            }

            start_point = detals_matrix[end_order_mas[0] - 1][0];
            for (size_t i = 0; i < num_detal; i++)
            {
                out << start_point << " " << detals_matrix[end_order_mas[i] - 1][1] << " ";
                if (i != num_detal - 1)
                    start_point = max(mas_of_ends[i + 1], start_point + detals_matrix[end_order_mas[i] - 1][2]);
                else
                    out << endl << start_point + detals_matrix[end_order_mas[i] - 1][2] << endl;
            }
            out.close();
        }
    }


    system("pause");
    return 0;


   
}

/*do {

        // Анализируем простой
        last_value = detals_matrix[buf_mas[0]-1][0];
        max = last_value;

        for (size_t i = 1; i < num_detal; i++)
        {
            last_value += detals_matrix[buf_mas[i] - 1][0];
            last_value -= detals_matrix[buf_mas[i-1] - 1][1];

            if (max < last_value)
                max = last_value;
        }

        // Если общий простой меньше чем для предыдущей цепочки или он первый, то запомним его и цепочку 
        if (max < min || min == -1) {
            for (size_t i = 0; i < num_detal; i++)
            {
                min_mas[i] = buf_mas[i];
            }
            min = max;
        }

        // Получаем следующую цепочку
    } while ((NextSet(buf_mas, num_detal)));*/