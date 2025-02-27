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
	string studentSurname; // фамилия студента
	string studentName; // имя студента
	string studentSecondName; // отчество студента
	string studentEmail; // корпоративная почта студента
	string studentSportRanking; // спортивный разряд студента
	uint8_t studentHealthCategory; // номер группы здоровья студента
	string studentSportActivity; // спортивная секция, на которую записан студент
	unsigned short studentSportActivityAttendanceNumber; // число посещений студентом спортивной секции
};

/// <summary>
/// индекс-массив для фамилий студентов
/// </summary>
struct StudentSurnameIndex {
	int initialIndex;
	string studentSurname;
};

/// <summary>
/// индекс-массив для числа посещений секций студентами
/// </summary>
struct StudentAttendanceNumberIndex {
	int initialIndex;
	unsigned short studentSportActivityAttendanceNumber;
};

static bool CheckIfArrayIsNull(Student* ptr_studentsArray) {
	return ptr_studentsArray == NULL;
}

static unsigned short GetKeyValue(StudentAttendanceNumberIndex currentRecord) {
	return currentRecord.studentSportActivityAttendanceNumber;
}

static string GetKeyValue(StudentSurnameIndex currentRecord) {
	return currentRecord.studentSurname;
}

/// <summary>
/// функция добавления очередной записи из исходного массива в отсортированный индекс-массив (динамическая сортировка вставками)
/// </summary>
/// <param name="currentIndex"></param>
/// <param name="currentStudentSurname"></param>
/// <param name="ptr_indexesSurname"></param>
/// <returns></returns>
static void AddElementToIndexSurnameArray(int currentIndex, string currentStudentSurname, StudentSurnameIndex* ptr_indexesSurname) {
	int temporaryIndex = -1;
	string temporarySurname = "";
	int insertionIndex = currentIndex;
	for (int j = 0; j < currentIndex; j++)
	{
		if (ptr_indexesSurname[j].studentSurname > currentStudentSurname) {
			temporaryIndex = ptr_indexesSurname[j].initialIndex;
			temporarySurname = ptr_indexesSurname[j].studentSurname;
			insertionIndex = j;
			break;
		}
	}
	if (temporaryIndex == -1) {
		ptr_indexesSurname[currentIndex].initialIndex = currentIndex;
		ptr_indexesSurname[currentIndex].studentSurname = currentStudentSurname;
	}
	else {
		// Записываем на место вставки (indexSwap) введённые данные
		ptr_indexesSurname[insertionIndex].initialIndex = currentIndex;
		ptr_indexesSurname[insertionIndex].studentSurname = currentStudentSurname;
		// Сдвигаем элементы индекс-массива на 1 вниз
		for (int j = currentIndex; j > insertionIndex + 1; j--)
		{
			ptr_indexesSurname[j].initialIndex = ptr_indexesSurname[j - 1].initialIndex;
			ptr_indexesSurname[j].studentSurname = ptr_indexesSurname[j - 1].studentSurname;
		}
		// Записываем элемент (который был перезаписан) на следующий индекс
		ptr_indexesSurname[insertionIndex + 1].initialIndex = temporaryIndex;
		ptr_indexesSurname[insertionIndex + 1].studentSurname = temporarySurname;
	}
}

/// <summary>
/// сортировка индекс-массива с количеством посещений (сортировка выбором)
/// </summary>
/// <param name="ptr_indexesAttendance"></param>
/// <param name="arrayLength"></param>
static void SortAttendanceIndexArrayUsingSelection(StudentAttendanceNumberIndex* ptr_indexesAttendance, unsigned short arrayLength) {
	StudentAttendanceNumberIndex temporaryIndexRecord; // вспомогательная переменная
	unsigned __int8 i, j; // параметры цикла - индексы в массиве
	int maximumKeyValue; // вспомогательная переменная для поиска максимум по ключам
	unsigned __int8 maximumKeyValueIndex; // индекс элемента с минимальным значением ключа

	for (i = 0; i < arrayLength - 1; i++)
	{
		// Фиксируем первую позицию i в неотсортированной части,
		// в которую будем записывать максимум, найденный в неотсортированной части,
		// и начинаем искать минимум для обмена значений: 

		maximumKeyValue = GetKeyValue(ptr_indexesAttendance[i]);
		maximumKeyValueIndex = i;
		for (j = i + 1; j < arrayLength; j++)
		{
			if (GetKeyValue(ptr_indexesAttendance[j]) > maximumKeyValue) // сравниваем ключи, чтобы определить,
				// нужно ли сменить максимум?
			{
				maximumKeyValue = GetKeyValue(ptr_indexesAttendance[j]);
				maximumKeyValueIndex = j;
			}
		}
		// Переносим найденный максимум в первую позицию в неотсортированной части -
		// расширяем отсортированную часть. 
		// А элемент из этой позиции пишем на место выбранного максимум:  
		temporaryIndexRecord = ptr_indexesAttendance[maximumKeyValueIndex];
		ptr_indexesAttendance[maximumKeyValueIndex] = ptr_indexesAttendance[i];
		ptr_indexesAttendance[i] = temporaryIndexRecord;
	}
}

static void FillDynamicArrayUsingKeyboard(unsigned short arrayLength, Student* ptr_studentsArray, StudentSurnameIndex* ptr_indexesSurname, StudentAttendanceNumberIndex* ptr_indexesAttendance) {

	for (int i = 0; i < arrayLength; i++)
	{
		printf("Введите номер студенческого билета для %d записи: ", i + 1);
		cin >> ptr_studentsArray[i].studentID;

		printf("Введите фамилию студента для %d записи: ", i + 1);
		cin >> ptr_studentsArray[i].studentSurname;

		printf("Введите имя студента для %d записи: ", i + 1);
		cin >> ptr_studentsArray[i].studentName;

		printf("Введите отчество студента для %d записи: ", i + 1);
		cin >> ptr_studentsArray[i].studentSecondName;

		printf("Введите корпоративную электронную почту студента для %d записи: ", i + 1);
		cin >> ptr_studentsArray[i].studentEmail;

		printf("Введите спортивный разряд студента для %d записи (если разряда нет, ввести \"Нет\"): ", i + 1);
		cin >> ptr_studentsArray[i].studentSportRanking;

		printf("Введите группу здоровья студента для %d записи: ", i + 1);
		cin >> ptr_studentsArray[i].studentHealthCategory;

		printf("Введите спортивную секцию, на которую записан студент, для %d записи: ", i + 1);
		cin >> ptr_studentsArray[i].studentSportActivity;

		printf("Введите количество посещений спортивной секции у студента для %d записи: ", i + 1);
		cin >> ptr_studentsArray[i].studentSportActivityAttendanceNumber;

		AddElementToIndexSurnameArray(i, ptr_studentsArray[i].studentSurname, ptr_indexesSurname);

		ptr_indexesAttendance[i].initialIndex = i;
		ptr_indexesAttendance[i].studentSportActivityAttendanceNumber = ptr_studentsArray[i].studentSportActivityAttendanceNumber;
	}
	SortAttendanceIndexArrayUsingSelection(ptr_indexesAttendance, arrayLength);
	cout << "Заполнение массива исходных данных завершено." << endl;
}

static void FillDynamicArrayUsingFile(unsigned short arrayLength, Student* ptr_studentsArray, StudentSurnameIndex* ptr_indexesSurname, StudentAttendanceNumberIndex* ptr_indexesAttendance) {
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
		for (int i = 0; i < arrayLength && file >> ptr_studentsArray[i].studentID >> ptr_studentsArray[i].studentSurname
			>> ptr_studentsArray[i].studentName >> ptr_studentsArray[i].studentSecondName >> ptr_studentsArray[i].studentEmail
			>> ptr_studentsArray[i].studentSportRanking >> ptr_studentsArray[i].studentHealthCategory
			>> ptr_studentsArray[i].studentSportActivity >> ptr_studentsArray[i].studentSportActivityAttendanceNumber; i++)
		{
			AddElementToIndexSurnameArray(i, ptr_studentsArray[i].studentSurname, ptr_indexesSurname);
			ptr_indexesAttendance[i].initialIndex = i;
			ptr_indexesAttendance[i].studentSportActivityAttendanceNumber = ptr_studentsArray[i].studentSportActivityAttendanceNumber;
		}
		SortAttendanceIndexArrayUsingSelection(ptr_indexesAttendance, arrayLength);
	}
	file.close();
	cout << "Заполнение массива исходных данных завершено." << endl;
}


static void PrintSortedArray(unsigned short arrayLength, Student* ptr_studentsArray, StudentSurnameIndex* ptr_indexesSurname) {
	int currentInitialIndex;
	for (int i = 0; i < arrayLength; i++) {
		currentInitialIndex = ptr_indexesSurname[i].initialIndex;
		cout << "_________________________________________________________________________________" << endl;
		cout << "Номер студенческого билета для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentID << endl;
		cout << "Фамилия студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSurname << endl;
		cout << "Имя студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentName << endl;
		cout << "Отчество студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSecondName << endl;
		cout << "Корпоративная электронная почта студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentEmail << endl;
		cout << "Спортивный разряд студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSportRanking << endl;
		cout << "Группа здоровья студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentHealthCategory << endl;
		cout << "Спортивная секция, на которую записан студент, для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSportActivity << endl;
		cout << "Число посещений спортивной секции у студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSportActivityAttendanceNumber << endl;
	}
	cout << "_________________________________________________________________________________" << endl;
	cout << "_________________________________________________________________________________" << endl;
	cout << "_________________________________________________________________________________" << endl;
}

static void PrintSortedArray(unsigned short arrayLength, Student* ptr_studentsArray, StudentAttendanceNumberIndex* ptr_indexesAttendance) {
	int currentInitialIndex;
	for (int i = 0; i < arrayLength; i++) {
		currentInitialIndex = ptr_indexesAttendance[i].initialIndex;
		cout << "_________________________________________________________________________________" << endl;
		cout << "Номер студенческого билета для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentID << endl;
		cout << "Фамилия студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSurname << endl;
		cout << "Имя студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentName << endl;
		cout << "Отчество студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSecondName << endl;
		cout << "Корпоративная электронная почта студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentEmail << endl;
		cout << "Спортивный разряд студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSportRanking << endl;
		cout << "Группа здоровья студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentHealthCategory << endl;
		cout << "Спортивная секция, на которую записан студент, для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSportActivity << endl;
		cout << "Число посещений спортивной секции у студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSportActivityAttendanceNumber << endl;
	}
	cout << "_________________________________________________________________________________" << endl;
	cout << "_________________________________________________________________________________" << endl;
	cout << "_________________________________________________________________________________" << endl;
}

static StudentAttendanceNumberIndex* ReverseArray(StudentAttendanceNumberIndex* ptr_indexesAttendance, int arrayLength) {
	StudentAttendanceNumberIndex temporaryElement;
	for (int i = 0; i < arrayLength / 2; i++) {
		temporaryElement = ptr_indexesAttendance[i];
		ptr_indexesAttendance[i] = ptr_indexesAttendance[arrayLength - i - 1];
		ptr_indexesAttendance[arrayLength - i - 1] = temporaryElement;
	}
	return ptr_indexesAttendance;
}

static int ConductBinarySearch(string soughtSurname, StudentSurnameIndex* ptr_indexesSurname, int leftLimit, int rightLimit) {
	if (leftLimit > rightLimit) {
		return -1; // Элемент не найден
	}

	int middlePointer = leftLimit + (rightLimit - leftLimit) / 2; // Находим средний индекс

	if (GetKeyValue(ptr_indexesSurname[middlePointer]) == soughtSurname) {
		return middlePointer; // Элемент найден, возвращаем индекс
	}
	else if (GetKeyValue(ptr_indexesSurname[middlePointer]) > soughtSurname) {
		return ConductBinarySearch(soughtSurname, ptr_indexesSurname, leftLimit, middlePointer - 1); // Ищем в левой половине
	}
	else {
		return ConductBinarySearch(soughtSurname, ptr_indexesSurname, middlePointer + 1, rightLimit); // Ищем в правой половине
	}
}

static int ConductBinarySearch(int soughtAttendamceNumber, StudentAttendanceNumberIndex* ptr_indexesAttendance, unsigned short arrayLength) {
	ptr_indexesAttendance = ReverseArray(ptr_indexesAttendance, arrayLength);
	int leftLimit = 0;
	int rightLimit = arrayLength - 1;
	while (rightLimit > leftLimit)
	{
		int middlePointer = (rightLimit + leftLimit) / 2;
		if (GetKeyValue(ptr_indexesAttendance[middlePointer]) < soughtAttendamceNumber) {
			leftLimit = middlePointer + 1;
		}
		else if (GetKeyValue(ptr_indexesAttendance[middlePointer]) > soughtAttendamceNumber) {
			rightLimit = middlePointer - 1;
		}
		else {
			return middlePointer;
		}
	}
	if (GetKeyValue(ptr_indexesAttendance[leftLimit]) == soughtAttendamceNumber) {
		return leftLimit;
	}
	else {
		return -1; // если элемент не найден
	}
}

static void FindSurnameUsingRecursion(unsigned short arrayLength, Student* ptr_studentsArray, StudentSurnameIndex* ptr_indexesSurname) {
	string soughtSurname;
	cout << "Введите искомую строку - фамилию студента: ";
	cin >> soughtSurname;
	int indexSoughtElement = ConductBinarySearch(soughtSurname, ptr_indexesSurname, 0, arrayLength - 1);
	if (indexSoughtElement == -1) {
		cout << "Искомый элемент не найден." << endl;
	}
	else {
		int neighbourLeftElementIndex = indexSoughtElement;
		int neighbourRightElementIndex = indexSoughtElement;
		bool isSoughtNeighbourLeftElement = true;
		bool isSoughtNeighbourRightElement = true;
		while (isSoughtNeighbourLeftElement || isSoughtNeighbourRightElement) {
			if ((neighbourLeftElementIndex <= -1) || (GetKeyValue(ptr_indexesSurname[neighbourLeftElementIndex]) != soughtSurname)) {
				isSoughtNeighbourLeftElement = false;
			}
			else {
				neighbourLeftElementIndex--;
			}
			if ((neighbourRightElementIndex >= arrayLength) || (GetKeyValue(ptr_indexesSurname[neighbourRightElementIndex]) != soughtSurname)) {
				isSoughtNeighbourRightElement = false;
			}
			else {
				neighbourRightElementIndex++;
			}
		}
		cout << "Найденная(-ые) запись(-и) с искомым элементом:" << endl;
		int currentInitialIndex;
		for (int i = neighbourLeftElementIndex + 1; i < neighbourRightElementIndex; i++) {
			currentInitialIndex = ptr_indexesSurname[i].initialIndex;
			cout << "_________________________________________________________________________________" << endl;
			cout << "Номер студенческого билета для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentID << endl;
			cout << "Фамилия студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSurname << endl;
			cout << "Имя студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentName << endl;
			cout << "Отчество студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSecondName << endl;
			cout << "Корпоративная электронная почта студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentEmail << endl;
			cout << "Спортивный разряд студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSportRanking << endl;
			cout << "Группа здоровья студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentHealthCategory << endl;
			cout << "Спортивная секция, на которую записан студент, для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSportActivity << endl;
			cout << "Число посещений спортивной секции у студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSportActivityAttendanceNumber << endl;
		}
		cout << "_________________________________________________________________________________" << endl;
		cout << "_________________________________________________________________________________" << endl;
		cout << "_________________________________________________________________________________" << endl;
	}
}

static void FindAttendanceNumberUsingIterations(unsigned short arrayLength, Student* ptr_studentsArray, StudentAttendanceNumberIndex* ptr_indexesAttendance) {
	int soughtAttendanceNumber;
	cout << "Введите искомое число - число посещений у студента: ";
	cin >> soughtAttendanceNumber;
	int indexSoughtElement = ConductBinarySearch(soughtAttendanceNumber, ptr_indexesAttendance, arrayLength);
	if (indexSoughtElement == -1) {
		cout << "Искомый элемент не найден." << endl;
	}
	else {
		int neighbourLeftElementIndex = indexSoughtElement;
		int neighbourRightElementIndex = indexSoughtElement;
		bool isSoughtNeighbourLeftElement = true;
		bool isSoughtNeighbourRightElement = true;
		while (isSoughtNeighbourLeftElement || isSoughtNeighbourRightElement) {
			if ((neighbourLeftElementIndex <= -1) || (GetKeyValue(ptr_indexesAttendance[neighbourLeftElementIndex]) != soughtAttendanceNumber)) {
				isSoughtNeighbourLeftElement = false;
			}
			else {
				neighbourLeftElementIndex--;
			}
			if ((neighbourRightElementIndex >= arrayLength) || (GetKeyValue(ptr_indexesAttendance[neighbourRightElementIndex]) != soughtAttendanceNumber)) {
				isSoughtNeighbourRightElement = false;
			}
			else {
				neighbourRightElementIndex++;
			}
		}
		cout << "Найденная(-ые) запись(-и) с искомым элементом:" << endl;
		int currentInitialIndex;
		for (int i = neighbourLeftElementIndex + 1; i < neighbourRightElementIndex; i++) {
			currentInitialIndex = ptr_indexesAttendance[i].initialIndex;
			cout << "_________________________________________________________________________________" << endl;
			cout << "Номер студенческого билета для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentID << endl;
			cout << "Фамилия студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSurname << endl;
			cout << "Имя студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentName << endl;
			cout << "Отчество студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSecondName << endl;
			cout << "Корпоративная электронная почта студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentEmail << endl;
			cout << "Спортивный разряд студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSportRanking << endl;
			cout << "Группа здоровья студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentHealthCategory << endl;
			cout << "Спортивная секция, на которую записан студент, для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSportActivity << endl;
			cout << "Число посещений спортивной секции у студента для " << currentInitialIndex + 1 << " записи: " << ptr_studentsArray[currentInitialIndex].studentSportActivityAttendanceNumber << endl;
		}
		cout << "_________________________________________________________________________________" << endl;
		cout << "_________________________________________________________________________________" << endl;
		cout << "_________________________________________________________________________________" << endl;
	}
}

int main()
{
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);

	Student* ptr_studentsArray;
	StudentSurnameIndex* ptr_indexesSurname;
	StudentAttendanceNumberIndex* ptr_indexesAttendance;
	unsigned short arrayLength;

	cout << "Введите число записей в массиве: ";
	cin >> arrayLength;

	ptr_studentsArray = new Student[arrayLength];
	ptr_indexesSurname = new StudentSurnameIndex[arrayLength];
	ptr_indexesAttendance = new StudentAttendanceNumberIndex[arrayLength];

	if (!CheckIfArrayIsNull(ptr_studentsArray))
	{
		int menuPointInput;
		do {
			cout << "Выберите, какими данными следует заполнить массив:" << endl;
			cout << "1. Данные, введенные с клавиатуры через консоль." << endl;
			cout << "2. Данные из файла." << endl;
			cout << "3. Завершить ввод." << endl;
			cout << "Введите номер выбранного вами пункта: ";
			cin >> menuPointInput;

			switch (menuPointInput)
			{
			case 1:
			{
				FillDynamicArrayUsingKeyboard(arrayLength, ptr_studentsArray, ptr_indexesSurname, ptr_indexesAttendance);
			}
			break;
			case 2:
			{
				FillDynamicArrayUsingFile(arrayLength, ptr_studentsArray, ptr_indexesSurname, ptr_indexesAttendance);
			}
			break;
			default:
				break;
			}
		} while (menuPointInput != 3);

		int menuPointOutput;
		do {
			cout << "Выберите, по какому значению отсортировать и вывести массив:" << endl;
			cout << "1. По фамилии студента (в лексико-графическом порядке)." << endl;
			cout << "2. По количеству посещений спортивной секции у студента (по убыванию)." << endl;
			cout << "3. Завершить вывод." << endl;
			cout << "Введите номер выбранного вами пункта: ";
			cin >> menuPointOutput;

			switch (menuPointOutput)
			{
			case 1:
			{
				cout << "Записи исходного массива, упорядоченные в лексико-графическом порядке по фамилиям студентов:" << endl;
				PrintSortedArray(arrayLength, ptr_studentsArray, ptr_indexesSurname);
			}
			break;
			case 2:
			{
				cout << "Записи исходного массива, упорядоченные по убыванию количества посещений спортивной секции у студентов:" << endl;
				PrintSortedArray(arrayLength, ptr_studentsArray, ptr_indexesAttendance);
			}
			break;
			default:
				break;
			}
		} while (menuPointOutput != 3);

		int menuPointSearch;
		do {
			cout << "Выберите, по какому значению осуществлять поиск в массиве:" << endl;
			cout << "1. По фамилии студента." << endl;
			cout << "2. По количеству посещений спортивной секции у студента." << endl;
			cout << "3. Завершить поиск." << endl;
			cout << "Введите номер выбранного вами пункта: ";
			cin >> menuPointSearch;

			switch (menuPointSearch)
			{
			case 1:
			{
				FindSurnameUsingRecursion(arrayLength, ptr_studentsArray, ptr_indexesSurname);
			}
			break;
			case 2:
			{
				FindAttendanceNumberUsingIterations(arrayLength, ptr_studentsArray, ptr_indexesAttendance);
			}
			break;
			default:
				break;
			}
		} while (menuPointSearch != 3);

		int menuPointRedact;
		do {
			cout << "Выберите, какое значение вы хотите отредактировать:" << endl;
			cout << "1. Фамилию студента." << endl;
			cout << "2. Количество посещений спортивной секции у студента." << endl;
			cout << "3. Завершить редактирование." << endl;
			cout << "Введите номер выбранного вами пункта: ";
			cin >> menuPointRedact;

			switch (menuPointRedact)
			{
			case 1:
			{
				FindSurnameUsingRecursion(arrayLength, ptr_studentsArray, ptr_indexesSurname);
			}
			break;
			case 2:
			{
				FindAttendanceNumberUsingIterations(arrayLength, ptr_studentsArray, ptr_indexesAttendance);
			}
			break;
			default:
				break;
			}
		} while (menuPointRedact != 3);
	}
	else
	{
		cout << "Память для массива исходных данных не была выделена." << endl;
	}

	delete[] ptr_studentsArray;
	delete[] ptr_indexesSurname;
	delete[] ptr_indexesAttendance;

	return 0;
}
