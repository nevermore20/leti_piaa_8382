#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>      // std::setw

using namespace std;

class Path {
private:
    char nameFrom;
    char nameOut;
    double weightPath;

public:
    Path(char nameFrom, char nameOut, double weightPath)
        : nameFrom(nameFrom), nameOut(nameOut), weightPath(weightPath) {}

    char getNameFrom() const {
        return nameFrom;
    }

    char getNameOut() const {
        return nameOut;
    }

    double getWeightPath() const {
        return weightPath;
    }
};

bool comp(Path a, Path b) {//comparator
    return a.getWeightPath() < b.getWeightPath();
}

bool func(std::vector<Path>* vector, char curChar, char endChar, std::vector<char>* answer, int depth) {
    depth++;

    std::cout << setw(depth + 1) << ' ' << "Processing the vertex:    " << curChar << std::endl;

    if (curChar == endChar) {    //exit from recursion
        std::cout << setw(depth + 1) << ' ' << "Reached the desired peak    " << endChar << ". The function returns TRUE and shuts down." << std::endl;
        return true;
    }


    
    std::cout << setw(depth + 1) << ' ' << "Search for paths leading from this vertex." << std::endl;
    std::vector<Path> temporaryVector;
    temporaryVector.reserve(0);
    for (Path path : *vector) {// all vertexes in the vector will be passed
        if (path.getNameFrom() == curChar) {//selects all paths from the desired vertex
            std::cout << setw(depth + 1) << ' ' << "Since the vertex   " << path.getNameOut() << "   comes from the current vertex, writing this path to the vector." << std::endl;
            temporaryVector.emplace_back(path);//written to a vector
        }
    }

    //since we need the cheapest way we will cohabit

    std::cout << setw(depth + 1) << ' ' << "Sorting of vertices of minimum weight." << std::endl;

    std::sort(temporaryVector.begin(), temporaryVector.end(), comp);


    for (Path path : temporaryVector) {//going through all the vertexes
        if (func(vector, path.getNameOut(), endChar, answer, depth)) {//new variable
            depth--;
            std::cout << setw(depth + 1) << ' ' << "Writing a vertex    " << path.getNameOut() <<    "    in the response vector" << std::endl;
            answer->emplace_back(path.getNameOut());
            return true;
        }
    }

    return false;
}

int main() {
    setlocale(LC_ALL, "rus");
    int depth = 0;
    int flag = 1;

    std::vector<Path> vector;
    vector.reserve(0);

    std::vector<char> answer;
    answer.reserve(0);

    char startChar;
    char endChar;
    std::cout << "Please, enter the starting vertex and ending vertex, as well as the edges of the graph with its weight: " << std::endl;

    std::cin >> startChar;
    std::cin >> endChar;


    char start, end;
    double weight;

    
    while (std::cin >> start >> end >> weight) {
        vector.emplace_back(Path(start, end, weight));
    }

    std::cout << "The greedy algorithm function is started" << std::endl;

    if (!func(&vector, startChar, endChar, &answer, depth))
    {
        std::cout << "Error!" << std::endl;
        flag = 0;
    }

    if (flag)
    {
        std::cout << "The greedy algorithm function shuts down" << std::endl;
        std::cout << "The initial vertex is added to the response vector." << std::endl;
        answer.emplace_back(startChar);
        std::cout << "Reversing the response vector." << std::endl;
        std::reverse(answer.begin(), answer.end());

        std::cout << "Answer:    " << std::endl;
        for (char sym : answer) {
            std::cout << sym;
        }
    }


    return 0;
}