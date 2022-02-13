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
        list_next = other_graph.list_next;
        list_prev = other_graph.list_prev;
    }

    virtual void AddEdge(int from, int to) override {
        bool is_from_in_vertices = (list_next.find(from) != list_next.end());
        if (!is_from_in_vertices) verticesCounter++;

        bool is_to_in_vertices = (list_prev.find(to) != list_prev.end());
        if (!is_to_in_vertices && from != to) verticesCounter++;

        bool is_adge_from_to = false;
        if (is_from_in_vertices) {
            for (auto i : list_next.find(from)->second) {
                if (i == to) {
                    is_adge_from_to = true;
                    break;
                }
            }
        }

        if (!is_adge_from_to) {
            list_next[from].push_back(to);
            list_prev[to].push_back(from);
        }

        return;
    }

    virtual int VerticesCount() const override {
        return verticesCounter;
    }

    virtual std::vector<int> VerticesList() const override {
        std::vector<int> verticesList;
        for(auto i : list_next) verticesList.push_back(i.first);

        bool is_vertices = false;
        for(auto i : list_prev) {
            for(auto j : verticesList) {
                if(j == i.first) {
                    is_vertices = true;
                    break;
                }
            }
            if (!is_vertices) verticesList.push_back(i.first);
        }

        return verticesList;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        if (list_next.find(vertex) == list_next.end()) return;
        vertices.clear();
        for (auto i : list_next.find(vertex)->second) vertices.push_back(i);
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        if (list_prev.find(vertex) == list_prev.end()) return;
        vertices.clear();
        for (auto i : list_prev.find(vertex)->second) vertices.push_back(i);
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
        bool is_fron_in_vertices = (vertex_to_index.find(from) != vertex_to_index.end());
        if (!is_fron_in_vertices) {
            verticesCounter++;
            index_to_vertex.push_back(from);
            vertex_to_index[from] = verticesCounter - 1;
        }

        bool is_to_in_vertices = (vertex_to_index.find(to) != vertex_to_index.end());
        if (from != to && !is_to_in_vertices) {
            verticesCounter++;
            index_to_vertex.push_back(to);
            vertex_to_index[to] = verticesCounter - 1;
        }

        if (matrix.size() != verticesCounter) {
            matrix.resize(verticesCounter);
            for (int i = 0; i < matrix.size(); ++i) matrix[i].resize(verticesCounter);
        }

        int index_from = vertex_to_index.find(from)->second;
        int index_to = vertex_to_index.find(to)->second;

        if(matrix[index_from][index_to] != 1) matrix[index_from][index_to] = 1;

        return;
    }

    virtual int VerticesCount() const override {
        return verticesCounter;
    }

    virtual std::vector<int> VerticesList() const override {
        std::vector<int> verticesList;
        return verticesList;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        if (vertex_to_index.find(vertex) == vertex_to_index.end()) return;
        vertices.clear();
        for (int i = 0; i < verticesCounter; ++i) {
            if (matrix[vertex][i] == 1) vertices.push_back(i);
        }
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        if (vertex_to_index.find(vertex) == vertex_to_index.end()) return;
        vertices.clear();
        for (int i = 0; i < verticesCounter; ++i) {
            if (matrix[i][vertex] == 1) vertices.push_back(i);
        }
        return;
    }

private:
    std::map<int, int> vertex_to_index;
    std::vector<int> index_to_vertex;
    std::vector<std::vector<int>> matrix;
};

int main()
{
    MatrixGraph mg1;

    mg1.AddEdge(1, 1);
    mg1.AddEdge(1, 4);
    mg1.AddEdge(1, 2);
    mg1.AddEdge(100, 100);
    mg1.AddEdge(100, 100);
    mg1.AddEdge(-1, 1);



}