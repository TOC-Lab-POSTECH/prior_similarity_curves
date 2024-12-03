#include "free_space.h"

using namespace std;

// Constructor: initialize with two curves and epsilon
FreeSpace::FreeSpace(const PolygonalCurve& P, const PolygonalCurve& Q,
                     double epsilon)
    : P(P), Q(Q), epsilon(epsilon) {
  computeFreeSpace();
}

// Destructor
FreeSpace::~FreeSpace() {}

// Getter for curve P
const PolygonalCurve& FreeSpace::getCurveP() const { return P; }

// Getter for curve Q
const PolygonalCurve& FreeSpace::getCurveQ() const { return Q; }

// Getter for epsilon
double FreeSpace::getEpsilon() const { return epsilon; }

// Getter for L results
const PointPairVector& FreeSpace::getL() const { return L; }

// Getter for B results
const PointPairVector& FreeSpace::getB() const { return B; }

// Setter for epsilon
void FreeSpace::setEpsilon(double newEpsilon) {
  epsilon = newEpsilon;
  computeFreeSpace();  // Recompute L and B with the new epsilon value
}

// Function to compute L and B
void FreeSpace::computeFreeSpace() {
  L.clear();
  B.clear();
  processCurveForL();
  processCurveForB();
}

void FreeSpace::processCurveForL() {
  int p = P.numPoints();
  int q = Q.numPoints();

  for (int i = 0; i < p - 1; ++i) {
    Point_2 startP = P.getPoint(i);
    Point_2 endP = P.getPoint(i + 1);

    for (int j = 0; j < q; ++j) {
      Point_2 pointQ = Q.getPoint(j);

      auto result = checkPointsOnEdge(startP, endP, pointQ);

      if (result.first == 2) {
        float k = result.second[0];
        float l = result.second[1];
        L.push_back({Point_2(j, i + k), Point_2(j, i + l)});
      } else if (result.first == 1) {
        float k = result.second[0];
        L.push_back({Point_2(j, i + k), Point_2(j, i + k)});
      } else {
        L.push_back({Point_2(-1, -1), Point_2(-1, -1)});
      }
    }
  }
}

void FreeSpace::processCurveForB() {
  int p = P.numPoints();
  int q = Q.numPoints();

  for (int i = 0; i < q - 1; ++i) {
    Point_2 startQ = Q.getPoint(i);
    Point_2 endQ = Q.getPoint(i + 1);

    for (int j = 0; j < p; ++j) {
      Point_2 pointP = P.getPoint(j);

      auto result = checkPointsOnEdge(startQ, endQ, pointP);

      if (result.first == 2) {
        // Two points, add two pairs
        float k = result.second[0];
        float l = result.second[1];
        B.push_back({Point_2(i + k, j), Point_2(i + l, j)});
      } else if (result.first == 1) {
        // One point, add one pair
        float k = result.second[0];
        B.push_back({Point_2(i + k, j), Point_2(i + k, j)});
      } else {
        // No points, add (-1, -1) pairs
        B.push_back({Point_2(-1, -1), Point_2(-1, -1)});
      }
    }
  }
}

std::pair<int, std::vector<float>> FreeSpace::checkPointsOnEdge(
    const Point_2& start, const Point_2& end, const Point_2& point) const {
  std::vector<float> portions;

  // (x1, y1) is start, (x2, y2) is end, and (px, py) is the point
  float x1 = start.x(), y1 = start.y();
  float x2 = end.x(), y2 = end.y();
  float px = point.x(), py = point.y();

  // Compute the square of the Euclidean distance
  float dx = x2 - x1;
  float dy = y2 - y1;
  float len2 = dx * dx + dy * dy;

  if (len2 == 0) return {0, portions};  // Edge is degenerate (start == end)

  // Project point onto the line
  float t = ((px - x1) * dx + (py - y1) * dy) / len2;
  float nearestX = x1 + t * dx;
  float nearestY = y1 + t * dy;

  // Compute the distance from the point to the nearest point on the line
  float dist2 =
      (px - nearestX) * (px - nearestX) + (py - nearestY) * (py - nearestY);
  float eps2 = epsilon * epsilon;

  // Check if the projected point is on the edge
  if (std::fabs(dist2 - eps2) < 1e-6) {
    if (t >= 0 && t <= 1) {
      portions.push_back(t);
      return {1, portions};
    } else {
      return {0, portions};
    }
  } else if (dist2 < eps2) {
    float d = std::sqrt(eps2 - dist2);
    float t1 = t - d / std::sqrt(len2);
    float t2 = t + d / std::sqrt(len2);

    // Handle different cases based on t1 and t2 values
    if ((t1 < 0 && t2 < 0) || (t1 > 1 && t2 > 1)) {
      // Both points are outside the edge
      return {0, portions};
    } else if (0 <= t1 && t1 <= 1 && t2 > 1) {
      // One point is on the edge, t2 is beyond the edge
      portions.push_back(t1);
      portions.push_back(1.0f);  // t2 reaches the end of the edge
      return {2, portions};
    } else if (t1 < 0 && t2 > 1) {
      // Both t1 and t2 are beyond the edge bounds
      portions.push_back(0.0f);  // t1 starts from the beginning of the edge
      portions.push_back(1.0f);  // t2 reaches the end of the edge
      return {2, portions};
    } else if (t1 < 0 && 0 <= t2 && t2 <= 1) {
      // t1 is before the edge, but t2 is within the edge
      portions.push_back(0.0f);  // t1 starts from the beginning of the edge
      portions.push_back(t2);    // t2 is valid within the edge
      return {2, portions};
    } else {
      // Both points are on the edge
      portions.push_back(t1);
      portions.push_back(t2);
      return {2, portions};
    }
  } else {
    // No points at epsilon distance on the edge
    return {0, portions};
  }
}
