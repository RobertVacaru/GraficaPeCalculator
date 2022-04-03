#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "glut.h"

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

class CComplex {
public:
    CComplex() : re(0.0), im(0.0) {}

    CComplex(double re1, double im1) : re(re1 * 1.0), im(im1 * 1.0) {}

    CComplex(const CComplex& c) : re(c.re), im(c.im) {}

    ~CComplex() {}

    CComplex& operator=(const CComplex& c) {
        re = c.re;
        im = c.im;
        return *this;
    }

    double getRe() { return re; }

    void setRe(double re1) { re = re1; }

    double getIm() { return im; }

    void setIm(double im1) { im = im1; }

    double getModul() { return sqrt(re * re + im * im); }

    int operator==(CComplex& c1) {
        return ((re == c1.re) && (im == c1.im));
    }

    CComplex pow2() {
        CComplex rez;
        rez.re = powl(re * 1.0, 2) - powl(im * 1.0, 2);
        rez.im = 2.0 * re * im;
        return rez;
    }

    friend CComplex operator+(const CComplex& c1, const CComplex& c2);

    friend CComplex operator*(CComplex& c1, CComplex& c2);

    void print(FILE* f) {
        fprintf(f, "%.20f%+.20f i", re, im);
    }

private:
    double re, im;
};

CComplex operator+(const CComplex& c1, const CComplex& c2) {
    CComplex rez(c1.re + c2.re, c1.im + c2.im);
    return rez;
}

CComplex operator*(CComplex& c1, CComplex& c2) {
    CComplex rez(c1.re * c2.re - c1.im * c2.im,
        c1.re * c2.im + c1.im * c2.re);
    return rez;
}

int nivel = 0;


class C2coord {
public:
    C2coord() {
        m.x = m.y = 0;
    }

    C2coord(double x, double y) {
        m.x = x;
        m.y = y;
    }

    C2coord(const C2coord& p) {
        m.x = p.m.x;
        m.y = p.m.y;
    }

    C2coord& operator=(C2coord& p) {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(C2coord& p) {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

protected:
    struct SDate {
        double x, y;
    } m;
};

class CPunct : public C2coord {
public:
    CPunct() : C2coord(0.0, 0.0) {}

    CPunct(double x, double y) : C2coord(x, y) {}

    CPunct& operator=(const CPunct& p) {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    void getxy(double& x, double& y) {
        x = m.x;
        y = m.y;
    }

    int operator==(CPunct& p) {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    void marcheaza() {
        glBegin(GL_POINTS);
        glVertex2d(m.x, m.y);
        glEnd();
    }

    void print(FILE* fis) {
        fprintf(fis, "(%+f,%+f)", m.x, m.y);
    }

};

class CVector : public C2coord {
public:
    CVector() : C2coord(0.0, 0.0) {
        normalizare();
    }

    CVector(double x, double y) : C2coord(x, y) {
        normalizare();
    }

    CVector& operator=(CVector& p) {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(CVector& p) {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    CPunct getDest(CPunct& orig, double lungime) {
        double x, y;
        orig.getxy(x, y);
        CPunct p(x + m.x * lungime, y + m.y * lungime);
        return p;
    }

    void rotatie(double grade) {
        double x = m.x;
        double y = m.y;
        double t = 2 * (4.0 * atan(1.0)) * grade / 360.0;
        m.x = x * cos(t) - y * sin(t);
        m.y = x * sin(t) + y * cos(t);
        normalizare();
    }

    void deseneaza(CPunct p, double lungime) {
        double x, y;
        p.getxy(x, y);
        glColor3f(1.0, 0.1, 0.1);
        glBegin(GL_LINE_STRIP);
        glVertex2d(x/1.1, y/1.1);
        glVertex2d((x + m.x * lungime)/1.1, (y + m.y * lungime)/1.1);
        glEnd();
    }

    void print(FILE* fis) {
        fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
    }

private:
    void normalizare() {
        double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
        if (d != 0.0) {
            C2coord::m.x = C2coord::m.x * 1.0 / d;
            C2coord::m.y = C2coord::m.y * 1.0 / d;
        }
    }
};

class CMandelbrot {
public:
    CMandelbrot() {
        
    }


    int isIn(CComplex& x) {
        CComplex z0, zn;

        z0 = x;
        for (int i = 1; i < 30; i++) {
            zn = z0 * z0 + x;
            if (zn.getModul() > 2) {
                // convergenta la infinit
                return i;  
            }
            z0 = zn;
        }
        return 0; // punctul apartine multimii mandelbrot
    }

    void display(double xmin, double ymin, double xmax, double ymax) {
        glPushMatrix();
        glLoadIdentity();

        glScaled(0.5, 0.5, 1);
        glBegin(GL_POINTS);
        for (double x = xmin; x <= xmax; x += 0.01)
            for (double y = ymin; y <= ymax; y += 0.01) {
                CComplex z(x, y);
                int r = isIn(z);
                if (r == 0)
                    glColor3f(1, 0, 0);
                else
                    glColor3f(0, (double(r) / double(30)),0);  //  gradient 

                glVertex3d(x, y, 0);
            }

        fprintf(stdout, "STOP\n");
        glEnd();

        glPopMatrix();
    }

};


class Imaginea1 {
public:

    void recursie(double lungime, int nivel, CPunct p, CVector v) {
        if (nivel == 0) {
            deseneazaPatrat(p, lungime);
        }
        else {
            double x, y;
            CPunct p1;
            p.getxy(x, y);

            //centru
            deseneazaPatrat(p, lungime);

            //sus
            p1 = CPunct(x - lungime, y + lungime);
            recursie(lungime / 3, nivel - 1, p1, v);

            p1 = CPunct(x, y + lungime);
            recursie(lungime / 3, nivel - 1, p1, v);

            p1 = CPunct(x + lungime, y + lungime);
            recursie(lungime / 3, nivel - 1, p1, v);


            //dreapta stanga
            p1 = CPunct(x - lungime, y);
            recursie(lungime / 3, nivel - 1, p1, v);

            p1 = CPunct(x + lungime, y);
            recursie(lungime / 3, nivel - 1, p1, v);


            // jos
            p1 = CPunct(x - lungime, y - lungime);
            recursie(lungime / 3, nivel - 1, p1, v);

            p1 = CPunct(x, y - lungime);
            recursie(lungime / 3, nivel - 1, p1, v);

            p1 = CPunct(x + lungime, y - lungime);
            recursie(lungime / 3, nivel - 1, p1, v);
        }
    }

    void deseneazaPatrat(CPunct centru, double lungime) {
        double x, y;
        centru.getxy(x, y);
        //stanga sus
        CPunct colt = CPunct(x - lungime / 2, y + lungime / 2);
        CVector v = CVector(1, 0);
        for (int i = 0; i < 4; i++) {
            v.deseneaza(colt, lungime);
            colt = v.getDest(colt, lungime);
            v.rotatie(-90);
        }
    }

    void afisare(double lungime, int nivel) {
        CVector v(1, 0.0);
        CPunct p(0.0, 0.0);

        deseneazaPatrat(p, 1.99);

        recursie(lungime, nivel, p, v);
    }
};


class CArborePerron {
public:
    void arborePerron(double lungime,int nivel,double factordiviziune,CPunct p,CVector v) {
        assert(factordiviziune != 0);
        CPunct p1, p2;
        if (nivel != 0){
            v.rotatie(-45);
            v.deseneaza(p, lungime);
            p1 = v.getDest(p, lungime);
            arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            v.rotatie(+90);
            v.deseneaza(p, lungime);
            p1 = v.getDest(p, lungime);
            p2 = p1;

            v.rotatie(+10);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            p1 = p2;
            v.rotatie(-55);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            p2 = p1;

            lungime /= 2;
            v.rotatie(-90);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            arborePerron(lungime * 2 * factordiviziune, nivel - 1, factordiviziune, p1, v);

            p1 = p2;
            v.rotatie(+125);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            arborePerron(lungime * 2 * factordiviziune, nivel - 1, factordiviziune, p1, v);
        }
    }

    void afisare(double lungime, int nivel) {
        CVector v(0.0, -1.0);
        CPunct p(0.0, 1.0);

        v.deseneaza(p, 0.25);
        p = v.getDest(p, 0.25);
        arborePerron(lungime, nivel, 0.4, p, v);
    }
};

class Imaginea3 {
public:
    void recursie(double lungime, int nivel, CPunct p, CVector v, int semn) {
        if (nivel == 0) {
            v.deseneaza(p, lungime);
        }
        else {
            // desenare la +60/-60 - alternare semn
            v.rotatie(semn * 60);

            recursie(lungime / 2, nivel - 1, p, v, -semn);
            p = v.getDest(p, lungime / 2);

            v.rotatie(-semn * 60);

            recursie(lungime / 2, nivel - 1, p, v, semn);
            p = v.getDest(p, lungime / 2);

            v.rotatie(-semn * 60);

            recursie(lungime / 2, nivel - 1, p, v, -semn);
            p = v.getDest(p, lungime / 2);
        }
    }

    void afisare(double lungime, int nivel) {
        CVector v(0, 1);
        CPunct p(-0.3, -0.5);

        recursie(lungime, nivel, p, v, -1);
    }
};

void Display1() {
    CMandelbrot mandelbrot;

    mandelbrot.display(-2, -2, 2, 2);
}

void Display2() {
    Imaginea1 im;
    im.afisare(2.0 / 3.0, nivel);

    char c[3];
    sprintf_s(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    nivel++;
}

void Display3() {
    CArborePerron cap;
    glPushMatrix();
    glLoadIdentity();
    glScaled(0.4, 0.4, 1);
    glTranslated(-0.5, 0.5, 0.0);
    cap.afisare(1, nivel);
    glPopMatrix();

    char c[3];
    sprintf_s(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    nivel++;
}

void Display4() {
    Imaginea3 im;
    im.afisare(1, nivel);

    char c[3];
    sprintf_s
    (c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    nivel++;
}

void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    //   glPointSize(3);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
    switch (prevKey) {
    case '0':
        glClear(GL_COLOR_BUFFER_BIT);
        nivel = 0;
        fprintf(stderr, "nivel = %d\n", nivel);
        break;
    case '1':
        glClear(GL_COLOR_BUFFER_BIT);
        Display1();
        break;
    case '2':
        glClear(GL_COLOR_BUFFER_BIT);
        Display2();
        break;
    case '3':
        glClear(GL_COLOR_BUFFER_BIT);
        Display3();
        break;
    case '4':
        glClear(GL_COLOR_BUFFER_BIT);
        Display4();
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


