#include <vector>
#include <algorithm>
#include <limits.h>

struct ConvexHullTrick {
  struct Line {
    // line is in form of y = kx + b
    long long k, b;
    Line(long long k, long long b) : k(k), b(b) {}

    double intersect(Line& other) const {
      double db = other.b - b;
      double dk = k - other.k;
      return db / dk;
    }

    long long y(long long x) const {
      return k * x + b;
    }
  };
  struct Segment {
    double startX;
    Line line;

    Segment(double startX, Line line) : startX(startX), line(line) {}
  };
  std::vector<Segment> segments;

  void addLine(long long k, long long b) {
    addLine(Line(k, b));
  }

  void addLine(Line newLine) {
    removeWorseLines(newLine);
    addLineToTheEnd(newLine);
  }

  long long query(long long x) {
    return getBestLineAtPoint(x).y(x);
  }

 private:
  Line getBestLineAtPoint(long long x) {
    return (
        std::upper_bound(
            segments.begin(), segments.end(), x,
            [](long long x, const Segment& b) {
              return x < b.startX;
            })--
    )->line;
  }

  void removeWorseLines(Line newLine) {
    while (!segments.empty() && newLine.intersect(segments.back().line) <= segments.back().startX) {
      segments.pop_back();
    }
  }

  void addLineToTheEnd(Line newLine) {
    double startX = -1e15;
    if (!segments.empty()) {
      startX = newLine.intersect(segments.back().line);
    }

    segments.emplace_back(Segment(startX, newLine));
  }
};