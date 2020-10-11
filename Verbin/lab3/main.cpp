#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>      // std::setw

class Path {
private:
    char nameFrom;
    char nameOut;
    int bandwidth;//bandwidth
    int flow = 0;//flow

public:
    Path(char nameFrom, char nameOut, int bandwidth) : nameFrom(nameFrom), nameOut(nameOut), bandwidth(bandwidth) {}

    void setFlow(int flow) {
        std::cout << "\t\t" <<Path::nameFrom << " edge: " << Path::bandwidth - Path::flow << ", revers edge: " << Path::flow << std::endl;
        Path::flow = flow;
    }

    char getNameFrom() const {
        return nameFrom;
    }

    char getNameOut() const {
        return nameOut;
    }

    int getBandwidth() const {
        return bandwidth;
    }

    int getFlow() const {
        return flow;
    }


};

bool comp(Path a, Path b);

void findMin(std::vector<Path*>& local, int* maxFlow, long int depth) {//function for selecting the minimum difference between throughput and flow
    std::cout << std::setw(depth + 1) << ' ' << "The function of selecting the minimum difference between bandwidth and flow, as well as changing information, is started." << std::endl;
    int Min = local.front()->getBandwidth();   //front() Returns a reference to the first element in the vector container
    for (Path* path : local) {
        if (Min > (path->getBandwidth() - path->getFlow())) {
            Min = path->getBandwidth() - path->getFlow();
          //  std::cout << std::setw(depth + 1) << "Set additional network:" << path->getNameFrom()<< ": " << path->getBandwidth() << " - " << path->getFlow() <<std::endl;
        }
    }
    std::cout << std::setw(depth + 1) << ' ' << "Minimum =  " << Min << std::endl;
    std::cout << std::setw(depth + 1) << ' ' << "Adding the found minimum to the path threads." << std::endl;
    for (Path* path : local) {
        path->setFlow(path->getFlow() + Min);//now add the found minimum to the stream
    }

    *maxFlow = *maxFlow + Min;//increase the value of the maximum flow
    std::cout << std::setw(depth + 1) << ' ' << "The maximum flow is now equal to:  " << *maxFlow << std::endl;
}

bool comp(Path a, Path b) {
    if (a.getNameFrom() != b.getNameFrom()) {

        return a.getNameFrom() < b.getNameFrom();
    }
    else {
        return a.getNameOut() < b.getNameOut();
    }
}

bool comp2(Path* a, Path* b) {//the comparator is used to choose where to go. For example, there are 2 paths, one 10/40 and the other 15/30. It is necessary to go to the second, since 30-15 < 40-10

    return (a->getBandwidth() - a->getFlow()) < (b->getBandwidth() - b->getFlow());
}

bool isVisitedPath(std::vector<Path*>& local, char element, long int depth) {
    std::cout << std::setw(depth + 1) << ' ' << "A function is launched to check whether this vertex has already been visited." << std::endl;
    for (Path* path : local) {
        if (element == path->getNameFrom()) {
            std::cout << std::setw(depth + 1) << ' ' << "The check function shuts down. Top  " << element << "  is already in sight. This means that we will not go along this edge." << std::endl;
            return false;
        }
    }
    std::cout << std::setw(depth + 1) << ' ' << "The check function shuts down. Top  " << element << "  hasn't been viewed yet." << std::endl;
    return true;
}

bool findPath(std::vector<Path>& paths, std::vector<Path*>& local, std::vector<Path*>& local2, char myPoint, char* endPoint, long int depth) {
    depth++;

    std::cout << std::setw(depth + 1) << ' ' << "The path search function starts." << std::endl;

    std::cout << std::setw(depth + 1) << ' ' << "Checking is in progress: the vertex is not the one being searched for." << std::endl;
    if (myPoint == *endPoint) {
        std::cout << std::setw(depth + 1) << ' ' << "The vertex is the one you are looking for. This means that the function returns true." << std::endl;
        return true;
    }

    std::vector<Path*> localPaths;

    std::cout << std::setw(depth + 1) << ' ' << "Selecting edges that originate from the current vertex." << std::endl;
    for (auto& path : paths) {//selecting which edges come from the current vertex
        if (path.getNameFrom() == myPoint) {
            std::cout << std::setw(depth + 1) << ' ' << "Since the edge:  " << path.getNameFrom() << ' ' << path.getNameOut() << "  originates from the current vertex  " << myPoint << ". Then we add this edge to the temporary path vector." << std::endl;

            localPaths.emplace_back(&path);
        }
    }

    std::cout << std::setw(depth + 1) << ' ' << "Sorting edges by the smallest difference between throughput and flow." << std::endl;
    std::sort(localPaths.begin(), localPaths.end(), comp2);//choosing where to go first

    for (Path* path : localPaths) {
        std::cout << std::setw(depth + 1) << ' ' << "Checking that the edge  " << path->getNameFrom() << ' ' << path->getNameOut() << "  is not overfilled." << std::endl;
        if (path->getFlow() < path->getBandwidth()) {//checking that the path flow is less than the bandwidth
            std::cout << std::setw(depth + 1) << ' ' << "Checking that the vertex where the edge enters has not been visited yet." << std::endl;
            if (isVisitedPath(local2, path->getNameOut(), depth)) {//if we haven't visited the top yet
                std::cout << std::setw(depth + 1) << ' ' << "Writing an edge  " << path->getNameFrom() << ' ' << path->getNameOut() << "  in the vector of viewed edges." << std::endl;
                local2.emplace_back(path);
                std::cout << std::setw(depth + 1) << ' ' << "Recursively calling the function, but now the current vertex will be:  " << path->getNameOut() << std::endl;
                if (findPath(paths, local, local2, path->getNameOut(), endPoint, depth)) {
                    std::cout << std::setw(depth + 1) << ' ' << "Since the function returned true, it means that the vertex was found. Writing an edge  " << path->getNameFrom() << ' ' << path->getNameOut() << "  in the response vector." << std::endl;
                    depth--;
                    local.emplace_back(path);
                    return true;
                }
                else {
                    std::cout << std::setw(depth + 1) << ' ' << "Making a rollback." << std::endl;
                    local2.pop_back();
                }
            }
        }
        std::cout << std::setw(depth + 1) << ' ' << "The edge is full or there are no more possible paths." << std::endl;
    }
    std::cout << std::setw(depth + 1) << ' ' << "The Pathfinder function shuts down." << std::endl;

    return false;

}

int main() {
    setlocale(LC_ALL, "Russian");
    char startPoint, endPoint;//source and drain
    char start, end;
    int weight;
    int count;
    int flag = 0;
    int flag2 = 0;
    long int depth = 0;
    std::vector<Path*> local;

    std::vector<Path*> local2;

    std::vector<Path> paths;

    int maxFlow = 0;
    std::cout << "Hello! Please enter the number of oriented edges of the graph, the source, drain, and edges of the graph." << std::endl;
    std::cin >> count;
    std::cin >> startPoint;
    std::cin >> endPoint;
    if (count != 0)
    {
        flag2 = 1;
        while (count != 0) {
            std::cin >> start >> end >> weight;
            paths.emplace_back(Path(start, end, weight));
            count--;
        }
    }
    else
        std::cout << "Zero number of oriented edges of the graph is introduced!" << std::endl;

    if (flag2)
    {
        std::cout << "The path search function is called." << std::endl;
        while (findPath(paths, local, local2, startPoint, &endPoint, depth)) {//as soon as we find the path to the drain, we call the function to find the minimum and change the flow
            findMin(local, &maxFlow, depth);
            std::cout << std::setw(depth + 1) << ' ' << "Clearing the response vector and the vector of viewed edges." << std::endl;
            local.clear();
            local2.clear();
        }


        std::cout << std::setw(depth + 1) << ' ' << "Sorting the output edges in lexicographic order. " << std::endl;

        std::sort(paths.begin(), paths.end(),
            [](const Path& a, const Path& b)
            {
                if (a.getNameFrom() != b.getNameFrom()) {

                    return a.getNameFrom() < b.getNameFrom();
                }
                else {
                    return a.getNameOut() < b.getNameOut();
                }

            });

        //std::sort(paths.begin(), paths.end(), comp);//sort the vertices in lexicographical order
        std::cout << std::endl;
        std::cout << "Answer:  " << std::endl;
        std::cout << "The value of maximum flow =  " << maxFlow << std::endl;

        for (Path path : paths) {
            std::cout << "Edge of the graph with the actual value of the flowing stream:   " << path.getNameFrom() << " " << path.getNameOut() << " " << path.getFlow() << std::endl;
        }

        std::cout << "Goodbye!" << std::endl;

    }

    return 0;
}
