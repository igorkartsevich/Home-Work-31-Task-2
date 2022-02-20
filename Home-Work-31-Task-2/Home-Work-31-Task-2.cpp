#include <iostream>
#include <vector>
#include <map>
#include <set>

class IGraph {
public:
    virtual ~IGraph() {}
    IGraph() {};
    IGraph(const IGraph& other_graph) {};
    virtual void AddEdge(int from, int to) = 0; // Метод принимает вершины начала и конца ребра и добавляет ребро
    virtual int VerticesCount() const = 0; // Метод должен считать текущее количество вершин
    virtual std::set<int> VerticesList() const = 0; // Метод должен вернуть текущие список текущих вершин
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную

};

class ListGraph : public IGraph {
public:
    virtual ~ListGraph() {}

    ListGraph() = default;

    ListGraph(const ListGraph&) = default;

    ListGraph(const IGraph& other_graph) : IGraph(other_graph) {
        for (auto vertex : other_graph.VerticesList()) {
            std::vector<int> verticesList;

            other_graph.GetNextVertices(vertex, verticesList); // получить список next
            for (auto element : verticesList) {
                list_next[vertex].emplace(element);
                list_prev[element].emplace(vertex);
            }
        }
    }

    virtual void AddEdge(int from, int to) override {
        list_next[from].emplace(to);
        list_prev[to].emplace(from);

        return;
    }

    virtual int VerticesCount() const override {
        return VerticesList().size();
    }

    virtual std::set<int> VerticesList() const override {
        std::set<int> verticesList;
        for(auto i : list_next) verticesList.emplace(i.first); // все вершины из list_next
        for (auto i : list_prev) verticesList.emplace(i.first);

        return verticesList;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();

        if (list_next.find(vertex) != list_next.end()) // если vertex есть в list_next
            for (auto vertex : list_next.find(vertex)->second)
                vertices.push_back(vertex); 

        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();

        if (list_prev.find(vertex) != list_prev.end()) // если vertex есть в list_prev
            for (auto i : list_prev.find(vertex)->second)
                vertices.push_back(i);

        return;
    }

private:
    std::map<int, std::set<int>> list_next;
    std::map<int, std::set<int>> list_prev;
};

class MatrixGraph : public IGraph {
public:
    virtual ~MatrixGraph() {}

    MatrixGraph() = default;

    MatrixGraph(const MatrixGraph&) = default;

    MatrixGraph(const IGraph& other_graph) : IGraph(other_graph) {
        index_to_vertex = other_graph.VerticesList(); // получить список вершин графа
        int verticesCounter = index_to_vertex.size();

        int counter = 0;
        for (auto element : index_to_vertex) {
            vertex_to_index[element] = counter++;
        }

        matrix.resize(verticesCounter); // ресайз матриц
        for (int i = 0; i < verticesCounter; ++i)
            matrix[i].resize(verticesCounter);

        for (auto vertex : index_to_vertex) {
            std::vector<int> next_vertices;
            other_graph.GetNextVertices(vertex, next_vertices); // получить спиок next

            for (auto next_vertex : next_vertices) // заполнить матрицу
                matrix[vertex_to_index.find(vertex)->second][vertex_to_index.find(next_vertex)->second] = 1;
        }
    }

    virtual void AddEdge(int from, int to) override {
        if (index_to_vertex.emplace(from).second)
            vertex_to_index[from] = index_to_vertex.size() - 1;

        if (index_to_vertex.emplace(to).second)
            vertex_to_index[to] = index_to_vertex.size() - 1;

        if (matrix.size() != index_to_vertex.size()) { // резайз матриц, если были добавлены вершины
            matrix.resize(index_to_vertex.size());
            for (int i = 0; i < matrix.size(); ++i)
                matrix[i].resize(index_to_vertex.size());
        }

        int index_from = vertex_to_index.find(from)->second;
        int index_to = vertex_to_index.find(to)->second;
        matrix[index_from][index_to] = 1;

        return;
    }

    virtual int VerticesCount() const override {
        return index_to_vertex.size();
    }

    virtual std::set<int> VerticesList() const override {
        return index_to_vertex;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (vertex_to_index.find(vertex) != vertex_to_index.end()) { // если vertex есть среди вершин графа

            int index_from = vertex_to_index.find(vertex)->second;
            int counter = 0;
            for (auto element : index_to_vertex)
                if (matrix[index_from][counter++] == 1) vertices.push_back(element);
        }
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (vertex_to_index.find(vertex) != vertex_to_index.end()) { // если vertex есть среди вершин графа

            int index_to = vertex_to_index.find(vertex)->second;
            int counter = 0;
            for (auto element : index_to_vertex)
                if (matrix[counter++][index_to] == 1) vertices.push_back(element);
        }
        
        return;
    }

private:
    std::map<int, int> vertex_to_index;
    std::set<int> index_to_vertex;
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

    std::set<int> vertices_list;
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