#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <queue>
#include <map>
#include <cassert>

using namespace std;

vector<string> numbers = { "2111", "3111", "4111", "1211", "1311", "1411", "1121", "1131", "1141", "1112", "1113", "1114" };

vector<string> epors = { "1111", "1121", "2111", "1211", "1112", "3111", "1113", "4111", "1131", "1311", "1114", "1411", "1141" };

vector<pair<string, int>> ranked_numbers;

int initial[4][4] = {
    {4, 1, 2, 3},
    { 3, 4, 1, 2},
    { 2, 3, 4, 1},
    { 1, 2, 3, 4}
};
int criteriaValues[4][4] = {
    { 1111, 1111, 1111, 1111},
  { 2111, 1211, 1121, 1112},
  { 3111, 1311, 1131, 1113},
  { 4111, 1411, 1141, 1114}
};

// Function to compare two numbers based on user input and existing matrix
int compareNumbers(const string& num1, const string& num2, vector<vector<int>>& matrix)
{
    int i = find(numbers.begin(), numbers.end(), num1) - numbers.begin();
    int j = find(numbers.begin(), numbers.end(), num2) - numbers.begin();

    if (matrix[i][j] != 0)
    {
        return matrix[i][j];
    }

    int choice;
    do
    {
        cout << "Comparing " << num1 << " and " << num2 << ". Enter 1 if " << num1 << " is better, 2 if they are equal, 3 if " << num2 << " is better: ";
        cin >> choice;
    } while (choice < 1 || choice > 3);

    matrix[i][j] = choice; // Update the matrix with the user's choice
    return choice;
}

void updateTransitiveRelations(vector<vector<int>>& matrix)
{
    int n = matrix.size();
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (matrix[i][j] == 1)
            {
                for (int k = 0; k < n; ++k)
                {
                    if (matrix[j][k] != 0 && matrix[i][k] == 0)
                    {
                        if (matrix[j][k] == 1 || matrix[j][k] == 2)
                        {
                            matrix[i][k] = 1;
                        }
                    }
                }
            }
            else if (matrix[i][j] == 3)
            {
                for (int k = 0; k < n; ++k)
                {
                    if (matrix[j][k] != 0 && matrix[i][k] == 0)
                    {
                        if (matrix[j][k] == 3 || matrix[j][k] == 2)
                        {
                            matrix[i][k] = 3;
                        }
                    }
                }
            }
        }
    }
}

// Function to print the matrix
void printMatrix(const vector<string>& numbers, const vector<vector<int>>& matrix)
{
    // Print the header row
    cout << setw(5) << " ";
    for (const auto& num : numbers) {
        cout << setw(5) << num;
    }
    cout << endl;

    // Print the matrix with diagonal elements
    for (int i = 0; i < matrix.size(); ++i)
    {
        cout << setw(5) << numbers[i];
        for (int j = 0; j < matrix.size(); ++j)
        {
            if (i == j)
            {
                cout << setw(5) << matrix[i][j]; // Print diagonal elements
            }
            else if (i < j)
            {
                cout << setw(5) << matrix[i][j]; // Print upper triangle
            }
            else
            {
                cout << setw(5) << " "; // Print spaces for the lower triangle
            }
        }
        cout << endl;
    }
}

//
// Tests
//
void test_compareNumbers(double& tests_passed)
{
    vector<vector<int>> matrix(12, vector<int>(12, 0));
    bool allTestsPassed = true;

    // Тестируем, когда первое число лучше
    if (compareNumbers("2111", "3111", matrix) != 1) {
        cout << "Test failed: Expected '2111' to be better than '3111'." << endl;
        allTestsPassed = false;
    }
    if (matrix[0][1] != 1) {
        cout << "Test failed: Matrix value not updated correctly for '2111' vs '3111'." << endl;
        allTestsPassed = false;
    }
    if (matrix[1][0] != 0) {
        cout << "Test failed: Matrix value updated incorrectly for '2111' vs '3111'." << endl;
        allTestsPassed = false;
    }

    // Тестируем, когда второе число лучше
    if (compareNumbers("3111", "2111", matrix) != 3) {
        cout << "Test failed: Expected '3111' to be worse than '2111'." << endl;
        allTestsPassed = false;
    }
    if (matrix[0][1] != 1) {
        cout << "Test failed: Matrix value not updated correctly for '3111' vs '2111'." << endl;
        allTestsPassed = false;
    }
    if (matrix[1][0] != 3) {
        cout << "Test failed: Matrix value updated incorrectly for '3111' vs '2111'." << endl;
        allTestsPassed = false;
    }

    // Тестируем, когда числа равны
    if (compareNumbers("2111", "2111", matrix) != 2) {
        cout << "Test failed: Expected '2111' and '2111' to be equal." << endl;
        allTestsPassed = false;
    }
    if (matrix[0][0] != 2) {
        cout << "Test failed: Matrix value not updated correctly for '2111' vs '2111'." << endl;
        allTestsPassed = false;
    }

    if (allTestsPassed) {
        cout << "test_compareNumbers passed." << endl << endl;
        tests_passed++;
    }
    else
        cout << "test_compareNumbers failed." << endl << endl;
}

void test_matrix_initialization(double& tests_passed)
{
    vector<vector<int>> matrix(12, vector<int>(12, 0));

    // Заполняем диагональ элементами 2
    for (int i = 0; i < 12; ++i)
    {
        matrix[i][i] = 2;
    }

    bool allTestsPassed = true;
    // Проверяем диагональные элементы
    for (int i = 0; i < 12; ++i)
    {
        if (matrix[i][i] != 2) {
            cout << "Test failed: Matrix value not initialized correctly at position [" << i << "][" << i << "]." << endl;
            allTestsPassed = false;
        }
    }

    if (allTestsPassed) {
        cout << "test_matrix_initialization passed." << endl << endl;
        tests_passed++;
    }
    else {
        cout << "test_matrix_initialization failed." << endl << endl;
    }
}

void test_ranked_numbers(double& tests_passed)
{
    ranked_numbers.clear();

    // Заполняем вектор пар числами из numbers и их рангами из epors
    for (int i = 0; i < numbers.size(); ++i)
    {
        ranked_numbers.push_back(make_pair(numbers[i], i + 1));
    }

    bool allTestsPassed = true;
    // Проверяем, что ranked_numbers содержит правильные пары число-ранг
    for (int i = 0; i < numbers.size(); ++i)
    {
        if (ranked_numbers[i].first != numbers[i] || ranked_numbers[i].second != i + 1) {
            cout << "Test failed: Incorrect pair at index " << i << " in ranked_numbers." << endl;
            allTestsPassed = false;
        }
    }

    if (allTestsPassed) {
        cout << "test_ranked_numbers_initialization passed." << endl << endl;
        tests_passed++;
    }
    else {
        cout << "test_ranked_numbers_initialization failed." << endl << endl;
    }
}

void test_updateTransitiveRelations(double& tests_passed)
{
    vector<vector<int>> matrix = {
        {2, 1, 0, 0},
        {0, 2, 1, 0},
        {0, 0, 2, 1},
        {0, 0, 0, 2}
    };

    vector<vector<int>> expectedMatrix = {
        {2, 1, 1, 1},
        {0, 2, 1, 1},
        {0, 0, 2, 1},
        {0, 0, 0, 2}
    };

    updateTransitiveRelations(matrix);

    bool allTestsPassed = true;

    for (int i = 0; i < matrix.size(); ++i)
    {
        for (int j = 0; j < matrix.size(); ++j)
        {
            if (matrix[i][j] != expectedMatrix[i][j])
            {
                cout << "Test failed: Matrix value not updated correctly at position [" << i << "][" << j << "]." << endl;
                cout << "Expected: " << expectedMatrix[i][j] << ", got: " << matrix[i][j] << endl;
                allTestsPassed = false;
            }
        }
    }

    if (allTestsPassed)
    {
        cout << "test_updateTransitiveRelations passed." << endl << endl;
        tests_passed++;
    }
    else
    {
        cout << "test_updateTransitiveRelations failed." << endl << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "Ukrainian");
    char user_choice;
    cout << "Enter 'R' to run the program or 'T' to run tests: ";
    cin >> user_choice;

    if (user_choice == 'T') {
        double tests_passed = 0;
        double all_tests = 4;
        cout << "Running tests..." << endl << endl;
        test_compareNumbers(tests_passed);
        test_matrix_initialization(tests_passed);
        test_ranked_numbers(tests_passed);
        test_updateTransitiveRelations(tests_passed);

        cout << "Values of passed tests: " << tests_passed << endl;

        if (tests_passed == all_tests)
        {
            cout << "All tests passed - 100%" << endl;
        }
        else
        {
            cout << "Not all tests have been passed. Passed : " << tests_passed << " of " << all_tests << endl;
            cout << "Percentage: " << (tests_passed / all_tests) * 100 << "%" << endl << endl;
        }

    }
    else if (user_choice == 'R') {

        cout << "Running program..." << endl << endl;

        // Matrix to store the comparison results
        vector<vector<int>> matrix = {
          {2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 2, 1, 1, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 2, 1, 1, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}
        };

        // Fill in the diagonal elements with 2
        for (int i = 0; i < numbers.size(); ++i)
        {
            matrix[i][i] = 2;
        }
        cout << "Initial matrix:" << endl;
        // Compare each pair of numbers and fill in the matrix
        for (int i = 0; i < numbers.size(); ++i)
        {
            for (int j = i + 1; j < numbers.size(); ++j)
            {
                cout << "print 1 if better, 2 if equal, 3 if worse" << endl;
                cout << "Comparing " << numbers[i] << " and " << numbers[j] << ": " << endl;
                printMatrix(numbers, matrix);
                int result = compareNumbers(numbers[i], numbers[j], matrix);
                updateTransitiveRelations(matrix); // Apply transitive property after every comparison
                cout << endl;
            }
        }

        // Print the final matrix
        cout << "Final matrix:" << endl;
        printMatrix(numbers, matrix);

        cout << "A set of alternatives to the first reference situation:" << endl;
        for (const auto& num : numbers) {
            cout << num << "    ";
        }
        cout << endl;


        // Заполняем вектор пар числами из numbers и их рангами из epors
        for (int i = 0; i < numbers.size(); ++i)
        {
            ranked_numbers.push_back(make_pair(numbers[i], i + 1)); // Используем i+1 как начальный ранг
        }

        // Сортируем ranked_numbers по числовым значениям в соответствии с порядком в epors
        sort(ranked_numbers.begin(), ranked_numbers.end(), [&](const pair<string, int>& a, const pair<string, int>& b) {
            int posA = find(epors.begin(), epors.end(), a.first) - epors.begin();
            int posB = find(epors.begin(), epors.end(), b.first) - epors.begin();
            return posA < posB;
            });

        // Выводим упорядоченные числа
        cout << "Sorted set of alternatives to the first reference situation:" << endl;
        for (const auto& num : ranked_numbers) {
            cout << num.first << "\t";
        }
        cout << endl;

        cout << "Single ordinal scale" << endl;
        for (const auto& ep : epors) {
            cout << ep << "\t";
        }
        cout << endl;

        cout << endl;

        cout << "Vector valuation (initial):" << endl;

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                cout << initial[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;

        // Создаем карту для сопоставления чисел из epors их порядковыми номерами
        map<string, int> eporsToIndex;
        for (int i = 0; i < epors.size(); ++i)
        {
            eporsToIndex[epors[i]] = i + 1;
        }

        // Создаем новую матрицу initialByEpors
        int initialByEpors[4][4];
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                int criterionIndex = initial[i][j] - 1;
                string criterion = to_string(criteriaValues[criterionIndex][j]);
                int rank = eporsToIndex[criterion];
                initialByEpors[i][j] = rank;
            }
        }

        // Выводим новую матрицу initialByEpors
        cout << "Initial by a single ordinal scale:" << endl;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                cout << initialByEpors[i][j] << " ";
            }
            cout << endl;
        }

        // Создаем новую матрицу sortedInitialByEpors
        int sortedInitialByEpors[4][4];
        for (int i = 0; i < 4; ++i)
        {
            // Копируем строчку из initialByEpors в sortedInitialByEpors
            for (int j = 0; j < 4; ++j)
            {
                sortedInitialByEpors[i][j] = initialByEpors[i][j];
            }
            // Сортируем строчку
            std::sort(sortedInitialByEpors[i], sortedInitialByEpors[i] + 4);
        }
        cout << endl;

        // Выводим новую матрицу sortedInitialByEpors
        cout << "Sorted Initial by a single ordinal scale:" << endl;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                cout << sortedInitialByEpors[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;

        // Находим строчку с наименьшими числами
        int minRowIndex = 0;
        for (int i = 1; i < 4; ++i)
        {
            if (sortedInitialByEpors[i][0] < sortedInitialByEpors[minRowIndex][0])
            {
                minRowIndex = i;
            }
        }
        // Выводим строчку с наименьшими числами
        cout << "The best alternative:" << endl;
        for (int j = 0; j < 4; ++j)
        {
            cout << sortedInitialByEpors[minRowIndex][j] << " ";
        }
        cout << endl;
    }

    else {
        std::cout << "Invalid choice. Please enter 'R' or 'T'." << std::endl;
    }

    return 0;
}

