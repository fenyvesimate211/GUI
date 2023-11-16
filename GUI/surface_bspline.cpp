#include "surface.h"
#include <algorithm>
BsplineSurface::BsplineSurface() : Surface() {
	degreeU = 3;
	degreeV = 3;
	int knotCountU = controlPoints.size() + degreeU + 1;
	int knotCountV = controlPoints[0].size() + degreeV + 1;
	knotVectorU.resize(knotCountU);
	knotVectorV.resize(knotCountV);
	for (int i = 0; i < knotCountU; i++)
		knotVectorU[i] = float(i) / (knotCountU - 1);
	for (int j = 0; j < knotCountV; j++)
		knotVectorV[j] = float(j) / (knotCountV - 1);
}
void BsplineSurface::CalculateSurfacePoints() {
	for (int uIndex = 0; uIndex <= resolutionU; uIndex++) {
		float u = float(uIndex) / resolutionU;
		for (int vIndex = 0; vIndex <= resolutionV; vIndex++) {
			float v = float(vIndex) / resolutionV;
			glm::vec3 point(0.0f, 0.0f, 0.0f);
			float weightSum = 0.0f;
			for (int i = 0; i < controlPoints.size(); i++) {
				for (int j = 0; j < controlPoints[i].size(); j++) {
					float basisU = BasisFunction(i, degreeU, u, knotVectorU);
					float basisV = BasisFunction(j, degreeV, v, knotVectorV);
					float weight = basisU * basisV;
					point += controlPoints[i][j] * weight;
					weightSum += weight;
				}
			}
			if (weightSum != 0.0f) {
				point /= weightSum;
			}
			surfacePoints[uIndex][vIndex] = point;
		}
	}
}
float BsplineSurface::BasisFunction(int i, int p, float t, const std::vector<float>& knots) {
	if (p == 0) { return (knots[i] <= t && t < knots[i + 1]) ? 1.0f : 0.0f; }
	float left = 0.0f;
	float right = 0.0f;
	float leftDenominator = knots[i + p] - knots[i];
	if (leftDenominator != 0.0f) {
		left = (t - knots[i]) / leftDenominator * BasisFunction(i, p - 1, t, knots);
	}
	float rightDenominator = knots[i + p + 1] - knots[i + 1];
	if (rightDenominator != 0.0f) {
		right = (knots[i + p + 1] - t) / rightDenominator * BasisFunction(i + 1, p - 1, t, knots);
	}
	return left + right;
}

glm::vec3 BsplineSurface::CalculateBsplineSurfacePoint(float u, float v) {
	glm::vec3 point(0.0f, 0.0f, 0.0f);
	float weightSum = 0.0f;
	for (int i = 0; i < controlPoints.size(); i++) {
		for (int j = 0; j < controlPoints[i].size(); j++) {
			float basisU = BasisFunction(i, degreeU, u, knotVectorU);
			float basisV = BasisFunction(j, degreeV, v, knotVectorV);
			float weight = basisU * basisV;
			point += controlPoints[i][j] * weight;
			weightSum += weight;
		}
	}
	if (weightSum != 0.0f) {
		point /= weightSum;
	}
	return point;
}