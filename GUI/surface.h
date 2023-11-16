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
    BsplineSurface();
    void CalculateSurfacePoints() override;

private:
    float BasisFunction(int i, int p, float t, const std::vector<float>& knots);
    glm::vec3 CalculateBsplineSurfacePoint(float u, float v);

    std::vector<float> knotVectorU;
    std::vector<float> knotVectorV;
    int degreeU;
    int degreeV;
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