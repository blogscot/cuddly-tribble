#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <unordered_set>
#include <vector>

using namespace std;

struct Point {
    uint64_t x;
    uint64_t y;

    Point(uint64_t x, uint64_t y) : x(x), y(y) {}

    uint64_t area(Point const& point) const {
        return (abs(static_cast<int64_t>(y - point.y)) + 1) *
               (abs(static_cast<int64_t>(x - point.x)) + 1);
    }
};

vector<pair<Point, Point>> combinations(vector<Point> const& points) {
    vector<pair<Point, Point>> result;
    for (int i = 0; i < points.size() - 1; i++) {
        for (int j = i + 1; j < points.size(); j++) {
            result.emplace_back(points[i], points[j]);
        }
    }
    return result;
}

enum class Axis { Vertical, Horizontal };

struct Edge {
    uint64_t along;
    uint64_t start;
    uint64_t end;

    bool operator==(Edge const& other) const {
        return along == other.along && start == other.start && end == other.end;
    }
};

class MyHashFunction {
   public:
    size_t operator()(const Edge& e) const { return (e.along * 100) ^ (e.start * 10) ^ e.end; }
};

using MySet = unordered_set<Edge, MyHashFunction>;

MySet find_edges(vector<Point> const& points, Axis axis) {
    return combinations(points) | views::filter([&axis](pair<Point, Point> const& pair) {
               auto [p1, p2] = pair;
               if (axis == Axis::Vertical) {
                   return p1.x == p2.x;
               } else {
                   return p1.y == p2.y;
               }
           }) |
           views::transform([&axis](pair<Point, Point> const& pair) {
               auto [p1, p2] = pair;
               if (axis == Axis::Vertical) {
                   return Edge{p1.x, min(p1.y, p2.y), max(p1.y, p2.y)};
               } else {
                   return Edge{p1.y, min(p1.x, p2.x), max(p1.x, p2.x)};
               }
           }) |
           ranges::to<MySet>();
}

struct Part2 {
    vector<Point> points;
    MySet vertical_edges;
    MySet horizontal_edges;

    Part2(vector<Point> const& points)
        : points(std::move(points)),
          vertical_edges(find_edges(points, Axis::Vertical)),
          horizontal_edges(find_edges(points, Axis::Horizontal)) {}

    bool area_contains_no_edges(Point const& p1, Point const& p2) {
        auto [min_x, max_x] = minmax(p1.x, p2.x);
        auto [min_y, max_y] = minmax(p1.y, p2.y);
        for (auto const& edge : vertical_edges) {
            if (min_x < edge.along && edge.along < max_x && edge.end > min_y &&
                edge.start < max_y) {
                return false;
            }
        }
        for (auto const& edge : horizontal_edges) {
            if (min_y < edge.along && edge.along < max_y && edge.end > min_x &&
                edge.start < max_x) {
                return false;
            }
        }
        return true;
    }

    uint64_t solve() {
        auto areas = combinations(points) | views::filter([&](pair<Point, Point> const& pair) {
                         auto [p1, p2] = pair;
                         return area_contains_no_edges(p1, p2);
                     }) |
                     views::transform([&](pair<Point, Point> const& points) {
                         auto [p1, p2] = points;
                         return p1.area(p2);
                     }) |
                     ranges::to<vector<uint64_t>>();
        return ranges::fold_left(
            areas, 0ull, [](uint64_t max_area, uint64_t area) { return max(max_area, area); });
    }
};

uint64_t part1(vector<Point> const& points) {
    auto combos = combinations(points);
    uint64_t max_area = transform_reduce(
        combos.begin(), combos.end(), 0ull,
        [](uint64_t max_area, uint64_t area) { return max(max_area, area); },
        [&](pair<Point, Point> const& combo) {
            auto const& [p1, p2] = combo;
            return p1.area(p2);
        });

    return max_area;
}

int main() {
    ifstream file("puzzle.txt");

    vector<Point> points;
    string line;
    while (getline(file, line)) {
        size_t pos = line.find(",");
        int x = stoi(line.substr(0, pos));
        int y = stoi(line.substr(pos + 1));
        points.push_back(Point(x, y));
    }

    println("{}", part1(points));
    Part2 part2(points);
    println("{}", part2.solve());
    return 0;
}
