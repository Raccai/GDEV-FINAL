#ifndef CLOUD_H
#define CLOUD_H

#include "raylib.h"

struct Cloud {
    Vector2 position;
    Vector2 controlPoints[4]; // Control points for bezier curve
    float t; // Parameter for interpolating along the curve
    float speed; // Speed of cloud movement
    float size; // Size of cloud
    Color color; // Color of the cloud
    Texture2D texture; // Add texture member
};

void UpdateCloud(Cloud& cloud, float deltaTime);
void DrawCloud(const Cloud& cloud);

#endif // CLOUD_H