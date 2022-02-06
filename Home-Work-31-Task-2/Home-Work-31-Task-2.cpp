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

class MatrixGraph : public IGraph {
public:
    virtual ~MatrixGraph() {}

    MatrixGraph() : verticesCounter(0) {}

    MatrixGraph(const MatrixGraph& other_graph) : IGraph(other_graph) {
        graph_next = other_graph.graph_next;
        graph_prev = other_graph.graph_prev;
    }

    virtual void AddEdge(int from, int to) override {
        bool is_from_not_in_from = (graph_next.find(from) == graph_next.end());
        bool is_from_not_in_to = (graph_prev.find(from) == graph_prev.end());
        bool is_to_not_in_from = (graph_next.find(to) == graph_next.end());
        bool is_to_not_in_to = (graph_prev.find(to) == graph_prev.end());

        if (is_from_not_in_from || is_to_not_in_to) {

            if (is_from_not_in_from && is_from_not_in_to) verticesCounter++;
            if (is_to_not_in_to && is_to_not_in_from) verticesCounter++;
            if (from == to) verticesCounter--;

            graph_next[from].push_back(to);
            graph_prev[to].push_back(from);
        }
        return;
    }

    virtual int VerticesCount() const override {
        return verticesCounter;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (vertex < std::max(graph_next.size(), graph_prev.size())) {
            for (auto i : graph_next.find(vertex)->second) vertices.push_back(i); 
        }
        else std::cout << "This Graph does not contain vertex number " << vertex;
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (vertex < std::max(graph_next.size(), graph_prev.size())) {
            for (auto i : graph_prev.find(vertex)->second) vertices.push_back(i);
        }
        else std::cout << "This Graph does not contain vertex number " << vertex;
        return;
    }

private:
    int verticesCounter;
    std::map<int, std::vector<int>> graph_next;
    std::map<int, std::vector<int>> graph_prev;
};

int main()
{
    MatrixGraph g1;

    g1.AddEdge(1, 2);
    g1.AddEdge(1, 4);
    g1.AddEdge(1, 2);
    g1.AddEdge(100, 100);
    g1.AddEdge(100, 100);
    g1.AddEdge(-1, 1);

    std::cout << "This Graph contains " << g1.VerticesCount() << " vertices\n";
    
    std::vector<int> vertices;
    g1.GetNextVertices(1, vertices);
    for (auto i : vertices) std::cout << i << " ";
    std::cout << "\n";
    g1.GetPrevVertices(1, vertices);
    for (auto i : vertices) std::cout << i << " ";
    std::cout << "\n";

    MatrixGraph g2(g1);

    //ListGraph g2 = g1;
}