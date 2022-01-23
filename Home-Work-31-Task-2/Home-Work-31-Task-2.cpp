﻿#include <iostream>
#include <vector>
#include <map>

class IGraph {
public:
    virtual ~IGraph() {}
    IGraph() {
        graph_from.clear();
        graph_to.clear();
    }
    IGraph(const IGraph& other) {
        *this = other;
    }
    virtual void AddEdge(int from, int to) = 0; // Метод принимает вершины начала и конца ребра и добавляет ребро
    virtual int VertexesCount() const = 0; // Метод должен считать текущее количество вершин
    virtual void GetNextVertexes(int vertex, std::vector<int>& vertexes) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной  
    virtual void GetPrevVertexes(int vertex, std::vector<int>& vertexes) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную
    virtual void printVertexes(int number, std::vector<int>& vertexes, std::string next_prev) const = 0;

protected:
    std::vector<std::vector<int>> graph_from;
    std::vector<std::vector<int>> graph_to;

};

class ListGraph : public IGraph {
public:
    virtual ~ListGraph() {}
    ListGraph() : IGraph() {}
    ListGraph(const IGraph& other) : IGraph(other) {}

    void AddEdge(int from, int to) override {
        if (graph_from.size() < from) graph_from.resize(from);
        graph_from[from - 1].push_back(to);

        if (graph_to.size() < to) graph_to.resize(to);
        graph_to[to - 1].push_back(from);
    }

    int VertexesCount() const override {
        return std::max(graph_from.size(), graph_to.size());
    }

    void GetNextVertexes(int vertex, std::vector<int>& vertexes) const override {
        if (graph_from.size() >= vertex) vertexes = graph_from[--vertex];
        printVertexes(vertex, vertexes, "next");
    }

    void GetPrevVertexes(int vertex, std::vector<int>& vertexes) const override {
        if (graph_to.size() >= vertex) vertexes = graph_to[--vertex];
        printVertexes(vertex, vertexes, "previous");
    }

    void printVertexes(int number, std::vector<int>& vertexes, std::string next_prev) const override {
        std::cout << "\n\tVetrex number " << number << " has following vertexes " << next_prev << ": ";

        if (vertexes.size() == 0) {
            std::cout << " No vertexes";
            return;
        }

        for (auto i : vertexes) std::cout << i << " ";
        vertexes.clear();
    }

};

class MatrixGraph : public IGraph {
public:
    virtual ~MatrixGraph() {}
    MatrixGraph() : IGraph() {}
    MatrixGraph(const IGraph& other) : IGraph(other) {}

    void AddEdge(int from, int to) override {
        int matrixSize = std::max(from, to);
        
        if (graph_from.size() < matrixSize) {
            graph_from.resize(matrixSize);
            graph_to.resize(matrixSize);
            for (int i = 0; i < matrixSize; ++i) {
                graph_from[i].resize(matrixSize);
                graph_to[i].resize(matrixSize);
            }
        }

        from--, to--;
        graph_from[from][to] = 1;
        graph_to[to][from] = 1;
    }

    int VertexesCount() const override {
        return graph_from.size();
    }

    void GetNextVertexes(int vertex, std::vector<int>& vertexes) const override {
        vertex--;
        int maxVertex = VertexesCount();
        for (int i = 0; i < maxVertex; ++i)
            if (graph_from[vertex][i] == 1) vertexes.push_back(i + 1);
        printVertexes(vertex, vertexes, "next");
    }

    void GetPrevVertexes(int vertex, std::vector<int>& vertexes) const override {
        vertex--;
        int maxVertex = VertexesCount();
        for (int i = 0; i < maxVertex; ++i)
            if (graph_to[vertex][i] == 1) vertexes.push_back(i + 1);
        printVertexes(vertex, vertexes, "previous");
    }

    void printVertexes(int number, std::vector<int>& vertexes, std::string next_prev) const override {
        std::cout << "\n\tVetrex number " << number << " has following vertexes " << next_prev << ": ";

        if (vertexes.size() == 0) {
            std::cout << " No vertexes";
            return;
        }

        for (auto i : vertexes) std::cout << i << " ";
        vertexes.clear();
    }

};

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

        if (vertexNumber > matrixGraph->VertexesCount()) {
            std::cerr << "This Graph does not contains vertex number " << vertexNumber;
            continue;
        }

        std::vector<int> vertexes;
        
        std::cout << "ListGraph";
        listGraph->GetNextVertexes(vertexNumber, vertexes);
        listGraph->GetPrevVertexes(vertexNumber, vertexes);

        std::cout << "\nMatrixGraph";
        matrixGraph->GetNextVertexes(vertexNumber, vertexes);
        matrixGraph->GetPrevVertexes(vertexNumber, vertexes);
    }
}

int main()
{
    IGraph* listGraph = new ListGraph();
    IGraph* matrixGraph = new MatrixGraph();

    std::cout << "Enter data for Graph.\n";
    setData_vertexes(listGraph, matrixGraph);

    std::cout << "\nThe ListGraph has " << listGraph->VertexesCount() << " vertexes.\n";
    std::cout << "The MatrixGraph has " << matrixGraph->VertexesCount() << " vertexes.\n";

    std::cout << "\nGetting information about vertexes of Graph.";
    getData_vertexes(listGraph, matrixGraph);

    IGraph* listGraph_new_1 = new ListGraph(*listGraph);
    IGraph* matrixGraph_new_1 = new MatrixGraph(*matrixGraph);

    return 0;
}