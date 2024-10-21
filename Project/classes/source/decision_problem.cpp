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
  // Start the recursive process from (0, 0)
  Point_2 startPoint(0, 0);

  const PointPairVector& L = freeSpace.getL();
  const PointPairVector& B = freeSpace.getB();

  bool fromL = L[0].first == startPoint;
  bool fromB = B[0].first == startPoint;

  if (fromL && recursiveCheck(startPoint, true)) {
    return true;
  }
  if (fromB && recursiveCheck(startPoint, false)) {
    return true;
  }

  // No valid monotone curve found
  return false;
}

bool DecisionProblem::recursiveCheck(const Point_2& currentPoint,
                                     bool isFromL) {
  int p = P.numPoints();
  int q = Q.numPoints();

  int i = static_cast<int>(currentPoint.y());
  int j = static_cast<int>(currentPoint.x());

  // Check if the current point is the endpoint (q-1, p-1)
  if (currentPoint == Point_2(q - 1, p - 1)) {
    return true;
  }

  const PointPairVector& L = freeSpace.getL();
  const PointPairVector& B = freeSpace.getB();

  // Determine candidates based on whether the point is from L or B
  if (isFromL) {  // Current point is from B
    // First candidate: i * (q + 1) + j from L
    int candidateIndex = i * (q + 1) + j;
    if (candidateIndex >= 0 && candidateIndex < L.size()) {
      const auto& candidate = L[candidateIndex];
      if (candidate.first != Point_2(-1, -1)) {
        if (candidate.second == Point_2(q - 1, p - 1)) return true;

        if (candidate.first == Point_2(j, i + 1) &&
            L[i * q + j].second == Point_2(j, i + 1)) {
          if (recursiveCheck(Point_2(j, i + 1), true)) return true;
        }
      }
    }

    // Second candidate: j * p + i from B
    candidateIndex = j * p + i;
    if (candidateIndex >= 0 && candidateIndex < B.size()) {
      const auto& candidate = B[candidateIndex];
      if (candidate.first != Point_2(-1, -1)) {
        if (candidate.second == Point_2(q - 1, p - 1)) return true;

        if (candidate.first.y() >= currentPoint.y() &&
            candidate.second.y() >= currentPoint.y() &&
            candidate.first != candidate.second) {
          if (recursiveCheck(candidate.second, false)) return true;
        }
      }
    }

    // Third candidate: j * p + i + 1 from B
    candidateIndex = j * p + i + 1;
    if (candidateIndex >= 0 && candidateIndex < B.size()) {
      const auto& candidate = B[candidateIndex];
      if (candidate.first != Point_2(-1, -1)) {
        if (candidate.second == Point_2(q - 1, p - 1)) return true;

        if (recursiveCheck(candidate.first, false)) return true;
      }
    }

    // Fourth candidate: i * q + j + 1 from L
    candidateIndex = i * q + j + 1;
    if (candidateIndex >= 0 && candidateIndex < L.size()) {
      const auto& candidate = L[candidateIndex];
      if (candidate.first != Point_2(-1, -1)) {
        if (candidate.second == Point_2(q - 1, p - 1)) return true;

        if (candidate.first.x() >= currentPoint.x() &&
            candidate.first.y() >= currentPoint.y() &&
            candidate.second.x() >= currentPoint.x() &&
            candidate.second.y() >= currentPoint.y()) {
          if (recursiveCheck(candidate.first, true)) return true;
        } else if (candidate.second.x() >= currentPoint.x() &&
                   candidate.second.y() >= currentPoint.y()) {
          if (recursiveCheck(candidate.second, true)) return true;
        }
      }
    }

  } else {  // Current point is from B
    // First candidate: j * (p + 1) + i from B
    int candidateIndex = j * (p + 1) + i;
    if (candidateIndex >= 0 && candidateIndex < B.size()) {
      const auto& candidate = B[candidateIndex];
      if (candidate.first != Point_2(-1, -1)) {
        if (candidate.second == Point_2(q - 1, p - 1)) return true;

        if (candidate.first == Point_2(j + 1, i) &&
            B[j * p + i].second == Point_2(j + 1, i)) {
          if (recursiveCheck(Point_2(j + 1, i), false)) return true;
        }
      }
    }

    // Second candidate: i * q + j from L
    candidateIndex = i * q + j;
    if (candidateIndex >= 0 && candidateIndex < L.size()) {
      const auto& candidate = L[candidateIndex];
      if (candidate.first != Point_2(-1, -1)) {
        if (candidate.second == Point_2(q - 1, p - 1)) return true;

        if (candidate.first.x() >= currentPoint.x() &&
            candidate.second.x() >= currentPoint.x() &&
            candidate.first != candidate.second) {
          if (recursiveCheck(candidate.second, true)) return true;
        }
      }
    }

    // Third candidate: i * q + j + 1 from L
    candidateIndex = i * q + j + 1;
    if (candidateIndex >= 0 && candidateIndex < L.size()) {
      const auto& candidate = L[candidateIndex];
      if (candidate.first != Point_2(-1, -1)) {
        if (candidate.second == Point_2(q - 1, p - 1)) return true;

        if (recursiveCheck(candidate.first, true)) return true;
      }
    }

    // Fourth candidate: j * p + i + 1 from B
    candidateIndex = j * p + i + 1;
    if (candidateIndex >= 0 && candidateIndex < B.size()) {
      const auto& candidate = B[candidateIndex];
      if (candidate.first != Point_2(-1, -1)) {
        if (candidate.second == Point_2(q - 1, p - 1)) return true;

        if (candidate.first.x() >= currentPoint.x() &&
            candidate.first.y() >= currentPoint.y() &&
            candidate.second.x() >= currentPoint.x() &&
            candidate.second.y() >= currentPoint.y()) {
          if (recursiveCheck(candidate.first, false)) return true;
        } else if (candidate.second.x() >= currentPoint.x() &&
                   candidate.second.y() >= currentPoint.y()) {
          if (recursiveCheck(candidate.second, false)) return true;
        }
      }
    }
  }

  return false;
}
