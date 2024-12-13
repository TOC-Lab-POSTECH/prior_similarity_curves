#include "ged.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <limits>
#include <random>

using namespace std;

namespace GED {

// Computes O(n^(1/2))-approximation of GED
double computeSquareRootApproxGED(const PolygonalCurve& P,
                                  const PolygonalCurve& Q) {
  size_t n = min(P.numPoints(), Q.numPoints());

  // Step 1: Check the sum of distances between corresponding points
  double totalDistance = 0.0;
  for (size_t i = 0; i < n; ++i) {
    double dx = P.getPoint(i).x() - Q.getPoint(i).x();
    double dy = P.getPoint(i).y() - Q.getPoint(i).y();
    totalDistance += sqrt(dx * dx + dy * dy);
  }

  // If sum of distances is less or equal to 1
  if (totalDistance <= 1.0) {
    // Construct a matching [(0, 0), (1, 1), ..., (n-1, n-1)]
    Matching approximationMatching;
    for (size_t i = 0; i < n; ++i) {
      approximationMatching.emplace_back(i, i);
    }
    return computeCost(P, Q, approximationMatching);
  }

  // Step 2: Approximation
  for (int i = 0; i <= ceil(log2(n)); ++i) {
    int g = static_cast<int>(pow(2, i));
    int maxJ = static_cast<int>(ceil(9.0 * log(n)));  // Assuming c=9

    for (int j = 0; j <= maxJ; ++j) {
      // Transform curves into strings
      CurveStringPair transformedStrings = transformCurvesToStrings(P, Q, g);

      // Compute String Edit Distance (SED)
      Matching approximationMatching =
          SED(transformedStrings.first, transformedStrings.second,
              12 * sqrt(n) + 2 * g);

      // If a matching is found, return the cost(which is
      // O(n^(1/2))-approximation of GED)
      if (!approximationMatching.empty()) {
        return computeCost(P, Q, approximationMatching);
      }
    }
  }

  // Step 3: Return cost for empty matching if no matching found during the
  // iteration
  Matching emptyMatching;
  return computeCost(P, Q, emptyMatching);
}

// Computes the GED cost for given matching for two polygonal curves P, Q
double computeCost(const PolygonalCurve& P, const PolygonalCurve& Q,
                   const Matching& matching) {
  double cost = 0.0;

  // Step 1: Compute the sum of distances for the matching
  for (const auto& match : matching) {
    int i = match.first;
    int j = match.second;

    double dx = P.getPoint(i).x() - Q.getPoint(j).x();
    double dy = P.getPoint(i).y() - Q.getPoint(j).y();

    cost += sqrt(dx * dx + dy * dy);
  }

  // Step 2: Add the gap penalty for unmatched points
  size_t matchingLength = matching.size();
  cost += (P.numPoints() - matchingLength) + (Q.numPoints() - matchingLength);

  return cost;
}

// Transfroms the curves into string by randomly shifted grid
CurveStringPair transformCurvesToStrings(const PolygonalCurve& P,
                                         const PolygonalCurve& Q, int g) {
  size_t n = min(P.numPoints(), Q.numPoints());

  // Step 1: Calculate delta
  double delta = g / sqrt(n);

  // Step 2: Pick random values x_o, y_o in [0, delta]
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(0.0, delta);

  double x_o = dis(gen);
  double y_o = dis(gen);

  // Step 3: Copy P and Q
  PolygonalCurve convertedP = P;
  PolygonalCurve convertedQ = Q;

  // Step 4: Set the new origin for both curves
  convertedP.shiftOrigin(Point_2(x_o, y_o));
  convertedQ.shiftOrigin(Point_2(x_o, y_o));

  // Step 5: Scale the grid with delta as the scaling factor
  convertedP.scaleGrid(delta);
  convertedQ.scaleGrid(delta);

  // Step 6: Floor the coordinates of the points
  convertedP.floorCoordinates();
  convertedQ.floorCoordinates();

  // Step 7: Set CurveStrings with converted curves
  CurveString stringP, stringQ;
  for (size_t i = 0; i < convertedP.numPoints(); ++i) {
    stringP.emplace_back(static_cast<int>(convertedP.getPoint(i).x()),
                         static_cast<int>(convertedP.getPoint(i).y()));
  }
  for (size_t i = 0; i < convertedQ.numPoints(); ++i) {
    stringQ.emplace_back(static_cast<int>(convertedQ.getPoint(i).x()),
                         static_cast<int>(convertedQ.getPoint(i).y()));
  }

  // Step 8: Return the CurveStringPair
  return {stringP, stringQ};
}

// Computes the String Edit Distance (SED) for GED
Matching SED(const CurveString& S, const CurveString& T, double threshold) {
  size_t n = S.size();
  size_t m = T.size();
  int k = static_cast<int>(floor(threshold));

  // Step 1: Initialize the (n+1) by (n+1) DP table D
  vector<vector<int>> D(n + 1, vector<int>(m + 1, -1));
  for (size_t i = 0; i <= n; ++i) {
    D[i][0] = static_cast<int>(i);
  }
  for (size_t j = 0; j <= m; ++j) {
    D[0][j] = static_cast<int>(j);
  }

  // Step 2: Initialize the vector L(that saves L values for each h and e) with
  // length 2k+3
  vector<int> L(2 * (k + 1) + 1, -1);

  // Step 3: Dynamic programming loop
  for (int e = 0; e <= k; ++e) {
    // Set initial values for L_h,e of diagonal -(e+1), (e+1)
    L[(k + 1) - (e + 1)] = e;   // h < 0: L_h,|h|-2 to |h|-1
    L[(k + 1) + (e + 1)] = -1;  // h >= 0: L_h,|h|-2 to -1

    for (int h = -e; h <= e; ++h) {
      // Algorithm 3(from referenced paper)
      if (abs(h) % 2 != e % 2)
        continue;  // L_h,e is well-defined only if (h mod 2) == (e mod 2)

      // Check if substitution values are infinity
      if (L[(k + 1) + h - 1] == numeric_limits<int>::max() ||
          L[(k + 1) + h + 1] == numeric_limits<int>::max()) {
        L[(k + 1) + h] =
            numeric_limits<int>::max();  // Set L[k + h] to infinity
        continue;
      }

      // Compute r based on L
      int r = max(L[(k + 1) + h - 1], L[(k + 1) + h + 1] + 1);

      // Slide
      if (r >= 0 && r <= n && r + h >= 0 && r + h <= m) D[r][r + h] = e;
      while (r + 1 > 0 && r + 1 <= n && r + h + 1 > 0 && r + h + 1 <= m &&
             (S[(r + 1) - 1] == T[(r + h + 1) - 1])) {
        D[r + 1][r + h + 1] = e;
        ++r;
      }

      // Set L_h,e
      if (r > static_cast<int>(n) || r + h > static_cast<int>(m)) {
        L[(k + 1) + h] = numeric_limits<int>::max();  // Set to "infinity"
      } else {
        L[(k + 1) + h] = r;
      }
    }
  }

  // Step 4: Check if the edit distance is within the threshold
  if (D[n][m] == -1) {
    return {};  // Return empty matching
  }

  // Step 5: Backtrace the DP table and return matching
  return backtrace(D);
}

// Backtrace the DP table and return matching
Matching backtrace(const vector<vector<int>>& D) {
  size_t n = D.size() - 1;
  size_t m = D[0].size() - 1;
  size_t x = n, y = m;

  Matching M;  // Initialize an empty matching

  while (x > 0 || y > 0) {
    if (x > 0 && y > 0 && D[x][y] == D[x - 1][y - 1]) {
      // Diagonal move: match (x, y)
      M.emplace_back(x - 1, y - 1);
      --x;
      --y;
    } else if (y > 0 && D[x][y] == D[x][y - 1] + 1) {
      // Left move: skip a point in T
      --y;
    } else if (x > 0 && D[x][y] == D[x - 1][y] + 1) {
      // Up move: skip a point in S
      --x;
    }
  }

  return M;
}

}  // namespace GED
