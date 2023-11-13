#ifndef SURFACE_H
#define SURFACE_H

#include <glm/glm.hpp>
#include <vector>


// (abstract class)
class Surface 
{
public:

    Surface();

    virtual void CalculateSurfacePoints() = 0;

    int resolutionU;
    int resolutionV;
    std::vector<std::vector<glm::vec3>> controlPoints;
    std::vector<std::vector<glm::vec3>> surfacePoints;

};



class BezierSurface : public Surface
{
public:

    BezierSurface() : Surface() {};
    void CalculateSurfacePoints();

private:

    int BinomialCoefficient(int n, int k);
    glm::vec3 CalculateBezierSurfacePoint(float u, float v, const std::vector<std::vector<glm::vec3>>& controlPoints);

};



class BsplineSurface : public Surface
{
public:

    BsplineSurface() : Surface() {};
    void CalculateSurfacePoints();

private:

    int BinomialCoefficient(int n, int k);
    glm::vec3 CalculateBezierSurfacePoint(float u, float v, const std::vector<std::vector<glm::vec3>>& controlPoints);
    float CalculateBasisFunction(int i, int p, float t, const std::vector<float>& knotVector);
    glm::vec3 CalculateBSplineSurfacePoint(float u, float v, const std::vector<std::vector<glm::vec3>>& controlPointsU, const std::vector<std::vector<glm::vec3>>& controlPointsV, const std::vector<float>& knotVectorU, const std::vector<float>& knotVectorV);


};


class NURBSSurface : public Surface {
public:
    NURBSSurface();
    void CalculateSurfacePoints() override;

private:
    float Nip(int i, int p, float u, const std::vector<float>& U);
    glm::vec3 CalculateNURBSSurfacePoint(float u, float v);

    std::vector<float> knotVectorU;
    std::vector<float> knotVectorV;
    std::vector<std::vector<float>> weights;

    int degreeU;
    int degreeV;
};



#endif