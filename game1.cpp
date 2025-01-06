// DECODECODE SHAPES GAME 
//Necessary header files and libraries
#include <gl/glut.h>
#include <iostream>
#include <string>
#include <cmath>
#include <fstream> 
#include <vector>
using namespace std;
#define M_PI 3.14159265358979323846

//declaring all global variables
int windowWidth = 1280;
int windowHeight = 730;
int score = 0;
int highscore = 0;
int level = 1;
int qs = 1;
int ans[9] = { 2,3,1,2,1,0,2,1,2 };
float rectX, rectY, rectSize, rectWidth, rectHeight;
double mainmenubar[7] = { 550, 200, 200, 50,0.102, 0.306, 0.596 };
double optionrectangles[4][7] = {
    {50, 100, 250, 250, 0, 0, 0},
    {350, 100, 250, 250, 0, 0, 0},
    {650, 100, 250, 250, 0, 0, 0},
    {950, 100, 250, 250, 0, 0, 0},
};
double menurectangles[4][7] = {
    {220, 450, 200, 60, 0.102, 0.306, 0.596},
    {220, 350, 200, 60, 0.102, 0.306, 0.596},
    {220, 250, 200, 60, 0.102, 0.306, 0.596},
    {220, 150, 200, 60,  0.102, 0.306, 0.59}
};
bool mainmenu = true;
bool showquestion = false;
bool showhighscore = false;
bool resumegame = false;
bool gamewon = false;
bool gameover = false;

const int TIMER_WHEEL_SIZE = 8; // Timer duration (in seconds)
vector<vector<string>> timingWheel(TIMER_WHEEL_SIZE); // Timing wheel with 8 slots
int currentSlot = 0; // Points to the current active slot
bool timerRunning = false;
int timerCount = 8; // Tracks seconds remaining for the current question

void addEventToTimingWheel(int seconds, const string& event) {
    int targetSlot = (currentSlot + seconds) % TIMER_WHEEL_SIZE;
    timingWheel[targetSlot].push_back(event);
}

void processTimingWheel() {
    if (!timingWheel[currentSlot].empty()) {
        for (const string& event : timingWheel[currentSlot]) {
            if (event == "penalize") {
                score -= 50; 
                qs++;        
                if (qs > 9) { 
                    timerRunning = false;
                }
                else {
                    timerCount = 8; 
                    addEventToTimingWheel(8, "penalize");
                }
            }
        }
        timingWheel[currentSlot].clear(); 
    }
}

void timerFunction(int value) {
    if (timerRunning) {
        currentSlot = (currentSlot + 1) % TIMER_WHEEL_SIZE;
        timerCount--;
        if (timerCount < 0) timerCount = 8; 
        processTimingWheel();
        glutTimerFunc(1000, timerFunction, 0); 
        glutPostRedisplay(); 
    }
}

void resetTimer() {
    currentSlot = 0;                       
    timerCount = 8;                        
    timingWheel = vector<vector<string>>(TIMER_WHEEL_SIZE); 
    addEventToTimingWheel(8, "penalize");  
    if (!timerRunning) {
        timerRunning = true;
        glutTimerFunc(1000, timerFunction, 0); 
    }
}

void drawFilledCircle(float x, float y, float radius, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
    }
    glEnd();
}


void drawCircularTimer(float x, float y, float outerRadius, float innerRadius, int segments, int activeSegments) {
    float angleStep = 360.0f / segments;
    for (int i = 0; i < activeSegments; i++) {
        float startAngle = i * angleStep;
        float endAngle = (i + 1) * angleStep;

        glColor3f(0.102f, 0.306f, 0.596f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (float angle = startAngle; angle <= endAngle; angle += 1.0f) {
            float radian = angle * M_PI / 180.0f;
            glVertex2f(x + outerRadius * cos(radian), y + outerRadius * sin(radian));
        }
        glVertex2f(x + outerRadius * cos(endAngle * M_PI / 180.0f), y + outerRadius * sin(endAngle * M_PI / 180.0f));
        glEnd();
    }
    drawFilledCircle(x, y, innerRadius, 1.0f, 1.0f, 1.0f);
    glColor3f(0, 0, 0);
    string timeText = to_string(timerCount);
    float textWidth = timeText.length() * 9;
    glRasterPos2f(x - textWidth / 2, y - 6); 
    for (char c : timeText) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}

void init() { //user defined to perform additional initializations like setting clearcolour
    glClearColor(0.9569, 0.9176, 0.8784, 1.0);//sets the colour of window
    glMatrixMode(GL_PROJECTION);//set the matrix mode to projection
    glLoadIdentity();//load identity matrix to ensure that no transformation is applied initially
    gluOrtho2D(0, windowWidth, 0, windowHeight);//set up an orthographic projection for 2D scene
}

//functions to draw shapes
void rectangleborder(float x, float y, float width, float height, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

}
void rectangle(float x, float y, float width, float height, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}
void triangle(float x, float y, float sideLength, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x + sideLength, y);
    glVertex2f(x + sideLength / 2, y + (sideLength * sqrt(3)) / 2);
    glEnd();
}
void invertedtriangle(float x, float y, float sideLength, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x + sideLength / 2, y - (sideLength * sqrt(3)) / 2);
    glVertex2f(x + sideLength, y);
    glEnd();
}
void square(float x, float y, float sideLength, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    glVertex2f(x + sideLength, y);
    glVertex2f(x + sideLength, y + sideLength);
    glVertex2f(x, y + sideLength);
    glEnd();
}
void circle(float x, float y, float radius, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 300; i++) {
        float angle = 2.0f * 3.1416f * float(i) / float(300);
        float pointX = x + (radius * cos(angle));
        float pointY = y + (radius * sin(angle));
        glVertex2f(pointX, pointY);
    }
    glEnd();
}
void pentagon(float x, float y, float sideLength, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 5; ++i) {
        float angle = 2 * 3.14159265f / 5 * i + 3.14159265f / 2;
        glVertex2f(x + sideLength * cos(angle), y + sideLength * sin(angle));
    }
    glEnd();
}
void hexagon(float x, float y, float sideLength, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; ++i) {
        float angle = 2 * 3.14159265f / 6 * i + 3.14159265f / 2;
        glVertex2f(x + sideLength * cos(angle), y + sideLength * sin(angle));
    }
    glEnd();
}
void diamond(float x, float y, float sideLength, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_QUADS);
    glVertex2f(x + sideLength / 2, y);
    glVertex2f(x + sideLength, y + sideLength / 2);
    glVertex2f(x + sideLength / 2, y + sideLength);
    glVertex2f(x, y + sideLength / 2);
    glEnd();
}
void drawDottedLine(float startx, float starty, float endx, float endy, float red, float green, float blue) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0xAAAA);
    glColor3f(red, green, blue);
    glBegin(GL_LINES);
    glVertex2f(startx, starty);
    glVertex2f(endx, endy);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}
void drawInt(int value, int x, int y, float red, float green, float blue) {
    string stringValue = to_string(value);
    glColor3f(red, green, blue);
    glRasterPos2f(x, y);
    for (char c : stringValue) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}
void downmoon(float x, float y, float radius, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 180; i++) {
        float angle = 3.1416f * float(i) / float(180);
        float pointX = x + (radius * cos(angle));
        float pointY = y + (radius * sin(angle));
        glVertex2f(pointX, pointY);
    }
    glEnd();
}
void upmoon(float x, float y, float radius, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 180; i++) {
        float angle = 3.1416f * float(i) / float(180);
        float pointX = x + (radius * cos(angle));
        float pointY = y - (radius * sin(angle));
        glVertex2f(pointX, pointY);
    }
    glEnd();
}
void drawminion() {
    rectangle(850, 300, 150, 250, 1.0, 1.0, 0.0); // body
    circle(900, 520, 20, 1.0, 1.0, 1.0); // Left eyeball
    circle(950, 520, 20, 1.0, 1.0, 1.0); // Right eyeball
    circle(900, 520, 13, 0.0, 0.0, 0.0); // Left pupil
    circle(950, 520, 13, 0.0, 0.0, 0.0); // Right pupil
    rectangle(860, 300, 130, 150, 0.102, 0.306, 0.596); // clothing
    upmoon(925, 490, 20, 0, 0, 0);//face
    circle(857, 440, 20, 0.102, 0.306, 0.596); // Left hand
    circle(990, 440, 20, 0.102, 0.306, 0.596); // Right hand
    rectangle(850, 350, 30, 80, 0.102, 0.306, 0.596); // Left hand
    rectangle(970, 350, 30, 80, 0.102, 0.306, 0.596);
    rectangle(885, 220, 30, 80, 0.102, 0.306, 0.596); // Left leg
    rectangle(935, 220, 30, 80, 0.102, 0.306, 0.596); // Right leg
    rectangle(865, 220, 50, 20, 0, 0, 0);
    circle(870, 232, 13, 0.0, 0.0, 0.0); // Left shoe
    rectangle(935, 220, 50, 20, 0, 0, 0);
    circle(977, 232, 13, 0.0, 0.0, 0.0); // Right shoe


}
void drawhalfminion() {
    rectangle(1050, 430, 100, 140, 1.0, 1.0, 0.0); //body
    circle(1085, 540, 15, 1.0, 1.0, 1.0); // Left eye 
    circle(1115, 540, 15, 1.0, 1.0, 1.0); // Right eye 
    circle(1085, 540, 10, 0.0, 0.0, 0.0); // Left pupil
    circle(1115, 540, 10, 0.0, 0.0, 0.0); // Right pupil
    rectangle(1055, 420, 90, 90, 0.102, 0.306, 0.596); //clothing
}

bool checkMouseClick(int mouseX, int mouseY, float x, float y, float width, float height) { //check if mouse is within specific rectangle
    return mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;
}

// Mouse click handling for gameplay
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (showquestion) {
            for (int i = 0; i < 4; ++i) {
                rectX = optionrectangles[i][0];
                rectY = optionrectangles[i][1];
                rectSize = optionrectangles[i][2];
                if (checkMouseClick(x, windowHeight - y, rectX, rectY, rectSize, rectSize)) {
                    if (i == ans[qs - 1]) { 
                        optionrectangles[i][4] = 0;
                        optionrectangles[i][5] = 1;
                        optionrectangles[i][6] = 0;
                        score += 100; 
                        qs++;         
                        if (qs <= 9) {
                            resetTimer(); // Reset timer for the next question
                        }
                    }
                    else { // Wrong Answer
                        optionrectangles[i][4] = 1;
                        optionrectangles[i][5] = 0;
                        optionrectangles[i][6] = 0;
                        score -= 50; // Penalize score for incorrect answer
                    }
                }
            }
        }
        if (mainmenu) {
            for (int i = 0; i < 4; ++i) {
                rectX = menurectangles[i][0];
                rectY = menurectangles[i][1];
                rectWidth = menurectangles[i][2];
                rectHeight = menurectangles[i][3];
                if (checkMouseClick(x, windowHeight - y, rectX, rectY, rectWidth, rectHeight)) {
                    if (i == 0) { // Start Game
                        score = 0;
                        showquestion = true;
                        mainmenu = false;
                        gameover = false;
                        showhighscore = false;
                        resumegame = false;
                        gamewon = false;
                        resetTimer(); // Start timer when the game starts
                    }
                    else if (i == 2) {
                        showhighscore = true;
                        mainmenu = false;
                        showquestion = false;
                        gameover = false;
                        resumegame = false;
                        gamewon = false;
                    }
                    else if (i == 3) {
                        exit(0);
                    }
                }
            }
        }
        if (gameover) {
            // Check if "Try Again" button is clicked
            if (checkMouseClick(x, windowHeight - y, mainmenubar[0], mainmenubar[1], mainmenubar[2], mainmenubar[3])) {
                score = 0;
                showquestion = true;
                gameover = false;
                mainmenu = false;
                gamewon = false;
                qs = 1; // Reset question index
                resetTimer(); // Restart the timer
            }
        }
    }
    glutPostRedisplay(); //update screen
}

void mouseHover(int x, int y) { //transitions in colours of various rectangles when mouse hovers over them
    bool isInsideButton = checkMouseClick(x, windowHeight - y, 550, 200, 200, 50);
    if (isInsideButton) {
        mainmenubar[4] = 0;
        mainmenubar[5] = 0;
        mainmenubar[6] = 0.5;
    }
    else {
        mainmenubar[4] = 0.102;
        mainmenubar[5] = 0.306;
        mainmenubar[6] = 0.596;

    }
    for (int i = 0; i < 4; ++i) {
        rectX = optionrectangles[i][0];
        rectY = optionrectangles[i][1];
        rectSize = optionrectangles[i][2];
        bool isHover = checkMouseClick(x, windowHeight - y, rectX, rectY, rectSize, rectSize);
        if (isHover) {
            optionrectangles[i][4] = 0;
            optionrectangles[i][5] = 0;
            optionrectangles[i][6] = 1;
        }
        else {
            optionrectangles[i][4] = 0;
            optionrectangles[i][5] = 0;
            optionrectangles[i][6] = 0;
        }
    }
    for (int i = 0; i < 4; ++i) {
        rectX = menurectangles[i][0];
        rectY = menurectangles[i][1];
        rectWidth = menurectangles[i][2];
        rectHeight = menurectangles[i][3];
        bool isHover = checkMouseClick(x, windowHeight - y, rectX, rectY, rectWidth, rectHeight);
        if (isHover) {
            menurectangles[i][4] = 0;
            menurectangles[i][5] = 0;
            menurectangles[i][6] = 0.5;
        }
        else {
            menurectangles[i][4] = 0.102;
            menurectangles[i][5] = 0.306;
            menurectangles[i][6] = 0.596;
        }
    }
    glutPostRedisplay(); //  update display
}

void text(const string& text, float x, float y, float red, float green, float blue) {//to write text on open gl window
    glColor3f(red, green, blue);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}


void saveHighScore(int highestScore) {
    ofstream outfile("highscore.txt");
    if (outfile.is_open()) {
        outfile << highestScore;
        outfile.close();
    }
}
void savegameprogress(int qs) {
    ofstream outfile("gameprogress.txt");
    if (outfile.is_open()) {
        outfile << qs << endl;
        outfile.close();
    }
}
int loadgameprogress() {
    ifstream infile("gameprogress.txt");
    if (infile.is_open()) {
        infile >> qs;

        infile.close();
    }
    return level, qs;
}

int loadHighScore() {
    ifstream infile("highscore.txt");
    if (infile.is_open()) {
        infile >> highscore;
        infile.close();
    }
    return highscore;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (mainmenu) {
        glClear(GL_COLOR_BUFFER_BIT);
        rectangleborder(100, 100, 1080, 530, 0.102, 0.306, 0.596);
        rectangle(200, 620, 900, 60, 0.102, 0.306, 0.596);
        text("DECODECODE", 550, 640, 1, 1, 1);
        for (int i = 0; i < 4; i++) {
            rectangle(menurectangles[i][0], menurectangles[i][1], menurectangles[i][2], menurectangles[i][3],
                menurectangles[i][4], menurectangles[i][5], menurectangles[i][6]);
        }
        text("PLAY", 290, 470, 1, 1, 1);
        text("RESUME", 270, 370, 1, 1, 1);
        text("HIGHSCORE", 250, 270, 1, 1, 1);
        text("QUIT GAME", 250, 170, 1, 1, 1);
        diamond(600, 400, 100, 0.102, 0.306, 0.596);
        square(600, 275, 100, 0.941, 0.871, 0.212);
        hexagon(760, 330, 50, 0.102, 0.306, 0.596);
        invertedtriangle(720, 500, 100, 0.941, 0.871, 0.212);
        circle(720, 410, 40, 0.102, 0.306, 0.596);
        drawminion();
    }
    if (showquestion) {
       
        mainmenu = false;
        gamewon = false;
        gameover = false;
        resumegame = false;
        showhighscore = false;
        glClear(GL_COLOR_BUFFER_BIT);
        int activeSegments = timerCount;
        drawCircularTimer(1100, 650, 70, 35, 8, activeSegments);
        if (score >= 0) {
            drawhalfminion();
            upmoon(1100, 530, 5, 0, 0, 0);
            text("LEVEL", windowWidth / 2 - 50, windowHeight - 50, 0.3608, 0.1804, 0.4941);
            drawInt(level, 670, 680, 0.3608, 0.1804, 0.4941);
            rectangleborder(50, 600, 900, 70, 0, 0, 0);
            text(" Which shape should come next in the sequence?", 400, 625, 0.0, 0.0, 0.0);
            drawDottedLine(830, 400, 950, 400, 0, 0, 0);
            rectangle(1000, 400, 200, 50, 0.2118, 0.1843, 0.8509);
            text("SCORE", 1030, 415, 1.0, 1.0, 1.0);
            drawInt(score, 1150, 415, 1.0, 1.0, 1.0);
            for (int i = 0; i < 4; i++) {
                rectangleborder(optionrectangles[i][0], optionrectangles[i][1], optionrectangles[i][2], optionrectangles[i][3],
                    optionrectangles[i][4], optionrectangles[i][5], optionrectangles[i][6]);
            }
            if (qs == 1) {
                //sequence
                triangle(200, 400, 150, 0.102, 0.306, 0.596);
                square(400, 400, 150, 0.941, 0.871, 0.212);
                triangle(600, 400, 150, 0.102, 0.306, 0.596);
                //options
                square(100, 150, 150, 0.102, 0.306, 0.596);
                triangle(400, 150, 150, 0.941, 0.871, 0.212);
                square(700, 150, 150, 0.941, 0.871, 0.212);
                triangle(1000, 150, 150, 0.102, 0.306, 0.596);
            }
            if (qs == 2) {
                //sequence
                triangle(100, 400, 150, 0.102, 0.306, 0.596);
                diamond(280, 400, 150, 0.941, 0.871, 0.212);
                triangle(480, 400, 150, 0.102, 0.306, 0.596);
                diamond(680, 400, 150, 0.941, 0.871, 0.212);
                //options
                triangle(100, 150, 150, 0.941, 0.871, 0.212);
                square(400, 150, 150, 0.102, 0.306, 0.596);
                diamond(700, 150, 150, 0.941, 0.871, 0.212);
                triangle(1000, 150, 150, 0.102, 0.306, 0.596);
            }
            if (qs == 3) {
                //sequence
                circle(150, 470, 70, 0.102, 0.306, 0.596);
                pentagon(340, 460, 75, 0.941, 0.871, 0.212);
                triangle(440, 400, 150, 0.102, 0.306, 0.596);
                circle(700, 470, 70, 0.941, 0.871, 0.212);
                //options
                circle(175, 225, 70, 0.941, 0.871, 0.212);
                pentagon(475, 215, 75, 0.102, 0.306, 0.596);
                triangle(700, 150, 150, 0.941, 0.871, 0.212);
                square(1000, 150, 150, 0.102, 0.306, 0.596);
            }
            if (qs == 4) {
                level = 2;
                //sequence
                pentagon(150, 460, 75, 0.941, 0.871, 0.212);
                diamond(265, 400, 150, 0.102, 0.306, 0.596);
                circle(525, 460, 70, 0.941, 0.871, 0.212);
                pentagon(720, 450, 75, 0.102, 0.306, 0.596);
                //options
                pentagon(175, 215, 75, 0.941, 0.871, 0.212);
                circle(475, 225, 70, 0.102, 0.306, 0.596);
                diamond(700, 150, 150, 0.941, 0.871, 0.212);
                diamond(1000, 150, 150, 0.102, 0.306, 0.596);
            }
            if (qs == 5) {
                level = 2;
                //sequence
                hexagon(150, 470, 70, 0.102, 0.306, 0.596);
                pentagon(340, 460, 75, 0.941, 0.871, 0.212);
                triangle(440, 400, 150, 0.102, 0.306, 0.596);
                hexagon(700, 470, 70, 0.941, 0.871, 0.212);
                //options
                hexagon(175, 225, 70, 0.941, 0.871, 0.212);
                pentagon(475, 215, 75, 0.102, 0.306, 0.596);
                diamond(700, 150, 150, 0.941, 0.871, 0.212);
                square(1000, 150, 150, 0.102, 0.306, 0.596);

            }
            if (qs == 6) {
                level = 2;
                //sequence
                triangle(60, 400, 150, 0.102, 0.306, 0.596);
                invertedtriangle(250, 530, 150, 0.941, 0.871, 0.212);
                invertedtriangle(440, 530, 150, 0.941, 0.871, 0.212);
                triangle(640, 400, 150, 0.102, 0.306, 0.596);
                //options
                invertedtriangle(100, 285, 150, 0.941, 0.871, 0.212);
                triangle(400, 150, 150, 0.102, 0.306, 0.596);
                triangle(700, 150, 150, 0.941, 0.871, 0.212);
                invertedtriangle(1000, 285, 150, 0.102, 0.306, 0.596);
            }
            if (qs == 7) {
                level = 3;
                //sequence
                pentagon(150, 460, 75, 0.941, 0.871, 0.212);
                diamond(265, 400, 150, 0.102, 0.306, 0.596);
                invertedtriangle(440, 530, 150, 0.941, 0.871, 0.212);
                pentagon(720, 450, 75, 0.102, 0.306, 0.596);
                //options
                pentagon(175, 215, 75, 0.941, 0.871, 0.212);
                invertedtriangle(415, 285, 150, 0.102, 0.306, 0.596);
                diamond(700, 150, 150, 0.941, 0.871, 0.212);
                diamond(1000, 150, 150, 0.102, 0.306, 0.596);
            }
            if (qs == 8) {
                level = 3;
                //sequence
                hexagon(150, 470, 70, 0.102, 0.306, 0.596);
                pentagon(340, 460, 75, 0.941, 0.871, 0.212);
                circle(520, 470, 70, 0.102, 0.306, 0.596);
                hexagon(700, 470, 70, 0.941, 0.871, 0.212);
                //options
                hexagon(175, 225, 70, 0.941, 0.871, 0.212);
                pentagon(475, 215, 75, 0.102, 0.306, 0.596);
                diamond(700, 150, 150, 0.941, 0.871, 0.212);
                square(1000, 150, 150, 0.102, 0.306, 0.596);

            }
            if (qs == 9) {
                level = 3;
                //sequence
                invertedtriangle(60, 530, 150, 0.102, 0.306, 0.596);
                triangle(250, 400, 150, 0.941, 0.871, 0.212);
                diamond(440, 400, 150, 0.941, 0.871, 0.212);
                invertedtriangle(640, 530, 150, 0.102, 0.306, 0.596);
                //options
                invertedtriangle(100, 285, 150, 0.941, 0.871, 0.212);
                triangle(400, 150, 150, 0.102, 0.306, 0.596);
                triangle(700, 150, 150, 0.941, 0.871, 0.212);
                diamond(1000, 160, 150, 0.102, 0.306, 0.596);

            }
            if (score >= highscore) {
                saveHighScore(score);
            }
            if (qs > 1) {
                savegameprogress(qs);
            }
            if (qs == 10) {
                gamewon = true;
            }
        }
        else if (score < 0) {
            gameover = true;
            gamewon = false;
            showquestion = false;
            showhighscore = false;
            mainmenu = false;
            resumegame = false;
        }
    }
    if (gamewon) {
        glClear(GL_COLOR_BUFFER_BIT);
        drawhalfminion();
        upmoon(1100, 530, 5, 0, 0, 0);
        text("YOU WON", 580, 450, 0, 1, 0);
        rectangle(1000, 400, 200, 50, 0.2118, 0.1843, 0.8509);
        text("SCORE", 1030, 415, 1.0, 1.0, 1.0);
        drawInt(score, 1150, 415, 1.0, 1.0, 1.0);
        rectangle(mainmenubar[0], mainmenubar[1], mainmenubar[2], mainmenubar[3], mainmenubar[4], mainmenubar[5], mainmenubar[6]);
        text("MAIN MENU", 580, 220, 1, 1, 1);

    }
    if (gameover) {
        showquestion = false;
        resumegame = false;
        showhighscore = false;
        mainmenu = false;
        gamewon = false;
        glClear(GL_COLOR_BUFFER_BIT);
        drawhalfminion();
        downmoon(1100, 520, 5, 0, 0, 0);
        rectangle(1000, 400, 200, 50, 0.2118, 0.1843, 0.8509);
        text("SCORE", 1030, 415, 1.0, 1.0, 1.0);
        drawInt(score, 1150, 415, 1.0, 1.0, 1.0);
        text("GAME OVER", 580, 450, 1, 0, 0);
        rectangle(mainmenubar[0], mainmenubar[1], mainmenubar[2], mainmenubar[3], mainmenubar[4], mainmenubar[5], mainmenubar[6]);
        text("TRY AGAIN", 580, 220, 1, 1, 1);
    }
    if (resumegame) {
        highscore = loadHighScore();
        qs = loadgameprogress();
        score = highscore;
        showquestion = true;
        resumegame = false;
        showhighscore = false;
        mainmenu = false;
        gameover = false;
        gamewon = false;
    }
    if (showhighscore) {
        loadHighScore();
        glClear(GL_COLOR_BUFFER_BIT);
        rectangleborder(100, 100, 1080, 530, 0.102, 0.306, 0.596);
        rectangle(200, 620, 900, 60, 0.102, 0.306, 0.596);
        text("DECODECODE", 550, 640, 1, 1, 1);
        rectangle(300, 350, 250, 150, 0.102, 0.306, 0.596);
        text("HIGH SCORE", 350, 440, 1, 1, 1);
        drawInt(highscore, 400, 400, 1, 1, 1);
        drawminion();
        rectangle(mainmenubar[0], mainmenubar[1], mainmenubar[2], mainmenubar[3], mainmenubar[4], mainmenubar[5], mainmenubar[6]);
        text("MAIN MENU", 580, 220, 1, 1, 1);
    }
    glFlush();
}

//main function
int main(int argc, char** argv) {
    //built in opengl functions
    glutInit(&argc, argv); //initialize glut and pass command line arguments
    glutInitWindowSize(windowWidth, windowHeight); //set the initial size of window
    glutCreateWindow("Decodecode"); //create a window with name
    glutDisplayFunc(display);//called when window needs to be redrawn
    glutPassiveMotionFunc(mouseHover);//called when mouse cursor moves without clicking
    glutMouseFunc(mouseClick);//called when mouse button is clicked
    init(); //user defined to perform additional initializations like setting clearcolour
    glutMainLoop(); //Event processing loop of GLUT checks for events like mouse, display until the application is closed.
    return 0;
}
