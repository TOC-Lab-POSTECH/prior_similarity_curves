#include "decision_problem.h"

// Constructor to initialize with two curves and epsilon
DecisionProblem::DecisionProblem(const PolygonalCurve& P,
                                 const PolygonalCurve& Q, double epsilon)
    : P(P),
      Q(Q),
      epsilon(epsilon),
      freeSpace(P, Q, epsilon),
      monotoneCurveExists(false) {
  checkMonotoneCurve();
}

// Getter for the result
bool DecisionProblem::doesMonotoneCurveExist() const {
  return monotoneCurveExists;
}

// Getter for polygonal curve P
const PolygonalCurve& DecisionProblem::getCurveP() const { return P; }

// Getter for polygonal curve Q
const PolygonalCurve& DecisionProblem::getCurveQ() const { return Q; }

// Getter for epsilon
double DecisionProblem::getEpsilon() const { return epsilon; }

// Setter for epsilon and recompute monotone curve existence
void DecisionProblem::setEpsilon(double newEpsilon) {
  epsilon = newEpsilon;
  freeSpace.setEpsilon(newEpsilon);  // Update FreeSpace with new epsilon
  checkMonotoneCurve();              // Recheck if a monotone curve exists
}

// Function to check if there is a monotone curve
void DecisionProblem::checkMonotoneCurve() {
  // Step 1: Check start and end conditions
  if (!checkStartAndEndConditions()) {
    monotoneCurveExists = false;
    return;
  }

  // Step 2: Check if a monotone curve exists
  monotoneCurveExists = checkIfMonotoneCurveExists();
}

// Helper function to check if (0,0) exists in the first element and (q-1, p-1)
// in the last element
bool DecisionProblem::checkStartAndEndConditions() {
  const PointPairVector& L = freeSpace.getL();
  const PointPairVector& B = freeSpace.getB();

  // Check for (0,0) in the first elements of L or B
  Point_2 startPoint(0, 0);
  bool startCondition =
      (L[0].first == startPoint || L[0].second == startPoint ||
       B[0].first == startPoint || B[0].second == startPoint);

  int p = P.numPoints();
  int q = Q.numPoints();
  Point_2 endPoint(q - 1, p - 1);

  // Check for (q-1, p-1) in the last elements of L or B
  bool endCondition =
      (L.back().first == endPoint || L.back().second == endPoint ||
       B.back().first == endPoint || B.back().second == endPoint);

  // If both conditions are satisfied, we can proceed
  return startCondition && endCondition;
}

// Function to check if a monotone curve exists
bool DecisionProblem::checkIfMonotoneCurveExists() {
  int p = P.numPoints();
  int q = Q.numPoints();

  // Step 1: Initialize L_R and B_R
  PointPairVector L_R((p - 1) * q, {Point_2(-1, -1), Point_2(-1, -1)});
  PointPairVector B_R((q - 1) * p, {Point_2(-1, -1), Point_2(-1, -1)});

  // Set the initial elements based on L and B
  for (int i = 0; i < p - 1; ++i) {
    int index = i * q;
    if (index < L_R.size()) {
      L_R[index] = freeSpace.getL()[index];
    }
  }

  for (int j = 0; j < q - 1; ++j) {
    int index = j * p;
    if (index < B_R.size()) {
      B_R[index] = freeSpace.getB()[index];
    }
  }

  // Step 2: Nested loop for processing L_R and B_R
  for (int i = 0; i < p - 1; ++i) {
    int l_base = i * q;
    int b_base = i;

    for (int j = 0; j < q - 1; ++j) {
      int l = l_base + j;
      int b = b_base + j * p;

      // Compare L_R[l], B_R[b], and L[l + 1]
      const auto& point1 = L_R[l].first;
      const auto& point2 = L_R[l].second;
      const auto& point3 = B_R[b].first;
      const auto& point4 = B_R[b].second;
      const auto& point5 = freeSpace.getL()[l + 1].first;
      const auto& point6 = freeSpace.getL()[l + 1].second;

      // Task 1: Handling point3 from B_R
      if (point3 != Point_2(-1, -1)) {
        L_R[l + 1] = freeSpace.getL()[l + 1];
      } else {
        if (point1 == Point_2(-1, -1) || point5 == Point_2(-1, -1)) {
          L_R[l + 1] = {Point_2(-1, -1), Point_2(-1, -1)};
        } else {
          double y_L = std::max(point1.y(), point5.y());
          double y_U = std::min(point2.y(), point6.y());
          if (y_L > y_U) {
            L_R[l + 1] = {Point_2(-1, -1), Point_2(-1, -1)};
          } else {
            L_R[l + 1] = {Point_2(j, y_L), Point_2(j, y_U)};
          }
        }
      }

      // Compare B_R[b], L_R[b], and B[b + 1]
      const auto& b_point1 = B_R[b].first;
      const auto& b_point2 = B_R[b].second;
      const auto& b_point3 = L_R[b].first;
      const auto& b_point4 = L_R[b].second;
      const auto& b_point5 = freeSpace.getB()[b + 1].first;
      const auto& b_point6 = freeSpace.getB()[b + 1].second;

      // Task 2: Handling point3 from L_R
      if (b_point3 != Point_2(-1, -1)) {
        B_R[b + 1] = freeSpace.getB()[b + 1];
      } else {
        if (b_point1 == Point_2(-1, -1) || b_point5 == Point_2(-1, -1)) {
          B_R[b + 1] = {Point_2(-1, -1), Point_2(-1, -1)};
        } else {
          double x_L = std::max(b_point1.x(), b_point5.x());
          double x_U = std::min(b_point2.x(), b_point6.x());
          if (x_L > x_U) {
            B_R[b + 1] = {Point_2(-1, -1), Point_2(-1, -1)};
          } else {
            B_R[b + 1] = {Point_2(x_L, j + 1), Point_2(x_U, j + 1)};
          }
        }
      }
    }
  }

  // Step 3: Check if a valid path exists
  Point_2 start(0, 0);
  Point_2 end(q - 1, p - 1);

  if ((L_R[0].first == start || L_R[0].second == start ||
       B_R[0].first == start || B_R[0].second == start) &&
      (L_R.back().first == end || L_R.back().second == end ||
       B_R.back().first == end || B_R.back().second == end)) {
    return true;
  }

  return false;
}
