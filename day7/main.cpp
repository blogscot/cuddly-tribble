#include <fstream>
#include <iostream>
#include <numeric>
#include <print>
#include <string>
#include <vector>

using namespace std;

int part1(const vector<string>& lines) {
    size_t width = lines.front().size();
    size_t midpoint = width / 2;
    vector<bool> visited(width, false);
    visited[midpoint] = true;

    int num_splits{0};
    for (auto& line : lines) {
        for (size_t i = 0; i < width; ++i) {
            if (visited[i] && line[i] == '^') {
                visited[i - 1] = true;
                visited[i + 1] = true;
                visited[i] = false;
                ++num_splits;
            }
        }
    }
    return num_splits;
}

uint64_t part2(const vector<string>& lines) {
    size_t width = lines.front().size();
    size_t midpoint = width / 2;
    vector<uint64_t> visited(width, 0);
    visited[midpoint] = 1;

    for (auto& line : lines) {
        for (size_t i = 0; i < width; ++i) {
            if (visited[i] > 0 && line[i] == '^') {
                visited[i - 1] += visited[i];
                visited[i + 1] += visited[i];
                visited[i] = 0;
            }
        }
    }
    return accumulate(visited.begin(), visited.end(), 0ull);
}

int main() {
    ifstream file("puzzle.txt");
    if (!file) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }
    string line;
    vector<string> lines;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    println("Part 1: {}", part1(lines));
    println("Part 2: {}", part2(lines));
    return 0;
}
