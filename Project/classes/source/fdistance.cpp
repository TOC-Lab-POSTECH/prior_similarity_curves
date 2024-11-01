#include "fdistance.h"

// Constructor to initialize with two curves and set the F-distance
FDistance::FDistance(const PolygonalCurve& P, const PolygonalCurve& Q)
    : P(P), Q(Q), criticalVal(P, Q), decision(P, Q, 0.0), fDistance(-1.0) {
  // Compute the F-distance using binary search on the critical values
  computeFDistance();
}

// Getter
double FDistance::getFDistance() const { return fDistance; }

// Helper function to compute F-distance using binary search on critical values
void FDistance::computeFDistance() {
  // Get the sorted critical values from the CriticalValue object
  const std::vector<double>& criticalValues = criticalVal.getCriticalValues();
  if (criticalValues.empty()) {
    fDistance = -1.0;  // No critical values, set F-distance to -1
    return;
  }

  // Perform binary search on critical values
  int left = 0;
  int right = criticalValues.size() - 1;
  double result = -1.0;  // To store the last true result

  while (left <= right) {
    int mid = left + (right - left) / 2;
    double currentEpsilon = criticalValues[mid];

    // Update the DecisionProblem with the current epsilon
    decision.setEpsilon(currentEpsilon);

    // Check if there is a monotone curve for this epsilon
    if (decision.doesMonotoneCurveExist()) {
      // If true, move to the left half (try smaller values)
      result = currentEpsilon;
      right = mid - 1;
    } else {
      // If false, move to the right half (try larger values)
      left = mid + 1;
    }
  }

  // Set the F-distance based on the result of the binary search
  fDistance = (result == -1.0) ? -1.0 : result;
}
