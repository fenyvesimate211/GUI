#include "surface.h"

void BsplineSurface::CalculateSurfacePoints() {
    for (int i = 0; i <= resolutionU; ++i) {
        float u = static_cast<float>(i) / static_cast<float>(resolutionU);

        for (int j = 0; j <= resolutionV; ++j) {
            float v = static_cast<float>(j) / static_cast<float>(resolutionV);

            std::vector<float> knotVectorU = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 };
            std::vector<float> knotVectorV = { 0, 0, 0, 0, 0, 1, 1, 1, 1, 1 };

            surfacePoints[i][j] = CalculateBSplineSurfacePoint(u, v, controlPoints, controlPoints, knotVectorU, knotVectorV);
        }
    }
}

float BsplineSurface::CalculateBasisFunction(int i, int p, float t, const std::vector<float>& knotVector) {
    if (p == 0) {
        if (knotVector[i] <= t && t <= knotVector[i + 1]) {
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
    int n = controlPointsU.size() - 1;
    int m = controlPointsV[0].size() - 1;

    int pU = controlPointsU.size() - 1;
    int pV = controlPointsV[0].size() - 1;

    glm::vec3 point(0.0f, 0.0f, 0.0f);

    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            float basisU = CalculateBasisFunction(i, pU, u, knotVectorU);
            float basisV = CalculateBasisFunction(j, pV, v, knotVectorV);
            point += basisU * basisV * controlPointsV[i][j];
        }
    }

    return point;
}