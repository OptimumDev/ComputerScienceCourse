#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
using namespace std;

struct Point {
    double X;
    double Y;

    Point(double x, double y) {
        X = x;
        Y = y;
    }
};

vector<Point> create_polygon(int vertex_count) {
    double radius = vertex_count;

    random_device dev;
    mt19937 rng(dev());
    uniform_real_distribution<double> dist(0, 2 * M_PI);

    vector<double> angles;
    for (int i = 0; i < vertex_count; ++i) {
        angles.push_back(dist(rng));
    }
    sort(angles.begin(), angles.end());

    vector<Point> polygon;
    for (auto angle : angles) {
        double x = radius * cos(angle);
        double y = radius * sin(angle);
        polygon.emplace_back(x, y);
    }

    return polygon;
}

void print_polygon(const vector<Point>& polygon) {
    cout << "Polygon(";
    auto first = true;
    for (auto point : polygon) {
        if (!first) {
            cout << ", ";
        }
        first = false;
        cout << "(" << point.X << ", " << point.Y << ")";
    }
    cout << ")" << endl;
}

void count_area(const vector<Point>& polygon, int thread_count) {
    omp_set_num_threads(thread_count);
    double start = omp_get_wtime();
    double sum = 0;
    int n = polygon.size();
#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; ++i) {
        auto current_point = polygon[i];
        auto next_point = polygon[(i + 1) % n];
        sum += current_point.X * next_point.Y - next_point.X * current_point.Y;
    }
    sum /= 2;
    double stop = omp_get_wtime();
    cout << "| " << thread_count << " | " << fixed << stop - start << " | " << sum << " |" << endl;
}

int main() {
    int n;
    cout << "Enter polygon's vertex count: ";
    cin >> n;
    auto polygon = create_polygon(n);

    if (n < 1000) {
        print_polygon(polygon);
    }

    cout << "| Thread count | Time (s) | Area |" << endl;
    cout << "|---|---|---|" << endl;

    for (int i = 1; i <= 16; i++) {
        count_area(polygon, i);
    }

    return 0;
}
