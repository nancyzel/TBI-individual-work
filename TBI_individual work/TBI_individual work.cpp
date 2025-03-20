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
		cout << "Запись №" << currentInitialIndex + 1 << ": " << endl;
		cout << "Номер студенческого билета: " << ptr_studentsArray[currentInitialIndex].studentID << endl;
		cout << "Фамилия студента: " << ptr_studentsArray[currentInitialIndex].studentSurname << endl;
		cout << "Имя студента: " << ptr_studentsArray[currentInitialIndex].studentName << endl;
		cout << "Отчество студента: " << ptr_studentsArray[currentInitialIndex].studentSecondName << endl;
		cout << "Корпоративная электронная почта студента: " << ptr_studentsArray[currentInitialIndex].studentEmail << endl;
		cout << "Спортивный разряд студента: " << ptr_studentsArray[currentInitialIndex].studentSportRanking << endl;
		cout << "Группа здоровья студента: " << ptr_studentsArray[currentInitialIndex].studentHealthCategory << endl;
		cout << "Спортивная секция, на которую записан студент: " << ptr_studentsArray[currentInitialIndex].studentSportActivity << endl;
		cout << "Число посещений спортивной секции у студента: " << ptr_studentsArray[currentInitialIndex].studentSportActivityAttendanceNumber << endl;
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
		cout << "Запись №" << currentInitialIndex + 1 << ": " << endl;
		cout << "Номер студенческого билета: " << ptr_studentsArray[currentInitialIndex].studentID << endl;
		cout << "Фамилия студента: " << ptr_studentsArray[currentInitialIndex].studentSurname << endl;
		cout << "Имя студента: " << ptr_studentsArray[currentInitialIndex].studentName << endl;
		cout << "Отчество студента: " << ptr_studentsArray[currentInitialIndex].studentSecondName << endl;
		cout << "Корпоративная электронная почта студента: " << ptr_studentsArray[currentInitialIndex].studentEmail << endl;
		cout << "Спортивный разряд студента: " << ptr_studentsArray[currentInitialIndex].studentSportRanking << endl;
		cout << "Группа здоровья студента: " << ptr_studentsArray[currentInitialIndex].studentHealthCategory << endl;
		cout << "Спортивная секция, на которую записан студент: " << ptr_studentsArray[currentInitialIndex].studentSportActivity << endl;
		cout << "Число посещений спортивной секции у студента: " << ptr_studentsArray[currentInitialIndex].studentSportActivityAttendanceNumber << endl;
	}
	cout << "_________________________________________________________________________________" << endl;
	cout << "_________________________________________________________________________________" << endl;
	cout << "_________________________________________________________________________________" << endl;
}

static StudentAttendanceNumberIndex* ReverseArray(StudentAttendanceNumberIndex* ptr_indexesAttendance, int arrayLength) {
	StudentAttendanceNumberIndex* temporaryIndexesAttendance = new StudentAttendanceNumberIndex[arrayLength];
	for (int i = 0; i < arrayLength; i++) {
		temporaryIndexesAttendance[i] = ptr_indexesAttendance[i];
	}
	StudentAttendanceNumberIndex temporaryElement;
	for (int i = 0; i < arrayLength / 2; i++) {
		temporaryElement = temporaryIndexesAttendance[i];
		temporaryIndexesAttendance[i] = temporaryIndexesAttendance[arrayLength - i - 1];
		temporaryIndexesAttendance[arrayLength - i - 1] = temporaryElement;
	}
	return temporaryIndexesAttendance;
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
			cout << "Запись №" << currentInitialIndex + 1 << ": " << endl;
			cout << "Номер студенческого билета: " << ptr_studentsArray[currentInitialIndex].studentID << endl;
			cout << "Фамилия студента: " << ptr_studentsArray[currentInitialIndex].studentSurname << endl;
			cout << "Имя студента: " << ptr_studentsArray[currentInitialIndex].studentName << endl;
			cout << "Отчество студента: " << ptr_studentsArray[currentInitialIndex].studentSecondName << endl;
			cout << "Корпоративная электронная почта студента: " << ptr_studentsArray[currentInitialIndex].studentEmail << endl;
			cout << "Спортивный разряд студента: " << ptr_studentsArray[currentInitialIndex].studentSportRanking << endl;
			cout << "Группа здоровья студента: " << ptr_studentsArray[currentInitialIndex].studentHealthCategory << endl;
			cout << "Спортивная секция, на которую записан студент: " << ptr_studentsArray[currentInitialIndex].studentSportActivity << endl;
			cout << "Число посещений спортивной секции у студента: " << ptr_studentsArray[currentInitialIndex].studentSportActivityAttendanceNumber << endl;
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
	StudentAttendanceNumberIndex* temporaryIndexesAttendance = ReverseArray(ptr_indexesAttendance, arrayLength);
	int indexSoughtElement = ConductBinarySearch(soughtAttendanceNumber, ReverseArray(ptr_indexesAttendance, arrayLength), arrayLength);
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
			cout << "Запись №" << currentInitialIndex + 1 << ": " << endl;
			cout << "Номер студенческого билета: " << ptr_studentsArray[currentInitialIndex].studentID << endl;
			cout << "Фамилия студента: " << ptr_studentsArray[currentInitialIndex].studentSurname << endl;
			cout << "Имя студента: " << ptr_studentsArray[currentInitialIndex].studentName << endl;
			cout << "Отчество студента: " << ptr_studentsArray[currentInitialIndex].studentSecondName << endl;
			cout << "Корпоративная электронная почта студента: " << ptr_studentsArray[currentInitialIndex].studentEmail << endl;
			cout << "Спортивный разряд студента: " << ptr_studentsArray[currentInitialIndex].studentSportRanking << endl;
			cout << "Группа здоровья студента: " << ptr_studentsArray[currentInitialIndex].studentHealthCategory << endl;
			cout << "Спортивная секция, на которую записан студент: " << ptr_studentsArray[currentInitialIndex].studentSportActivity << endl;
			cout << "Число посещений спортивной секции у студента: " << ptr_studentsArray[currentInitialIndex].studentSportActivityAttendanceNumber << endl;
		}
		cout << "_________________________________________________________________________________" << endl;
		cout << "_________________________________________________________________________________" << endl;
		cout << "_________________________________________________________________________________" << endl;
	}
	delete[] temporaryIndexesAttendance;
}

static int ConductBinarySearchWhileEditing(string soughtSurname, StudentSurnameIndex* ptr_indexesSurname, int leftLimit, int rightLimit) {
	while (rightLimit > leftLimit)
	{
		int middlePointer = (rightLimit + leftLimit) / 2;
		if (GetKeyValue(ptr_indexesSurname[middlePointer]) < soughtSurname) {
			leftLimit = middlePointer + 1;
		}
		else {
			rightLimit = middlePointer - 1;
		}
	}
	return leftLimit;
}

static int ConductBinarySearchWhileEditing(int soughtAttendanceNumber, StudentAttendanceNumberIndex* ptr_indexesAttendance, int leftLimit, int rightLimit) {
	while (rightLimit > leftLimit)
	{
		int middlePointer = (rightLimit + leftLimit) / 2;
		if (GetKeyValue(ptr_indexesAttendance[middlePointer]) < soughtAttendanceNumber) {
			leftLimit = middlePointer + 1;
		}
		else {
			rightLimit = middlePointer - 1;
		}
	}
	return leftLimit;
}

static void EditIndexesArray(unsigned short arrayLength, Student* ptr_studentsArray, StudentSurnameIndex* ptr_indexesSurname) {
	PrintSortedArray(arrayLength, ptr_studentsArray, ptr_indexesSurname);
	string neededSurname;
	cout << "Выберите фамилию, которую хотите отредактировать: ";
	cin >> neededSurname;
	int editedRecordIndex = ConductBinarySearch(neededSurname, ptr_indexesSurname, 0, arrayLength - 1);
	if (editedRecordIndex < 0) {
		cout << "Искомая фамилия не найдена." << endl;
	}
	else {
		int neededInitialIndex = ptr_indexesSurname[editedRecordIndex].initialIndex;
		string currentSurname;
		cout << "Введите новую фамилию для студента: ";
		cin >> currentSurname;
		ptr_studentsArray[neededInitialIndex].studentSurname = currentSurname;
		int temporaryIndex = -1;
		string temporarySurname = "";
		int insertionIndex = -1;
		if (editedRecordIndex > 0 && GetKeyValue(ptr_indexesSurname[editedRecordIndex - 1]) > currentSurname) {
			int checkedInsertionIndex = ConductBinarySearchWhileEditing(currentSurname, ptr_indexesSurname, 0, editedRecordIndex - 1);
			cout << "checkedInsertionIndex=" << checkedInsertionIndex << endl;
			if (GetKeyValue(ptr_indexesSurname[checkedInsertionIndex]) < currentSurname) {
				insertionIndex = min(checkedInsertionIndex + 1, editedRecordIndex);
			}
			else {
				insertionIndex = max(checkedInsertionIndex, 0);
			}
			temporaryIndex = ptr_indexesSurname[insertionIndex].initialIndex;
			temporarySurname = GetKeyValue(ptr_indexesSurname[insertionIndex]);
			ptr_indexesSurname[insertionIndex].initialIndex = neededInitialIndex;
			ptr_indexesSurname[insertionIndex].studentSurname = currentSurname;
			for (int j = editedRecordIndex; j > insertionIndex + 1; j--) {
				ptr_indexesSurname[j].initialIndex = ptr_indexesSurname[j - 1].initialIndex;
				ptr_indexesSurname[j].studentSurname = ptr_indexesSurname[j - 1].studentSurname;
			}
			ptr_indexesSurname[insertionIndex + 1].initialIndex = temporaryIndex;
			ptr_indexesSurname[insertionIndex + 1].studentSurname = temporarySurname;
		}
		else if (editedRecordIndex < arrayLength - 1 && GetKeyValue(ptr_indexesSurname[editedRecordIndex + 1]) < currentSurname) {
			int checkedInsertionIndex = ConductBinarySearchWhileEditing(currentSurname, ptr_indexesSurname, editedRecordIndex + 1, arrayLength - 1);
			cout << "checkedInsertionIndex=" << checkedInsertionIndex << endl;
			if (GetKeyValue(ptr_indexesSurname[checkedInsertionIndex]) < currentSurname) {
				insertionIndex = min(checkedInsertionIndex, arrayLength - 1);
			}
			else {
				insertionIndex = max(checkedInsertionIndex - 1, 0);
			}
			temporaryIndex = ptr_indexesSurname[insertionIndex].initialIndex;
			temporarySurname = GetKeyValue(ptr_indexesSurname[insertionIndex]);
			ptr_indexesSurname[insertionIndex].initialIndex = neededInitialIndex;
			ptr_indexesSurname[insertionIndex].studentSurname = currentSurname;
			for (int j = editedRecordIndex; j < insertionIndex - 1; j++) {
				ptr_indexesSurname[j].initialIndex = ptr_indexesSurname[j + 1].initialIndex;
				ptr_indexesSurname[j].studentSurname = ptr_indexesSurname[j + 1].studentSurname;
			}
			ptr_indexesSurname[insertionIndex - 1].initialIndex = temporaryIndex;
			ptr_indexesSurname[insertionIndex - 1].studentSurname = temporarySurname;
		}
		PrintSortedArray(arrayLength, ptr_studentsArray, ptr_indexesSurname);
	}
}

static void EditIndexesArray(unsigned short arrayLength, Student* ptr_studentsArray, StudentAttendanceNumberIndex* ptr_indexesAttendance) {
	PrintSortedArray(arrayLength, ptr_studentsArray, ptr_indexesAttendance);
	ptr_indexesAttendance = ReverseArray(ptr_indexesAttendance, arrayLength);
	int neededAttendanceNumber;
	cout << "Выберите число посещений, которое хотите отредактировать: ";
	cin >> neededAttendanceNumber;
	int editedRecordIndex = ConductBinarySearch(neededAttendanceNumber, ptr_indexesAttendance, arrayLength);
	if (editedRecordIndex < 0) {
		cout << "Искомое число посещений не найдено." << endl;
	}
	else {
		int currentAttendanceNumber;
		cout << "Введите новое число посещений для записи: ";
		cin >> currentAttendanceNumber;
		int neededInitialIndex = ptr_indexesAttendance[editedRecordIndex].initialIndex;
		ptr_studentsArray[neededInitialIndex].studentSportActivityAttendanceNumber = currentAttendanceNumber;
		int temporaryIndex = -1;
		int temporaryAttendanceNumber = 0;
		int insertionIndex = -1;
		if (editedRecordIndex > 0 && GetKeyValue(ptr_indexesAttendance[editedRecordIndex - 1]) > currentAttendanceNumber) {
			int checkedInsertionIndex = ConductBinarySearchWhileEditing(currentAttendanceNumber, ptr_indexesAttendance, 0, editedRecordIndex - 1);
			cout << "checkedInsertionIndex=" << checkedInsertionIndex << endl;
			if (GetKeyValue(ptr_indexesAttendance[checkedInsertionIndex]) < currentAttendanceNumber) {
				insertionIndex = min(checkedInsertionIndex + 1, editedRecordIndex);
			}
			else {
				insertionIndex = max(checkedInsertionIndex, 0);
			}
			temporaryIndex = ptr_indexesAttendance[insertionIndex].initialIndex;
			temporaryAttendanceNumber = GetKeyValue(ptr_indexesAttendance[insertionIndex]);
			ptr_indexesAttendance[insertionIndex].initialIndex = neededInitialIndex;
			ptr_indexesAttendance[insertionIndex].studentSportActivityAttendanceNumber = currentAttendanceNumber;
			for (int j = editedRecordIndex; j > insertionIndex + 1; j--) {
				ptr_indexesAttendance[j].initialIndex = ptr_indexesAttendance[j - 1].initialIndex;
				ptr_indexesAttendance[j].studentSportActivityAttendanceNumber = ptr_indexesAttendance[j - 1].studentSportActivityAttendanceNumber;
			}
			ptr_indexesAttendance[insertionIndex + 1].initialIndex = temporaryIndex;
			ptr_indexesAttendance[insertionIndex + 1].studentSportActivityAttendanceNumber = temporaryAttendanceNumber;
		}
		else if (editedRecordIndex < arrayLength - 1 && GetKeyValue(ptr_indexesAttendance[editedRecordIndex + 1]) < currentAttendanceNumber) {
			int checkedInsertionIndex = ConductBinarySearchWhileEditing(currentAttendanceNumber, ptr_indexesAttendance, editedRecordIndex + 1, arrayLength - 1);
			cout << "checkedInsertionIndex=" << checkedInsertionIndex << endl;
			if (GetKeyValue(ptr_indexesAttendance[checkedInsertionIndex]) < currentAttendanceNumber) {
				insertionIndex = min(checkedInsertionIndex, arrayLength - 1);
			}
			else {
				insertionIndex = max(checkedInsertionIndex - 1, 0);
			}
			temporaryIndex = ptr_indexesAttendance[insertionIndex].initialIndex;
			temporaryAttendanceNumber = GetKeyValue(ptr_indexesAttendance[insertionIndex]);
			ptr_indexesAttendance[insertionIndex].initialIndex = neededInitialIndex;
			ptr_indexesAttendance[insertionIndex].studentSportActivityAttendanceNumber = temporaryAttendanceNumber;
			for (int j = editedRecordIndex; j < insertionIndex - 1; j++) {
				ptr_indexesAttendance[j].initialIndex = ptr_indexesAttendance[j + 1].initialIndex;
				ptr_indexesAttendance[j].studentSportActivityAttendanceNumber = ptr_indexesAttendance[j + 1].studentSportActivityAttendanceNumber;
			}
			ptr_indexesAttendance[insertionIndex - 1].initialIndex = temporaryIndex;
			ptr_indexesAttendance[insertionIndex - 1].studentSportActivityAttendanceNumber = temporaryAttendanceNumber;
		}
	}
	ptr_indexesAttendance = ReverseArray(ptr_indexesAttendance, arrayLength);
	PrintSortedArray(arrayLength, ptr_studentsArray, ptr_indexesAttendance);
}

static void DeleteOrderBySurname(unsigned short arrayLength, Student* ptr_studentsArray, StudentAttendanceNumberIndex* ptr_indexesAttendance, StudentSurnameIndex* ptr_indexesSurname) {
	PrintSortedArray(arrayLength, ptr_studentsArray, ptr_indexesSurname);
	string deletedSurname;
	cout << "Введите фамилию, запись с которой хотите удалить: ";
	cin >> deletedSurname;
	int deletedSurnameIndex = ConductBinarySearch(deletedSurname, ptr_indexesSurname, 0, arrayLength - 1);
	if (deletedSurnameIndex < 0) {
		cout << "Записи с искомой фамилией нет в массиве." << endl;
	}
	else {
		int deletedOrderIndex = ptr_indexesSurname[deletedSurnameIndex].initialIndex;
		Student* temporaryStudentsArray = new Student[arrayLength - 1];
		StudentAttendanceNumberIndex* temporaryIndexesAttendance = new StudentAttendanceNumberIndex[arrayLength - 1];
		StudentSurnameIndex* temporaryIndexesSurname = new StudentSurnameIndex[arrayLength - 1];
		for (int i = 0; i < arrayLength - 1; i++) {
			if (i < deletedOrderIndex) {
				temporaryStudentsArray[i] = ptr_studentsArray[i];
			}
			else {
				temporaryStudentsArray[i] = ptr_studentsArray[i + 1];
			}
		}
		delete[] ptr_studentsArray;
		//располагаем в верном порядке записи индекс-массива по фамилиям
		int j = 0;
		while (ptr_indexesSurname[j].initialIndex != deletedOrderIndex && j < arrayLength - 1) {
			if (ptr_indexesSurname[j].initialIndex < deletedOrderIndex) {
				temporaryIndexesSurname[j] = ptr_indexesSurname[j];
			}
			else {
				temporaryIndexesSurname[j].initialIndex = ptr_indexesSurname[j].initialIndex - 1;
				temporaryIndexesSurname[j].studentSurname = ptr_indexesSurname[j].studentSurname;
			}
			j++;
		}
		for (j; j < arrayLength - 1; j++) {
			if (ptr_indexesSurname[j + 1].initialIndex < deletedOrderIndex) {
				temporaryIndexesSurname[j] = ptr_indexesSurname[j + 1];
			}
			else {
				temporaryIndexesSurname[j].initialIndex = ptr_indexesSurname[j + 1].initialIndex - 1;
				temporaryIndexesSurname[j].studentSurname = ptr_indexesSurname[j + 1].studentSurname;
			}
		}

		//располагаем в верном порядке записи индекс-массива по числу посещений
		j = 0;
		while (ptr_indexesAttendance[j].initialIndex != deletedOrderIndex && j < arrayLength - 1) {
			if (ptr_indexesAttendance[j].initialIndex < deletedOrderIndex) {
				temporaryIndexesAttendance[j] = ptr_indexesAttendance[j];
			}
			else {
				temporaryIndexesAttendance[j].initialIndex = ptr_indexesAttendance[j].initialIndex - 1;
				temporaryIndexesAttendance[j].studentSportActivityAttendanceNumber = ptr_indexesAttendance[j].studentSportActivityAttendanceNumber;
			}
			j++;
		}
		for (j; j < arrayLength - 1; j++) {
			if (ptr_indexesAttendance[j + 1].initialIndex < deletedOrderIndex) {
				temporaryIndexesAttendance[j] = ptr_indexesAttendance[j + 1];
			}
			else {
				temporaryIndexesAttendance[j].initialIndex = ptr_indexesAttendance[j + 1].initialIndex - 1;
				temporaryIndexesAttendance[j].studentSportActivityAttendanceNumber = ptr_indexesAttendance[j + 1].studentSportActivityAttendanceNumber;
			}
		}
		delete[] ptr_indexesAttendance;
		delete[] ptr_indexesSurname;
		ptr_studentsArray = new Student[arrayLength - 1];
		ptr_indexesAttendance = new StudentAttendanceNumberIndex[arrayLength - 1];
		ptr_indexesSurname = new StudentSurnameIndex[arrayLength - 1];
		for (int n = 0; n < arrayLength - 1; n++) {
			ptr_studentsArray[n] = temporaryStudentsArray[n];
			ptr_indexesAttendance[n] = temporaryIndexesAttendance[n];
			ptr_indexesSurname[n] = temporaryIndexesSurname[n];
		}
		delete[] temporaryStudentsArray;
		delete[] temporaryIndexesAttendance;
		delete[] temporaryIndexesSurname;
		PrintSortedArray(arrayLength - 1, ptr_studentsArray, ptr_indexesSurname);
	}
}

static void DeleteOrderByAttendanceNumber(unsigned short arrayLength, Student* ptr_studentsArray, StudentAttendanceNumberIndex* ptr_indexesAttendance, StudentSurnameIndex* ptr_indexesSurname) {
	PrintSortedArray(arrayLength, ptr_studentsArray, ptr_indexesAttendance);
	int deletedAttendanceNumber;
	cout << "Введите число посещений, запись с которым хотите удалить: ";
	cin >> deletedAttendanceNumber;
	ptr_indexesAttendance = ReverseArray(ptr_indexesAttendance, arrayLength);
	int deletedAttendanceIndex = ConductBinarySearch(deletedAttendanceNumber, ptr_indexesAttendance, arrayLength);
	if (deletedAttendanceIndex < 0) {
		cout << "Записи с искомым числом посещений нет в массиве." << endl;
	}
	else {
		int deletedOrderIndex = ptr_indexesAttendance[deletedAttendanceIndex].initialIndex;
		ptr_indexesAttendance = ReverseArray(ptr_indexesAttendance, arrayLength);
		Student* temporaryStudentsArray = new Student[arrayLength - 1];
		StudentAttendanceNumberIndex* temporaryIndexesAttendance = new StudentAttendanceNumberIndex[arrayLength - 1];
		StudentSurnameIndex* temporaryIndexesSurname = new StudentSurnameIndex[arrayLength - 1];
		for (int i = 0; i < arrayLength - 1; i++) {
			if (i < deletedOrderIndex) {
				temporaryStudentsArray[i] = ptr_studentsArray[i];
			}
			else {
				temporaryStudentsArray[i] = ptr_studentsArray[i + 1];
			}
		}
		delete[] ptr_studentsArray;
		//располагаем в верном порядке записи индекс-массива по фамилиям
		int j = 0;
		while (ptr_indexesSurname[j].initialIndex != deletedOrderIndex && j < arrayLength - 1) {
			if (ptr_indexesSurname[j].initialIndex < deletedOrderIndex) {
				temporaryIndexesSurname[j] = ptr_indexesSurname[j];
			}
			else {
				temporaryIndexesSurname[j].initialIndex = ptr_indexesSurname[j].initialIndex - 1;
				temporaryIndexesSurname[j].studentSurname = ptr_indexesSurname[j].studentSurname;
			}
			j++;
		}
		for (j; j < arrayLength - 1; j++) {
			if (ptr_indexesSurname[j + 1].initialIndex < deletedOrderIndex) {
				temporaryIndexesSurname[j] = ptr_indexesSurname[j + 1];
			}
			else {
				temporaryIndexesSurname[j].initialIndex = ptr_indexesSurname[j + 1].initialIndex - 1;
				temporaryIndexesSurname[j].studentSurname = ptr_indexesSurname[j + 1].studentSurname;
			}
		}

		//располагаем в верном порядке записи индекс-массива по числу посещений
		j = 0;
		while (ptr_indexesAttendance[j].initialIndex != deletedOrderIndex && j < arrayLength - 1) {
			if (ptr_indexesAttendance[j].initialIndex < deletedOrderIndex) {
				temporaryIndexesAttendance[j] = ptr_indexesAttendance[j];
			}
			else {
				temporaryIndexesAttendance[j].initialIndex = ptr_indexesAttendance[j].initialIndex - 1;
				temporaryIndexesAttendance[j].studentSportActivityAttendanceNumber = ptr_indexesAttendance[j].studentSportActivityAttendanceNumber;
			}
			j++;
		}
		for (j; j < arrayLength - 1; j++) {
			if (ptr_indexesAttendance[j + 1].initialIndex < deletedOrderIndex) {
				temporaryIndexesAttendance[j] = ptr_indexesAttendance[j + 1];
			}
			else {
				temporaryIndexesAttendance[j].initialIndex = ptr_indexesAttendance[j + 1].initialIndex - 1;
				temporaryIndexesAttendance[j].studentSportActivityAttendanceNumber = ptr_indexesAttendance[j + 1].studentSportActivityAttendanceNumber;
			}
		}
		delete[] ptr_indexesAttendance;
		delete[] ptr_indexesSurname;
		ptr_studentsArray = new Student[arrayLength - 1];
		ptr_indexesAttendance = new StudentAttendanceNumberIndex[arrayLength - 1];
		ptr_indexesSurname = new StudentSurnameIndex[arrayLength - 1];
		for (int n = 0; n < arrayLength - 1; n++) {
			ptr_studentsArray[n] = temporaryStudentsArray[n];
			ptr_indexesAttendance[n] = temporaryIndexesAttendance[n];
			ptr_indexesSurname[n] = temporaryIndexesSurname[n];
		}
		delete[] temporaryStudentsArray;
		delete[] temporaryIndexesAttendance;
		delete[] temporaryIndexesSurname;
		PrintSortedArray(arrayLength - 1, ptr_studentsArray, ptr_indexesAttendance);
	}
}
static void PrintArray(Student* ptr_studentsArray, int arrayLength) {
	for (int i = 0; i < arrayLength; i++) {
		cout << "_________________________________________________________________________________" << endl;
		cout << "Запись №" << i + 1 << ": " << endl;
		cout << "Номер студенческого билета: " << ptr_studentsArray[i].studentID << endl;
		cout << "Фамилия студента: " << ptr_studentsArray[i].studentSurname << endl;
		cout << "Имя студента: " << ptr_studentsArray[i].studentName << endl;
		cout << "Отчество студента: " << ptr_studentsArray[i].studentSecondName << endl;
		cout << "Корпоративная электронная почта студента: " << ptr_studentsArray[i].studentEmail << endl;
		cout << "Спортивный разряд студента: " << ptr_studentsArray[i].studentSportRanking << endl;
		cout << "Группа здоровья студента: " << ptr_studentsArray[i].studentHealthCategory << endl;
		cout << "Спортивная секция, на которую записан студент: " << ptr_studentsArray[i].studentSportActivity << endl;
		cout << "Число посещений спортивной секции у студента: " << ptr_studentsArray[i].studentSportActivityAttendanceNumber << endl;
	}
	cout << "_________________________________________________________________________________" << endl;
	cout << "_________________________________________________________________________________" << endl;
	cout << "_________________________________________________________________________________" << endl;
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
				EditIndexesArray(arrayLength, ptr_studentsArray, ptr_indexesSurname);
			}
			break;
			case 2:
			{
				EditIndexesArray(arrayLength, ptr_studentsArray, ptr_indexesAttendance);
			}
			break;
			default:
				break;
			}
		} while (menuPointRedact != 3);
		int menuPointDelete;

		cout << "Выберите, по какому значению вы хотите удалить запись:" << endl;
		cout << "1. По фамилии студента." << endl;
		cout << "2. По количеству посещений спортивной секции у студента." << endl;
		cout << "3. Завершить удаление." << endl;
		cout << "Введите номер выбранного вами пункта: ";
		cin >> menuPointDelete;
		switch (menuPointDelete)
		{
		case 1:
		{
			DeleteOrderBySurname(arrayLength, ptr_studentsArray, ptr_indexesAttendance, ptr_indexesSurname);
		}
		break;
		case 2:
		{
			DeleteOrderByAttendanceNumber(arrayLength, ptr_studentsArray, ptr_indexesAttendance, ptr_indexesSurname);
		}
		break;
		default:
			break;
		}
	}
	else
	{
		cout << "Память для массива исходных данных не была выделена." << endl;
	}
	cout << "Работа завершена.";

	delete[] ptr_studentsArray;
	delete[] ptr_indexesAttendance;
	delete[] ptr_indexesSurname;

	return 0;
}
