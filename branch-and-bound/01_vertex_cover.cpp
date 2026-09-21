#include <cstdint>
#include <iostream>
#include <vector>

using Mask = std::uint64_t;

namespace {

std::vector<Mask> adjacency;

int firstVertex(Mask mask) {
    return __builtin_ctzll(mask);
}

// Максимальное по включению паросочетание даёт нижнюю оценку размера вершинного
// покрытия: его рёбра не имеют общих концов, поэтому для каждого из них нужна
// отдельная выбранная вершина.
int matchingLowerBound(Mask alive) {
    int matchingSize = 0;
    Mask unused = alive;

    while (unused != 0) {
        const int vertex = firstVertex(unused);
        unused &= ~(Mask{1} << vertex);

        const Mask neighbors = adjacency[vertex] & unused;
        if (neighbors == 0) {
            continue;
        }

        const int neighbor = firstVertex(neighbors);
        unused &= ~(Mask{1} << neighbor);
        ++matchingSize;
    }

    return matchingSize;
}

bool hasVertexCover(Mask alive, int remaining) {
    if (remaining < 0) {
        return false;
    }

    int branchVertex = -1;
    int bestDegree = 0;
    Mask candidates = alive;

    // Для ветвления подходит любое непокрытое ребро. Выбор его конца с
    // максимальной текущей степенью — лишь эвристика, часто сокращающая перебор.
    while (candidates != 0) {
        const int vertex = firstVertex(candidates);
        candidates &= candidates - 1;

        const int degree = __builtin_popcountll(adjacency[vertex] & alive);
        if (degree > bestDegree) {
            bestDegree = degree;
            branchVertex = vertex;
        }
    }

    if (branchVertex == -1) {
        return true;  // Непокрытых рёбер не осталось.
    }
    if (remaining == 0 || matchingLowerBound(alive) > remaining) {
        return false;
    }

    const Mask liveNeighbors = adjacency[branchVertex] & alive;
    const int neighbor = firstVertex(liveNeighbors);
    const Mask branchBit = Mask{1} << branchVertex;
    const Mask neighborBit = Mask{1} << neighbor;

    // Для непокрытого ребра (branchVertex, neighbor) в покрытие должен входить
    // хотя бы один конец. Удаление выбранной вершины покрывает все её рёбра.
    return hasVertexCover(alive & ~branchBit, remaining - 1) ||
           hasVertexCover(alive & ~neighborBit, remaining - 1);
}

}  // пространство имён

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int vertexCount;
    int edgeCount;
    int coverLimit;
    if (!(std::cin >> vertexCount >> edgeCount >> coverLimit)) {
        return 0;
    }

    adjacency.assign(vertexCount, 0);
    for (int i = 0; i < edgeCount; ++i) {
        int from;
        int to;
        std::cin >> from >> to;
        --from;
        --to;

        // Кратные рёбра лишь повторно устанавливают те же биты.
        adjacency[from] |= Mask{1} << to;
        adjacency[to] |= Mask{1} << from;
    }

    const Mask allVertices = vertexCount == 0
                                 ? 0
                                 : (Mask{1} << vertexCount) - 1;
    std::cout << (hasVertexCover(allVertices, coverLimit) ? "YES\n" : "NO\n");
}
