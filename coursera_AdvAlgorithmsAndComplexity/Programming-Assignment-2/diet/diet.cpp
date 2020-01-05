#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

const double EPS = 1e-3;
const int PRECISION = 20;

typedef vector<double> Column;
typedef vector<double> Row;
typedef vector<Row> Matrix;

struct Equation {
  Equation(const Matrix &a, const Column &b)
    : a(a), b(b) {}

  Matrix a;
  Column b;
};

struct Position {
  Position(int column, int row):
    column(column), row(row) {}

  int column;
  int row;
};


void choose_helper(int i, int n, int k, vector<int> &tmp,
    vector<bool> &visited, vector<vector<int>> &result) {
  visited[i] = true;
  tmp.push_back(i);

  if (tmp.size() == k) {
    result.push_back(tmp);
  } else {
    for (int j = i+1; j < n; ++j) {
      if (!visited[j])
        choose_helper(j, n, k, tmp, visited, result);
    }
  }
  visited[i] = false;
  tmp.pop_back();
  return;
}

vector<vector<int>> choose_k_from_n(int n, int k) {
  vector<vector<int>> result;
  vector<int> tmp;
  vector<bool> visited(n, false);

  for(int i = 0; i <= n - k; ++i) {
    choose_helper(i, n, k, tmp, visited, result);
  }
  return result;
}

Equation construct(vector<int> &eq_ids, Matrix &A, Column &b, int m, int n) {
  // m: # of variables
  // n: # of input inequalities
  Matrix new_A;
  Column new_b;
  for(auto id: eq_ids) {
    if (id < n) {
      new_A.push_back(A[id]);
      new_b.push_back(b[id]);
    } else if (n <= id && id < m + n) {
      Row tmp(m, 0.0);
      tmp[id - n] = 1;
      new_A.push_back(tmp);
      new_b.push_back(0);
    } else if (id == m + n) {
      Row tmp(m, 1.0);
      new_A.push_back(tmp);
      new_b.push_back(1000000000);
    }
  }
  return Equation(new_A, new_b);
}

double dot_product(int m,
    const vector<double> &c,
    const vector<double> &res) {
  double target = 0.0;
  for (int i = 0; i < m; ++i)
    target += c[i]*res[i];

  return target;
}

Position SelectPivotElement(
  const Matrix &a,
  std::vector <bool> &used_rows,
  std::vector <bool> &used_columns) {
    // This algorithm selects the first free element.
    // You'll need to improve it to pass the problem.
    Position pivot_element(0, 0);
    while (used_rows[pivot_element.row])
        ++pivot_element.row;
    while (used_columns[pivot_element.column]
           || (a[pivot_element.row][pivot_element.column] < EPS
             && a[pivot_element.row][pivot_element.column] >-EPS )
        )
        ++pivot_element.column;
    return pivot_element;
}

void SwapLines(Matrix &a, Column &b, std::vector <bool> &used_rows, Position &pivot_element) {
    std::swap(a[pivot_element.column], a[pivot_element.row]);
    std::swap(b[pivot_element.column], b[pivot_element.row]);
    std::swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
    pivot_element.row = pivot_element.column;
}

void ProcessPivotElement(Matrix &a, Column &b, const Position &pivot_element) {
    // Write your code here
    int n = b.size(), m = a[pivot_element.row].size();

    // rescale to make pivot 1
    double coefficient = a[pivot_element.row][pivot_element.column];
    for (int i = 0; i < m; ++i)
        a[pivot_element.row][i] /= coefficient;
    b[pivot_element.row] /= coefficient;

    // subtract pivot row from other rows
    for (int i = 0; i < n; ++i) {
        if (i == pivot_element.row) continue;
        double scale = a[i][pivot_element.column] / a[pivot_element.row][pivot_element.column];
        for (int j = 0; j < m; ++j)
            a[i][j] = a[i][j] - scale * a[pivot_element.row][j];
        b[i] = b[i] - scale * b[pivot_element.row];
    }
}

void MarkPivotElementUsed(const Position &pivot_element, std::vector <bool> &used_rows, std::vector <bool> &used_columns) {
    used_rows[pivot_element.row] = true;
    used_columns[pivot_element.column] = true;
}

pair<bool, vector<double>> SolveEquation(Equation equation) {
    Matrix &a = equation.a;
    Column &b = equation.b;
    int size = a.size();
    bool has_solution = false;
    std::vector <bool> used_columns(size, false);
    std::vector <bool> used_rows(size, false);
    for (int step = 0; step < size; ++step) {
        Position pivot_element = SelectPivotElement(a, used_rows, used_columns);
        if (pivot_element.row >= size || pivot_element.column >= size) {
          has_solution = false;
          break;
        }
        SwapLines(a, b, used_rows, pivot_element);
        ProcessPivotElement(a, b, pivot_element);
        MarkPivotElementUsed(pivot_element, used_rows, used_columns);
        has_solution = true;
    }
    return {has_solution, b};
}

bool satisfy_all_inequality(
    const Column &solution,
    const Matrix &A,
    const Column &b,
    const int m,
    const int n) {

  for (int i = 0; i < m; ++i) {
    if (solution[i] < -EPS)
      return false;
  }

  for (int i = 0; i < n; ++i) {
    if (dot_product(m, A[i], solution) > b[i])
     return false;
  }
  return true;
}


pair<int, vector<double>> solve_diet_problem(
    int n,
    int m,
    Matrix &A,
    vector<double> &b,
    vector<double> &c) {
  // Write your code here
  Column result(m, 0.0);
  bool use_last_equation = false;
  bool has_solution = false;
  double target, max_target = -10e9;
  vector<double> solution;
  vector<vector<int>> all_equations = choose_k_from_n(m+n+1, m);
  for (auto eq_ids: all_equations) {
    Equation equation = construct(eq_ids, A, b, m, n);
    pair<bool, vector<double>> ans = SolveEquation(equation);
    solution = ans.second;
    if (!ans.first)
      continue;
    if (satisfy_all_inequality(solution, A, b, m, n)) {
      has_solution = true;
      target = dot_product(m, c, solution);

      if (target > max_target) {
        max_target = target; result = solution;
        if (find(eq_ids.begin(), eq_ids.end(), m+n) != eq_ids.end())
          use_last_equation = true;
       }
    }
  }
  int flag = -100;
  if (!has_solution)
    flag = -1;
  else if (use_last_equation)
    flag = 1;
  else
    flag = 0;
  return {flag, result};
}

int main(){
  int n=0, m=0;
  cin >> n >> m;
  Matrix A(n, vector<double>(m));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> A[i][j];
    }
  }

  Column b(n);
  for (int i = 0; i < n; i++) {
    cin >> b[i];
  }

  Column c(m);
  for (int i = 0; i < m; i++) {
    cin >> c[i];
  }

  pair<int, vector<double>> ans = solve_diet_problem(n, m, A, b, c);

  switch (ans.first) {
    case -1:
      cout << "No solution" << endl;
      break;
    case 0:
      printf("Bounded solution\n");
      for (int i = 0; i < m; i++) {
        printf("%.18f%c", ans.second[i], " \n"[i + 1 == m]);
      }
      break;
    case 1:
      cout << "Infinity" << endl;
      break;
  }
  return 0;
}
