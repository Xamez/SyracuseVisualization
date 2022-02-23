#include <iostream>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Line {

public:
    Line(int x1, int y1, int x2, int y2) {
        this->x1 = x1;
        this->y1 = y1;
        this->x2 = x2;
        this->y2 = y2;
    }
    int x1, y1, x2, y2;

    void draw(olc::PixelGameEngine* pge) {
        pge->DrawLine(x1, y1-1, x2, y2-1, olc::BLUE);
        pge->DrawLine(x1, y1, x2, y2, olc::BLUE);
        pge->DrawLine(x1, y1+1, x2, y2+1, olc::BLUE);
    }
};

class SyracuseVisualization : public olc::PixelGameEngine
{

public:

    int graphScale = 20;

    float updateTime = 0.5f;
    bool running = false;

    SyracuseVisualization()
    {
        sAppName = "Syracuse Visualization (Paused)";
    }

public:

    bool OnUserCreate() override
    {
        Clear(olc::WHITE);
        DrawGraph();
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        HandleInputs();
        if (running) {
            timer += fElapsedTime;
            if (timer >= updateTime) {
                timer = 0.0f;
                Clear(olc::WHITE);
                DrawGraph();
                Cycle();
            }
        }
        return true;
    }

private:

    float timer = 0.0f;

    std::vector<Line> lines;
    int64_t currentNumber = 5;
    int currentX = graphScale;
    int currentY = ScreenHeight() - graphScale - (currentNumber * graphScale);
    int previousX = currentX;
    int previousY = currentY;

    void Cycle() {
        std::cout << ScreenHeight() << " / " << graphScale << " / " << ScreenHeight() - graphScale << " / "
                  << currentNumber * graphScale << " / " << ScreenHeight() - graphScale - (currentNumber * graphScale)
                  << std::endl;
        if (currentNumber % 2 == 0)
            currentNumber = odd(currentNumber);
        else
            currentNumber = even(currentNumber);
        std::cout << previousX << " / "  << previousY << " / " << currentX << std::endl;
        lines.emplace_back(previousX, previousY, currentX, currentY);
        previousX = currentX;
        previousY = currentY;
        currentX += graphScale;
        currentY =  ScreenHeight() - graphScale - (currentNumber*graphScale);
        for (auto& line : lines) {
            line.draw(this);
            FillCircle(line.x1, line.y1, graphScale/6, olc::RED);
        }
    }

private:

    int64_t odd(int64_t n) {
        return n * 3 + 1;
    };

    int64_t even(int64_t n) {
        return n / 2;
    };

private:

    void DrawGraph() {
        // draw rows
        for (int i = 0; i < ScreenHeight(); i+=graphScale) {
            DrawLine(0, i, ScreenWidth(), i, olc::GREY);
        }

        // draw columns
        for (int i = 0; i < ScreenWidth(); i+=graphScale) {
            DrawLine(i, 0, i, ScreenHeight(), olc::GREY);
        }

        // draw abscissa
        DrawLine(graphScale, 0, graphScale, ScreenHeight(), olc::BLACK);

        // draw ordinate
        DrawLine(0, ScreenHeight() - graphScale, ScreenWidth(), ScreenHeight() - graphScale, olc::BLACK);

        // draw abscissa label
        DrawString(graphScale - graphScale / 2, ScreenHeight() - graphScale + 5, "0", olc::BLACK);

    }

private:

    void HandleInputs() {
        HandleKeyBoardInputs();
        HandleMouseInputs();
    }

    void HandleKeyBoardInputs() {
        if (GetKey(olc::Key::SPACE).bPressed) {
            running = !running;
            sAppName = running ? "Syracuse Visualization (Running)" : "Syracuse Visualization (Paused)";
        }

        if (GetKey(olc::Key::R).bPressed) {
            lines.clear();
            running = false;
            sAppName = running ? "Syracuse Visualization (Running)" : "Syracuse Visualization (Paused)";
        }
    }

    void HandleMouseInputs() {

        if (GetMouseWheel() > 0) {
            updateTime += 0.05f;
        } else if (GetMouseWheel() < 0 && updateTime > 0.0f) {
            updateTime -= 0.05f;
        }

    }

};

int main() {
    SyracuseVisualization game;
    if (game.Construct(1000, 600, 1, 1))
        game.Start();
    return 0;
}
