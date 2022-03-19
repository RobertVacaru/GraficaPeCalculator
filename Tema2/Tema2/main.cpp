
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>

#include "glut.h"

#define dim 300


unsigned char prevKey;


void Display1() {
    double xmax, ymax, xmin, ymin;
    double a = 1, b = 2;
    double pi = 4 * atan(1);
    double ratia = 0.05;

   
    xmax = a - b - 1;
    xmin = a + b + 1;
    ymax = ymin = 0;
    for (double t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x1, y1, x2, y2;
        x1 = a + b * cos(t);
        xmax = (xmax < x1) ? x1 : xmax;
        xmin = (xmin > x1) ? x1 : xmin;

        x2 = a - b * cos(t);
        xmax = (xmax < x2) ? x2 : xmax;
        xmin = (xmin > x2) ? x2 : xmin;

        y1 = a * tan(t) + b * sin(t);
        ymax = (ymax < y1) ? y1 : ymax;
        ymin = (ymin > y1) ? y1 : ymin;

        y2 = a * tan(t) - b * sin(t);
        ymax = (ymax < y2) ? y2 : ymax;
        ymin = (ymin > y2) ? y2 : ymin;
    }

    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x1, y1, x2, y2;
        x1 = (a + b * cos(t)) / xmax;
        x2 = (a - b * cos(t)) / xmax;
        y1 = (a * tan(t) + b * sin(t)) / ymax;
        y2 = (a * tan(t) - b * sin(t)) / ymax;

        glVertex2f(x1, y1);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (double t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x1, y1, x2, y2;
        x1 = (a + b * cos(t)) / xmax;
        x2 = (a - b * cos(t)) / xmax;
        y1 = (a * tan(t) + b * sin(t)) / ymax;
        y2 = (a * tan(t) - b * sin(t)) / ymax;

        glVertex2f(x2, y2);
    }
    glEnd();
}


void Display2() {
    double pi = 4 * atan(1);
    double xmax = 8 * pi;
    double ymax = exp(1.1);
    double ratia = 0.05;

  
    glColor3f(1, 0.1, 0.1); 
    glBegin(GL_LINE_STRIP);
    for (double x = 0; x < xmax; x += ratia) {
        double x1, y1;
        x1 = x / xmax;
        y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

        glVertex2f(x1, y1);
    }
    glEnd();
}

void Display3() {
    double ratio = 0.05, marginX = 0.15, marginY = 0.05;
    double xmax = 30;
    glBegin(GL_LINE_STRIP);
    double y; 
    for (double x = 0; x <= xmax; x += ratio) {
        
        if (x == 0)
            y = 1;
        else 
            y = (fabs(x - round(x)) / x);
        glVertex2f(x/xmax-marginX  , y );
    }

    glEnd();
}
void Display4() {
    double ratio = 0.05;
    double a = 0.3, b = 0.2, pi = 4 * atan(1);
 

    glBegin(GL_LINE_LOOP);

    for (double t = -pi + ratio; t < pi; t += ratio) {
        double x = 2 * (a * cos(t) + b) * cos(t), y = 2 * (a * cos(t) + b) * sin(t);
        glVertex2f(x/1.1 , y/1.1 );
    }

    glEnd();
}
void Display5() {
    double pi = 4 * atan(1);
    double a = 0.2;
    double ratio = 0.005, margin = 0.05;

    glColor3f(0, 0.1, 1.0); // albastru

    glBegin(GL_LINE_STRIP);
    std::vector<double> xValues;
    std::vector<double> yValues;

    glVertex2f(-1 + margin, 1 - margin);

    for (double i = -pi / 2 + ratio; i < -pi / 6; i += ratio)
    {
        double c = cos(i) * cos(i);
        double x = std::max(a / (4.0 * c - 3.0), -1.0);
        double y = std::min((a * tan(i)) / (4.0 * c - 3.0), 1.0);
        glVertex2f(x + margin, y - margin);
        xValues.push_back(x);
        yValues.push_back(y);
    }

    glEnd();

    glLineWidth(1);
    int n = xValues.size();
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.1, 0.1); // rosu
    for (int i = 1; i < (n - 1) / 4; i += 2)
    {
        glVertex2f(-1 + margin, 1 - margin);
        glVertex2f(xValues[i] + margin, yValues[i] - margin);
        glVertex2f(xValues[i + 1] + margin, yValues[i + 1] - margin);
    }

    for (int i = 3 * (n - 1) / 4; i < (n - 1); i += 2)
    {
        glVertex2f(-1 + margin, 1 - margin);
        glVertex2f(xValues[i] + margin, yValues[i] - margin);
        glVertex2f(xValues[i + 1] + margin, yValues[i + 1] - margin);
    }
    glEnd();
}
void Display6() {
    double ratio = 0.05;
    double a = 0.1, b = 0.2, margin = 10; 
   

    glBegin(GL_LINE_STRIP);

    for (double t = -margin + ratio; t < margin; t += ratio)
    {
        double x = a * t - b * sin(t), y = a - b * cos(t);
        glVertex2f(x / 1.2, y );
    }

    glEnd();
}
void Display7() {
    double ratio = 0.05;
    double r = 0.3, R = 0.1, pi = 4 * atan(1);
  
    glBegin(GL_LINE_STRIP);

    for (double t = 0; t <= 2 * pi; t += ratio)
    {
        double x = (R + r) * cos((r / R) * t) - r * cos(t + (r / R) * t), y = (R + r) * sin((r / R) * t) - r * sin(t + (r / R) * t);
        glVertex2f(x , y );
    }

    glEnd();

}
void Display8() {
    double ratio = 0.05;
    double r = 0.3, R = 0.1, pi = 4 * atan(1);
    

    glBegin(GL_LINE_STRIP);

    for (double t = 0; t <= 2 * pi; t += ratio)
    {
        double x = (R - r) * cos((r / R) * t) - r * cos(t - (r / R) * t), y = (R - r) * sin((r / R) * t) - r * sin(t - (r / R) * t);
        glVertex2f(x , y );
    }

    glEnd();
}
void Display9() {
    double ratio = 0.01, scale = 0.5;
    double a = 0.4, pi = 4 * atan(1);
    double rmin = -a * sqrt(2 * cos(2 * (pi / 4))), rmax = a * sqrt(2 * cos(2 * (pi / 4)));
   
    glBegin(GL_LINE_STRIP);
    glVertex2f(0.0, 0.0);
    for (double t = (-pi / 4) + ratio; t < pi / 4; t += ratio)
    {
        double r = -a * sqrt(2 * cos(2 * t));
        double x = r * cos(t), y = r * sin(t);
        glVertex2f(x , y );
    }

    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex2f(0.0, 0.0);
    for (double t = (-pi / 4) + ratio; t < pi / 4; t += ratio)
    {
        double r = a * sqrt(2 * cos(2 * t));
        double x = r * cos(t), y = r * sin(t);
        glVertex2f(x , y );
    }

    glEnd();

}
void Display10() {
    double ratio = 0.05, tmax = 3.0, tmin = 0;
    double a = 0.02;

    glBegin(GL_LINE_STRIP);

    for (double t = tmin; t <= tmax; t += ratio)
    {
        double r = a * exp(1.0 + t);
        double x = r * cos(t), y = r * sin(t);
        glVertex2f(x/1.15, y);
    }

    glEnd();
}


void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    //   glPointSize(4);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (prevKey) {
    case '1':
        Display1();
        break;
    case '2':
        Display2();
        break;
    case '3':
        Display3();
        break;
    case '4':
        Display4();
        break;
    case '5':
        Display5();
        break;
    case '6':
        Display6();
        break;
    case '7':
        Display7();
        break;
    case '8':
        Display8();
        break;
    case '9':
        Display9();
        break;
    case '0':
        Display10();
        break;
    default:
        break;
    }

    glFlush();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitWindowSize(dim, dim);

    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutCreateWindow(argv[0]);

    Init();

    glutReshapeFunc(Reshape);

    glutKeyboardFunc(KeyboardFunc);

    glutMouseFunc(MouseFunc);

    glutDisplayFunc(Display);

    glutMainLoop();

    return 0;
}