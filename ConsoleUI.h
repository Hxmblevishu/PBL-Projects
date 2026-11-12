#ifndef CONSOLEUI_H
#define CONSOLEUI_H

class ConsoleUI
{
public:
    enum Color
    {
        DEFAULT_COLOR,
        BLUE,
        LIGHT_BLUE,
        PINK,
        RED,
        WHITE,
        SELECTED
    };

private:
    unsigned short defaultAttributes;

public:
    ConsoleUI();

    void initialize() const;
    void clear() const;
    void setColor(Color color) const;
    void resetColor() const;
    void printBanner() const;
    int readMenuKey(int itemCount) const;
    void pause() const;
    void divider(char character = '-') const;
};
#endif
