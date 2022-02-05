#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

GLboolean left1 = 0;
GLboolean right1 = 0;
GLboolean up1 = 0;
GLboolean down1 = 0;
GLboolean fire1 = 0;
GLboolean dead1 = 0;

GLboolean left2 = 0;
GLboolean right2 = 0;
GLboolean up2 = 0;
GLboolean down2 = 0;
GLboolean fire2 = 0;
GLboolean dead2 = 0;

const double velCoe = 0.1;

double xStar[35], yStar[35], starColor[35]; int starCount;

double colorPulsar = 0.0;

double xShip1 = -0.7, yShip1 = -0.7, xVel1 = 0.0, yVel1 = 0.0, shipAngle1 = M_PI/2;
double xShip2 = 0.7, yShip2 = 0.7, xVel2 = 0.0, yVel2 = 0.0, shipAngle2 = 3*M_PI/2;

double xLaser1, yLaser1, xLaserVel1, yLaserVel1, laserAngle1;
double xLaser2, yLaser2, xLaserVel2, yLaserVel2, laserAngle2;

double deathTick1 = 0.0, deathTick2 = 0.0;
double deathAngle1[8], deathAngle2[8]; int deathCount1, deathCount2;

unsigned int score1 = 0, score2 = 0;

/* WINDOW RESIZE FUNCTION */
void resize(int w, int h);

/* TIMER FUNCTION */
void timer(int n);

/* KEYBOARD FUNCTIONS */
void pressKeySpec(int key, int x, int y);
void pressKeyNorm(unsigned char key, int x, int y);
void releaseKeySpec(int key, int x, int y);
void releaseKeyNorm(unsigned char key, int x, int y);

/* PRE-GENERATION FUNCTION */
void generateStars(void);

/* MECHANICS AND GAMEPLAY FUNCTIONS */
void moveShip1(void);
void moveShip2(void);
void ship2AI(void);
void laser1(void);
void laser2(void);
void gravityShip1(void);
void gravityShip2(void);
void gravityLaser1(void);
void gravityLaser2(void);
void detectLaserCollision(void);
void detectShipCollision(void);
void detectPulsarCollision(void);

void deathShip1(void);
void deathShip2(void);

/* DISPLAY FUNCTIONS */
void displayStars(void);
void displayPulsar(void);
void displayShip1(void);
void displayShip2(void);
void displayLaser1(void);
void displayLaser2(void);
void displayFuel(void);
void displayScores(void);

void renderScene(void);

int main(int argc, char **argv)
{
	generateStars();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(640,640);
	glutCreateWindow("Celestiar");
	glutDisplayFunc(renderScene);
	glutReshapeFunc(resize);
	glutTimerFunc(17, timer, 0);

	glutSpecialFunc(pressKeySpec);
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(releaseKeySpec);

	glutKeyboardFunc(pressKeyNorm);
	glutIgnoreKeyRepeat(1);
	glutKeyboardUpFunc(releaseKeyNorm);

	glutMainLoop();

	return 0;
}

void resize(int w, int h)
{
	glViewport(0,0,w,h);
}

void timer(int n)
{
	glutPostRedisplay();
	glutTimerFunc(17, timer, 0);
}

void pressKeySpec(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT :
			left1 = 1;
			break;
		case GLUT_KEY_RIGHT :
			right1 = 1;
			break;
/*		case GLUT_KEY_UP :
			up1 = 1;
			break;*/
        case GLUT_KEY_UP:
            if (!dead1)
				fire1 = 1;
			xLaser1 = xShip1;
			yLaser1 = yShip1;
			laserAngle1 = shipAngle1;
			xLaserVel1 = 1.*cos(laserAngle1)+xVel1;
			yLaserVel1 = 1.*sin(laserAngle1)+yVel1;
            break;
		case GLUT_KEY_DOWN :
			down1 = 1;
			break;
/*		case GLUT_KEY_F12 :
			if (!dead1)
				fire1 = 1;
			xLaser1 = xShip1;
			yLaser1 = yShip1;
			laserAngle1 = shipAngle1;
			xLaserVel1 = 0.3*cos(laserAngle1);
			yLaserVel1 = 0.3*sin(laserAngle1);
			break;
		case GLUT_KEY_F1 :
			if (!dead2)
				fire2 = 1;
			xLaser2 = xShip2;
			yLaser2 = yShip2;
			laserAngle2 = shipAngle2;
			xLaserVel2 = 0.3*cos(laserAngle2);
			yLaserVel2 = 0.3*sin(laserAngle2);
			break;
		case GLUT_KEY_F5 :
			dead1 = 1;
			break;
		case GLUT_KEY_F6 :
			dead2 = 1;
			break;*/
	}
}

void pressKeyNorm(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a' :
			left2 = 1;
			break;
		case 'e' :
			right2 = 1;
			break;
/*		case ',' :
            up2 = 1;
			break;*/
		case 'o' :
			down2 = 1;
			break;
        case ',' :
            if (!dead2)
				fire2 = 1;
			xLaser2 = xShip2;
			yLaser2 = yShip2;
			laserAngle2 = shipAngle2;
			xLaserVel2 = 1.*cos(laserAngle2)+xVel2;
			yLaserVel2 = 1.*sin(laserAngle2)+yVel2;
            break;
 /*       case '\'' :
            if (!dead2)
				fire2 = 1;
			xLaser2 = xShip2;
			yLaser2 = yShip2;
			laserAngle2 = shipAngle2;
			xLaserVel2 = 0.3*cos(laserAngle2);
			yLaserVel2 = 0.3*sin(laserAngle2);
            break;*/
		case 27 :
			exit(0);
			break;
	}
}

void releaseKeySpec(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT :
			left1 = 0;
			break;
		case GLUT_KEY_RIGHT :
			right1 = 0;
			break;
/*		case GLUT_KEY_UP :
			up1 = 0;
			break;*/
		case GLUT_KEY_DOWN :
			down1 = 0;
			break;
	}
}

void releaseKeyNorm(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a' :
			left2 = 0;
			break;
		case 'e' :
			right2 = 0;
			break;
/*		case ',' :
			up2 = 0;
			break;*/
		case 'o' :
			down2 = 0;
			break;
	}
}

void generateStars(void)
{
	srand(time(NULL));
	for (starCount = 0; starCount <= 34; starCount++)
	{
		starColor[starCount] = rand() % 9 + 2;
		starColor[starCount] /=  10;

		xStar[starCount] = rand() % 200 - 99;
		xStar[starCount] /= 100;
		yStar[starCount] = rand() % 200 - 99;
		yStar[starCount] /= 100;
	}
}

void moveShip1(void)
{
	if (left1 && !dead1)
		shipAngle1 += 0.3*velCoe;
	if (right1 && !dead1)
		shipAngle1 -= 0.3*velCoe;

	if (shipAngle1 > 2*M_PI)
		shipAngle1 = 0.0;
	else if (shipAngle1 < 0.0)
		shipAngle1 = 2*M_PI;

	if (down1 && !dead1)
	{
		xVel1 += 0.0015*velCoe*cos(shipAngle1);
		yVel1 += 0.0015*velCoe*sin(shipAngle1);
	}
	/*if (down1 && !dead1)
	{
		xVel1 -= 0.0005*velCoe*cos(shipAngle1);
		yVel1 -= 0.0005*velCoe*sin(shipAngle1);
	}*/

	xShip1 += 0.3*xVel1;
	yShip1 += 0.3*yVel1;

	if (xShip1 >= 1.0)
		xShip1 = -0.99;
	if (xShip1 <= -1.0)
		xShip1 = 0.99;
	if (yShip1 >= 1.0)
		yShip1 = -0.99;
	if (yShip1 <= -1.0)
		yShip1 = 0.99;
    
  /*  double distance, velocity, momentum;
    distance = sqrt( pow(xShip1,2) + pow(yShip1,2) );
    velocity = sqrt( pow(xVel1,2) + pow(yVel1,2) );
    momentum = distance * velocity;
    printf("%f\n", momentum);*/
}

void moveShip2(void)
{
	if (left2 && !dead2)
		shipAngle2 += 0.3*velCoe;
	if (right2 && !dead2)
		shipAngle2 -= 0.3*velCoe;

	if (shipAngle2 > 2*M_PI)
		shipAngle2 = 0.0;
	else if (shipAngle2 < 0.0)
		shipAngle2 = 2*M_PI;

	if (down2 && !dead2)
	{
		xVel2 += 0.0015*velCoe*cos(shipAngle2);
		yVel2 += 0.0015*velCoe*sin(shipAngle2);
	}
/*	if (down2 && !dead2)
	{
		xVel2 -= 0.0005*velCoe*cos(shipAngle2);
		yVel2 -= 0.0005*velCoe*sin(shipAngle2);
	}*/

	xShip2 += 0.3*xVel2;
	yShip2 += 0.3*yVel2;

	if (xShip2 >= 1.0)
		xShip2 = -0.99;
	if (xShip2 <= -1.0)
		xShip2 = 0.99;
	if (yShip2 >= 1.0)
		yShip2 = -0.99;
	if (yShip2 <= -1.0)
		yShip2 = 0.99;
}

void ship2AI(void)
{
/* STAY AWAY FROM PULSAR */
	double gravityAngle;
	double shipAngle;
	double angleFact;
	double angleFactShip;
	double shipDist;

	if (xShip2 < 0.0)
		gravityAngle = atan(yShip2/xShip2);
	else
		gravityAngle = atan(yShip2/xShip2) + M_PI;

	angleFact = sin(shipAngle2 - gravityAngle);
//	angleFactShip = sin(shipAngle2 - shipAngle1);

	if (xShip2 - xShip1 < 0.0)
		shipAngle = atan((yShip2 - yShip1) / (xShip2 - xShip1));
	else
		shipAngle = atan((yShip2 - yShip1) / (xShip2 - xShip1)) + M_PI;

	angleFactShip = sin(shipAngle2 - shipAngle);

	shipDist = sqrt(pow(xShip2,2)+pow(yShip2,2));

	if (shipDist < 0.75)
	{
		if (angleFact > 0.0)
		{
			left2 = 1;
			right2 = 0;
		}
		else
		{
			left2 = 0;
			right2 = 1;
		}
		down2 = 1;
	}
	else
	{
		down2 = 0;
		if (angleFactShip > 0.0)
		{
			left2 = 0;
			right2 = 1;
		}
		else
		{
			left2 = 1;
			right2 = 0;
		}
	}

	if (!dead2 && !fire2 && angleFactShip < 0.2 && angleFactShip > -0.2)
	{
		fire2 = 1;
		xLaser2 = xShip2;
		yLaser2 = yShip2;
		laserAngle2 = shipAngle2;
		xLaserVel2 = 1.*cos(laserAngle2)+xVel2;
		yLaserVel2 = 1.*sin(laserAngle2)+yVel2;
	}
//	printf("%f\n", angleFactShip);
}

void laser1(void)
{
	if (fire1)
	{
		xLaser1 += 0.3*velCoe*xLaserVel1;
		yLaser1 += 0.3*velCoe*yLaserVel1;

		if (xLaser1 <= -1.0 || xLaser1 >= 1.0 || yLaser1 <= -1.0 || yLaser1 >= 1.0)
			fire1 = 0;
	}
}

void laser2(void)
{
	if (fire2)
	{
		xLaser2 += 0.3*velCoe*xLaserVel2;
		yLaser2 += 0.3*velCoe*yLaserVel2;

		if (xLaser2 <= -1.0 || xLaser2 >= 1.0 || yLaser2 <= -1.0 || yLaser2 >= 1.0)
			fire2 = 0;
	}
}

void gravityShip1(void)
{
	double gravityAngle;

	if (xShip1 < 0.0)
		gravityAngle = atan(yShip1/xShip1);
	else
		gravityAngle = atan(yShip1/xShip1)+M_PI;

	if (!dead1)
	{
		xVel1 += 0.00025/(pow(xShip1,2)+pow(yShip1,2))*velCoe*cos(gravityAngle);
		yVel1 += 0.00025/(pow(xShip1,2)+pow(yShip1,2))*velCoe*sin(gravityAngle);
	}
	else
	{
		xVel1 = 0.0;
		yVel1 = 0.0;
	}
}

void gravityShip2(void)
{
	double gravityAngle;

	if (xShip2 < 0.0)
		gravityAngle = atan(yShip2/xShip2);
	else
		gravityAngle = atan(yShip2/xShip2)+M_PI;

	if (!dead2)
	{
		xVel2 += 0.00025/(pow(xShip2,2)+pow(yShip2,2))*velCoe*cos(gravityAngle);
		yVel2 += 0.00025/(pow(xShip2,2)+pow(yShip2,2))*velCoe*sin(gravityAngle);
	}
	else
	{
		xVel2 = 0.0;
		yVel2 = 0.0;
	}
}

void gravityLaser1(void)
{
	double gravityAngle;

	if (xLaser1 < 0.0)
		gravityAngle = atan(yLaser1/xLaser1);
	else
		gravityAngle = atan(yLaser1/xLaser1)+M_PI;

	xLaserVel1 += 0.001/(pow(xLaser1,2)+pow(yLaser1,2))*velCoe*cos(gravityAngle);
	yLaserVel1 += 0.001/(pow(xLaser1,2)+pow(yLaser1,2))*velCoe*sin(gravityAngle);
}

void gravityLaser2(void)
{
	double gravityAngle;

	if (xLaser2 < 0.0)
		gravityAngle = atan(yLaser2/xLaser2);
	else
		gravityAngle = atan(yLaser2/xLaser2)+M_PI;

	xLaserVel2 += 0.001/(pow(xLaser2,2)+pow(yLaser2,2))*velCoe*cos(gravityAngle);
	yLaserVel2 += 0.001/(pow(xLaser2,2)+pow(yLaser2,2))*velCoe*sin(gravityAngle);
}

void detectLaserCollision(void)
{
	double a1, b1, c1, x1_1, x2_1, x3_1, y1_1, y2_1, y3_1, den1;
	double a2, b2, c2, x1_2, x2_2, x3_2, y1_2, y2_2, y3_2, den2;

    x1_1 = xShip1;
    x2_1 = xShip1-0.1*cos(shipAngle1+0.17);
    x3_1 = xShip1-0.1*cos(shipAngle1-0.17);
    y1_1 = yShip1;
    y2_1 = yShip1-0.1*sin(shipAngle1+0.17);
    y3_1 = yShip1-0.1*sin(shipAngle1-0.17);

    den1 = (y2_1-y3_1)*(x1_1-x3_1)+(x3_1-x2_1)*(y1_1-y3_1);
    a1 = ((y2_1-y3_1)*(xLaser2-x3_1)+(x3_1-x2_1)*(yLaser2-y3_1))/den1;
    b1 = ((y3_1-y1_1)*(xLaser2-x3_1)+(x1_1-x3_1)*(yLaser2-y3_1))/den1;
    c1 = 1-a1-b1;

    if (0 <= a1 && a1 <= 1 && 0 <= b1 && b1 <= 1 && 0 <= c1 && c1 <= 1)
    {
        fire2 = 0;
        dead1 = 1;
    }

    x1_2 = xShip2;
    x2_2 = xShip2-0.1*cos(shipAngle2+0.17);
    x3_2 = xShip2-0.1*cos(shipAngle2-0.17);
    y1_2 = yShip2;
    y2_2 = yShip2-0.1*sin(shipAngle2+0.17);
    y3_2 = yShip2-0.1*sin(shipAngle2-0.17);

    den2 = (y2_2-y3_2)*(x1_2-x3_2)+(x3_2-x2_2)*(y1_2-y3_2);
    a2 = ((y2_2-y3_2)*(xLaser1-x3_2)+(x3_2-x2_2)*(yLaser1-y3_2))/den2;
    b2 = ((y3_2-y1_2)*(xLaser1-x3_2)+(x1_2-x3_2)*(yLaser1-y3_2))/den2;
    c2 = 1-a2-b2;

    if (0 <= a2 && a2 <= 1 && 0 <= b2 && b2 <= 1 && 0 <= c2 && c2 <= 1)
    {
        fire1 = 0;
        dead2 = 1;
    }
}

void detectShipCollision(void)
{

}

void detectPulsarCollision(void)
{
	if (xShip1 >= -0.05 && xShip1 <= 0.05
		&& yShip1 >= -0.05 && yShip1 <= 0.05)
		dead1 = 1;

	if (xShip2 >= -0.05 && xShip2 <= 0.05
		&& yShip2 >= -0.05 && yShip2 <= 0.05)
		dead2 = 1;

	if (xLaser1 <= 0.05 && xLaser1 >= -0.05
		&& yLaser1 <= 0.05 && yLaser1 >= -0.05)
		fire1 = 0;

	if (xLaser2 <= 0.05 && xLaser2 >= -0.05
		&& yLaser2 <= 0.05 && yLaser2 >= -0.05)
		fire2 = 0;
}

void deathShip1(void)
{
	if (dead1)
	{
		if (deathTick1 == 0.0)
		{
			score2++;
			for (deathCount1 = 0; deathCount1 < 8; deathCount1++)
			{
				deathAngle1[deathCount1] = rand() % 1256 - 628;
			}
		}

		deathTick1 += 0.01*velCoe;
		if (deathTick1 >= 1.5*velCoe)
		{
			xVel1 = 0.0;
			yVel1 = 0.0;

			do
			{
					xShip1 = rand() % 200 - 99;
				xShip1 /= 100;
				yShip1 = rand() % 200 - 99;
				yShip1 /= 100;
			} while (xShip1 <= 0.5 && xShip1 >= -0.5 || yShip1 <= 0.5 && yShip1 >= -0.5);

			dead1 = 0;
			deathTick1 = 0.0;
		}
	}
}

void deathShip2(void)
{
	if (dead2)
	{
		if (deathTick2 == 0.0)
		{
			score1++;
			for (deathCount2 = 0; deathCount2 < 8; deathCount2++)
			{
				deathAngle2[deathCount2] = rand() % 1256 - 628;
			}
		}

		deathTick2 += 0.01*velCoe;
		if (deathTick2 >= 1.5*velCoe)
		{
			xVel2 = 0.0;
			yVel2 = 0.0;

			do
			{
				xShip2 = rand() % 200 - 99;
				xShip2 /= 100;
				yShip2 = rand() % 200 - 99;
				yShip2 /= 100;
			} while (xShip2 <= 0.5 && xShip2 >= -0.5 || yShip2 <= 0.5 && yShip2 >= -0.5);

			dead2 = 0;
			deathTick2 = 0.0;
		}
	}
}

void displayStars(void)
{
	glBegin(GL_POINTS);
	for (starCount = 0; starCount <= 34; starCount++)
	{
		glColor3f(starColor[starCount],starColor[starCount],starColor[starCount]);
		glVertex2f(xStar[starCount],yStar[starCount]);
	}
	glEnd();
}

void displayPulsar(void)
{
	glColor3f(0.5*sin(colorPulsar)+0.5,0.5*sin(colorPulsar)+0.5,0.5*sin(colorPulsar)+0.5);
	glBegin(GL_POLYGON);
	for (double angle = 0.0; angle < 6.28; angle += 0.8)
	{
		glVertex2f(0.025*cos(angle),0.025*sin(angle));
	}
	glEnd();

	colorPulsar += 2.0*velCoe;
	if (colorPulsar >= 6.28)
		colorPulsar = 0.0;
}

void displayShip1(void)
{
	if (!dead1)
	{
		glColor3f(0.0,0.0,1.0);
		glBegin(GL_QUADS);
			glVertex2f(xShip1,yShip1);
			glVertex2f(xShip1-0.1*cos(shipAngle1+0.17),yShip1-0.1*sin(shipAngle1+0.17));
			glVertex2f(xShip1-0.07*cos(shipAngle1),yShip1-0.07*sin(shipAngle1));
			glVertex2f(xShip1-0.1*cos(shipAngle1-0.17),yShip1-0.1*sin(shipAngle1-0.17));
		glEnd();

		if (down1)
		{
			glBegin(GL_LINES);
				glColor3f(0.2,1.0,0.0);
				glVertex2f(xShip1-0.07*cos(shipAngle1),yShip1-0.07*sin(shipAngle1));
				glColor3f(1.0,0.2,0.0);
				glVertex2f(xShip1-0.15*cos(shipAngle1),yShip1-0.15*sin(shipAngle1));
			glEnd();
		}
	/*	if (down1)
		{
			glBegin(GL_LINES);
				glColor3f(0.2,1.0,0.0);
				glVertex2f(xShip1-0.07*cos(shipAngle1+0.17),yShip1-0.07*sin(shipAngle1+0.17));
				glColor3f(1.0,0.2,0.0);
				glVertex2f(xShip1-0.02*cos(shipAngle1+0.511),yShip1-0.02*sin(shipAngle1+0.511));

				glColor3f(0.2,1.0,0.0);
				glVertex2f(xShip1-0.07*cos(shipAngle1-0.17),yShip1-0.07*sin(shipAngle1-0.17));
				glColor3f(1.0,0.2,0.0);
				glVertex2f(xShip1-0.02*cos(shipAngle1-0.511),yShip1-0.02*sin(shipAngle1-0.511));
			glEnd();
		}*/
	}
	else
	{
		glColor3f(0.0,0.0,1.0);
		glPointSize(2.0);
		glBegin(GL_POINTS);
		for (deathCount1 = 0; deathCount1 < 8; deathCount1++)
		{
			glVertex2f(xShip1+deathTick1*cos(deathAngle1[deathCount1]/100),yShip1+deathTick1*sin(deathAngle1[deathCount1]/100));
		}
		glEnd();
	}
}

void displayShip2(void)
{
	if (!dead2)
	{
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_QUADS);
			glVertex2f(xShip2,yShip2);
			glVertex2f(xShip2-0.1*cos(shipAngle2+0.17),yShip2-0.1*sin(shipAngle2+0.17));
			glVertex2f(xShip2-0.07*cos(shipAngle2),yShip2-0.07*sin(shipAngle2));
			glVertex2f(xShip2-0.1*cos(shipAngle2-0.17),yShip2-0.1*sin(shipAngle2-0.17));
		glEnd();

		if (down2)
		{
			glBegin(GL_LINES);
				glColor3f(0.2,1.0,0.0);
				glVertex2f(xShip2-0.07*cos(shipAngle2),yShip2-0.07*sin(shipAngle2));
				glColor3f(1.0,0.2,0.0);
				glVertex2f(xShip2-0.15*cos(shipAngle2),yShip2-0.15*sin(shipAngle2));
			glEnd();
		}
	/*	if (down2)
		{
			glBegin(GL_LINES);
				glColor3f(0.2,1.0,0.0);
				glVertex2f(xShip2-0.07*cos(shipAngle2+0.17),yShip2-0.07*sin(shipAngle2+0.17));
				glColor3f(1.0,0.2,0.0);
				glVertex2f(xShip2-0.02*cos(shipAngle2+0.511),yShip2-0.02*sin(shipAngle2+0.511));

				glColor3f(0.2,1.0,0.0);
				glVertex2f(xShip2-0.07*cos(shipAngle2-0.17),yShip2-0.07*sin(shipAngle2-0.17));
				glColor3f(1.0,0.2,0.0);
				glVertex2f(xShip2-0.02*cos(shipAngle2-0.511),yShip2-0.02*sin(shipAngle2-0.511));
			glEnd();
		}*/
	}
	else
	{
		glColor3f(1.0,0.0,0.0);
		glPointSize(2.0);
		glBegin(GL_POINTS);
		for (deathCount2 = 0; deathCount2 < 8; deathCount2++)
		{
			glVertex2f(xShip2+deathTick2*cos(deathAngle2[deathCount2]/100),yShip2+deathTick2*sin(deathAngle2[deathCount2]/100));
		}
		glEnd();
	}
}

void displayLaser1(void)
{
	if (fire1)
	{
		glColor3f(0.0,1.0,0.0);
		glBegin(GL_LINES);
			glVertex2f(xLaser1,yLaser1);
			glVertex2f(xLaser1-0.02*cos(laserAngle1),yLaser1-0.02*sin(laserAngle1));
		glEnd();
	}
}

void displayLaser2(void)
{
	if (fire2)
	{
		glColor3f(0.0,1.0,0.0);
		glBegin(GL_LINES);
			glVertex2f(xLaser2,yLaser2);
			glVertex2f(xLaser2-0.02*cos(laserAngle2),yLaser2-0.02*sin(laserAngle2));
		glEnd();
	}
}

void displayScores(void)
{
	char cscore1[5], cscore2[5];

	sprintf(cscore1, "%u", score1);
	sprintf(cscore2, "%u", score2);

	glColor3f(0.0,0.0,1.0);
	glRasterPos2f(-0.9,0.9);
	for (char* c = cscore1; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*c);

	glColor3f(1.0,0.0,0.0);
	glRasterPos2f(-0.7,0.9);
	for (char* c = cscore2; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*c);
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	moveShip1();
	moveShip2();
	ship2AI();
	laser1();
	laser2();
	gravityShip1();
	gravityShip2();
	gravityLaser1();
	gravityLaser2();
	detectLaserCollision();
	detectShipCollision();
	detectPulsarCollision();

	deathShip1();
	deathShip2();

	displayStars();
	displayPulsar();

	displayShip1();
	displayShip2();
	displayLaser1();
	displayLaser2();
	displayScores();

	glFlush();
}
