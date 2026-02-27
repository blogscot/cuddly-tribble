#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <ranges>
#include <string>
#include <vector>

using namespace std;

vector<int> parse_numbers(string data) {
    return data | views::split(',') |
           views::transform([](auto&& s) { return stoi(std::string(begin(s), end(s))); }) |
           std::ranges::to<std::vector<int>>();
}

struct Entry {
    int lights{};
    int presses{};
};

struct Machine {
    Machine() = delete;
    Machine(string data) {
        vector<std::string> parts;
        auto start = data.begin();
        auto it = find(start, end(data), ' ');

        while (it != end(data)) {
            parts.push_back(string(start, it));
            start = it + 1;
            it = find(start, end(data), ' ');
        }
        parts.push_back(string(start, end(data)));

        string last = parts.back();
        parts.pop_back();
        joltages = parse_numbers(last.substr(1, last.size() - 2));

        string front = parts.front();
        parts.erase(parts.begin());
        auto lights = front.substr(1, front.size() - 2);
        goal = ranges::fold_left(string(lights.rbegin(), lights.rend()), 0,
                                 [](int acc, char c) { return c == '#' ? acc * 2 + 1 : acc * 2; });

        buttons =
            parts |
            views::transform([](auto&& s) { return parse_numbers(s.substr(1, s.size() - 2)); }) |
            std::ranges::to<std::vector<vector<int>>>();
    }

    void press(Entry& entry, vector<int> buttons) {
        for (auto& button : buttons) {
            entry.lights ^= 1 << button;
        }
        entry.presses++;
    }

    int find_fewest_presses() {
        queue<Entry> q;
        q.push({0, 0});
        vector<int> seen;

        while (!q.empty()) {
            auto entry = q.front();
            q.pop();

            if (entry.lights == goal) {
                return entry.presses;
            }

            if (find(seen.begin(), seen.end(), entry.lights) != seen.end()) {
                continue;
            }
            seen.push_back(entry.lights);

            for (auto& button : buttons) {
                auto new_entry = entry;
                press(new_entry, button);
                q.push(new_entry);
            }
        }
        throw std::runtime_error("no solution");
    }

    int goal;
    vector<int> joltages;
    vector<vector<int>> buttons;
};

int part1(vector<Machine> const& machines) {
    return accumulate(machines.begin(), machines.end(), 0,
                      [](int acc, Machine machine) { return acc + machine.find_fewest_presses(); });
}
int main() {
    std::ifstream file("puzzle.txt");

    std::vector<Machine> machines;
    std::string line;
    while (std::getline(file, line)) {
        machines.push_back(Machine(line));
    }

    try {
        println("{}", part1(machines));
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
