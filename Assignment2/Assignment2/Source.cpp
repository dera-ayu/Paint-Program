#include <GL/glut.h> // Include the GLUT header file  
#include <Windows.h>
#include <math.h>
#define RED 2
#define YELLOW 3
#define BLUE 4
#define ERASER 5 

GLsizei winWidth = 500, winHeight = 500; // Initial display-window size.
float red = 1.0, green = 0.0, blue = 1.0;
int brushSz = 3; // Determine the initial size of the brush(point)
int shape = 1; // Determine the code for the shape
double tmpX, tmpY; // To save location of coordinate
int count = 0;

class scrPt {
public:
	GLint x, y;
};
static scrPt point[1];

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0); // Set display-window color to white.
	glColor3f(red, green, blue);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 250.0, 0.0, 250.0);
}
void displayFcn(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(3.0);
	glFlush();
}

void winReshapeFcn(GLint newWidth, GLint newHeight)
{
	/* Reset viewport and projection parameters */
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));
	/* Reset display-window size parameters. */
	winWidth = newWidth;
	winHeight = newHeight;
}

//Draw Point
void pointLine(GLint x, GLint y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd;
}

void key(unsigned char keys, int mouseX, int mouseY)
{
	switch (keys)
	{
	case '+':
		brushSz++;
		break;
	case '-':
		brushSz--;
		break;
	case 'e':
		exit(-1);
	default:
		break;
	}
}

//Draw Line

inline GLint round(const float a) { return int(a + 0.5); }

void lineDDA(double x0, double y0, double xEnd, double yEnd)
{
	float dx = xEnd - x0, dy = yEnd - y0, x, y;
	int steps, k;
	if (fabs(dx) > fabs(dy))
		steps = fabs(dx);
	else
		steps = fabs(dy);
	dx = float(dx) / float(steps);
	dy = float(dy) / float(steps);
	x = x0;
	y = y0;
	for (k = 0; k < steps; k++) {
		glBegin(GL_POINTS);
		glVertex2i(round(x), round(y));
		glEnd();
		glFlush();
		x = x + dx;
		y = y + dy;
	}
	glFlush();
}

//Draw Circle
void setPixelCircle(GLint xCoord, GLint yCoord)
{
	glBegin(GL_POINTS);
	glVertex2i(xCoord, yCoord);
	glEnd();
}
void drawCircle(int x1, int y1, int x2, int y2)
{
	int r = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	int x, y;

	x = 0;
	y = r;
	int p = 1 - r;

	while (x <= y)
	{
		x++;
		if (p < 0)
			p += 2 * x + 1;
		else
		{
			y--;
			p += 2 * (x - y) + 1;
		}
		setPixelCircle(x1 + x, y1 + y);
		setPixelCircle(x1 - x, y1 + y);
		setPixelCircle(x1 + x, y1 - y);
		setPixelCircle(x1 - x, y1 - y);
		setPixelCircle(x1 + y, y1 + x);
		setPixelCircle(x1 - y, y1 + x);
		setPixelCircle(x1 + y, y1 - x);
		setPixelCircle(x1 - y, y1 - x);
	}
}
//Draw Ellipse
void ellipseMidpoint(int xCenter, int yCenter, int Rx, int Ry)
{
	int Rx2 = Rx * Rx;
	int Ry2 = Ry * Ry;
	int twoRx2 = 2 * Rx2;
	int twoRy2 = 2 * Ry2;
	int p;
	int x = 0;
	int y = Ry;
	int px = 0;
	int py = twoRx2 * y;
	void ellipsePlotPoints(int, int, int, int);
	/* Plot the initial point in each quadrant. */
	ellipsePlotPoints(xCenter, yCenter, x, y);
	/* Region 1 */
	p = round(Ry2 - (Rx2 * Ry) + (0.25 * Rx2));
	while (px < py) {
		x++;
		px += twoRy2;
		if (p < 0)
			p += Ry2 + px;
		else {
			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		ellipsePlotPoints(xCenter, yCenter, x, y);
	}
	/* Region 2 */
	p = round(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y - 1) - Rx2 * Ry2);
	while (y > 0) {
		y--;
		py -= twoRx2;
		if (p > 0)
			p += Rx2 - py;
		else {
			x++;
			px += twoRy2;
			p += Rx2 - py + px;
		}
		ellipsePlotPoints(xCenter, yCenter, x, y);
	}
}
void ellipsePlotPoints(int xCenter, int yCenter, int x, int y)
{
	setPixelCircle(xCenter + x, yCenter + y);
	setPixelCircle(xCenter - x, yCenter + y);
	setPixelCircle(xCenter + x, yCenter - y);
	setPixelCircle(xCenter - x, yCenter - y);
}

//Mouse Function
void mouseMotion(int motX, int motY)
{
	glColor3f(red, green, blue);
	GLint x = motX;
	GLint y = winHeight - motY;
	if (shape == 1)
	{
		glRecti(x, y, x + brushSz, y + brushSz);
		glRecti(x, y, x + brushSz, y + brushSz);
	}
	glFlush();
}

void mousePtPlot(GLint button, GLint action, GLint xMouse, GLint yMouse)
{
	if (button == GLUT_LEFT_BUTTON && action == GLUT_UP) {
		if (shape == 2) {
			lineDDA(point[0].x, point[0].y, tmpX, tmpY);
		}
	}
	
	if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
	{
		glColor3f(red, green, blue);
		if (shape == 2) {
			if (count == 0) {
				point[count].x = xMouse;
				point[count].y = winHeight - yMouse;
				tmpX = xMouse;
				tmpY = winHeight - yMouse;
				count++;
			}
			else if (count == 1)
			{
				lineDDA(point[0].x, point[0].y, xMouse, winHeight - yMouse);
				point[0].x = xMouse;
				point[0].y = winHeight - yMouse;
			}
		}
		else if (shape == 3)
			drawCircle(tmpX, tmpY, xMouse, winHeight - yMouse);
		else if (shape == 4)
			ellipseMidpoint(tmpX, tmpY, xMouse, winHeight - yMouse);
	}
	glFlush();
}

//Menu Function
void processMainMenu(int value)
{
	switch (value)
	{
	case 0:
		exit(-1);
	case 1:
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();
		break;
	}
	glutPostRedisplay();
}

void shapeMenu(int drawingType)
{
	shape = drawingType;

	switch (shape)
	{
	case 1:
		glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		break;
	case 2:
		break;
	case 3:
		glutSetCursor(GLUT_CURSOR_CROSSHAIR);
		break;
	}
	glutPostRedisplay();
}

void colorMenu(int option)
{
	switch (option)
	{
	case 1: //RED
		red = 1.0, green = 0.0, blue = 0.0;
		break;
	case 2: //YELLOW
		red = 1.0, green = 1.0, blue = 0.0;
		break;
	case 3: //BLUE
		red = 0.0, green = 0.0, blue = 1.0;
		break;
	case 4: //ERASER
		red = 1.0, green = 1.0, blue = 1.0;
	}
	glutPostRedisplay();
}

void createMenu()
{
	GLint color = glutCreateMenu(colorMenu);
	glutAddMenuEntry("RED", 1);
	glutAddMenuEntry("YELLOW", 2);
	glutAddMenuEntry("BLUE", 3);
	glutAddMenuEntry("ERASER", 4);

	GLint shapeSelect = glutCreateMenu(shapeMenu);
	glutAddMenuEntry("POINT", 1);
	glutAddMenuEntry("LINE", 2);
	glutAddMenuEntry("CIRCLE", 3);
	glutAddMenuEntry("ELLIPSE", 4);

	int main_id = glutCreateMenu(processMainMenu);
	glutAddSubMenu("SHAPES", shapeSelect);
	glutAddSubMenu("COLOUR", color);
	glutAddMenuEntry("CLEAR", 1);
	glutAddMenuEntry("QUIT", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(winWidth, winHeight);

	glutCreateWindow("Paint Aja");
	glutDisplayFunc(displayFcn);
	glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(mousePtPlot);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(key);
	init();
	createMenu();
	glutMainLoop();
	return 0;
}