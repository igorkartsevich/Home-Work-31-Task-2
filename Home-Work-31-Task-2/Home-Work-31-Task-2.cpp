#include <iostream>
#include <vector>
#include <map>

class IGraph {
public:
    virtual ~IGraph() {}
    IGraph() {};
    IGraph(const IGraph& other_graph) {};
    virtual void AddEdge(int from, int to) = 0; // Метод принимает вершины начала и конца ребра и добавляет ребро
    virtual int VerticesCount() const = 0; // Метод должен считать текущее количество вершин
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную
};

class ListGraph : public IGraph {
public:
    virtual ~ListGraph() {}

    ListGraph() : verticesCounter(0) {}

    ListGraph(const ListGraph& other_graph) : IGraph(other_graph) {
        verticesCounter = 0;
        list_next = other_graph.list_next;
        list_prev = other_graph.list_prev;
    }

    virtual void AddEdge(int from, int to) override {
        bool is_adge_from_to = (list_next.find(from) != list_next.end() && list_prev.find(to) != list_prev.end());
        bool is_from_in_vertices = (list_next.find(from) != list_next.end() || list_prev.find(from) != list_prev.end());
        bool is_to_in_vertices = (list_next.find(to) != list_next.end() || list_prev.find(to) != list_prev.end());

        if (!is_adge_from_to) {

            if (!is_from_in_vertices) verticesCounter++;
            if (!is_to_in_vertices) verticesCounter++;
            if (from == to) verticesCounter--;

            list_next[from].push_back(to);
            list_prev[to].push_back(from);
        }
        return;
    }

    virtual int VerticesCount() const override {
        return verticesCounter;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (list_next.find(vertex) == list_next.end()) std::cout << "Vertex " << vertex << " does not have next vertices";
        else for (auto i : list_next.find(vertex)->second) vertices.push_back(i);
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (list_prev.find(vertex) == list_prev.end()) std::cout << "Vertex " << vertex << " does not have previous vertices";
        else for (auto i : list_prev.find(vertex)->second) vertices.push_back(i);
        return;
    }

private:
    int verticesCounter;
    std::map<int, std::vector<int>> list_next;
    std::map<int, std::vector<int>> list_prev;
};

class MatrixGraph : public IGraph {
public:
    virtual ~MatrixGraph() {}

    MatrixGraph() : verticesCounter(0) {}

    MatrixGraph(const MatrixGraph& other_graph) : IGraph(other_graph) {
        verticesCounter = 0;
        matrix_next = other_graph.matrix_next;
        matrix_next = other_graph.matrix_prev;
    }

    virtual void AddEdge(int from, int to) override {
        bool is_adge_from_to = false;
        if (matrix_next.find(from) != matrix_next.end())
            if (matrix_next.find(from)->second.find(to) != matrix_next.find(from)->second.end()) is_adge_from_to = true;

        bool is_from_in_vertices = (matrix_next.find(from) != matrix_next.end() || matrix_prev.find(from) != matrix_prev.end());
        bool is_to_in_vertices =   (matrix_next.find(to)   != matrix_next.end() || matrix_prev.find(to)   != matrix_prev.end());

        if (!is_adge_from_to) {

            if (!is_from_in_vertices) verticesCounter++;
            if (!is_to_in_vertices) verticesCounter++;
            if (from == to) verticesCounter--;

            matrix_next[from][to] = 1;
            matrix_prev[to][from] = -1;
        }
        return;
    }

    virtual int VerticesCount() const override {
        return verticesCounter;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        /*vertices.clear();
        if (vertex < std::max(list_next.size(), list_prev.size()))
            for (auto i : list_prev.find(vertex)->second) vertices.push_back(i);
        else std::cout << "This Graph does not contain vertex number " << vertex;*/
        return;
    }

private:
    int verticesCounter;
    std::map<int, std::map<int, int>> matrix_next;
    std::map<int, std::map<int, int>> matrix_prev;
};

int main()
{
    ListGraph lg1;

    lg1.AddEdge(1, 2);
    lg1.AddEdge(1, 4);
    lg1.AddEdge(1, 2);
    lg1.AddEdge(100, 100);
    lg1.AddEdge(100, 100);
    lg1.AddEdge(-1, 1);

    std::cout << "This Graph contains " << lg1.VerticesCount() << " vertices\n";
    
    std::vector<int> vertices;
    lg1.GetNextVertices(14, vertices);
    for (auto i : vertices) std::cout << i << " ";
    std::cout << "\n";
    lg1.GetPrevVertices(1, vertices);
    for (auto i : vertices) std::cout << i << " ";
    std::cout << "\n";

    ListGraph lg2(lg1);
    ListGraph lg3;
    lg3 = lg2;

    MatrixGraph mg1;

    mg1.AddEdge(1, 2);
    mg1.AddEdge(1, 4);
    mg1.AddEdge(1, 2);
    mg1.AddEdge(100, 100);
    mg1.AddEdge(100, 100);
    mg1.AddEdge(-1, 1);

    std::cout << "This Graph contains " << mg1.VerticesCount() << " vertices\n";
}