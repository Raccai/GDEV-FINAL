#include <string>
#include <raylib.h>

struct UiLibrary {
    int hot;
    int active;

    UiLibrary();

    bool Button(int id, const std::string& text, const Rectangle& bounds, Color defaultColor, Color hoverColor, Color clickedColor);
};