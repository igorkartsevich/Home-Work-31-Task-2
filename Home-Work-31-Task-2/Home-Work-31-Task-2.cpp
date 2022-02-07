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
    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0; // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную

protected:
    int verticesCounter;
    std::vector<int> index_to_vertex;
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
        if (list_next.find(vertex) == list_next.end()) std::cout << "This Graph does not contain vertex " << vertex;
        else {
            std::cout << "Vertex " << vertex << " has following NEXT vertices: ";
            for (auto i : list_next.find(vertex)->second) vertices.push_back(i);
        }
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (list_prev.find(vertex) == list_prev.end()) std::cout << "This Graph does not contain vertex " << vertex;
        else {
            std::cout << "Vertex " << vertex << " has following PREVIOUS vertices: ";
            for (auto i : list_prev.find(vertex)->second) vertices.push_back(i);
        }
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

    MatrixGraph() : IGraph() {}

    MatrixGraph(const MatrixGraph& other_graph) : IGraph(other_graph) { 
        vertex_to_index = other_graph.vertex_to_index;
        index_to_vertex = other_graph.index_to_vertex;
        matrix = other_graph.matrix;
    }

    virtual void AddEdge(int from, int to) override {
        bool is_from_in_vertices = (vertex_to_index.find(from) != vertex_to_index.end());
        bool is_to_in_vertices = (vertex_to_index.find(to) != vertex_to_index.end());

        if (!is_from_in_vertices || !is_to_in_vertices) {
            if (!is_from_in_vertices) {
                vertex_to_index[from] = ++verticesCounter;
                index_to_vertex.push_back(from);
            }
            if (!is_to_in_vertices) {
                vertex_to_index[to] = ++verticesCounter;
                index_to_vertex.push_back(to);
            }
            
            matrix.resize(verticesCounter);
            for (int i = 0; i < matrix.size(); ++i) matrix[i].resize(verticesCounter);
        }

        int index_from = vertex_to_index.find(from)->second - 1;
        int index_to = vertex_to_index.find(to)->second - 1;

        if (matrix[index_from][index_to] != 1) {
            matrix[index_from][index_to] = 1;
        }
        
        return;
    }

    virtual int VerticesCount() const override {
        return matrix.size() - 1;
    }

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (vertex_to_index.find(vertex) == vertex_to_index.end()) std::cout << "This Graph does not contain vertex " << vertex;
        else {
            std::cout << "Vertex " << vertex << " has following NEXT vertices: ";
            int vertex_ID_ = vertex_to_index.find(vertex)->second - 1;
            for (int i = 0; i < matrix[vertex_ID_].size(); ++i)
                if(matrix[vertex_ID_][i] == 1) std::cout << index_to_vertex[i] << " ";
        }
        return;
    }

    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const override {
        vertices.clear();
        if (vertex_to_index.find(vertex) == vertex_to_index.end()) std::cout << "This Graph does not contain vertex " << vertex;
        else {
            std::cout << "Vertex " << vertex << " has following PREVIOUS vertices: ";
            int vertex_ID_ = vertex_to_index.find(vertex)->second - 1;
            for (int i = 0; i < matrix.size(); ++i)
                if (matrix[i][vertex_ID_] == 1) std::cout << index_to_vertex[i] << " ";
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

    mg1.AddEdge(1, 2);
    mg1.AddEdge(1, 4);
    mg1.AddEdge(1, 2);
    mg1.AddEdge(100, 100);
    mg1.AddEdge(100, 100);
    mg1.AddEdge(-1, 1);

    std::cout << "This Graph contains " << mg1.VerticesCount() << " vertices\n";

    std::vector<int> vertices;
    mg1.GetNextVertices(1, vertices);
    for (auto i : vertices) std::cout << i << " ";
    std::cout << "\n";
    mg1.GetPrevVertices(1, vertices);
    for (auto i : vertices) std::cout << i << " ";
    std::cout << "\n";

    MatrixGraph mg2(mg1);

    //ListGraph lg1(mg2);
}