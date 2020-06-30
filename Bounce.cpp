#include<stdlib.h>
#include<GL/glut.h>
#include<stdio.h>
#include<string.h>
#include <math.h>
int score = 0;
int max_score = 0;
int life = 3;
int disp_flag = 0;
int brick_color = 1, ball_color = 3, level = 0, paddle_color = 2, text_color = 5, size = 2;
GLfloat twoModel[] = { GL_TRUE };
int game_level[] = { 20,15,10 };
GLfloat rate = game_level[level];
int brick_count = 0;
GLfloat brick_color_array[][3] = { {1,0,0},{0,0,1},{0,1,0},{1,0,1},{1,1,0},{0,1,1} };
GLfloat paddle_color_array[][3] = { {1,0,0},{0,0,1},{0,1,0},{1,0,1},{1,1,0},{0,1,1} };
GLfloat text_color_array[][4] =
{ {1,0,0,1},{0,0,1,1},{0,1,0,1},{1,0,1,1},{1,1,0,1},{0,1,1,1} };
GLfloat paddle_size[] = { 2,4,6 };
//The grid parameters for the bricks
int rows = 4;
int columns = 10;
// Structure to store the coordinates of each brick
struct brick_coords {
	GLfloat x;
	GLfloat y;
	GLint type;
};
//Array to store the bricks
brick_coords brick_array[50][50];
GLfloat px, bx = 0, by = -12.8, speed = 0, dirx = 0, diry = 0, start = 0;
// Function to draw the paddle
void draw_paddle()
{
	glDisable(GL_LIGHTING);
	glColor3fv(paddle_color_array[paddle_color]);
	glBegin(GL_POLYGON);
	glVertex3f(-paddle_size[size] + px, 0 - 15, 0);
	glVertex3f(paddle_size[size] + px, 0 - 15, 0);
	glVertex3f(paddle_size[size] + px, 1 - 15, 0);
	glVertex3f(-paddle_size[size] + px, 1 - 15, 0);
	glEnd();
	glEnable(GL_LIGHTING);
}
//Function to draw a single brick
void brick(GLfloat x, GLfloat y, GLfloat z, GLint type)
{
	glDisable(GL_LIGHTING);
	if (type == 0)
		glColor3fv(brick_color_array[brick_color]);
	else
		glColor3fv(brick_color_array[(brick_color + 1) % 6]);
	glBegin(GL_QUADS);
	glVertex3f(x, y, z);
	glVertex3f(x + 3, y, z);
	glVertex3f(x + 3, y + 1, z);
	glVertex3f(x, y + 1, z);
	glEnd();
	glEnable(GL_LIGHTING);
}
// Function to draw hearts
void draw_hearts()
{
	int hx = -15;
	int hy = 12;

	for (int i = 0; i < life; i++)
	{
		glDisable(GL_LIGHTING);
		glColor3fv(brick_color_array[0]); // Color Red
		glBegin(GL_POLYGON);
		glVertex2f(hx, hy - 0.2);
		glVertex2f(hx - 0.6, hy + 0.5);
		//glVertex2f(hx-0.3, hy+0.4);
		glVertex2f(hx - 0.3, hy + 1);
		glVertex2f(hx - 0.1, hy + 0.8);
		glVertex2f(hx, hy + 0.6);
		glVertex2f(hx + 0.1, hy + 0.8);
		glVertex2f(hx + 0.3, hy + 1);
		//glVertex2f(hx+0.3, hy+0.4);
		glVertex2f(hx + 0.6, hy + 0.5);
		glEnd();
		glEnable(GL_LIGHTING);
		hx += 2;
	}
}
// Function to draw the grid of bricks
void draw_bricks()
{

	int i, j;
	if (start == 0 && life == 3)
	{

		for (i = 1; i <= rows; i++)
		{
			for (j = 1; j <= columns; j++)
			{

				brick_array[i][j].x = (GLfloat)(j * 4 * 0.84);
				brick_array[i][j].y = (GLfloat)(i * 2 * 0.6);
				brick_array[i][j].type = (GLint)(rand() % 2);
			}
		}
	}

	glPushMatrix();
	glTranslatef(-19.5, 5, 0);
	for (i = 1; i <= rows; i += 1)
	{
		for (j = 1; j <= columns; j += 1)
		{
			if (brick_array[i][j].x == 0 || brick_array[i][j].y == 0)
			{
				continue;
			}
			brick(brick_array[i][j].x, brick_array[i][j].y, 0, brick_array[i][j].type);

		}
	}
	glPopMatrix();

}
//Function to draw the spherical ball
void draw_ball()
{
	GLfloat ambient1[] = { 1,1,1 };
	GLfloat diffuse1[] = { 0.4,0.4,0.4 };
	GLfloat specular1[] = { 1,1,1 };

	GLfloat position[] = { 0,0,-50,1 };
	GLfloat ambient2[] = { 0,0,0 };
	GLfloat diffuse2[] = { 1,1,1 };
	GLfloat specular2[] = { 0,1,1 };
	float materialColours[][3] = { {1,0,0},{0,0,1},{0,1,0},{1,0,1},{1,1,0},{0,1,1} };
	GLfloat matAmbient1[] = { 1,1,1 };
	GLfloat matDiffuse1[] = { 1,1,1 };
	GLfloat matSpecular1[] = { 1,1,1 };
	GLfloat shininess[] = { 1000 };

	glLightfv(GL_LIGHT0, GL_SPECULAR, specular1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse1);
	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular2);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialColours[ball_color]);
	glPushMatrix();
	glTranslatef(bx, by, 0);
	glScalef(1.0, 1.0, 0.5);
	//glScalef(size[i], size[], size[]);
	glutSolidSphere(1.0, 52, 52);
	glPopMatrix();
}
//Mouse function
void mousemotion(int x, int y)
{
	if (start == 1)
	{
		px = (x - glutGet(GLUT_WINDOW_WIDTH) / 2) / 20;
		if (px > 16)
		{
			px = 16;
		}
		if (px < -15)
		{
			px = -15;
		}
	}
	else
		glutSetCursor(GLUT_CURSOR_INHERIT);
}
//Handle brick color
void change_brick_color(int action)
{
	brick_color = action - 1;
}
//Handle ball color
void change_ball_color(int action)
{
	ball_color = action - 1;
}
//Handle level
void change_difficulty(int action)
{
	level = action - 1;
}
//Handle menu

void handle_menu(int action)
{
}
//Handle paddle color
void change_paddle_color(int action)
{
	paddle_color = action - 1;
}
//Handle text color
void change_text_color(int action)
{
	text_color = action - 1;
}
//Handle paddle size
void change_paddle_size(int action)
{
	size = action - 1;
}
//add menu
void addMenu()
{
	int submenu1 = glutCreateMenu(change_brick_color);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Blue", 2);


	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Purple", 4);
	glutAddMenuEntry("Yellow", 5);
	glutAddMenuEntry("Cyan", 6);
	int submenu2 = glutCreateMenu(change_ball_color);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Blue", 2);
	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Purple", 4);
	glutAddMenuEntry("Yellow", 5);
	glutAddMenuEntry("Cyan", 6);
	int submenu4 = glutCreateMenu(change_paddle_color);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Blue", 2);
	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Purple", 4);
	glutAddMenuEntry("Yellow", 5);
	glutAddMenuEntry("Cyan", 6);
	int submenu3 = glutCreateMenu(change_difficulty);
	glutAddMenuEntry("Easy", 1);
	glutAddMenuEntry("Medium", 2);
	glutAddMenuEntry("Hard", 3);
	int submenu5 = glutCreateMenu(change_text_color);
	glutAddMenuEntry("Red", 1);

	glutAddMenuEntry("Blue", 2);
	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Purple", 4);
	glutAddMenuEntry("Yellow", 5);
	glutAddMenuEntry("Cyan", 6);
	int submenu6 = glutCreateMenu(change_paddle_size);
	glutAddMenuEntry("Small", 1);
	glutAddMenuEntry("Medium", 2);
	glutAddMenuEntry("Large", 3);
	glutCreateMenu(handle_menu);
	glutAddSubMenu("Bricks Color", submenu1);
	glutAddSubMenu("Ball Color", submenu2);
	glutAddSubMenu("Paddle Color", submenu4);
	glutAddSubMenu("Text Color", submenu5);
	glutAddSubMenu("Difficulty", submenu3);
	glutAddSubMenu("Paddle Size", submenu6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//Function to draw boundary
void draw_boundary()
{
	glDisable(GL_LIGHTING);
	glColor3fv(brick_color_array[4]);
	glBegin(GL_LINE_LOOP);

	glVertex2f(-17, -14.4);
	glVertex2f(18, -14.4);
	glVertex2f(18, 14.3);
	glVertex2f(-17, 14.3);
	glEnd();
	glEnable(GL_LIGHTING);
}
//Function to print the score on the screen
void text(int brick_count, int sc)
{
	glDisable(GL_LIGHTING);
	char text[50];
	char difficulty[10];
	char max_string[20];
	char start_hint[20];
	sprintf_s(max_string, "Maximum Score: %d", max_score);
	if (level == 0)
	{
		sprintf_s(difficulty, "Easy");
	}
	if (level == 1)
	{
		sprintf_s(difficulty, "Medium");
	}
	if (level == 2)
	{
		sprintf_s(difficulty, "Hard");
	}
	if (brick_count < 40 && disp_flag == 1) {
		sprintf_s(text, " Sorry, you lost!");
	}
	else if (brick_count < 40 && life != 0)
		sprintf_s(text, "Difficulty: %s Your Score: %d", difficulty, sc);
	else
	{
		sprintf_s(text, " You have won!");
		start = 0;
		by = -12.8;
		bx = 0;
		dirx = 0;
		diry = 0;
		px = 0;
		life = 3;
	}
	// The color
	glColor4fv(text_color_array[text_color]);
	// Position of the text to be printer

	glPushMatrix();
	glTranslatef(-1, 0, 0);
	glRasterPos3f(0, 0, 20);
	for (int i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
	glRasterPos3f(0.4, -0.5, 20);
	for (int i = 0; max_string[i] != '\0'; i++)

		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, max_string[i]);

	if (start == 0) {
		glRasterPos3f(0.2, -1, 20);
		//printf(start_hint, "Press \'S\' to start the game!");
		for (int i = 0; start_hint[i] != '\0'; i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, start_hint[i]);
	}
	glEnable(GL_LIGHTING);
	glPopMatrix();
}
//The main display function
void idisplay(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, -25, 0, 1, 0);
	glTranslatef(0, 0, -25);

	draw_boundary();
	draw_paddle();
	draw_bricks();
	draw_ball();
	draw_hearts();
	text(brick_count, score);
	glutSwapBuffers();
}
//function to turn on lights
void lightsOn()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}
//function to take in keyboard entries
void keyboard(unsigned char key, int x, int y)

{
	switch (key)
	{
	case 'd': px += 3; break;
	case 'a': px -= 3; break;
	case 'q': exit(0); break;
	case 's':
		if (!start)
		{
			if (rand() % 2)

				dirx = 1;
			else
				dirx = -1;

			diry = 1;
			rate = game_level[level];
			start = 1;
			disp_flag = 0;
			if (life == 3) {

				score = 0;
				brick_count = 0;
			}
			glutSetCursor(GLUT_CURSOR_NONE);
		}
		break;
	}
	if (px > 15)
	{
		px = 15;
	}
	if (px < -15)
	{
		px = -15;
	}
	if (start == 0)
	{
		px = 0;
	}
	glutPostRedisplay();
}
//Function to handle the case when the ball strikes the bricks
void hit()
{
	int i, j;
	for (i = 1; i <= rows; i++)
		for (j = 1; j <= columns; j++)
		{
			if ((bx >= brick_array[i][j].x - 19.5 - 0.1) && (bx <= brick_array[i][j].x + 3 - 19.5 + 0.1))

			{
				if (by >= brick_array[i][j].y + 5 - 0.1 && by

					<= brick_array[i][j].y + 5 + 1.2 + 0.1)
				{
					brick_array[i][j].x = 0;
					brick_array[i][j].y = 0;
					diry = diry * -1;
					if (brick_array[i][j].type == 0)
						score++;
					else
						score += 2;
					brick_count++;

					max_score = (max_score > score) ? max_score : score;
				}
				//cout<<bx<<" "<<by<<"\t"<<brick_array[i][j].x<<""<<brick_array[i][j].y;
			}
			else if (by >= brick_array[i][j].y + 5 - 0.1 && by <= brick_array[i][j].y + 5

				+ 1.2 + 0.1)

			{
				if ((bx >= brick_array[i][j].x - 19.5 - 0.1) && (bx <= brick_array[i][j].x + 3 - 19.5 + 0.1))

				{
					brick_array[i][j].x = 0;
					brick_array[i][j].y = 0;
					dirx = dirx * -1;

					if (brick_array[i][j].type == 0)

						score++;
					else
						score += 2;
					brick_count++;
					max_score = (max_score > score) ? max_score : score;

				}
			}
		}

}
//The idle function. Handles the motion of the ball along with rebounding from varioussurfaces
void idle()
{
	hit();
	if (bx < -16 || bx>17 && start == 1)
	{
		dirx = dirx * -1;
	}
	if (by < -15 || by>14 && start == 1)
	{
		diry = diry * -1;
	}
	bx += dirx / (rate);
	by += diry / (rate);
	rate -= 0.001; // Rate at which the speed of ball increases

	float x = paddle_size[size];
	//Make changes here for the different position of ball after rebounded by paddle
	if (by <= -12.8 && bx<(px + x * 2 / 3) && bx>(px + x / 3) && start == 1)
	{
		dirx = 1;
		diry = 1;
	}
	else if (by <= -12.8 && bx<(px - x / 3) && bx>(px - x * 2 / 3) && start == 1)
	{
		dirx = -1;
		diry = 1;
	}
	else if (by <= -12.8 && bx<(px + x / 3) && bx>(px - x / 3) && start == 1)
	{
		dirx = dirx;
		diry = 1;
	}
	else if (by <= -12.8 && bx<(px - (x * 2 / 3)) && bx>(px - (x + 0.3)) && start == 1)
	{
		dirx = -1.5;
		diry = 0.8;
	}
	else if (by <= -12.8 && bx<(px + (x + 0.3)) && bx>(px + x / 3) && start == 1)
	{
		dirx = 1.5;
		diry = 0.8;
	}
	else if (by < -13)
	{
		life = life - 1;
		start = 0;
		by = -12.8;
		bx = 0;
		dirx = 0;
		diry = 0;
		px = 0;
		if (life == 0) {
			disp_flag = 1;
			life = 3;
		}
	}
	glutPostRedisplay();
}
int display(void) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Gonzo");
	glutDisplayFunc(idisplay);
	glutReshapeFunc(reshape);
	glEnable(GL_DEPTH_TEST);
	glutIdleFunc(idle);
	glutPassiveMotionFunc(mousemotion);
	glutKeyboardFunc(keyboard);
	lightsOn();
	addMenu();
	return 0;
}

int flag = 0;

void drawstring(int x, int y, const char* string)
{
	int i;
	int len = (int)strlen(string);
	glRasterPos2f(x, y);
	for (i = 0; i < len; i++)
	{
		glColor3f(0.0, 0.0, 0.0);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	}
}

void frontscreen()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0, 0, 1);
	drawstring(-20, 70, "GLOBAL ACADEMY OF TECHNOLOGY");
	glColor3f(0.7, 0, 1);
	drawstring(-35, 60, "DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING");
	glColor3f(1, 0.5, 0);
	drawstring(-45, 50, "COMPUTER GRAPHICS LABORATORY WITH MINIPROJECT (VI SEM - 15CSL68)");
	glColor3f(1, 0.5, 0);
	drawstring(-12, 30, "A MINI PROJECT TITLE:");
	glColor3f(1, 0, 0);
	drawstring(-20, 20, "---------GONZO GAME---------");
	glColor3f(1, 0.5, 0);
	drawstring(-20, 0, "BY:");
	glColor3f(0.5, 0, 0.5);
	drawstring(-15, -10, "KUSHAL KUMAR K A  (1GA16CS067)");
	glColor3f(1, 0.5, 0);
	drawstring(-45, -30, "UNDER THE GUIDANCE OF:");
	glColor3f(0.5, 0.2, 0.2);
	drawstring(-35, -40, "MRS.BHAGYASHRI R HANJI , ASSISTANT PROFESSOR,DEPT OF CSE");
	drawstring(-35, -50, "MRS.SOWMYA M , ASSISTANT PROFESSOR,DEPT OF CSE");
	glColor3f(1, 0.1, 1);
	drawstring(-20, -65, "<<<<<<<PRESS ENTER TO START>>>>>>>>");
	glFlush();
}

void mydisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (flag == 0)
		frontscreen();
	if (flag == 1)
		display();
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 13:if (flag == 0)
		flag = 1;
		break;
	}
	mydisplay();
}

void init1(void)
{
	glClearColor(1.0, 0.9, 0.7, 0);
	glOrtho(-100.0, 100.0, -100.0, 100.0, -50.0, 50.0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(1000, 480);
	glutCreateWindow("Start-Screen");
	init1();
	glutDisplayFunc(frontscreen);
	glutKeyboardFunc(myKeyboardFunc);
	glutMainLoop();
	return 0;
}
