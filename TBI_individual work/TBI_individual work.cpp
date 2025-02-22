#include <iostream>
#include <windows.h>

using namespace std;

/// <summary>
/// структура записей для массива исходных данных - студенты
/// </summary>
struct Student {
    unsigned int studentID; // номер студенческого билета (уникальный ключ)
    string studentSurname; // фамилия студента
    string studentName; // имя студента
    string studentSecondName; // отчество студента
    string studentEmail; // корпоративная почта студента
    string studentSportRanking; // спортивный разряд студента
    unsigned char studentHealthCategory; // номер группы здоровья студента
    string studentSportActivity; // спортивная секция, на которую записан студент
    unsigned short studentSportActivityAttendanceNumber; // число посещений студентом спортивной секции
};

bool CheckIfArrayIsEmpty(Student* ptr_studentsArray) {
    return ptr_studentsArray == NULL;
}

int main()
{
    SetConsoleOutputCP(1251);

    Student* ptr_studentsArray;
    unsigned int arrayLength;

    cout << "Введите число записей в массиве: ";
    cin >> arrayLength;

    ptr_studentsArray = (Student*)malloc(arrayLength * sizeof(Student));

    if (CheckIfArrayIsEmpty(ptr_studentsArray)) 
    {
        cout << "Память для массива исходных данных была выделена." << endl;
    }
    else 
    {
        cout << "Память для массива исходных данных не была выделена." << endl;
    }

    free(ptr_studentsArray);

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
