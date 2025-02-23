#include <iostream>
#include <windows.h>
#include <fstream>
#include <filesystem>
#include <cstdint>

#pragma execution_character_set("utf-8") //Установка кодировки UTF-8 для компилятора

using namespace std;

/// <summary>
/// структура записей для массива исходных данных - студенты
/// </summary>
struct Student {
	unsigned int studentID; // номер студенческого билета (уникальный ключ)
	char* studentSurname; // фамилия студента
	char* studentName; // имя студента
	char* studentSecondName; // отчество студента
	char* studentEmail; // корпоративная почта студента
	char* studentSportRanking; // спортивный разряд студента
	uint8_t studentHealthCategory; // номер группы здоровья студента
	char* studentSportActivity; // спортивная секция, на которую записан студент
	unsigned short studentSportActivityAttendanceNumber; // число посещений студентом спортивной секции
};

static bool CheckIfArrayIsNull(Student* ptr_studentsArray) {
	return ptr_studentsArray == NULL;
}

static Student* FillDynamicArrayUsingKeyboard(int arrayLength, Student* ptr_studentsArray) {
	string currentStudentSurname;
	string currentStudentName;
	string currentStudentSecondName;
	string currentStudentEmail;
	string currentStudentSportRanking;
	string currentStudentSportActivity;

	for (int i = 0; i < arrayLength; i++)
	{
		printf("Введите номер студенческого билета для %d записи: ", i + 1);
		cin >> ptr_studentsArray[i].studentID;

		printf("Введите фамилию студента для %d записи: ", i + 1);
		cin >> currentStudentSurname;
		ptr_studentsArray[i].studentSurname = (char*)malloc(currentStudentSurname.length() * sizeof(char));
		ptr_studentsArray[i].studentSurname = currentStudentSurname.data();

		printf("Введите имя студента для %d записи: ", i + 1);
		cin >> currentStudentName;
		ptr_studentsArray[i].studentName = (char*)malloc(currentStudentName.length() * sizeof(char));
		ptr_studentsArray[i].studentName = currentStudentName.data();

		printf("Введите отчество студента для %d записи: ", i + 1);
		cin >> currentStudentSecondName;
		ptr_studentsArray[i].studentSecondName = (char*)malloc(currentStudentSecondName.length() * sizeof(char));
		ptr_studentsArray[i].studentSecondName = currentStudentSecondName.data();

		printf("Введите корпоративную почту студента для %d записи: ", i + 1);
		cin >> currentStudentEmail;
		ptr_studentsArray[i].studentEmail = (char*)malloc(currentStudentEmail.length() * sizeof(char));
		ptr_studentsArray[i].studentEmail = currentStudentEmail.data();

		printf("Введите спортивный разряд студента для %d записи (если разряда нет, ввести \"Нет\"): ", i + 1);
		cin >> currentStudentSportRanking;
		ptr_studentsArray[i].studentSportRanking = (char*)malloc(currentStudentSportRanking.length() * sizeof(char));
		ptr_studentsArray[i].studentSportRanking = currentStudentSportRanking.data();

		printf("Введите группу здоровья студента для %d записи: ", i + 1);
		cin >> ptr_studentsArray[i].studentHealthCategory;

		printf("Введите спортивную секцию, на которую записан студент, для %d записи: ", i + 1);
		cin >> currentStudentSportActivity;
		ptr_studentsArray[i].studentSportActivity = (char*)malloc(currentStudentSportActivity.length() * sizeof(char));
		ptr_studentsArray[i].studentSportActivity = currentStudentSportActivity.data();

		printf("Введите количество посещений спортивной секции у студента для %d записи: ", i + 1);
		cin >> ptr_studentsArray[i].studentSportActivityAttendanceNumber;
	}
	cout << "Заполнение массива исходных данных завершено." << endl;
	return ptr_studentsArray;
}

static Student* FillDynamicArrayUsingFile(int arrayLength, Student* ptr_studentsArray) {
	filesystem::path currentPath = filesystem::current_path();

	string fileName;
	cout << "Введите имя файла: ";
	cin >> fileName;

	ifstream file;
	file.open(currentPath.string() + "\\" + fileName, ios::in); //Открытие выбранного файла, расположенного в директории программы

	if (!file.is_open())
	{
		cout << "Не удалось открыть файл." << endl;
	}
	else
	{
		int i = 0;
		unsigned short j = 0;
		unsigned int a = 0;
		string currentStudentSurname;
		string currentStudentName;
		string currentStudentSecondName;
		string currentStudentEmail;
		string currentStudentSportRanking;
		string currentStudentSportActivity;

		for (int i = 0; i < arrayLength && file >> ptr_studentsArray[i].studentID >> currentStudentSurname
			>> currentStudentName >> currentStudentSecondName >> currentStudentEmail
			>> currentStudentSportRanking >> ptr_studentsArray[i].studentHealthCategory
			>> currentStudentSportActivity >> ptr_studentsArray[i].studentSportActivityAttendanceNumber; i++)
		{
			ptr_studentsArray[i].studentSurname = (char*)malloc(currentStudentSurname.length() * sizeof(char));
			ptr_studentsArray[i].studentSurname = currentStudentSurname.data();

			ptr_studentsArray[i].studentName = (char*)malloc(currentStudentName.length() * sizeof(char));
			ptr_studentsArray[i].studentName = currentStudentName.data();

			ptr_studentsArray[i].studentSecondName = (char*)malloc(currentStudentSecondName.length() * sizeof(char));
			ptr_studentsArray[i].studentSecondName = currentStudentSecondName.data();

			ptr_studentsArray[i].studentEmail = (char*)malloc(currentStudentEmail.length() * sizeof(char));
			ptr_studentsArray[i].studentEmail = currentStudentEmail.data();

			ptr_studentsArray[i].studentSportRanking = (char*)malloc(currentStudentSportRanking.length() * sizeof(char));
			ptr_studentsArray[i].studentSportRanking = currentStudentSportRanking.data();

			ptr_studentsArray[i].studentSportActivity = (char*)malloc(currentStudentSportActivity.length() * sizeof(char));
			ptr_studentsArray[i].studentSportActivity = currentStudentSportActivity.data();

			cout << ptr_studentsArray[i].studentEmail << endl;
		}
	}
	file.close();
	return ptr_studentsArray;
}

int main()
{
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);

	Student* ptr_studentsArray;
	cout << 4 * sizeof(Student) << endl;
	int arrayLength;

	cout << "Введите число записей в массиве: ";
	cin >> arrayLength;

	ptr_studentsArray = (Student*)malloc(arrayLength * sizeof(Student));

	if (!CheckIfArrayIsNull(ptr_studentsArray))
	{
		int menuPoint;
		do {

			cout << "Память для массива исходных данных была выделена." << endl;
			cout << "Выберите, какими данными следует заполнить массив:" << endl;
			cout << "1. Данные, введенные с клавиатуры через консоль." << endl;
			cout << "2. Данные из файла." << endl;
			cout << "3. Выход." << endl;
			cout << "Введите номер выбранного вами пункта: ";
			cin >> menuPoint;

			switch (menuPoint)
			{
			case 1:
			{
				ptr_studentsArray = FillDynamicArrayUsingKeyboard(arrayLength, ptr_studentsArray);
			}
			break;
			case 2:
			{
				ptr_studentsArray = FillDynamicArrayUsingFile(arrayLength, ptr_studentsArray);
			}
			break;
			default:
				break;
			}
		} while (menuPoint != 3);
	}
	else
	{
		cout << "Память для массива исходных данных не была выделена." << endl;
	}

	free(ptr_studentsArray);

	return 0;
}
