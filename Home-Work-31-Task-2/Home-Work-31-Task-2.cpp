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
    virtual std::vector<int> VerticesList() const = 0; // Метод должен вернуть текущие список текущих вершин
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную

};

class ListGraph : public IGraph {
public:
    virtual ~ListGraph() {}

    ListGraph() : IGraph() {}

    ListGraph(const ListGraph& other_graph) : IGraph(other_graph) {
        list_next = other_graph.list_next;
        list_prev = other_graph.list_prev;
    }

    ListGraph(const class MatrixGraph& other_graph);

    virtual void AddEdge(int from, int to) override {
        bool is_from_in_vertices = (list_next.find(from) != list_next.end() || list_prev.find(from) != list_prev.end());

        bool is_adge_from_to = false;
        if (is_from_in_vertices) { // если from уже есть среди вершин графа, проверка на наличие ребра с to

            for (auto vertex_next : list_next.find(from)->second) {
                if (to == vertex_next) {
                    is_adge_from_to = true;
                    break;
                }
            }
        }

        if (!is_adge_from_to) { // если ребра fron - to нет
            list_next[from].push_back(to);
            list_prev[to].push_back(from);
        }

        return;
    }

    virtual int VerticesCount() const override {
        int verticesCounter = list_next.size(); // все вершины next

        for (auto vertex_prev : list_prev) { // проверка повторения вершин из prev в списке вершин next
            bool is_prev_in_next = false;

            for (auto vertex_next : list_next) {
                if (vertex_prev.first == vertex_next.first) {
                    is_prev_in_next = true;
                    break;
                }
            }

            if (!is_prev_in_next) verticesCounter++; // если prev нет среди next
        }

        return verticesCounter;
    }

    virtual std::vector<int> VerticesList() const override {
        std::vector<int> verticesList;
        for(auto i : list_next) verticesList.push_back(i.first); // все вершины из list_next

        for(auto vertex_prev : list_prev) { // проверка повторения vertex_prev в list_next
            bool is_prev_in_next = false;

            for(auto vertex_next : list_next) {
                if(vertex_prev.first == vertex_next.first) {
                    is_prev_in_next = true;
                    break;
                }
            }

            if (!is_prev_in_next) verticesList.push_back(vertex_prev.first); // если vertex_prev нет в list_next
        }

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
    std::map<int, std::vector<int>> list_next;
    std::map<int, std::vector<int>> list_prev;
};

class MatrixGraph : public IGraph {
public:
    virtual ~MatrixGraph() {}

    MatrixGraph() : IGraph() {}

    MatrixGraph(const MatrixGraph& other_graph) : IGraph(other_graph) {
        vertex_to_index = other_graph.vertex_to_index;
        index_to_vertex = other_graph.index_to_vertex;
        matrix = other_graph.matrix;
    }

    MatrixGraph(const class ListGraph& other_graph);

    virtual void AddEdge(int from, int to) override {
        bool is_fron_in_vertices = (vertex_to_index.find(from) != vertex_to_index.end());
        if (!is_fron_in_vertices) { // если from нет среди вершин графа - добавить
            index_to_vertex.push_back(from);
            vertex_to_index[from] = index_to_vertex.size() - 1;
        }

        bool is_to_in_vertices = (vertex_to_index.find(to) != vertex_to_index.end());
        if (from != to && !is_to_in_vertices) { // если to нет среди вершин графа и не равен from - добавить
            index_to_vertex.push_back(to);
            vertex_to_index[to] = index_to_vertex.size() - 1;
        }

        if (matrix.size() != index_to_vertex.size()) { // резайз матриц, если были добавлены вершины
            matrix.resize(index_to_vertex.size());
            for (int i = 0; i < matrix.size(); ++i)
                matrix[i].resize(index_to_vertex.size());
        }

        int index_from = vertex_to_index.find(from)->second;
        int index_to = vertex_to_index.find(to)->second;

        if(matrix[index_from][index_to] != 1) // если ребра from - to нет --- добавть
            matrix[index_from][index_to] = 1;

        return;
    }

    virtual int VerticesCount() const override {
        return index_to_vertex.size();
    }

    virtual std::vector<int> VerticesList() const override {
        return index_to_vertex;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (vertex_to_index.find(vertex) != vertex_to_index.end()) { // если vertex есть среди вершин графа

            int index_from = vertex_to_index.find(vertex)->second;
            for (int i = 0; i < index_to_vertex.size(); ++i)
                if (matrix[index_from][i] == 1) vertices.push_back(index_to_vertex[i]);
        }
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (vertex_to_index.find(vertex) != vertex_to_index.end()) { // если vertex есть среди вершин графа

            int index_to = vertex_to_index.find(vertex)->second;
            for (int i = 0; i < index_to_vertex.size(); ++i)
                if (matrix[i][index_to] == 1) vertices.push_back(index_to_vertex[i]);
        }
        
        return;
    }

private:
    std::map<int, int> vertex_to_index;
    std::vector<int> index_to_vertex;
    std::vector<std::vector<int>> matrix;
};

ListGraph::ListGraph(const class MatrixGraph& other_graph) : IGraph(other_graph) {
    for (auto vertex : other_graph.VerticesList()) {
        std::vector<int> verticesList;
        other_graph.GetNextVertices(vertex, verticesList); // получить список next
        if (verticesList.size() != 0) // если список не пустой добавть list_next
            for (auto next_vertex : verticesList) list_next[vertex].push_back(next_vertex);

        other_graph.GetPrevVertices(vertex, verticesList); // получить список prev
        if (verticesList.size() != 0) //если список не пустой добавть list_prev
            for (auto next_prev : verticesList) list_prev[vertex].push_back(next_prev);
    }
}

MatrixGraph::MatrixGraph(const class ListGraph& other_graph) : IGraph(other_graph) {
    index_to_vertex = other_graph.VerticesList(); // получить список вершин графа
    int verticesCounter = index_to_vertex.size();
    
    for (int i = 0; i < verticesCounter; ++i) // заполнить мап графа
        vertex_to_index[index_to_vertex[i]] = i;

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

int main()
{
    MatrixGraph mg1;

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

    ListGraph lg2;
    lg2 = mg1;

    MatrixGraph mg2;
    mg2 = lg2;

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