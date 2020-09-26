#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

class Top {//vertex class

public:

    char name;
    double pathToTop;//path to the current vertex - g
    double heuristicF;//heuristic function - f
    char nameFromT;
    std::vector<char> coupled;//vector for vertexes originating from a vertex

    
    Top(char name)//constructor1-required to fill in the initial vertex
        : name(name) {
        heuristicF = 0;
        pathToTop = -1;//will be used for unprocessed vertexes instead of the infinity sign
        nameFromT = '-';
    }

    Top() {			//constructor2
        name = '!';		//
        heuristicF = 0;
        pathToTop = -1;	//
        nameFromT = '-';
    }

};

class Path {//path class. It stores only the path: from where to where and how much the path weighs

public:

    char nameFromP;
    char nameOutP;
    double weightPath;

    
    Path(char nameFromP, char nameOutP, double weightPath)
        : nameFromP(nameFromP), nameOutP(nameOutP), weightPath(weightPath) {}

 

    char getNameFromP() const {
        return nameFromP;
    }

    char getNameOutP() const {
        return nameOutP;
    }

    double getWeightPath() const {
        return weightPath;
    }
};

bool check(std::vector<Path>& vectorPath, std::vector<Top>& vectorTops, char endTop, bool flagM, bool flagAd)
{
    std::cout << "The monotony and tolerance check function is started  " << std::endl;
    if (abs(endTop - endTop) != 0) {
        std::cout << "The heuristic estimate of the target state is not zero!" << std::endl;
        flagM = false;
        
    }

    for (unsigned int i = 0; i < vectorPath.size(); i++) {
         
        if ((abs(endTop - vectorPath[i].nameFromP) - abs(endTop - vectorPath[i].nameOutP)) > vectorPath[i].weightPath) {
            std::cout << "The monotony is broken." << std::endl;
            flagM = false;
            
        }
    }
    //checking for validity
    if (!flagM)
    {
        for (unsigned int i = 0; i < vectorTops.size(); i++) {


            if ((abs(endTop - vectorTops[i].name) > (vectorTops[vectorTops.size() - 1].pathToTop - vectorTops[i].pathToTop)))
            {
                std::cout << "Tolerance is violated." << std::endl;
                flagAd = false;
            }


        }
    }
    if (flagM)
    {
        std::cout << "The heuristic is monotonous and valid!" << std::endl;//monottonna i dopystima
        return true;
     }
    else if (!flagM && flagAd)
    {
        std::cout << "The heuristic is valid!" << std::endl;//dopustima
        return true;
    }
    else
    {
        std::cout << "The heuristic is not monotonous and is not allowed!" << std::endl;
        return false;
    }
    
}





int whatNumber(char a, std::vector<Top>& vectorTops) {
   
    for (unsigned int i = 0; i < vectorTops.size(); i++) {
        if (vectorTops[i].name == a) {
            return i;
        }
    }
    return -1;
}

bool comp(Top a, Top b) {//comparator, used for sorting in an open list
    return a.heuristicF < b.heuristicF;
}

void answer(std::vector<Top>& vectorTops, char startTop, char endTop)
{
    std::cout << "The response output function starts.  " << std::endl;
    std::vector<Top> answer;
    answer.reserve(0);
    Top temp = vectorTops[whatNumber(endTop, vectorTops)];
    std::cout << "Writing the last vertex to the response vector  " << endTop << std::endl;
    answer.emplace_back(temp);
    while (temp.name != startTop) {
        temp = vectorTops[whatNumber(temp.nameFromT, vectorTops)];
        std::cout << "Writing a vertex to the response vector  " << temp.name << std::endl;
        answer.emplace_back(temp);
    }
    std::cout << "Reversing the response vector  " << std::endl;
    std::reverse(answer.begin(), answer.end());//since it was filled in the reverse order, we do reverse
    std::cout << "Answer:  " << std::endl;
    for (Top ans : answer) {
        std::cout << ans.name;
    }
    std::cout << std::endl;
}

void changeInfo(std::vector<Top>& vectorTops, std::vector<Top>& openVertexes, char a, char name, double temp_G, char endTop )
{
    std::cout << "Update information:  " << std::endl;

    vectorTops[whatNumber(a, vectorTops)].nameFromT = name;
    
    vectorTops[whatNumber(a, vectorTops)].pathToTop = temp_G;
    openVertexes[whatNumber(a, openVertexes)].nameFromT = name;
    openVertexes[whatNumber(a, openVertexes)].pathToTop = temp_G;
    openVertexes[whatNumber(a, openVertexes)].heuristicF = temp_G + abs(endTop - a);
    std::cout << "Vertex  " << a << "   set that comes from   " << name << std::endl;
    std::cout << "Path to the vertex  " << a << "   =   " << temp_G << std::endl;
    std::cout << "Heuristic estimation for a vertex  " << a << "   =   " << temp_G + abs(endTop - a) << std::endl;
    std::cout << "The end of the update information.  " << std::endl;
}

bool A(std::vector<Path>& vectorPath, std::vector<Top>& vectorTops, char startTop, char endTop) {

    Top temp;
    double temp_G;
    std::vector<Top> closedVertexes;
    closedVertexes.reserve(0);
    std::vector<Top> openVertexes;
    openVertexes.reserve(0);

    std::cout << "Adding a vertex to the vector of open vertexes    " << vectorTops[0].name << std::endl;

    openVertexes.emplace_back(vectorTops[0]);

    while (!openVertexes.empty()) {
        Top min = openVertexes[0];
        std::cout << "Sort the open tops of the " << std::endl;
        std::sort(openVertexes.begin(), openVertexes.end(), comp);
        temp = openVertexes[0]; //minimum f from openVertexes
        std::cout << "Current vertex:    "<< temp.name << std::endl;

        if (temp.name == endTop) {
            std::cout << "The current vertex is equal to the one you are looking for, so we call the response output function." << std::endl;
            answer(vectorTops, startTop, endTop);
            return true;
        }
        std::cout << "Adding a vertex    " << openVertexes[0].name << "    in the vector of closed vertexes. And delete it from the vector of open vertexes." << std::endl;
        closedVertexes.emplace_back(temp); //adding the processed vertex
        openVertexes.erase(openVertexes.begin()); //deleting the processed vertex

        for (unsigned int i = 0; i < temp.coupled.size(); i++) { //for each neighbor
            if (whatNumber(temp.coupled[i], closedVertexes) != -1) { //if the neighbor is in closedVertexes (already processed)
                continue;
            }
            int j = 0;
            while (true) {
                if (vectorPath[j].nameFromP == temp.name && vectorPath[j].nameOutP == temp.coupled[i]) {
                    std::cout << "Counting the value of the shortest path to the vertex    " << vectorPath[j].nameOutP << std::endl;
                    temp_G = vectorPath[j].weightPath + temp.pathToTop;
                    std::cout << "Shortest path to the top =  " << temp_G << std::endl;
                    break;
                }
                j++;
            }

            if (whatNumber(temp.coupled[i], openVertexes) == -1) { //if the neighbor is not in openVertexes
                std::cout << "Since the neighboring vertex    " << temp.coupled[i] << "    if it is not in the vector of open vertexes, then we add it to it." << std::endl;
                openVertexes.emplace_back(vectorTops[whatNumber(temp.coupled[i], vectorTops)]); //adding a neighbor
                std::cout << "Calling the information update function."  << std::endl;
                changeInfo(vectorTops, openVertexes, temp.coupled[i], temp.name, temp_G, endTop);
            }
            else {
                if (temp_G < openVertexes[whatNumber(temp.coupled[i], openVertexes)].pathToTop) {
                    std::cout << "Since a shorter path was found("   << temp_G <<   ") up to top    " << temp.coupled[i] <<".  Update information."<< std::endl;
                    changeInfo(vectorTops, openVertexes, temp.coupled[i], temp.name, temp_G, endTop);
                }
            }

           
        }
    }
     return false;
 }


int main() {

    setlocale(LC_ALL, "Russian");
    bool flag = true;
    bool flagM = true;
    bool flagAd = true;
    std::vector<Path> vectorPath;//vector paths
    vectorPath.reserve(0);
    std::vector<Top> vectorTops;//vector tops
    vectorTops.reserve(0);

    char startTop;
    char endTop;
    std::cout << "Please enter the starting vertex and ending vertex, as well as the edges of the graph with its weight: " << std::endl;

    std::cin >> startTop;
    std::cin >> endTop;

    char start, end;
    double weight;


    while (std::cin >> start >> end >> weight) {
        vectorPath.emplace_back(Path(start, end, weight));
    }

    std::set<char> set;//

    set.insert(startTop);//inserting the first vertex

    vectorTops.emplace_back(Top(startTop));//creating the initial vertex and putting it in the vector

    int number;

    for (Path path : vectorPath) {//going through the path vector
        char from = path.getNameFromP();//
        char out = path.getNameOutP();

        if (set.find(from) == set.end()) {//checks that there is no from in the set
            set.insert(from);
            vectorTops.emplace_back(Top(from));
        }
        if (set.find(out) == set.end()) {
            set.insert(out);
            vectorTops.emplace_back(Top(out));

        }
    }
    //the path vector is full, but the neighbor vector is not => 
    //performing a pass through the path vector again
    for (Path path : vectorPath) {//going through the path vector
        char from = path.getNameFromP();//
        char out = path.getNameOutP();

        if (set.find(from) != set.end()) {//checks that the set has from
            number = whatNumber(from, vectorTops);
            vectorTops[number].coupled.emplace_back(out);//adding a vertex neighbor
        }

    }

    vectorTops[0].pathToTop = 0; 
    vectorTops[0].heuristicF = abs(endTop - startTop); 
    std::cout << "Launching the algorithm function A*!" << std::endl;

    if (!A(vectorPath, vectorTops, startTop, endTop)) {
        flag = false;
        std::cout << "ERROR! ERROR! ERROR!" << std::endl;
    }


    if (flag)
    {
        check(vectorPath, vectorTops, endTop, flagM, flagAd);
    }
        
}