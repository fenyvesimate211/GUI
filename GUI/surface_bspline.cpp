#include "surface.h"

void BsplineSurface::CalculateSurfacePoints()
{
    // Loop through the grid of (u, v) parameter values
    for (int i = 0; i <= resolutionU; i++) {
        float u = static_cast<float>(i) / static_cast<float>(resolutionU);

        for (int j = 0; j <= resolutionV; j++) {
            float v = static_cast<float>(j) / static_cast<float>(resolutionV);

            // Calculate the surface point for the current (u, v)
            glm::vec3 surfacePoint = CalculateBezierSurfacePoint(u, v, controlPoints);


            std::vector<float> knotVectorU = { 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 12, 12, 12 };
            std::vector<float> knotVectorV = { 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 12, 12, 12 };

            // Calculate B Spline surface point for the current (u, v)
            glm::vec3 surfacePoint2 = CalculateBSplineSurfacePoint(u, v, controlPoints, controlPoints, knotVectorU, knotVectorV);


            // Console log the B Spline points
            // std::cout << "Point at (u=" << i << "/" << resolutionU << ", v=" << j << "/" << resolutionV << "): ";
            // std::cout << "X=" << surfacePoint2.x << ", Y=" << surfacePoint2.y << ", Z=" << surfacePoint2.z << std::endl;
            // Now, 'surfacePoint' contains the 3D coordinates of a point on the Bezier surface
            // You can use this point for rendering or other purposes
            surfacePoints[i][j] = surfacePoint2;
            //surfacePointsBSpline[i][j] = surfacePoint2;
        }
    }
}



// Calculate binomial coefficient C(n, k)
int BsplineSurface::BinomialCoefficient(int n, int k) {
    if (k < 0 || k > n) {
        return 0;
    }

    int result = 1;
    for (int i = 0; i < k; ++i) {
        result *= (n - i);
        result /= (i + 1);
    }

    return result;
}

glm::vec3 BsplineSurface::CalculateBezierSurfacePoint(float u, float v, const std::vector<std::vector<glm::vec3>>& controlPoints) {
    int n = controlPoints.size() - 1; // Rows
    int m = controlPoints[0].size() - 1; // Columns

    glm::vec3 point(0.0f, 0.0f, 0.0f);

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            int coeff = BinomialCoefficient(n, i) * BinomialCoefficient(m, j);
            float term = coeff * std::pow(u, i) * std::pow(1 - u, n - i) * std::pow(v, j) * std::pow(1 - v, m - j);
            point += term * controlPoints[i][j];
        }
    }

    return point;
}

// Calculate B-Spline basis functions using Cox-de Boor recursion
float BsplineSurface::CalculateBasisFunction(int i, int p, float t, const std::vector<float>& knotVector) {
    if (p == 0) {
        if (knotVector[i] <= t && t < knotVector[i + 1]) {
            return 1.0f;
        }
        else {
            return 0.0f;
        }
    }

    float basis1 = 0.0f;
    float basis2 = 0.0f;

    if (knotVector[i + p] - knotVector[i] != 0.0f) {
        basis1 = (t - knotVector[i]) / (knotVector[i + p] - knotVector[i]) * CalculateBasisFunction(i, p - 1, t, knotVector);
    }

    if (knotVector[i + p + 1] - knotVector[i + 1] != 0.0f) {
        basis2 = (knotVector[i + p + 1] - t) / (knotVector[i + p + 1] - knotVector[i + 1]) * CalculateBasisFunction(i + 1, p - 1, t, knotVector);
    }

    return basis1 + basis2;
}

glm::vec3 BsplineSurface::CalculateBSplineSurfacePoint(float u, float v, const std::vector<std::vector<glm::vec3>>& controlPointsU, const std::vector<std::vector<glm::vec3>>& controlPointsV, const std::vector<float>& knotVectorU, const std::vector<float>& knotVectorV) {
    int n = controlPointsU.size() - 1; // Rows
    int m = controlPointsV.size() - 1; // Columns


    // Change these values to make the surface different
    int pU = 1;
    int pV = 1;

    glm::vec3 point(0.0f, 0.0f, 0.0f);

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            float basisU = CalculateBasisFunction(i, pU, u, knotVectorU);
            float basisV = CalculateBasisFunction(j, pV, v, knotVectorV);
            point += basisU * basisV * controlPointsU[i][j];
        }
    }


    return point;
}