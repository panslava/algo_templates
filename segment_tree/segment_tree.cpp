#include <vector>

template<typename T>
struct segment_tree {
  explicit segment_tree(int input_size, T null_value)
      : null_value(null_value),
        leaves_size(next_power_2(input_size)),
        nodes(2 * leaves_size, null_value) {}

  explicit segment_tree(const std::vector<T>& input, T null_value) : segment_tree(
      input.size(),
      null_value) {
    for (int i = 0; i < input.size(); i++) {
      set_single(i, input[i]);
    }
  }

  // set_single sets value at 0-indexed position
  void set_single(int position, T value) {
    set_single(1, 0, leaves_size - 1, position, value);
  }

  // range_query returns query result on 0-indexed inclusive range [l, r]
  T range_query(int l, int r) {
    return range_query(1, l, r, 0, leaves_size - 1);
  }

 private:
  int leaves_size;

  std::vector<T> nodes;

  // null_value merged with some non-null value should always be non-null value
  // (example: if merge is max, good null_value = INT_MIN)
  T null_value;

  // merge calculates value in upper node, based on calculated values on 2 down nodes
  T merge(T firstValue, T secondValue) {
    return min(firstValue, secondValue);
  }

  void set_single(int v, int tl, int tr, int position, T value) {
    if (tl > position || tr < position) {
      return;
    }
    if (tl == tr && tl == position) {
      nodes[v] = value;
      return;
    }

    int middle = (tl + tr) / 2;
    set_single(v * 2, tl, middle, position, value);
    set_single(v * 2 + 1, middle + 1, tr, position, value);
    nodes[v] = merge(nodes[v * 2], nodes[v * 2 + 1]);
  }

  T range_query(int v, int l, int r, int tl, int tr) {
    if (l > r) {
      return null_value;
    }

    if (l == tl && r == tr) {
      return nodes[v];
    }

    int middle = (tl + tr) / 2;
    return merge(
        range_query(v * 2, l, std::min(r, middle), tl, middle),
        range_query(v * 2 + 1, std::max(l, middle + 1), r, middle + 1, tr)
    );
  }

  int next_power_2(int n) {
    while (__builtin_popcount(n) != 1) n++;
    return n;
  }
};