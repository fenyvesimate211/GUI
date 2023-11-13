#include "surface.h"

void NURBSSurface::CalculateSurfacePoints() {
    for (int i = 0; i <= resolutionU; i++) { // Изменено с <= на <
        float u = static_cast<float>(i) / static_cast<float>(resolutionU); // Нормализация u

        for (int j = 0; j <= resolutionV; j++) { // Изменено с <= на <
            float v = static_cast<float>(j) / static_cast<float>(resolutionV); // Нормализация v

            glm::vec3 surfacePoint = CalculateNURBSSurfacePoint(u, v); // Вызов функции для вычисления точки
            surfacePoints[i][j] = surfacePoint;
        }
    }
}


NURBSSurface::NURBSSurface() : Surface(), degreeU(3), degreeV(3) {
    // Установка размеров узловых векторов
    int knotCountU = controlPoints.size() + degreeU + 1;
    knotVectorU.resize(knotCountU);

    // Первые и последние 'degreeU + 1' узлы должны быть равны 0 и 1 соответственно
    for (int i = 0; i <= degreeU; ++i) {
        knotVectorU[i] = 0.0f;
    }
    for (int i = knotCountU - degreeU - 1; i < knotCountU; ++i) {
        knotVectorU[i] = 1.0f;
    }
    // Промежуточные значения равномерно распределены между 0 и 1
    for (int i = degreeU + 1; i < knotCountU - degreeU - 1; ++i) {
        knotVectorU[i] = float(i - degreeU) / (knotCountU - 2 * degreeU - 1);
    }

    int knotCountV = controlPoints[0].size() + degreeV + 1;
    knotVectorV.resize(knotCountV);

    for (int i = 0; i <= degreeV; ++i) {
        knotVectorV[i] = 0.0f;
    }
    for (int i = knotCountV - degreeV - 1; i < knotCountV; ++i) {
        knotVectorV[i] = 1.0f;
    }
    for (int i = degreeV + 1; i < knotCountV - degreeV - 1; ++i) {
        knotVectorV[i] = float(i - degreeV) / (knotCountV - 2 * degreeV - 1);
    }
    // Инициализация весов
    weights = std::vector<std::vector<float>>(controlPoints.size(), std::vector<float>(controlPoints[0].size(), 1.0f));
}







float NURBSSurface::Nip(int i, int p, float u, const std::vector<float>& U) {
    // Базовый случай рекурсии
    if (p == 0) {
        return (U[i] <= u && u <= U[i + 1]) ? 1.0f : 0.0f;
    }

    float left = 0.0f;
    float right = 0.0f;

    // Левая часть рекурсии
    if ((U[i + p] - U[i]) != 0.0f) { // Проверяем, не происходит ли деление на ноль
        left = (u - U[i]) / (U[i + p] - U[i]) * Nip(i, p - 1, u, U);
    }

    // Правая часть рекурсии
    if ((U[i + p + 1] - U[i + 1]) != 0.0f) { // Проверяем, не происходит ли деление на ноль
        right = (U[i + p + 1] - u) / (U[i + p + 1] - U[i + 1]) * Nip(i + 1, p - 1, u, U);
    }

    // Суммируем результаты
    return left + right;
}



glm::vec3 NURBSSurface::CalculateNURBSSurfacePoint(float u, float v) {
    glm::vec3 numerator(0.0f, 0.0f, 0.0f);
    float denominator = 0.0f;

    int n = controlPoints.size() - 1;
    int m = controlPoints[0].size() - 1;

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            float Ni = Nip(i, degreeU, u, knotVectorU);  // Базисная функция NURBS для U
            float Nj = Nip(j, degreeV, v, knotVectorV);  // Базисная функция NURBS для V
            float weight = weights[i][j];

            numerator += weight * Ni * Nj * controlPoints[i][j];
            denominator += weight * Ni * Nj;
        }
    }

    if (denominator != 0.0f) {
        return numerator / denominator;
    }
    else {
        return glm::vec3(0.0f, 0.0f, 0.0f);  // Возвращаем нулевую точку в случае ошибки
    }
}