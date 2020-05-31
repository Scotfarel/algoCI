//
//  Created by ivan on 30/05/20.
//
//  Задача 3. «Города»
//  Требуется отыскать самый выгодный маршрут между городами.
//  Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.
//  Формат входных данных.
//  Первая строка содержит число N – количество городов.
//  Вторая строка содержит число M - количество дорог.
//  Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
//  Последняя строка содержит маршрут (откуда и куда нужно доехать).
//  Формат выходных данных.
//  Вывести длину самого выгодного маршрута.
//

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <sstream>
#include <cassert>


typedef std::pair<int, int> pair;

class Graph {
public:
    explicit Graph(size_t vertices_count) : vertices(vertices_count), verticesCount(vertices_count) {};

    void AddEdge(int from, int to, int distance) {
        vertices[from].push_back(std::make_pair(to, distance));
        vertices[to].push_back(std::make_pair(from, distance));
    }

    int VerticesCount() const { return verticesCount; }

    std::vector<pair> GetNextVertices(int vertex) const {
        std::vector<pair> result;
        result.resize(vertices[vertex].size());
        std::copy(vertices[vertex].begin(), vertices[vertex].end(), result.begin());
        return result;
    }

private:
    std::vector<std::vector<pair>> vertices;
    int verticesCount;
};

int Djikstra(const Graph &graph, int start, int end) {
    if (start == end) return 0;

    std::vector<int> distances(graph.VerticesCount(), INT_MAX);
    distances[start] = 0;

    std::priority_queue<pair, std::vector<pair>, std::greater<>> priorityQueue;
    priorityQueue.push(std::make_pair(0, start));

    std::vector<bool> visited(graph.VerticesCount(), false);

    while(!priorityQueue.empty()) {
        pair currentVertices = priorityQueue.top();
        priorityQueue.pop();

        std::vector<pair> neighbours = graph.GetNextVertices(currentVertices.second);

        for (const pair& vertex: neighbours) {
            if (visited[vertex.first]) continue;
            if (distances[vertex.first] > distances[currentVertices.second] + vertex.second) {
                distances[vertex.first] = distances[currentVertices.second] + vertex.second;
                priorityQueue.push(std::make_pair(distances[vertex.first], vertex.first));
            }
        }

        visited[currentVertices.second] = true;
    }

    return distances[end];
}

int run(std::istream &input, std::ostream &output) {
    //  Input
    int vertices = 0;
    int edges = 0;
    input >> vertices >> edges;

    std::vector<int> froms;
    std::vector<int> tos;
    std::vector<int> weights;

    for (int i = 0; i < edges; i++) {
        int from = 0;
        int to = 0;
        int weight = 0;
        input >> from >> to >> weight;
        froms.push_back(from);
        tos.push_back(to);
        weights.push_back(weight);
    }

    int start = 0;
    int end = 0;
    input >> start >> end;

    //  Init graph
    Graph graph(vertices);
    for (int i = 0; i < edges; i++) {
        graph.AddEdge(froms.at(i), tos.at(i), weights.at(i));
    }

    //  Processing
    output << Djikstra(graph, start, end);
    return 0;
}

void testDjikstra() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "6 9 0 3 1 0 4 2 1 2 7 1 3 2 1 4 3 1 5 3 2 5 3 3 4 4 3 5 6 0 2";
        run(input, output);
        assert(output.str() == "9");
    }
    {
        //       1.
        //   1/  |  \4
        // 0.   1|    3.
        //   3\  |  /2
        //       2.

        std::stringstream input;
        std::stringstream output;
        input << "4 5 0 1 1 0 2 3 1 2 1 1 3 4 2 3 2 0 3";
        run(input, output);
        assert(output.str() == "4");
    }
    {
        //       1.
        //   1/  |  \4
        // 0.   6|    3.
        //   3\  |  /2
        //       2.

        std::stringstream input;
        std::stringstream output;
        input << "4 5 0 1 1 0 2 3 1 2 6 1 3 4 2 3 2 0 3";
        run(input, output);
        assert(output.str() == "5");
    }
    {
        //  0.       2.
        //   2\   1/ |
        //      1.   |4
        //   7/   2\ |
        //  4.       3.

        std::stringstream input;
        std::stringstream output;
        input << "5 5 0 1 2 1 2 1 1 3 2 1 4 7 2 3 4 0 3";
        run(input, output);
        assert(output.str() == "4");
    }
    {
        //  0.       2.
        //   2\   1/ |
        //      1.   |4
        //   7/   9\ |
        //  4.       3.

        std::stringstream input;
        std::stringstream output;
        input << "5 5 0 1 2 1 2 1 1 3 9 1 4 7 2 3 4 3 4";
        run(input, output);
        assert(output.str() == "12");
    }
    {
        //  All edges have "1" weight :)
        //
        //    1   3   5
        //   / \ / \ / \
        //  0   2   4   6
        //   \ / \ / \ /
        //    9   8   7

        std::stringstream input;
        std::stringstream output;
        input << "10 12 0 1 1 0 9 1 1 2 1 2 3 1 2 8 1 2 9 1 3 4 1 4 5 1 4 7 1 4 8 1 5 6 1 6 7 1 0 6";
        run(input, output);
        assert(output.str() == "6");
    }
    std::cout << "All tests ran successfully!";
}

int main() {
    //run(std::cin, std::cout);
    testDjikstra();
    std::cout << "All tests ran successfully!";
    return 0;
}
