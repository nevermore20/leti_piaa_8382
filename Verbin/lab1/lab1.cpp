#include <iostream>
#include <vector>
#include <time.h>

//#define DEBUG
int add_count = 0;
int del_count = 0;
int rel_count = 0;
int com_count = 0;

using namespace std;

struct Square
{
    int y;
    int x;
    int width;
    int num;
};

void printField(int** field, int width)
{
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++)
            cout << field[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

void printVector(vector<Square> vector)
{
    
    for (int i = 0; i < vector.size(); ++i)
        cout  << vector[i].x + 1 << " " << vector[i].y + 1 << " " << vector[i].width  << endl;
}

int findSize(int** field, int widthOfField, int y0, int x0)
{
    int width;                                                                   //
    for (width = 2; width < widthOfField + 1; ++width) {                          //
        for (int y = y0; y < y0 + width; ++y) {                                  //
            for (int x = x0; x < x0 + width; ++x) {                              //  Нахождение максимальной ширины
                if (y >= widthOfField || x >= widthOfField || field[y][x] != 0)  //  квадрата, который можно вставить
                    return width - 1;                                            //  в данной точке
            }                                                                    //
        }                                                                        //
    }                                                                            //
    return width - 1;                                                            //
}

void drawSquare(int** field, int width, int y0, int x0, int num)
{
    for (int y = y0; y < y0 + width; y++)
        for (int x = x0; x < x0 + width; x++)
            field[y][x] = num;
}

void addToVector(vector<Square>& vector, int y, int x, int width, int num)
{
    Square square;
    square.y = y;
    square.x = x;
    square.width = width;
    square.num = num;
    vector.push_back(square);
    add_count++;
}

void even(int width)
{
    cout << "Minimal count of Squares: " << 4 << endl;
    cout << 1 << " Point ( x = " << "1, y = " << "1, width = " << width / 2 << ")" << endl;
    cout << 2 << " Point ( x = " << "1, y = " << width / 2 + 1 << ", width = " << width / 2 << ")" << endl;
    cout << 3 << " Point ( x = " << width / 2 + 1 << ", y = " << "1, width = " << width / 2 << ")" << endl;
    cout << 4 << " Point ( x = " << width / 2 + 1 << ", y = " << width / 2 + 1 << ", width = " << width / 2 << ")" << endl;
}

void odd3(int width) {
    cout << "Minimal count of Squares: " << 6 << endl;
    cout << 1 << " Point ( x = " << "1, y = 1, width = " << 2 * width / 3 << ")" << endl;
    cout << 2 << " Point ( x = " << "1, y = " << 2 * width / 3 + 1 << ", width = " << width / 3 << ")" << endl;
    cout << 3 << " Point ( x = " << width / 3 + 1 << ", y = " << 2 * width / 3 + 1 << ", width = " << width / 3 << ")" << endl;
    cout << 4 << " Point ( x = " << 2 * width / 3 + 1 << ", y = " << 2 * width / 3 + 1 << ", width = " << width / 3 << ")" << endl;
    cout << 5 << " Point ( x = " << 2 * width / 3 + 1 << ", y = " << width / 3 + 1 << ", width = " << width / 3 << ")" << endl;
    cout << 6 << " Point ( x = " << 2 * width / 3 + 1 << ", y = " << 1 << ", width = " << width / 3 << ")" << endl;
}

void odd5(int width) {
    cout << "Minimal count of Squares: " << 8 << endl;
    cout << 1 << " Point ( x = " << "1, y = 1, width = " << 3 * width / 5 << ")" << endl;
    cout << 2 << " Point ( x = " << "1, y = " << 3 * width / 5 + 1 << ", width = " << 2 * width / 5 << ")" << endl;
    cout << 3 << " Point ( x = " << 2 * width / 5 + 1 << ", y = " << 3 * width / 5 + 1 << ", width = " << 2 * width / 5 << ")" << endl;
    cout << 4 << " Point ( x = " << 3 * width / 5 + 1 << ", y = 1, width = " << 2 * width / 5 << ")" << endl;
    cout << 5 << " Point ( x = " << 3 * width / 5 + 1 << ", y = " << 2 * width / 5 + 1 << ", width = " << width / 5 << ")" << endl;
    cout << 6 << " Point ( x = " << 4 * width / 5 + 1 << ", y = " << 2 * width / 5 + 1 << ", width = " << width / 5 << ")" << endl;
    cout << 7 << " Point ( x = " << 4 * width / 5 + 1 << ", y = " << 3 * width / 5 + 1 << ", width = " << width / 5 << ")" << endl;
    cout << 8 << " Point ( x = " << 4 * width / 5 + 1 << ", y = " << 4 * width / 5 + 1 << ", width = " << width / 5 << ")" << endl;
}

void vectorToBestVector(vector<Square>& bestVector, vector<Square> vector, int widthOfLargerSquare)
{
    bestVector[3].width = widthOfLargerSquare;
    int i;
    int size = bestVector.size() - 4;
    for (i = 0; i < size; ++i)
        bestVector.pop_back();
    for (i = 0; i < vector.size(); ++i) {
        addToVector(bestVector, vector[i].y, vector[i].x, vector[i].width, vector[i].num);
    }
    rel_count++;
}

void fill(int width, vector<Square>& bestVector)
{
    vector<Square> vector;

    int countOfSquares = 4;

    int newWidth = width / 2 + 1;             //  Ширина квадрата в левом верхнем углу

    int** field = new int* [newWidth];         //
    for (int y = 0; y < newWidth; ++y)         //  Выделение памяти под двумерный массив
        field[y] = new int[newWidth];         //

    int startWidth = newWidth / 2 + 1;
    //  Добавление четырех начальных квадратов
    addToVector(bestVector, newWidth - 1, newWidth - 1, newWidth, 1);   //   _ _ _  в вектор с лучшей конфигурацией
    addToVector(bestVector, 0, newWidth, width - newWidth, 2);          //  |_|_|_|
    addToVector(bestVector, newWidth, 0, width - newWidth, 3);          //  |_|   |
    addToVector(bestVector, 0, 0, startWidth, 4);                       //  |_|_ _|

    for (int widthOfLargerSquare = startWidth; widthOfLargerSquare >= 1; --widthOfLargerSquare) {

        bool flagToEnd = false;

#ifdef DEBUG
        cout << "Draw the first square with a width of one less: " << endl << endl;
#endif

        for (int y = 0; y < newWidth; ++y)                               //
            for (int x = 0; x < newWidth; ++x) {                         //
                if (x < widthOfLargerSquare && y < widthOfLargerSquare)  //  В верхний левый угол квадрата
                    field[y][x] = 1;                                     //  вставляется квадрат с определенной
                else                                                     //  шириной, остальные клетки зануляются
                    field[y][x] = 0;                                     //
            }                                                            //

        field[newWidth - 1][newWidth - 1] = -1;                          //  Крайний нижний угол замощаемого в процессе
                                                                         //  бэктрэкинга квадрата устанавливается в -1

#ifdef DEBUG
        printField(field, newWidth);
        cout << "Square filling: " << endl << endl;
#endif
        for (int y = 0, num = 2; y < newWidth && !flagToEnd; ++y)        //  Нулевые клетки замещаются квадратами
            for (int x = 0; x < newWidth && !flagToEnd; ++x) {           //  максимально возможной ширины, при этом
                if (field[y][x] == 0) {                                  //  частное решение заносится в вектор
                    if (vector.size() + 4 >= countOfSquares && widthOfLargerSquare != startWidth) {                    //  Если количество квадратов, необходимых
                        flagToEnd = true;                                                                             //  в получаемом частном решении, больше количества
#ifdef DEBUG                                                                                  //  квадратов, получаемых при лучшей конфигурации, поднимается
                        cout << "Already found a solution with the same or fewer squares!" << endl << endl;       //  флаг для прекращения поиска лучшего решения
#endif
                        break;
                    }
                    int size = findSize(field, newWidth, y, x);          //
                    drawSquare(field, size, y, x, num);                  //
                    addToVector(vector, y, x, size, num);                //
                    num++;                                               //
#ifdef DEBUG
                    printField(field, newWidth);
#endif
                }
            }
        com_count++;
        if (countOfSquares == 4 || vector.size() + 4 < countOfSquares) {  //  Если это первая итерация или найденное
            countOfSquares = vector.size() + 4;                          //  частное решение лучше найденного ранее, то
            rel_count++;
            vectorToBestVector(bestVector, vector, widthOfLargerSquare); //  новое решение становится лучшим
        }


        while (1) {

            flagToEnd = false;
            Square lastSquare;
#ifdef DEBUG
            cout << "Decreasing the width of the square per unit: " << endl << endl;
#endif
            for (int i = vector.size(); i != 0; --i) {                   //  Начиная с конца вектора, содержащего
                lastSquare = vector[vector.size() - 1];                  //  последнее частное решение, ищем первый
                if (lastSquare.width == 1) {                             //  квадрат ширины, большей 1, попутно обнуляя
                    field[lastSquare.y][lastSquare.x] = 0;               //  квадраты с шириной 1
                    vector.pop_back();                                   //
                    del_count++;
#ifdef DEBUG
                    printField(field, newWidth);
#endif
                }
                else {
#ifdef DEBUG
                    cout << "Reducing the width of the last from the end of the square with a width greater than one: " << endl << endl;
#endif
                    for (int y = lastSquare.y; y < lastSquare.y + lastSquare.width; ++y) {                                 //
                        for (int x = lastSquare.x; x < lastSquare.x + lastSquare.width; ++x) {                             //
                            if (y >= lastSquare.y + lastSquare.width - 1 || x >= lastSquare.x + lastSquare.width - 1)    //
                                field[y][x] = 0;                                                                        //  Если ширина очередного квадрата
                        }                                                                                               //  в векторе больше 1, то она
                    }                                                                                                   //  уменьшается на 1 и даннный цикл
                    vector[vector.size() - 1].width = lastSquare.width - 1;                                             //  завершает работу
#ifdef DEBUG                                                                                        //
                    printField(field, newWidth);                                                                    //
#endif                                                                                              //
                    break;                                                                                              //
                }                                                                                                       //
            }                                                                                                           //
                                                                         //  Если на предыдущем шаге были обнулены все
            if (vector.empty())                                           //  квадраты ширины 1, то цикл while завершает
                break;                                                   //  поиск оптимальных конфигураций при данной
                                                                         //  ширине квадрата в левом верхнем углу
#ifdef DEBUG
            cout << "Square filling: " << endl << endl;
#endif

            for (int y = 0, num = lastSquare.num + 1; y < newWidth && !flagToEnd; ++y)  //  Нулевые клетки замещаются квадратами
                for (int x = 0; x < newWidth && !flagToEnd; ++x) {                      //  максимально возможной ширины, при этом
                    if (field[y][x] == 0) {                                             //  частное решение заносится в вектор
                        if (vector.size() + 4 >= countOfSquares) {                                                  //  Если количество квадратов, необходимых
                            flagToEnd = true;                                                                      //  в получаемом частном решении, больше количества
#ifdef DEBUG                                                                           //  квадратов, получаемых при лучшей конфигурации, поднимается
                            cout << "Already found a solution with the same or fewer squares!" << endl << endl;//  флаг для прекращения поиска лучшего решения
#endif
                            break;
                        }
                        int size = findSize(field, newWidth, y, x);                     //
                        drawSquare(field, size, y, x, num);                             //
                        addToVector(vector, y, x, size, num);                           //
                        num++;                                                          //
#ifdef DEBUG
                        printField(field, newWidth);
#endif
                    }
                }
            com_count++;
            if (vector.size() + 4 < countOfSquares) {                                //  Если найденное частное решение лучше
                countOfSquares = vector.size() + 4;                                //  найденного ранее, то новое решение
                vectorToBestVector(bestVector, vector, widthOfLargerSquare);       //  становится лучшим
            }                                                                      //
        }
        vector.clear();                       //  Очищается вектор частных решений
    }
    cout << countOfSquares << endl;
}



int main() {
    int width;
    int count;
    vector<Square> bestVector;
    cout << "Enter width of square :";
    cin >> width;

        if (!(width % 2) )
            even(width);
        else if (!(width % 3))
            odd3(width);
        else if (!(width % 5))
            odd5(width);
        else {
            fill(width, bestVector);
            printVector(bestVector);
        }
        
        cout <<"Adding count:\t" <<  add_count << endl;
        cout <<"Delete count:\t" << del_count << endl;
        cout <<"Compare count:\t" << com_count << endl;
        cout << "Reload count:\t" << rel_count << endl;
        cout <<"Count:\t\t" << del_count + add_count + com_count + rel_count << endl;

 
}
