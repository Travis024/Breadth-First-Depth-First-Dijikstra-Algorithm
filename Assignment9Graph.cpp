#include "Assignment9Graph.hpp"

using namespace std;

//The constructor
Graph::Graph(){};

//THe destructor
Graph::~Graph(){};

//Adds a vertex with name 'name' to the vertices vector of the graph
void Graph::addVertex(std::string name){


    //Check to see if the vertex already exists in the list
    bool found = false;
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i].name == name){
            found = true;
            cout<<vertices[i].name<<" already in the graph."<<endl;
        }
    }

    //If not found, create a new vertex and push it onto the vertices vector
    if(!found){
        vertex newVertex;
        newVertex.name = name;
        //Initialize districtID to -1, visited to false, and the ID is equal to the size of the vector
        newVertex.districtID = -1;
        newVertex.ID = vertices.size();
        newVertex.visited = false;
        vertices.push_back(newVertex);
    }
}


//Adds an edge between two vectors with a given weight
void Graph::addEdge(std::string v1, std::string v2, int weight){

    //Check to make sure both vertices exist and aren't the same.
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i].name == v1){
            for(int j = 0; j < vertices.size(); j++){
                if(vertices[j].name == v2 && i != j){

                    //Create a pointer and put it in the list of adjacent vertices
                    adjVertex adjacent;
                    adjacent.v = &vertices[j];
                    adjacent.weight = weight;
                    vertices[i].adj.push_back(adjacent);

                }
            }
        }
    }

}


//A function to display every vertex and the vertices it is adjacent to
void Graph::displayEdges(){

    //Go through every vertex
    for(int i = 0; i < vertices.size(); i++){

        cout<<vertices[i].districtID<<":";
        cout<<vertices[i].name<<"-->";

        //Go through every adjacent vertex
        for(int j = 0; j < vertices[i].adj.size(); j++){
            cout<<vertices[i].adj[j].v -> name;
            if(j != vertices[i].adj.size() - 1){
                cout<<"***";
            }
        }

        cout<<endl;
    }

}

//Goes through connected components and assigns district IDs based on the component a vector belongs to
void Graph::assignDistricts(){

    //an index for the districtID; only increments when a new, un-visited vector is reached
    int districtIndex = 1;

    for(int i = 0; i < vertices.size(); i++){

        //If a vector hasn't been visited, go through its adjacency list and assign ID's
        if(vertices[i].visited != true){
            DFSLabel(vertices[i].name, districtIndex);
            districtIndex++;
        }

    }

}

void Graph::DFSLabel(std::string starterCity, int distID){

    vertex * vStart = findVertex(starterCity);

    //mark the vertex as visited and set its districtID
    vStart ->visited = true;
    vStart ->districtID = distID;

    //Go through the entire adjacency list
    //For vertices that haven't been visited, set the district ID and make a recursive call
    for(int i = 0; i < vStart -> adj.size(); i++){
        if(vStart -> adj[i].v ->visited != true){
            vStart ->adj[i].v ->districtID = distID;
            DFSLabel(vStart ->adj[i].v ->name, distID);
        }
    }
}

//Search through the list of vertices to find the vertex of a given name
vertex * Graph::findVertex(std::string name){

    //Iterate through every vertex and check their names
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i].name == name){
            return &vertices[i];
        }
    }

    //If not found, returns NULL
    return NULL;
}

void Graph::shortestPath(std::string startingCity, std::string endingCity){

    //Check to make sure both cities exist.
    vertex * vStart = findVertex(startingCity);
    vertex * vEnd = findVertex(endingCity);

    if(vStart == NULL || vEnd == NULL){
        cout<<"One or more cities doesn't exist"<<endl;
        return;
    }

    //Check to make sure districts have been set
    if(vStart ->districtID == -1 || vEnd ->districtID == -1){
        cout<<"Please identify the districts before checking distances"<<endl;
        return;
    }

    //Check to see if cities are in the safe district
    if(vStart ->districtID != vEnd ->districtID){
        cout<<"No safe path between cities"<<endl;
        return;
    }

    //Now, perform a breadth first search to find shortest unweighted distance

    //Reset the visited member variables since they're set when finding districts
    //Initialize the distances for Moodle's test cases as instructed by TA
    for(int i = 0; i < vertices.size(); i++){
        vertices[i].visited = false;
        vertices[i].unweightedDistance = 2147483647;
        vertices[i].weightedDistance = 2147483647;
    }


    vStart ->unweightedDistance = 0;
    vStart ->parent = NULL;

    //create a queue to enqueue and dequeue vertices
    queue<vertex*> vQueue;

    vQueue.push(vStart);

    //Keep searching until the end city is found
    while(vQueue.size() != 0){

        //Find the front element
        vertex * frontOfQueue = vQueue.front();
        vQueue.pop();

        //Search through the adjacency list of the current vertex
        for(int i = 0; i < frontOfQueue ->adj.size(); i++){

            if(frontOfQueue ->adj[i].v -> visited != true){

                //Increment the distance by 1
                frontOfQueue ->adj[i].v ->unweightedDistance = frontOfQueue ->unweightedDistance + 1;
                //Set the parent for finding the actual path
                frontOfQueue ->adj[i].v ->parent = frontOfQueue;
                //Set visited to true so we don't come back
                frontOfQueue ->adj[i].v ->visited = true;
                //Push this vertex on to the queue
                vQueue.push(frontOfQueue ->adj[i].v);
            }

        }

    }

    //Finally, use the parent pointers to print the shortest path

    //Start at the ending city
    vertex * shortestCityParentTransverse = findVertex(endingCity);

    cout<<shortestCityParentTransverse ->unweightedDistance<<", ";

    //iterate through the parents, print out their names
    for(int i = shortestCityParentTransverse ->unweightedDistance; i >= 0; i--){
        cout<<shortestCityParentTransverse ->name;

        if(i == 0){
            //Ensure the final city has a distance of 0
            shortestCityParentTransverse -> unweightedDistance = 0;
            cout<<endl;
        }
        else{
            cout<<", ";
        }

        shortestCityParentTransverse = shortestCityParentTransverse ->parent;
    }


}

void Graph::shortestWeightedPath(std::string startingCity, std::string endingCity){

    //Check to make sure both cities exist.
    vertex * vStart = findVertex(startingCity);
    vertex * vEnd = findVertex(endingCity);

    if(vStart == NULL || vEnd == NULL){
        cout<<"One or more cities doesn't exist"<<endl;
        return;
    }

    //Check to make sure districts have been set
    if(vStart ->districtID == -1 || vEnd ->districtID == -1){
        cout<<"Please identify the districts before checking distances"<<endl;
        return;
    }

    //Check to see if cities are in the safe district
    if(vStart ->districtID != vEnd ->districtID){
        cout<<"No safe path between cities"<<endl;
        return;
    }

    //Now, use Dijikstra's algorithm to find the shortest WEIGHTED path

    //Initialize visited as false and unweighted distance as max value of int
    //Visited will be set as true when added to the solvedVectorList
    //set parent as null to reset if previously run
    for(int i = 0; i < vertices.size(); i++){
        vertices[i].visited = false;
        vertices[i].unweightedDistance = 2147483647;
        vertices[i].weightedDistance = 2147483647;
        vertices[i].parent = NULL;
    }

    //Will store all solved vertices
    vector<vertex*>solvedVertexList;

    //Add the starting vertex as solved, mark as visited, set distance as 0
    solvedVertexList.push_back(vStart);
    vStart->visited = true;
    vStart->weightedDistance = 0;

    //Keep going until we solve the ending city
    while(vEnd ->visited != true){

        //minVertex will store the vertex with minimum distance
        vertex * minVertex = NULL;

        //stores the parent of the minimum vertex
        vertex * parent = NULL;

        //initialize min distance to max int value so that it will be replaced no matter what
        int minDistance = 2147483647;

        //Go through every vertex in the solved list
        for(int i = 0; i < solvedVertexList.size(); i++){

            //Go through the adjacency list of each vertex
            for(int j = 0; j < solvedVertexList[i] ->adj.size(); j++){

                //Only check adjacent vertices that haven't been solved
                if(solvedVertexList[i]->adj[j].v ->visited != true){

                    //Find the distance back to that start
                    int distance = solvedVertexList[i]->weightedDistance + solvedVertexList[i]->adj[j].weight;

                    //if the distance of the adjacent vector is less than the min distance:
                    //set minDistance to new value, set the parent, and set the minVertex
                    if(distance < minDistance){
                        minDistance = distance;
                        minVertex = solvedVertexList[i]->adj[j].v;
                        parent = solvedVertexList[i];
                    }
                }

            }

        }

    //Finally, the vertex with the shortest distance is marked as solved and added to the list
    minVertex ->weightedDistance = minDistance;
    minVertex ->visited = true;
    minVertex -> parent = parent;
    solvedVertexList.push_back(minVertex);

    }


    //Print out the distance and the path
    cout<<vEnd -> weightedDistance;

    while(vEnd != NULL){
        cout<<", "<<vEnd->name;
        vEnd = vEnd -> parent;
    }

    cout<<endl;

}
