#include "surface.h"



void BezierSurface::CalculateSurfacePoints() 
{
    // Loop through the grid of (u, v) parameter values
    for (int i = 0; i <= resolutionU; i++) {
        float u = static_cast<float>(i) / static_cast<float>(resolutionU);

        for (int j = 0; j <= resolutionV; j++) {
            float v = static_cast<float>(j) / static_cast<float>(resolutionV);

            // Calculate the surface point for the current (u, v)
            glm::vec3 surfacePoint = CalculateBezierSurfacePoint(u, v, controlPoints);

            //std::cout << "Point at (u=" << i << "/" << resolutionU << ", v=" << j << "/" << resolutionV << "): ";
            //std::cout << "X=" << surfacePoint.x << ", Y=" << surfacePoint.y << ", Z=" << surfacePoint.z << std::endl;
            // Now, 'surfacePoint' contains the 3D coordinates of a point on the Bezier surface
            // You can use this point for rendering or other purposes
            surfacePoints[i][j] = surfacePoint;
        }
    }
}

// Calculate binomial coefficient C(n, k)
int BezierSurface::BinomialCoefficient(int n, int k) {
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

glm::vec3 BezierSurface::CalculateBezierSurfacePoint(float u, float v, const std::vector<std::vector<glm::vec3>>& controlPoints) {
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