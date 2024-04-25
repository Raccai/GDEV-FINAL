#include "GameUI.hpp"

UiLibrary::UiLibrary() : hot(-1), active(-1) {}

bool UiLibrary::Button(int id, const std::string& text, const Rectangle& bounds, Color defaultColor, Color hoverColor, Color clickedColor) {
    bool result = false;
    bool isHover = CheckCollisionPointRec(GetMousePosition(), bounds);
    bool isClicked = (active == id);

    if (id == active) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (id == hot) {
                result = true;
            }
            active = -1;
        }
    }

    if (id == hot) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            active = id;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
        hot = id;
    } else if (hot == id) {
        hot = -1;
    }

    if (isHover && !isClicked) {
        DrawRectangleRec(bounds, hoverColor);
    } else if (isClicked) {
        DrawRectangleRec(bounds, clickedColor);
    } else {
        DrawRectangleRec(bounds, defaultColor);
    }

    // Center the text within the button bounds
    float textWidth = MeasureText(text.c_str(), 14);
    float textHeight = 14;
    Vector2 textPosition = {(bounds.width - textWidth) / 2 + bounds.x, (bounds.height - textHeight) / 2 + bounds.y};

    DrawText(text.c_str(), static_cast<int>(textPosition.x), static_cast<int>(textPosition.y), 14, BLACK);

    return result;
}
