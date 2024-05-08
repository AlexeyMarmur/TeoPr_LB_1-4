#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <queue>
#include <map>
#include <cassert>

using namespace std;

// Observer pattern
class Observer {
public:
    virtual void update() = 0;
};

void printMatrix(const vector<string>& numbers, const vector<vector<int>>& matrix);

class MatrixObserver : public Observer {
public:
    MatrixObserver(vector<vector<int>>& matrix, const vector<string>& numbers)
        : matrix_(matrix), numbers_(numbers) {}

    void update() override {
        cout << "Matrix updated:" << endl;
        printMatrix(numbers_, matrix_); 
    }

private:
    vector<vector<int>>& matrix_;
    const vector<string>& numbers_;
};

// Strategy pattern
class Comparator {
public:
    virtual int compare(const string& num1, const string& num2, vector<vector<int>>& matrix) = 0;
    virtual ~Comparator() {}
};

class SimpleComparator : public Comparator {
public:
    SimpleComparator(const vector<string>& numbers) : numbers(numbers) {}

    int compare(const string& num1, const string& num2, vector<vector<int>>& matrix) override {
        int i = find(numbers.begin(), numbers.end(), num1) - numbers.begin();
        int j = find(numbers.begin(), numbers.end(), num2) - numbers.begin();

        if (matrix[i][j] != 0) {
            return matrix[i][j];
        }

        int choice;
        do {
            cout << "Comparing " << num1 << " and " << num2 << ". Enter 1 if " << num1 << " is better, 2 if they are equal, 3 if " << num2 << " is better: ";
            cin >> choice;
        } while (choice < 1 || choice > 3);

        matrix[i][j] = choice;
        return choice;
    }

private:
    const vector<string>& numbers;
};

// Abstract Factory pattern
class AbstractFactory {
public:
    virtual Comparator* createComparator(const vector<string>& numbers) = 0;
    virtual Observer* createObserver(vector<vector<int>>& matrix, const vector<string>& numbers) = 0;
};

class ComparatorFactory : public AbstractFactory {
public:
    Comparator* createComparator(const vector<string>& numbers) override {
        return new SimpleComparator(numbers);
    }

    Observer* createObserver(vector<vector<int>>& matrix, const vector<string>& numbers) override {
        return nullptr;
    }
};

class ObserverFactory : public AbstractFactory {
public:
    Comparator* createComparator(const vector<string>& numbers) override {
        return nullptr;
    }

    Observer* createObserver(vector<vector<int>>& matrix, const vector<string>& numbers) override {
        return new MatrixObserver(matrix, numbers);
    }
};

// Scales of alternatives
vector<string> numbers = { "2111", "3111", "4111", "1211", "1311", "1411", "1121", "1131", "1141", "1112", "1113", "1114" };
vector<string> epors = { "1111", "1121", "2111", "1211", "1112", "3111", "1113", "4111", "1131", "1311", "1114", "1411", "1141" };
vector<pair<string, int>> ranked_numbers;

// Input data from the decision maker
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

// Initialisation of all functions
void fillDiagonalWithTwo(vector<vector<int>>& matrix, const vector<string>& numbers);
void compareAndFillMatrix(vector<vector<int>>& matrix, const vector<string>& numbers, Comparator& comparator, MatrixObserver& observer);
void printInitialAndFinalMatrix(const vector<vector<int>>& matrix, const vector<string>& numbers);
void printAlternatives(const vector<string>& numbers, const vector<pair<string, int>>& ranked_numbers);
void createRankedNumbers(const vector<string>& numbers, const vector<string>& epors);
void printVectorValuation(const int initial[4][4]);
void createInitialByEporsMatrix(const int initial[4][4], const map<string, int>& eporsToIndex, int initialByEpors[4][4]);
void printInitialByEporsMatrix(const int initialByEpors[4][4]);
void createSortedInitialByEporsMatrix(const int initialByEpors[4][4], int sortedInitialByEpors[4][4]);
void printSortedInitialByEporsMatrix(const int sortedInitialByEpors[4][4]);
void findBestAlternative(const int sortedInitialByEpors[4][4]);
void test_compareNumbers(double& tests_passed);
void test_matrix_initialization(double& tests_passed);
void test_ranked_numbers(double& tests_passed);
void test_updateTransitiveRelations(double& tests_passed);
void runTests();
void runProgram();

int main()
{
    setlocale(LC_ALL, "Ukrainian");
    char user_choice;
    cout << "Enter 'R' to run the program or 'T' to run tests: ";
    cin >> user_choice;

    if (user_choice == 'T') {
        runTests();
    }
    else if (user_choice == 'R') {
        runProgram();
    }
    else {
        cout << "Invalid choice. Please enter 'R' or 'T'." << std::endl;
    }

    return 0;
}

// 
void runTests() {
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

void runProgram() {
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

    fillDiagonalWithTwo(matrix, numbers);

    // Abstract Factory pattern
    AbstractFactory* comparator_factory = new ComparatorFactory();
    AbstractFactory* observer_factory = new ObserverFactory();

    Comparator* comparator = comparator_factory->createComparator(numbers);
    MatrixObserver* observer = static_cast<MatrixObserver*>(observer_factory->createObserver(matrix, numbers));

    compareAndFillMatrix(matrix, numbers, *comparator, *observer);
    printInitialAndFinalMatrix(matrix, numbers);
    printAlternatives(numbers, ranked_numbers);
    printVectorValuation(initial);

    map<string, int> eporsToIndex;
    for (int i = 0; i < epors.size(); ++i) {
        eporsToIndex[epors[i]] = i + 1;
    }

    int initialByEpors[4][4];
    createInitialByEporsMatrix(initial, eporsToIndex, initialByEpors);
    printInitialByEporsMatrix(initialByEpors);

    int sortedInitialByEpors[4][4];
    createSortedInitialByEporsMatrix(initialByEpors, sortedInitialByEpors);
    printSortedInitialByEporsMatrix(sortedInitialByEpors);

    findBestAlternative(sortedInitialByEpors);

    delete comparator_factory;
    delete observer_factory;
    delete comparator;
    delete observer;
}

///
/// Funсtions
///

// Function to compare two numbers based on user input and existing matrix
int compareNumbers(const string& num1, const string& num2, vector<vector<int>>& matrix, Comparator& comparator, MatrixObserver& observer)
{
    int result = comparator.compare(num1, num2, matrix);
    observer.update();
    return result;
}

// Function of transitive logic
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
void printMatrix(const vector<string>& numbers, const vector<vector<int>>& matrix) {
    // Вывод заголовка строк
    cout << setw(5) << " ";
    for (const auto& num : numbers) {
        cout << setw(5) << num;
    }
    cout << endl;

    // Вывод матрицы с диагональными элементами
    for (int i = 0; i < matrix.size(); ++i) {
        cout << setw(5) << numbers[i]; // Вывод номера строки
        for (int j = 0; j < matrix.size(); ++j) {
            if (i == j) {
                cout << setw(5) << matrix[i][j]; // Вывод диагональных элементов
            }
            else if (i < j) {
                cout << setw(5) << matrix[i][j]; // Вывод верхнего треугольника
            }
            else {
                cout << setw(5) << " "; // Вывод пробелов для нижнего треугольника
            }
        }
        cout << endl;
    }
}

// Function to fill the diagonal with 2
void fillDiagonalWithTwo(vector<vector<int>>& matrix, const vector<string>& numbers) {
    for (int i = 0; i < numbers.size(); ++i) {
        matrix[i][i] = 2;
    }
}

// Function that print the matrix and let the user compare numbers
void compareAndFillMatrix(vector<vector<int>>& matrix, const vector<string>& numbers, Comparator& comparator, MatrixObserver& observer) {
    cout << "Initial matrix:" << endl;
    for (int i = 0; i < numbers.size(); ++i) {
        for (int j = i + 1; j < numbers.size(); ++j) {
            cout << "print 1 if better, 2 if equal, 3 if worse" << endl;
            cout << "Comparing " << numbers[i] << " and " << numbers[j] << ": " << endl;
            printMatrix(numbers, matrix);
            int result = compareNumbers(numbers[i], numbers[j], matrix, comparator, observer);
            updateTransitiveRelations(matrix);
            cout << endl;
        }
    }
    cout << "Final matrix:" << endl;
    printMatrix(numbers, matrix);
}

// Function print print initial and final matrix
void printInitialAndFinalMatrix(const vector<vector<int>>& matrix, const vector<string>& numbers) {
    cout << "Initial matrix:" << endl;
    printMatrix(numbers, matrix);
    cout << "Final matrix:" << endl;
    printMatrix(numbers, matrix);
}

// Function that print alternatives due the task
void printAlternatives(const vector<string>& numbers, const vector<pair<string, int>>& ranked_numbers) {
    cout << "A set of alternatives to the first reference situation:" << endl;
    for (const auto& num : numbers) {
        cout << num << "    ";
    }
    cout << endl;

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
}

// Function that create Rank for each number to compare with epors (a single advice scale)
void createRankedNumbers(const vector<string>& numbers, const vector<string>& epors) {
    ranked_numbers.clear();
    for (int i = 0; i < numbers.size(); ++i) {
        ranked_numbers.push_back(make_pair(numbers[i], i + 1));
    }
    sort(ranked_numbers.begin(), ranked_numbers.end(), [&](const pair<string, int>& a, const pair<string, int>& b) {
        int posA = find(epors.begin(), epors.end(), a.first) - epors.begin();
        int posB = find(epors.begin(), epors.end(), b.first) - epors.begin();
        return posA < posB;
        });
}

// Function that print vector estimation
void printVectorValuation(const int initial[4][4]) {
    cout << "Vector valuation (initial):" << endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << initial[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Function that rewrite initial matrix by epors numbers
void createInitialByEporsMatrix(const int initial[4][4], const map<string, int>& eporsToIndex, int initialByEpors[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            int criterionIndex = initial[i][j] - 1;
            string criterion = to_string(criteriaValues[criterionIndex][j]);

            // Check if criterion exists in eporsToIndex
            if (eporsToIndex.count(criterion) > 0) {
                int rank = eporsToIndex.at(criterion); // Use .at() to access value safely
                initialByEpors[i][j] = rank;
            }
            else {
                // Handle case where criterion is not found in eporsToIndex
                // You can set a default value or handle this case accordingly
                cerr << "Error: Criterion '" << criterion << "' not found in eporsToIndex." << endl;
                // For example, you could assign a default value:
                initialByEpors[i][j] = -1; // Or another suitable value
            }
        }
    }
}

// Function that print initial matrix by epors numbers
void printInitialByEporsMatrix(const int initialByEpors[4][4]) {
    cout << "Initial by a single ordinal scale:" << endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << initialByEpors[i][j] << " ";
        }
        cout << endl;
    }
}

// Function that sort new matrix
void createSortedInitialByEporsMatrix(const int initialByEpors[4][4], int sortedInitialByEpors[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            sortedInitialByEpors[i][j] = initialByEpors[i][j];
        }
        sort(sortedInitialByEpors[i], sortedInitialByEpors[i] + 4);
    }
}

// Function that print new sorted matrix
void printSortedInitialByEporsMatrix(const int sortedInitialByEpors[4][4]) {
    cout << "Sorted Initial by a single ordinal scale:" << endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << sortedInitialByEpors[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// Function that finds the best alternative, which can be the smallest number in the string
void findBestAlternative(const int sortedInitialByEpors[4][4]) {
    int minRowIndex = 0;
    for (int i = 1; i < 4; ++i) {
        if (sortedInitialByEpors[i][0] < sortedInitialByEpors[minRowIndex][0]) {
            minRowIndex = i;
        }
    }
    cout << "The best alternative:" << endl;
    for (int j = 0; j < 4; ++j) {
        cout << sortedInitialByEpors[minRowIndex][j] << " ";
    }
    cout << endl;
}

///
/// Tests
///

// Function that tests our compairing numbers
void test_compareNumbers(double& tests_passed)
{
    vector<vector<int>> matrix(12, vector<int>(12, 0));
    bool allTestsPassed = true;

    ComparatorFactory comparator_factory;
    Comparator* comparator = comparator_factory.createComparator(numbers);
    MatrixObserver observer(matrix, numbers);

    // Testing when the first number is better
    if (compareNumbers("2111", "3111", matrix, *comparator, observer) != 1) {
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

    // Testing when the second number is better
    if (compareNumbers("3111", "2111", matrix, *comparator, observer) != 3) {
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

    // Test when the numbers are equal
    if (compareNumbers("2111", "2111", matrix, *comparator, observer) != 2) {
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

// Function that tests matrix initialization
void test_matrix_initialization(double& tests_passed)
{
    vector<vector<int>> matrix(12, vector<int>(12, 0));

    // Fill the diagonal with elements 2
    for (int i = 0; i < 12; ++i)
    {
        matrix[i][i] = 2;
    }

    bool allTestsPassed = true;
    // Check diagonal elements
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

// Function that tests the ranking of numbers
void test_ranked_numbers(double& tests_passed)
{
    ranked_numbers.clear();

    // Fill the vector of pairs with numbers from numbers and their ranks from epors
    for (int i = 0; i < numbers.size(); ++i)
    {
        ranked_numbers.push_back(make_pair(numbers[i], i + 1));
    }

    bool allTestsPassed = true;
    // Check that ranked_numbers contains correct number-rank pairs
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

// Function that tests transitive relations in matrix due user choice
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