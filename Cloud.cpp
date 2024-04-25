#include "raylib.h"
#include "raymath.h"
#include "Cloud.hpp"

// Calculate a point on a bezier curve given control points and t parameter
Vector2 CalculateBezierPoint(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    Vector2 p; 
    p.x = uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x;
    p.y = uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y;

    return p;
}

void UpdateCloud(Cloud& cloud, float deltaTime) {
    // Update parameter 't' for interpolation based on the direction of movement
    cloud.t += cloud.speed * cloud.direction * deltaTime;
    
    // If cloud reaches the end of the curve or the beginning, reverse its direction
    if (cloud.t >= 1.0f || cloud.t <= 0.0f) {
        cloud.direction *= -1; // Reverse direction
    }

    // Ensure 't' stays within the range [0, 1]
    cloud.t = Clamp(cloud.t, 0.0f, 1.0f);

    // Calculate position along bezier curve
    cloud.position = CalculateBezierPoint(cloud.controlPoints[0], cloud.controlPoints[1], 
                                          cloud.controlPoints[2], cloud.controlPoints[3], cloud.t);
}

void DrawCloud(const Cloud& cloud) {
    // Draw cloud texture at its current position
    DrawTexture(cloud.texture, static_cast<int>(cloud.position.x), static_cast<int>(cloud.position.y), WHITE); 
}