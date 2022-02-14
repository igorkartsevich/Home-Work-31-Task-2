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
        bool is_to_in_vertices = (list_next.find(to) != list_next.end() || list_prev.find(to) != list_prev.end());

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
        int verticesCounter = list_next.size();
        
        for (auto i : list_prev) {
            bool is_vertices = false;
            for (auto j : list_next) {
                if (j.first == i.first) {
                    is_vertices = true;
                    break;
                }
            }
            if (!is_vertices) verticesCounter++;
        }

        return verticesCounter;
    }

    virtual std::vector<int> VerticesList() const override {
        std::vector<int> verticesList;
        for(auto i : list_next) verticesList.push_back(i.first);

        for(auto i : list_prev) {
            bool is_vertices = false;
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
        vertex_to_index = other_graph.vertex_to_index;
        index_to_vertex = other_graph.index_to_vertex;
        matrix = other_graph.matrix;
    }

    MatrixGraph(const class ListGraph& other_graph);

    virtual void AddEdge(int from, int to) override {
        bool is_fron_in_vertices = (vertex_to_index.find(from) != vertex_to_index.end());
        if (!is_fron_in_vertices) {
            index_to_vertex.push_back(from);
            vertex_to_index[from] = index_to_vertex.size() - 1;
        }

        bool is_to_in_vertices = (vertex_to_index.find(to) != vertex_to_index.end());
        if (from != to && !is_to_in_vertices) {
            index_to_vertex.push_back(to);
            vertex_to_index[to] = index_to_vertex.size() - 1;
        }

        if (matrix.size() != index_to_vertex.size()) {
            matrix.resize(index_to_vertex.size());
            for (int i = 0; i < matrix.size(); ++i) matrix[i].resize(index_to_vertex.size());
        }

        int index_from = vertex_to_index.find(from)->second;
        int index_to = vertex_to_index.find(to)->second;

        if(matrix[index_from][index_to] != 1) matrix[index_from][index_to] = 1;

        return;
    }

    virtual int VerticesCount() const override {
        return index_to_vertex.size();
    }

    virtual std::vector<int> VerticesList() const override {
        return index_to_vertex;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        if (vertex_to_index.find(vertex) == vertex_to_index.end()) return;
        vertices.clear();
        int index_from = vertex_to_index.find(vertex)->second;
        for (int i = 0; i < index_to_vertex.size(); ++i) {
            if (matrix[index_from][i] == 1) vertices.push_back(index_to_vertex[i]);
        }
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        if (vertex_to_index.find(vertex) == vertex_to_index.end()) return;
        vertices.clear();
        int index_to = vertex_to_index.find(vertex)->second;
        for (int i = 0; i < index_to_vertex.size(); ++i) {
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
    std::vector<int> vertices_list;
    vertices_list = other_graph.VerticesList();

    for (auto vertex : vertices_list) {
        std::vector<int> vertices;
        other_graph.GetNextVertices(vertex, vertices);
        if (vertices.size() != 0)
            for (auto next_vertex : vertices) list_next[vertex].push_back(next_vertex);

        other_graph.GetPrevVertices(vertex, vertices);
        if (vertices.size() != 0)
            for (auto next_prev : vertices) list_prev[vertex].push_back(next_prev);
    }
}

MatrixGraph::MatrixGraph(const class ListGraph& other_graph) : IGraph(other_graph) {
    index_to_vertex = other_graph.VerticesList();
    int verticesCounter = index_to_vertex.size();
    
    for (int i = 0; i < verticesCounter; ++i)
        vertex_to_index[index_to_vertex[i]] = i;

    matrix.resize(verticesCounter);
    for (int i = 0; i < verticesCounter; ++i)
        matrix[i].resize(verticesCounter);

    for (auto i : index_to_vertex) {
        std::vector<int> vertices;
        other_graph.GetNextVertices(i, vertices);

        for (auto j : vertices)
            matrix[vertex_to_index.find(i)->second][vertex_to_index.find(j)->second] = 1;
    }
}

int main()
{
    ListGraph mg1;

    mg1.AddEdge(1, 2);
    mg1.AddEdge(1, 4);
    mg1.AddEdge(1, 2);
    mg1.AddEdge(100, 100);
    mg1.AddEdge(100, 100);
    mg1.AddEdge(-1, 1);

    std::cout << mg1.VerticesCount() << "\n";

    std::vector<int> vertices_list;
    vertices_list = mg1.VerticesList();
    for (auto i : vertices_list) std::cout << i << " ";
    std::cout << "\n";

    std::vector<int> vertices;
    mg1.GetNextVertices(1, vertices);
    for (auto i : vertices) std::cout << i << " ";
    std::cout << "\n";
    mg1.GetPrevVertices(1, vertices);
    for (auto i : vertices) std::cout << i << " ";
    std::cout << "\n";

    MatrixGraph lg1(mg1);

    std::cout << lg1.VerticesCount() << "\n";

    vertices_list = lg1.VerticesList();
    for (auto i : vertices_list) std::cout << i << " ";
    std::cout << "\n";

    lg1.GetNextVertices(4, vertices);
    for (auto i : vertices) std::cout << i << " ";
    std::cout << "\n";
    lg1.GetPrevVertices(4, vertices);
    for (auto i : vertices) std::cout << i << " ";
    std::cout << "\n";
}