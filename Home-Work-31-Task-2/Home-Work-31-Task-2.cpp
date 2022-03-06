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
        std::vector<int> verticesList = other_graph.VerticesList();
        std::for_each(verticesList.begin(), verticesList.end(), [&](int vertex) {

            std::vector<int> vertices_TMP;
            other_graph.GetNextVertices(vertex, vertices_TMP);

            std::for_each(vertices_TMP.begin(), vertices_TMP.end(), [&](int vertex_next) {
                list_next[vertex].push_back(vertex_next);
                list_prev[vertex_next].push_back(vertex);
                });
            });
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
        index_to_vertex = other_graph.VerticesList();

        int counter = 0;
        std::for_each(index_to_vertex.begin(), index_to_vertex.end(), [&](int vertex) {
            vertex_to_index[vertex] = counter++;
        });

        int size_matrix = index_to_vertex.size();
        matrix.resize(size_matrix);
        std::for_each(matrix.begin(), matrix.end(), [&](auto& element_vector) {
            element_vector.resize(size_matrix);
            });

        std::for_each(index_to_vertex.begin(), index_to_vertex.end(), [&](int vertex) {
            std::vector<int> vertices_TMP;
            other_graph.GetNextVertices(vertex, vertices_TMP);
            std::for_each(vertices_TMP.begin(), vertices_TMP.end(), [&](int vertex_next) {
                matrix[vertex_to_index.find(vertex)->second][vertex_to_index.find(vertex_next)->second] = 1;
            });
        });
    }

    virtual void AddEdge(int from, int to) override {
        bool is_from_in_vertices = vertex_to_index.find(from) != vertex_to_index.end();
        bool is_to_in_vertices = vertex_to_index.find(to) != vertex_to_index.end();

        if (!is_from_in_vertices) {
            index_to_vertex.push_back(from);
            vertex_to_index[from] = index_to_vertex.size() - 1;
        }
        if (!is_to_in_vertices && from != to) {
            index_to_vertex.push_back(to);
            vertex_to_index[to] = index_to_vertex.size() - 1;
        }
        if (!is_from_in_vertices || !is_to_in_vertices) {
            int size = index_to_vertex.size();
            matrix.resize(size);
            for (int i = 0; i < matrix.size(); ++i) matrix[i].resize(size);
        }

        matrix[vertex_to_index.find(from)->second][vertex_to_index.find(to)->second] = 1;
    }

    virtual int VerticesCount() const override {
        return index_to_vertex.size();
    }

    virtual std::vector<int> VerticesList() const override {
        return index_to_vertex;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (vertex_to_index.find(vertex) != vertex_to_index.end()) {
            int counter = 0;
            int index = vertex_to_index.find(vertex)->second;
            std::for_each(matrix[index].begin(), matrix[index].end(), [&](int a) {
                if (a == 1) vertices.push_back(index_to_vertex[counter]);
                ++counter;
                });
        }
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (vertex_to_index.find(vertex) != vertex_to_index.end()) {
            int counter = 0;
            int index = vertex_to_index.find(vertex)->second;
            std::for_each(matrix.begin(), matrix.end(), [&](auto vector) {
                if (vector[index] == 1) vertices.push_back(index_to_vertex[counter]);
                ++counter;
                });
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

    MatrixGraph mg2;
    mg2 = mg1;

    std::cout << mg2.VerticesCount() << "\n"; // количество вершин в графе

    vertices_list = mg2.VerticesList();
    for (auto i : vertices_list) std::cout << i << " "; // список вершин графа
    std::cout << "\n";

    mg2.GetNextVertices(1, vertices);
    for (auto i : vertices) std::cout << i << " "; // список next по вершине 1
    std::cout << "\n";
    mg2.GetPrevVertices(1, vertices);
    for (auto i : vertices) std::cout << i << " "; // список prev по вершине 1
    std::cout << "\n";
}