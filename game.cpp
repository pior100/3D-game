#include <iostream>
#include <GL/glut.h> // do obsługi grafiki OpenGL
#include <cmath>

// Wymiary ekranu
const int screenWidth = 800;
const int screenHeight = 600;

// Stałe dotyczące gry
const float gravity = 0.02f;
const float jumpForce = 0.4f;
const float playerSpeed = 0.1f;
const int maxObstacles = 10;

// Struktura przechowująca informacje o przeszkodzie
struct Obstacle {
    float x, y, z;
    float size;
    bool active;
};

// Aktualne położenie gracza
float playerX = 0.0f;
float playerY = 0.0f;
float playerZ = 0.0f;
float playerYVelocity = 0.0f;
bool isJumping = false;

// Tablica przeszkód
Obstacle obstacles[maxObstacles];

// Licznik punktów
int score = 0;

// Funkcja inicjalizująca przeszkody
void initObstacles() {
    for (int i = 0; i < maxObstacles; ++i) {
        obstacles[i].x = static_cast<float>(rand() % 10 - 5); // Losowa pozycja X w zakresie -5 do 5
        obstacles[i].y = 0.0f;
        obstacles[i].z = static_cast<float>(-i * 10); // Przesunięcie przeszkód wzdłuż osi Z
        obstacles[i].size = static_cast<float>(rand() % 3 + 1); // Losowy rozmiar przeszkody
        obstacles[i].active = true;
    }
}

// Funkcja rysująca przeszkody
void drawObstacles() {
    for (int i = 0; i < maxObstacles; ++i) {
        if (obstacles[i].active) {
            glPushMatrix();
            glTranslatef(obstacles[i].x, obstacles[i].y, obstacles[i].z);
            glColor3f(1.0f, 0.0f, 0.0f); // Czerwony kolor przeszkody
            glutSolidCube(obstacles[i].size);
            glPopMatrix();
        }
    }
}

// Funkcja rysująca tekst
void drawText(const std::string& text, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, 0, screenHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f); // Biały kolor tekstu
    glRasterPos2f(x, y);

    for (const char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    glPopMatrix();
    glMatrixMode(GL);
MODELVIEW);
glPopMatrix();
glMatrixMode(GL_PROJECTION);
glPopMatrix();
glMatrixMode(GL_MODELVIEW);
}
// Funkcja sprawdzająca kolizje
bool checkCollision(float x1, float y1, float z1, float size1, float x2, float y2, float z2, float size2) {
if (std::abs(x1 - x2) < (size1 + size2) / 2 &&
std::abs(y1 - y2) < (size1 + size2) / 2 &&
std::abs(z1 - z2) < (size1 + size2) / 2) {
return true;
}
return false;
}

// Funkcja aktualizująca logikę gry
void updateGame() {
// Aktualizacja położenia gracza
playerX += playerSpeed;
playerYVelocity -= gravity;
playerY += playerYVelocity;
// Sprawdzenie kolizji z przeszkodami
for (int i = 0; i < maxObstacles; ++i) {
    if (obstacles[i].active && checkCollision(playerX, playerY, playerZ, 1.0f, obstacles[i].x, obstacles[i].y, obstacles[i].z, obstacles[i].size)) {
        // Kolizja z przeszkodą
        obstacles[i].active = false;
        score -= 10; // Kara za kolizję
    }
}

// Skok gracza
if (isJumping && playerY <= 0.0f) {
    playerY = 0.0f;
    playerYVelocity = jumpForce;
    isJumping = false;
}

// Aktualizacja przeszkód
for (int i = 0; i < maxObstacles; ++i) {
    if (obstacles[i].active) {
        obstacles[i].z += playerSpeed;
        if (obstacles[i].z > 5.0f) {
            obstacles[i].active = false; // Przeszkoda opuściła planszę
            score += 10; // Punkt za zniszczenie przeszkody
        }
    }
}
// Funkcja wywoływana przy rysowaniu
void drawScene() {
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
glLoadIdentity();
// Ustawienie kamery na pozycji gracza
gluLookAt(playerX, playerY + 1.0f, playerZ + 5.0f, playerX, playerY, playerZ, 0.0f, 1.0f, 0.0f);

// Rysowanie przeszkód
drawObstacles();

// Rysowanie gracza
glPushMatrix();
glTranslatef(playerX, playerY, playerZ);
glColor3f(0.0f, 1.0f, 0.0f); // Zielony kolor gracza
glutSolidCube(1.0f);
glPopMatrix();

// Rysowanie tekstu z punktacją
drawText("Score: " + std::to_string(score), 10, screenHeight - 30);

glutSwapBuffers();
// Funkcja obsługująca naciśnięcie klawisza
void keyboard(unsigned char key, int x, int y) {
if (key == ' ') {
if (!isJumping && playerY <=0.1f) {
isJumping = true;
}
}

scss
Copy code
glutPostRedisplay();
}
/ Funkcja wywoływana przy aktualizacji logiki gry
void update(int value) {
updateGame();
glutPostRedisplay();
glutTimerFunc(16, update, 0); // 60 klatek na sekundę (1000ms/60 ≈ 16ms)
}

int main(int argc, char** argv) {
// Inicjalizacja GLUT
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
glutInitWindowSize(screenWidth, screenHeight);
glutCreateWindow("Moja Zaawansowana Gra 3D");
// Inicjalizacja OpenGL
glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Czarny kolor tła

// Ustawienie parametrów kamery
glMatrixMode(GL_PROJECTION);
gluPerspective(45.0, (double)screenWidth / (double)screenHeight, 1.0, 200.0);
glMatrixMode(GL_MODELVIEW);

// Inicjalizacja przeszkód
initObstacles();

// Rejestracja funkcji obsługi
glutDisplayFunc(drawScene);
glutKeyboardFunc(keyboard);
glutTimerFunc(0, update, 0);

// Rozpoczęcie pętli głównej programu
glutMainLoop();

return 0;

