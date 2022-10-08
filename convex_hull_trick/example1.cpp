// https://codeforces.com/contest/1715/problem/E Long Way Home

#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <functional>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace std;

#define int long long
#define DEBUG

int gcd(int a, int b) {
  if (b == 0) return a;
  return gcd(b, a % b);
}

const int MOD = 1e9 + 7;

int binpow(int a, int n) {
  int res = 1;
  while (n) {
    if (n & 1) {
      res *= a;
      res %= MOD;
    }
    a *= a;
    a %= MOD;
    n >>= 1;
  }
  return res;
}
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
    auto line = std::upper_bound(segments.begin(), segments.end(), x, [](long long x, const Segment& b) {
      return x < b.startX;
    });
    --line;
    return line->line;
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

const int inf = 1e10;

void solve() {
  int n, m, k;
  cin >> n >> m >> k;

  vector<vector<pair<int, int>>> g(n);
  for (int i = 0; i < m; i++) {
    int u, v, w;
    cin >> u >> v >> w;
    u--;
    v--;
    g[u].push_back({v, w});
    g[v].push_back({u, w});
  }
  vector<int> dist(n, inf);
  dist[0] = 0;

  while (k--) {
    set<pair<int, int>> dists;
    for(int i = 0; i < n; i++) {
      dists.insert({dist[i], i});
    }

    while (!dists.empty()) {
      auto best = *dists.begin();
      dists.erase(dists.begin());

      for (auto u: g[best.second]) {
        if (best.first + u.second < dist[u.first]) {
          dists.erase({dist[u.first], u.first});
          dist[u.first] = best.first + u.second;
          dists.insert({dist[u.first], u.first});
        }
      }
    }

    ConvexHullTrick cxt;
    cxt.addLine(0, 0);
    for (int i = 1; i < n; i++) {
      cxt.addLine(-2 * i, dist[i] + i * i);
    }
    for (int i = 0; i < n; i++) {
      int tmp = cxt.query(i);
      dist[i] = min(dist[i], tmp + i * i);
    }
  }

  set<pair<int, int>> dists;
  for (int i = 0; i < n; i++) {
    dists.insert({dist[i], i});
  }

  while (!dists.empty()) {
    auto best = *dists.begin();
    dists.erase(dists.begin());

    for (auto u: g[best.second]) {
      if (best.first + u.second < dist[u.first]) {
        dists.erase({dist[u.first], u.first});
        dist[u.first] = best.first + u.second;
        dists.insert({dist[u.first], u.first});
      }
    }
  }
  for (auto u: dist) {
    cout << u << ' ';
  }
}

int32_t main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  while (t--) {
    solve();
#ifdef DEBUG
    cout << flush;
#endif

  }

#ifdef DEBUG
  cerr << "\nTime: " << (double) clock() / CLOCKS_PER_SEC << endl;
#endif
  return 0;
}
