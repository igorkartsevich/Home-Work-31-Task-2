#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

class IGraph {
public:
    virtual ~IGraph() {}
    IGraph() {};
    IGraph(const IGraph& other_graph) {};
    virtual void AddEdge(int from, int to) = 0; // Метод принимает вершины начала и конца ребра и добавляет ребро
    virtual int VerticesCount() const = 0; // Метод должен считать текущее количество вершин
    virtual std::vector<int> VerticesList() const = 0; // Метод должен вернуть текущие список текущих вершин
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную

};

class ListGraph : public IGraph {
public:
    virtual ~ListGraph() {}

    ListGraph() = default;

    ListGraph(const ListGraph&) = default;

    ListGraph(const IGraph& other_graph) : IGraph(other_graph) {
        
    }

    virtual void AddEdge(int from, int to) override {
        if (std::find(list_next[from].begin(), list_next[from].end(), to) == list_next[from].end()) {
            list_next[from].push_back(to);
            list_prev[to].push_back(from);
        }       
    }

    virtual int VerticesCount() const override {
        return VerticesList().size();
    }

    virtual std::vector<int> VerticesList() const override {
        static auto get_first = [](const auto& pair) { return pair.first; };

        static auto vec_tmp = [](const auto& map) {
            std::vector<int> vec_tmp(map.size());
            std::transform(map.begin(), map.end(), vec_tmp.begin(), get_first);
            return vec_tmp;
        };

        auto vec_tmp_1 = vec_tmp(list_next);
        auto vec_tmp_2 = vec_tmp(list_prev);

        std::vector<int> verticesList(vec_tmp_1.size() + vec_tmp_2.size());
        std::merge(vec_tmp_1.begin(), vec_tmp_1.end(), vec_tmp_2.begin(), vec_tmp_2.end(), verticesList.begin());
        verticesList.erase(std::unique(verticesList.begin(), verticesList.end()), verticesList.end());

        return verticesList;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (list_next.find(vertex) != list_next.end()) vertices = list_next.find(vertex)->second;
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (list_prev.find(vertex) != list_prev.end()) vertices = list_prev.find(vertex)->second;
        return;
    }

private:
    std::map<int, std::vector<int>> list_next;
    std::map<int, std::vector<int>> list_prev;
};

class MatrixGraph : public IGraph {
public:
    virtual ~MatrixGraph() {}

    MatrixGraph() = default;

    MatrixGraph(const MatrixGraph&) = default;

    MatrixGraph(const IGraph& other_graph) : IGraph(other_graph) {
        
    }

    virtual void AddEdge(int from, int to) override {
        
    }

    virtual int VerticesCount() const override {
        return 0;
    }

    virtual std::vector<int> VerticesList() const override {
        std::vector<int> verticesList;
        return verticesList;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();

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

    mg1.AddEdge(1, 2);
    mg1.AddEdge(1, 4);
    mg1.AddEdge(1, 2);
    mg1.AddEdge(100, 100);
    mg1.AddEdge(100, 100);
    mg1.AddEdge(-1, 1);

    std::cout << mg1.VerticesCount() << "\n"; // количество вершин в графе

    std::vector<int> vertices_list;
    vertices_list = mg1.VerticesList();
    for (auto i : vertices_list) std::cout << i << " "; // список вершин графа
    std::cout << "\n";

    std::vector<int> vertices;
    mg1.GetNextVertices(1, vertices);
    for (auto i : vertices) std::cout << i << " "; // список next по вершине 1
    std::cout << "\n";
    mg1.GetPrevVertices(1, vertices);
    for (auto i : vertices) std::cout << i << " "; // список prev по вершине 1
    std::cout << "\n";

    //MatrixGraph mg2;
    //mg2 = mg1;

    //std::cout << mg2.VerticesCount() << "\n"; // количество вершин в графе

    //vertices_list = mg2.VerticesList();
    //for (auto i : vertices_list) std::cout << i << " "; // список вершин графа
    //std::cout << "\n";

    //mg2.GetNextVertices(1, vertices);
    //for (auto i : vertices) std::cout << i << " "; // список next по вершине 1
    //std::cout << "\n";
    //mg2.GetPrevVertices(1, vertices);
    //for (auto i : vertices) std::cout << i << " "; // список prev по вершине 1
    //std::cout << "\n";
}