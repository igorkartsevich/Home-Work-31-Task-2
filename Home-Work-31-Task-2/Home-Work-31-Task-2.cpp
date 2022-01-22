#include <iostream>
#include <vector>
#include <map>

class IGraph {
public:
    virtual ~IGraph() {}
    IGraph() {}
    virtual void AddEdge(int from, int to) = 0; // Метод принимает вершины начала и конца ребра и добавляет ребро
    virtual int VerticesCount() const = 0; // Метод должен считать текущее количество вершин
    virtual void GetNextVertices(int vertex, std::vector<int>& vertexes) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertexes) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную
};

class ListGraph : public IGraph {
public:
    virtual ~ListGraph() {}
    ListGraph() {}

    virtual void AddEdge(int from, int to) {
        listGraph_from[from].push_back(to);
        listGraph_to[to].push_back(from);
    }

    virtual int VerticesCount() const {
        return std::max(listGraph_from.rbegin()->first, listGraph_to.rbegin()->first);
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertexes) const {
        vertexes = listGraph_from.find(vertex)->second;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertexes) const {
        vertexes = listGraph_to.find(vertex)->second;
    }

private:
    std::map<int, std::vector<int>> listGraph_from;
    std::map<int, std::vector<int>> listGraph_to;
};

class MatrixGraph : public IGraph {
public:
    virtual ~MatrixGraph() {}
    MatrixGraph() {}

    virtual void AddEdge(int from, int to) {
        int matrixSize = std::max(from, to);
        
        if (matrixGraph_from.size() < matrixSize) {
            matrixGraph_from.resize(matrixSize);
            matrixGraph_to.resize(matrixSize);
            for (int i = 0; i < matrixSize; ++i) {
                matrixGraph_from[i].resize(matrixSize);
                matrixGraph_to[i].resize(matrixSize);
            }
        }

        from--, to--;
        matrixGraph_from[from][to] = 1;
        matrixGraph_to[to][from] = 1;
    }

    virtual int VerticesCount() const {
        return matrixGraph_from.size();
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertexes) const {
        vertex--;
        int maxVertex = VerticesCount();
        for (int i = 0; i < maxVertex; ++i)
            if (matrixGraph_from[vertex][i] == 1) vertexes.push_back(i + 1);
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertexes) const {
        vertex--;
        int maxVertex = VerticesCount();
        for (int i = 0; i < maxVertex; ++i)
            if (matrixGraph_to[vertex][i] == 1) vertexes.push_back(i + 1);
    }

private:
    std::vector<std::vector<int>> matrixGraph_from;
    std::vector<std::vector<int>> matrixGraph_to;

};

void printData_vertexes(int number, std::vector<int>& vertexes, std::string next_prev) {
    std::cout << "\n\tVetrex number " << number << " has following vertexes " << next_prev << ": ";
    for (auto i : vertexes) std::cout << i << " ";
    vertexes.clear();
}

void setData_vertexes(IGraph* listGraph, IGraph* matrixGraph) {
    while (true) {
        std::cout << "Enter vertex's data (from to) (0 0 - exit): ";

        int from, to;
        std::cin >> from >> to;
        if (from == 0 && to == 0) return;

        listGraph->AddEdge(from, to);
        matrixGraph->AddEdge(from, to);
    }
}

void getData_vertexes(IGraph* listGraph, IGraph* matrixGraph) {
    while (true) {
        std::cout << "\n\nEnter the number of vertex (0 - exit): ";

        int vertexNumber;
        std::cin >> vertexNumber;
        if (vertexNumber == 0) return;

        if (vertexNumber > listGraph->VerticesCount()) {
            std::cerr << "This Graph does not contains vertex number " << vertexNumber;
            continue;
        }

        std::vector<int> vertexes;
        
        std::cout << "ListGraph";
        listGraph->GetNextVertices(vertexNumber, vertexes);
        printData_vertexes(vertexNumber, vertexes, "next");
        listGraph->GetPrevVertices(vertexNumber, vertexes);
        printData_vertexes(vertexNumber, vertexes, "previous");

        std::cout << "\nMatrixGraph";
        matrixGraph->GetNextVertices(vertexNumber, vertexes);
        printData_vertexes(vertexNumber, vertexes, "next");
        matrixGraph->GetPrevVertices(vertexNumber, vertexes);
        printData_vertexes(vertexNumber, vertexes, "previous");
    }
}

int main()
{
    IGraph* listGraph = new ListGraph();
    IGraph* matrixGraph = new MatrixGraph();

    std::cout << "Enter data for Graph.\n";
    setData_vertexes(listGraph, matrixGraph);

    std::cout << "\nThe ListGraph has " << listGraph->VerticesCount() << " vertexes.\n";
    std::cout << "The MatrixGraph has " << matrixGraph->VerticesCount() << " vertexes.\n";

    std::cout << "\nGetting information about vertexes of Graph.";
    getData_vertexes(listGraph, matrixGraph);
}