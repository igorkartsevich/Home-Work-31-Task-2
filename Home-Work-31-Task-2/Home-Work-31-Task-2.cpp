#include <iostream>
#include <vector>
#include <map>

class IGraph {
public:
    virtual ~IGraph() {}
    IGraph() : verticesCounter(0) {};
    IGraph(const IGraph& other_graph) : verticesCounter(other_graph.verticesCounter) {};
    virtual void AddEdge(int from, int to) = 0; // Метод принимает вершины начала и конца ребра и добавляет ребро
    virtual int VerticesCount() const = 0; // Метод должен считать текущее количество вершин
    virtual std::vector<int> VerticesList() const = 0; // Метод должен вернуть текущие список текущих вершин
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную

protected:
    int verticesCounter;
};

class ListGraph : public IGraph {
public:
    virtual ~ListGraph() {}

    ListGraph() : IGraph() {}

    ListGraph(const ListGraph& other_graph) : IGraph(other_graph) {
        
    }

    virtual void AddEdge(int from, int to) override {
        
        return;
    }

    virtual int VerticesCount() const override {

        return;
    }

    virtual std::vector<int> VerticesList() const override {
        
        return;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        
        return;
    }

private:
    std::map<int, std::vector<int>> list_next;
    std::map<int, std::vector<int>> list_prev;
};

class MatrixGraph : public IGraph {
public:
    virtual ~MatrixGraph() {}

    MatrixGraph() : IGraph() {}

    MatrixGraph(const MatrixGraph& other_graph) : IGraph(other_graph) { 
        
    }

    virtual void AddEdge(int from, int to) override {

        return;
    }

    virtual int VerticesCount() const override {
        return;
    }

    virtual std::vector<int> VerticesList() const override {
        return;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        
        return;
    }

private:
    std::map<int, int> vertex_to_index;
    std::vector<int> index_to_vertex;
    std::vector<std::vector<int>> matrix;
};

int main()
{
    ListGraph mg1;

    mg1.AddEdge(1, 1);
    mg1.AddEdge(1, 4);
    mg1.AddEdge(1, 2);
    mg1.AddEdge(100, 100);
    mg1.AddEdge(100, 100);
    mg1.AddEdge(-1, 1);

}