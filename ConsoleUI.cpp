#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

#include "ConsoleUI.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
ConsoleUI::ConsoleUI()
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    defaultAttributes = GetConsoleScreenBufferInfo(output, &info) ? info.wAttributes : 7;
#else
    defaultAttributes = 7;
#endif
}

void ConsoleUI::initialize() const
{
#ifdef _WIN32
    SetConsoleTitleA("SurakshaPath | Dehradun Route Desk");
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void ConsoleUI::clear() const
{
#ifdef _WIN32
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(output, &info)) return;

    DWORD cellCount = static_cast<DWORD>(info.dwSize.X) * static_cast<DWORD>(info.dwSize.Y);
    DWORD written = 0;
    COORD firstCell = {0, 0};
    FillConsoleOutputCharacterA(output, ' ', cellCount, firstCell, &written);
    FillConsoleOutputAttribute(output, static_cast<WORD>(defaultAttributes), cellCount, firstCell, &written);
    SetConsoleCursorPosition(output, firstCell);
#else
    cout << "\033[2J\033[H";
#endif
}

void ConsoleUI::setColor(Color color) const
{
#ifdef _WIN32
    WORD attributes = static_cast<WORD>(defaultAttributes);
    switch (color)
    {
    case BLUE:      attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
    case LIGHT_BLUE:attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
    case PINK:      attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
    case RED:       attributes = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
    case WHITE:     attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
    case SELECTED:  attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE |
                                 FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY; break;
    case DEFAULT_COLOR: break;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attributes);
#else
    switch (color)
    {
    case BLUE: cout << "\033[1;34m"; break;
    case LIGHT_BLUE: cout << "\033[1;36m"; break;
    case PINK: cout << "\033[1;35m"; break;
    case RED: cout << "\033[1;31m"; break;
    case WHITE: cout << "\033[1;37m"; break;
    case SELECTED: cout << "\033[1;37;44m"; break;
    case DEFAULT_COLOR: cout << "\033[0m"; break;
    }
#endif
}

void ConsoleUI::resetColor() const
{
    setColor(DEFAULT_COLOR);
}

static const char* largeLetterRow(char letter, int row)
{
    static const char* S[5] = {"####", "#...", "###.", "...#", "####"};
    static const char* U[5] = {"#..#", "#..#", "#..#", "#..#", "####"};
    static const char* R[5] = {"###.", "#..#", "###.", "#.#.", "#..#"};
    static const char* A[5] = {".##.", "#..#", "####", "#..#", "#..#"};
    static const char* K[5] = {"#..#", "#.#.", "##..", "#.#.", "#..#"};
    static const char* H[5] = {"#..#", "#..#", "####", "#..#", "#..#"};
    static const char* P[5] = {"###.", "#..#", "###.", "#...", "#..."};
    static const char* T[5] = {"####", ".##.", ".##.", ".##.", ".##."};

    const char** glyph = S;
    switch (letter)
    {
    case 'U': glyph = U; break;
    case 'R': glyph = R; break;
    case 'A': glyph = A; break;
    case 'K': glyph = K; break;
    case 'H': glyph = H; break;
    case 'P': glyph = P; break;
    case 'T': glyph = T; break;
    default: break;
    }
    return glyph[row];
}

void ConsoleUI::printBanner() const
{
    const int insideWidth = 86;
    const int titleWidth = 64;
    const int titlePadding = (insideWidth - titleWidth) / 2;
    static const char* words[] = {"SURAKSHA", "PATH"};
    static const Color shadowPalette[] = {LIGHT_BLUE, PINK, BLUE, RED, LIGHT_BLUE, PINK,
                                          BLUE, RED, PINK, LIGHT_BLUE, RED, BLUE};

    for (int row = 0; row < 6; row++)
    {
        cout << "  " << string(titlePadding, ' ');
        int letterIndex = 0;
        for (int word = 0; word < 2; word++)
        {
            if (word == 1) cout << "    ";
            for (int i = 0; words[word][i] != '\0'; i++)
            {
                const char* glyph = largeLetterRow(words[word][i], row);
                for (int column = 0; column < 5; column++)
                {
                    bool face = row < 5 && column < 4 && glyph[column] == '#';
                    bool shadow = row > 0 && column > 0 && column <= 4 &&
                                  largeLetterRow(words[word][i], row - 1)[column - 1] == '#';
                    if (face)
                    {
                        setColor(WHITE);
                        cout << u8"█";
                    }
                    else if (shadow)
                    {
                        setColor(shadowPalette[letterIndex]);
                        cout << u8"▓";
                    }
                    else cout << ' ';
                }
                letterIndex++;
            }
        }
        resetColor();
        cout << "\n";
    }

    const std::string subtitle = "DEHRADUN SAFE ROUTE MANAGEMENT";
    const int subtitlePadding = (insideWidth - static_cast<int>(subtitle.length())) / 2;
    cout << "  " << string(subtitlePadding, ' ');
    setColor(LIGHT_BLUE);
    cout << subtitle;
    cout << "\n";

    const std::string status = "// ROUTE FINDER  |  LOCAL CSV NETWORK //";
    const int statusPadding = (insideWidth - static_cast<int>(status.length())) / 2;
    cout << "  " << string(statusPadding, ' ');
    setColor(PINK);
    cout << status;
    cout << "\n";
    setColor(BLUE);
    cout << "  " << string((insideWidth - 70) / 2, ' ')
              << string(70, '-') << "\n";
    resetColor();
}

int ConsoleUI::readMenuKey(int itemCount) const
{
#ifdef _WIN32
    HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    if (GetConsoleMode(input, &mode))
    {
        while (true)
        {
            int key = _getch();
            if (key == 0 || key == 224)
            {
                int arrow = _getch();
                if (arrow == 72) return -1; // Up
                if (arrow == 80) return 1;  // Down
                continue;
            }

            if (key == 13) return 0; // Enter
            if (key == 27) return 2; // Escape
            if (key >= '0' && key <= '9')
            {
                int choice = key - '0';
                int digits = 1;
                setColor(LIGHT_BLUE);
                cout << "\n  Option: " << static_cast<char>(key);
                resetColor();
                while (true)
                {
                    int next = _getch();
                    if (next == 13)
                    {
                        cout << "\n";
                        return choice >= 1 && choice <= itemCount ? 100 + choice : -99;
                    }
                    if (next == 27)
                    {
                        cout << "\n";
                        return 2;
                    }
                    if (next == 8 && digits > 1)
                    {
                        choice /= 10;
                        digits--;
                        cout << "\b \b";
                    }
                    else if (next >= '0' && next <= '9' && digits < 2)
                    {
                        choice = choice * 10 + (next - '0');
                        digits++;
                        cout << static_cast<char>(next);
                    }
                }
            }
        }
    }
#endif

    cout << "  Select 1-" << itemCount << " (or 0 to exit): ";
    string line;
    if (!getline(cin, line)) return 2;
    stringstream lineInput(line);
    int choice = -1;
    char extra;
    if ((lineInput >> choice) && !(lineInput >> extra) && choice >= 1 && choice <= itemCount)
        return 100 + choice;
    return choice == 0 ? 2 : -99;
}

void ConsoleUI::pause() const
{
    setColor(LIGHT_BLUE);
    cout << "\n  Press Enter to return to the route desk...";
    resetColor();
    string line;
    getline(cin, line);
}

void ConsoleUI::divider(char character) const
{
    setColor(BLUE);
    cout << "  +" << string(78, character) << "+\n";
    resetColor();
}
