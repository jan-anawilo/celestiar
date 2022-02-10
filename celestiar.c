#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

struct Ship {
	// bools for controls
	int down, left, right, is_shooting;

	// bools for kinematics
	double x_pos, y_pos;
	double x_vel, y_vel;
	double angle;

	// bool for dead
	int is_dead;

	// for laser
	double x_pos_laser, y_pos_laser;
	double x_vel_laser, y_vel_laser;
	double angle_laser;
} blue, red;

double xStar[35], yStar[35], starColor[35]; int starCount;

double colorPulsar = 0.0;

double deathTick1 = 0.0, deathTick2 = 0.0;
double deathAngle1[8], deathAngle2[8]; int deathCount1, deathCount2;

unsigned int score1 = 0, score2 = 0;

int triangle_collision(double x_test, double y_test,
	double x_a, double y_a,
	double x_b, double y_b,
	double x_c, double y_c);

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
void gravityShip1(void);
void gravityShip2(void);
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
	blue.x_pos = -0.7;
	blue.y_pos = -0.7;
	blue.x_vel = 0.0;
	blue.y_vel = 0.0;
	blue.angle = M_PI / 2.;

	blue.down = 0;
	blue.left = 0;
	blue.right = 0;
	blue.is_shooting = 0;
	blue.is_dead = 0;

	red.x_pos = 0.7;
	red.y_pos = 0.7;
	red.x_vel = 0.0;
	red.y_vel = 0.0;
	red.angle = 3. * M_PI / 2.;

	red.down = 0;
	red.left = 0;
	red.right = 0;
	red.is_shooting = 0;
	red.is_dead = 0;

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

int triangle_collison(double x_test, double y_test,
	double x_a, double y_a,
	double x_b, double y_b,
	double x_c, double y_c)
{
	double den, a, b, c;

	den = (y_b-y_c)*(x_a-x_c)+(x_c-x_b)*(y_a-y_c);
    a = ((y_b-y_c)*(x_test-x_c)+(x_c-x_b)*(y_test-y_c))/den;
    b = ((y_c-y_a)*(x_test-x_c)+(x_a-x_c)*(y_test-y_c))/den;
    c = 1-a-b;

    if (0. <= a && a <= 1. && 0. <= b && b <= 1. && 0. <= c && c <= 1.)
        return 1;

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
/*		case GLUT_KEY_LEFT :
			blue.left = 1;
			break;
		case GLUT_KEY_RIGHT :
			blue.right = 1;
			break;*/
/*		case GLUT_KEY_UP :
			up1 = 1;
			break;*/
/*        case GLUT_KEY_UP:
            if (!blue.is_dead)
				blue.is_shooting = 1;
			blue.x_pos_laser = blue.x_pos;
			blue.y_pos_laser = blue.y_pos;
			blue.angle_laser = blue.angle;
			blue.x_vel_laser = 1.*cos(blue.angle_laser)+blue.x_vel;
			blue.y_vel_laser = 1.*sin(blue.angle_laser)+blue.y_vel;
            break;
		case GLUT_KEY_DOWN :
			blue.down = 1;
			break;
/*		case GLUT_KEY_F12 :
			if (!blue.is_dead)
				blue.is_shooting = 1;
			blue.x_pos_laser = blue.x_pos;
			blue.y_pos_laser = blue.y_pos;
			blue.angle_laser = blue.angle;
			blue.x_vel_laser = 0.3*cos(blue.angle_laser);
			blue.y_vel_laser = 0.3*sin(blue.angle_laser);
			break;
		case GLUT_KEY_F1 :
			if (!red.is_dead)
				red.is_shooting = 1;
			red.x_pos_laser = red.x_pos;
			red.y_pos_laser = red.y_pos;
			red.angle_laser = red.angle;
			red.x_vel_laser = 0.3*cos(red.angle_laser);
			red.y_vel_laser = 0.3*sin(red.angle_laser);
			break;
		case GLUT_KEY_F5 :
			blue.is_dead = 1;
			break;
		case GLUT_KEY_F6 :
			red.is_dead = 1;
			break;*/
	}
}

void pressKeyNorm(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a' :
			blue.left = 1;
			break;
		case 'e' :
			blue.right = 1;
			break;
		case ',' :
            blue.down = 1;
			break;
		case 'o' :
			blue.down = 1;
			break;
        case ' ' :
            if (!blue.is_dead)
				blue.is_shooting = 1;
			blue.x_pos_laser = blue.x_pos;
			blue.y_pos_laser = blue.y_pos;
			blue.angle_laser = blue.angle;
			blue.x_vel_laser = 0.03*cos(blue.angle_laser);
			blue.y_vel_laser = 0.03*sin(blue.angle_laser);
            break;
		case 27 :
			exit(0);
			break;
	}
}

void releaseKeySpec(int key, int x, int y)
{
/*	switch (key)
	{
		case GLUT_KEY_LEFT :
			blue.left = 0;
			break;
		case GLUT_KEY_RIGHT :
			blue.right = 0;
			break;
/*		case GLUT_KEY_UP :
			up1 = 0;
			break;*/
/*		case GLUT_KEY_DOWN :
			blue.down = 0;
			break;
	}*/
}

void releaseKeyNorm(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a' :
			blue.left = 0;
			break;
		case 'e' :
			blue.right = 0;
			break;
		case ',' :
			blue.down = 0;
			break;
		case 'o' :
			blue.down = 0;
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
	if (blue.left && !blue.is_dead)
		blue.angle += 0.03;
	if (blue.right && !blue.is_dead)
		blue.angle -= 0.03;

	if (blue.angle > 2*M_PI)
		blue.angle = 0.0;
	else if (blue.angle < 0.0)
		blue.angle = 2*M_PI;

	if (blue.down && !blue.is_dead)
	{
		blue.x_vel += 0.00005 * cos(blue.angle);
		blue.y_vel += 0.00005 * sin(blue.angle);
	}

	blue.x_pos += blue.x_vel;
	blue.y_pos += blue.y_vel;

	if (blue.x_pos >= 1.0)
		blue.x_pos = -0.99;
	if (blue.x_pos <= -1.0)
		blue.x_pos = 0.99;
	if (blue.y_pos >= 1.0)
		blue.y_pos = -0.99;
	if (blue.y_pos <= -1.0)
		blue.y_pos = 0.99;
}

void moveShip2(void)
{
	if (red.left && !red.is_dead)
		red.angle += 0.03;
	if (red.right && !red.is_dead)
		red.angle -= 0.03;

	if (red.angle > 2*M_PI)
		red.angle = 0.0;
	else if (red.angle < 0.0)
		red.angle = 2*M_PI;

	if (red.down && !red.is_dead)
	{
		red.x_vel += 0.00005 * cos(red.angle);
		red.y_vel += 0.00005 * sin(red.angle);
	}

	red.x_pos += red.x_vel;
	red.y_pos += red.y_vel;

	if (red.x_pos >= 1.0)
		red.x_pos = -0.99;
	if (red.x_pos <= -1.0)
		red.x_pos = 0.99;
	if (red.y_pos >= 1.0)
		red.y_pos = -0.99;
	if (red.y_pos <= -1.0)
		red.y_pos = 0.99;
}

void ship2AI(void)
{
/* STAY AWAY FROM PULSAR */
	double gravityAngle;
	double shipAngle;
	double angleFact;
	double angleFactShip;
	double shipDist;

	double delta_t, x_prime, y_prime, v_l;

	// lead off calculation (what a doozy!)
	v_l = 0.03;
	delta_t = (
		- sqrt(
			pow(- 2.*red.x_pos*blue.x_vel - 2.*red.y_pos*blue.y_vel + 2.*blue.x_vel*blue.x_pos + 2.*blue.y_vel*blue.y_pos, 2)
			- 4.*(- v_l*v_l + blue.x_vel*blue.x_vel + blue.y_vel*blue.y_vel)*(red.x_pos*red.x_pos
			- 2.*red.x_pos*blue.x_pos + red.y_pos*red.y_pos
			- 2.*red.y_pos*blue.y_pos + blue.x_pos*blue.x_pos + blue.y_pos*blue.y_pos) // checks out
		)
		+ 2.*red.x_pos*blue.x_vel + 2.*red.y_pos*blue.y_vel - 2.*blue.x_vel*blue.x_pos - 2.*blue.y_vel*blue.y_pos // checks out
	)
	/ (
		2.*(- v_l*v_l + blue.x_vel*blue.x_vel + blue.y_vel*blue.y_vel) // checks out
	);

	x_prime = blue.x_vel * delta_t + blue.x_pos;
	y_prime = blue.y_vel * delta_t + blue.y_pos;

	// adjust for center of hitbox
	x_prime -= 0.05*cos(blue.angle);
	y_prime -= 0.05*sin(blue.angle);

	// check location of target
/*	glColor3f(1.0,0.0,1.0);
	glBegin(GL_POINTS);
	glVertex2d(x_prime, y_prime);
	glEnd();

	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex2d(red.x_pos, red.y_pos);
	glVertex2d(x_prime, y_prime);
	glEnd();
*/
	if (red.x_pos < 0.0)
		gravityAngle = atan(red.y_pos/red.x_pos);
	else
		gravityAngle = atan(red.y_pos/red.x_pos) + M_PI;

	angleFact = sin(red.angle - gravityAngle);

	if (red.x_pos - x_prime < 0.0)
		shipAngle = atan((red.y_pos - y_prime) / (red.x_pos - x_prime));
	else
		shipAngle = atan((red.y_pos - y_prime) / (red.x_pos - x_prime)) + M_PI;

	angleFactShip = sin(red.angle - shipAngle);

	shipDist = sqrt(pow(red.x_pos,2)+pow(red.y_pos,2));

	if (shipDist < 0.75)
	{
		if (angleFact > 0.0)
		{
			red.left = 1;
			red.right = 0;
		}
		else
		{
			red.left = 0;
			red.right = 1;
		}
		red.down = 1;
	}
	else
	{
		red.down = 0;
		if (angleFactShip > 0.0)
		{
			red.left = 0;
			red.right = 1;
		}
		else
		{
			red.left = 1;
			red.right = 0;
		}
	}

	if (!red.is_dead && !blue.is_dead && !red.is_shooting && angleFactShip < 0.2 && angleFactShip > -0.2)
	{
		red.is_shooting = 1;
		red.x_pos_laser = red.x_pos;
		red.y_pos_laser = red.y_pos;
		red.angle_laser = red.angle;
		red.x_vel_laser = 0.03*cos(red.angle_laser);
		red.y_vel_laser = 0.03*sin(red.angle_laser);
	}
}

void laser1(void)
{
	if (blue.is_shooting)
	{
		blue.x_pos_laser += blue.x_vel_laser;
		blue.y_pos_laser += blue.y_vel_laser;

		if (blue.x_pos_laser <= -1.0 || blue.x_pos_laser >= 1.0 || blue.y_pos_laser <= -1.0 || blue.y_pos_laser >= 1.0)
			blue.is_shooting = 0;
	}
}

void laser2(void)
{
	if (red.is_shooting)
	{
		red.x_pos_laser += red.x_vel_laser;
		red.y_pos_laser += red.y_vel_laser;

		if (red.x_pos_laser <= -1.0 || red.x_pos_laser >= 1.0 || red.y_pos_laser <= -1.0 || red.y_pos_laser >= 1.0)
			red.is_shooting = 0;
	}
}

void gravityShip1(void)
{
	double gravityAngle;

	if (blue.x_pos < 0.0)
		gravityAngle = atan(blue.y_pos/blue.x_pos);
	else
		gravityAngle = atan(blue.y_pos/blue.x_pos)+M_PI;

	if (!blue.is_dead)
	{
		blue.x_vel += 0.000008*cos(gravityAngle)/(pow(blue.x_pos,2)+pow(blue.y_pos,2));
		blue.y_vel += 0.000008*sin(gravityAngle)/(pow(blue.x_pos,2)+pow(blue.y_pos,2));
	}
	else
	{
		blue.x_vel = 0.0;
		blue.y_vel = 0.0;
	}
}

void gravityShip2(void)
{
	double gravityAngle;

	if (red.x_pos < 0.0)
		gravityAngle = atan(red.y_pos/red.x_pos);
	else
		gravityAngle = atan(red.y_pos/red.x_pos)+M_PI;

	if (!red.is_dead)
	{
		red.x_vel += 0.000008*cos(gravityAngle)/(pow(red.x_pos,2)+pow(red.y_pos,2));
		red.y_vel += 0.000008*sin(gravityAngle)/(pow(red.x_pos,2)+pow(red.y_pos,2));
	}
	else
	{
		red.x_vel = 0.0;
		red.y_vel = 0.0;
	}
}

void gravityLaser1(void)
{
	double gravityAngle;

	if (blue.x_pos_laser < 0.0)
		gravityAngle = atan(blue.y_pos_laser/blue.x_pos_laser);
	else
		gravityAngle = atan(blue.y_pos_laser/blue.x_pos_laser)+M_PI;

	blue.x_vel_laser += 0.000075*cos(gravityAngle)/(pow(blue.x_pos_laser,2)+pow(blue.y_pos_laser,2));
	blue.y_vel_laser += 0.000075*sin(gravityAngle)/(pow(blue.x_pos_laser,2)+pow(blue.y_pos_laser,2));
}

void gravityLaser2(void)
{
	double gravityAngle;

	if (red.x_pos_laser < 0.0)
		gravityAngle = atan(red.y_pos_laser/red.x_pos_laser);
	else
		gravityAngle = atan(red.y_pos_laser/red.x_pos_laser)+M_PI;

	red.x_vel_laser += 0.001*cos(gravityAngle)/(pow(red.x_pos_laser,2)+pow(red.y_pos_laser,2));
	red.y_vel_laser += 0.001*sin(gravityAngle)/(pow(red.x_pos_laser,2)+pow(red.y_pos_laser,2));
}

void detectLaserCollision(void)
{
	// use 1/2 of width = 0.017
	if (triangle_collison(red.x_pos_laser, red.y_pos_laser,
		blue.x_pos-0.017*cos(blue.angle-M_PI/2.), blue.y_pos-0.017*sin(blue.angle-M_PI/2.),
		blue.x_pos-0.1*cos(blue.angle-0.17), blue.y_pos-0.1*sin(blue.angle-0.17),
		blue.x_pos-0.1*cos(blue.angle+0.17), blue.y_pos-0.1*sin(blue.angle+0.17)))
	{
		red.is_shooting = 0;
		blue.is_dead = 1;
	}
	else if (triangle_collison(red.x_pos_laser, red.y_pos_laser,
		blue.x_pos-0.017*cos(blue.angle-M_PI/2.), blue.y_pos-0.017*sin(blue.angle-M_PI/2.),
		blue.x_pos-0.017*cos(blue.angle+M_PI/2.), blue.y_pos-0.017*sin(blue.angle+M_PI/2.),
		blue.x_pos-0.1*cos(blue.angle+0.17), blue.y_pos-0.1*sin(blue.angle+0.17)))
	{
		red.is_shooting = 0;
		blue.is_dead = 1;
	}

	if (triangle_collison(blue.x_pos_laser, blue.y_pos_laser,
		red.x_pos-0.017*cos(red.angle-M_PI/2.), red.y_pos-0.017*sin(red.angle-M_PI/2.),
		red.x_pos-0.1*cos(red.angle-0.17), red.y_pos-0.1*sin(red.angle-0.17),
		red.x_pos-0.1*cos(red.angle+0.17), red.y_pos-0.1*sin(red.angle+0.17)))
	{
		blue.is_shooting = 0;
		red.is_dead = 1;
	}
	else if (triangle_collison(blue.x_pos_laser, blue.y_pos_laser,
		red.x_pos-0.017*cos(red.angle-M_PI/2.), red.y_pos-0.017*sin(red.angle-M_PI/2.),
		red.x_pos-0.017*cos(red.angle+M_PI/2.), red.y_pos-0.017*sin(red.angle+M_PI/2.),
		red.x_pos-0.1*cos(red.angle+0.17), red.y_pos-0.1*sin(red.angle+0.17)))
	{
		blue.is_shooting = 0;
		red.is_dead = 1;
	}
}

void detectShipCollision(void)
{
	double shipDist;

	shipDist = pow(blue.x_pos-red.x_pos,2) + pow(blue.y_pos-red.y_pos,2);
	if (shipDist < 0.0025)
	{
		blue.is_dead = 1;
		red.is_dead = 1;
	}

}

void detectPulsarCollision(void)
{
	if (blue.x_pos >= -0.05 && blue.x_pos <= 0.05
		&& blue.y_pos >= -0.05 && blue.y_pos <= 0.05)
		blue.is_dead = 1;

	if (red.x_pos >= -0.05 && red.x_pos <= 0.05
		&& red.y_pos >= -0.05 && red.y_pos <= 0.05)
		red.is_dead = 1;

	if (blue.x_pos_laser <= 0.05 && blue.x_pos_laser >= -0.05
		&& blue.y_pos_laser <= 0.05 && blue.y_pos_laser >= -0.05)
		blue.is_shooting = 0;

	if (red.x_pos_laser <= 0.05 && red.x_pos_laser >= -0.05
		&& red.y_pos_laser <= 0.05 && red.y_pos_laser >= -0.05)
		red.is_shooting = 0;
}

void deathShip1(void)
{
	if (blue.is_dead)
	{
		if (deathTick1 == 0.0)
		{
			score2++;
			for (deathCount1 = 0; deathCount1 < 8; deathCount1++)
			{
				deathAngle1[deathCount1] = rand() % 1256 - 628;
			}
		}

		deathTick1 += 0.001;
		if (deathTick1 >= 0.15)
		{
			blue.x_vel = 0.0;
			blue.y_vel = 0.0;

			do
			{
					blue.x_pos = rand() % 200 - 99;
				blue.x_pos /= 100;
				blue.y_pos = rand() % 200 - 99;
				blue.y_pos /= 100;
			} while (blue.x_pos <= 0.5 && blue.x_pos >= -0.5 || blue.y_pos <= 0.5 && blue.y_pos >= -0.5);

			blue.is_dead = 0;
			deathTick1 = 0.0;
		}
	}
}

void deathShip2(void)
{
	if (red.is_dead)
	{
		if (deathTick2 == 0.0)
		{
			score1++;
			for (deathCount2 = 0; deathCount2 < 8; deathCount2++)
			{
				deathAngle2[deathCount2] = rand() % 1256 - 628;
			}
		}

		deathTick2 += 0.001;
		if (deathTick2 >= 0.15)
		{
			red.x_vel = 0.0;
			red.y_vel = 0.0;

			do
			{
				red.x_pos = rand() % 200 - 99;
				red.x_pos /= 100;
				red.y_pos = rand() % 200 - 99;
				red.y_pos /= 100;
			} while (red.x_pos <= 0.5 && red.x_pos >= -0.5 || red.y_pos <= 0.5 && red.y_pos >= -0.5);

			red.is_dead = 0;
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

	colorPulsar += 0.2;
	if (colorPulsar >= 6.28)
		colorPulsar = 0.0;
}

void displayShip1(void)
{
	if (!blue.is_dead)
	{
		glColor3f(0.0,0.0,1.0);
		glBegin(GL_QUADS);
			glVertex2f(blue.x_pos,blue.y_pos);
			glVertex2f(blue.x_pos-0.1*cos(blue.angle+0.17),blue.y_pos-0.1*sin(blue.angle+0.17));
			glVertex2f(blue.x_pos-0.07*cos(blue.angle),blue.y_pos-0.07*sin(blue.angle));
			glVertex2f(blue.x_pos-0.1*cos(blue.angle-0.17),blue.y_pos-0.1*sin(blue.angle-0.17));
		glEnd();

		if (blue.down)
		{
			glBegin(GL_LINES);
				glColor3f(0.2,1.0,0.0);
				glVertex2f(blue.x_pos-0.08*cos(blue.angle),blue.y_pos-0.08*sin(blue.angle));
				glColor3f(1.0,0.2,0.0);
				glVertex2f(blue.x_pos-0.15*cos(blue.angle),blue.y_pos-0.15*sin(blue.angle));
			glEnd();
		}
	/*	if (blue.down)
		{
			glBegin(GL_LINES);
				glColor3f(0.2,1.0,0.0);
				glVertex2f(blue.x_pos-0.07*cos(blue.angle+0.17),blue.y_pos-0.07*sin(blue.angle+0.17));
				glColor3f(1.0,0.2,0.0);
				glVertex2f(blue.x_pos-0.02*cos(blue.angle+0.511),blue.y_pos-0.02*sin(blue.angle+0.511));

				glColor3f(0.2,1.0,0.0);
				glVertex2f(blue.x_pos-0.07*cos(blue.angle-0.17),blue.y_pos-0.07*sin(blue.angle-0.17));
				glColor3f(1.0,0.2,0.0);
				glVertex2f(blue.x_pos-0.02*cos(blue.angle-0.511),blue.y_pos-0.02*sin(blue.angle-0.511));
			glEnd();
		}*/
		//Debugging hitbox
/*		glBegin(GL_QUADS);
				glColor3f(1.0, 0.0, 0.0);
				glVertex2f(
					blue.x_pos-0.017*cos(blue.angle-M_PI/2.),
					blue.y_pos-0.017*sin(blue.angle-M_PI/2.)
				);
				glColor3f(0.0, 1.0, 0.0);
				glVertex2f(
					blue.x_pos-0.017*cos(blue.angle+M_PI/2.),
					blue.y_pos-0.017*sin(blue.angle+M_PI/2.)
				);
				glColor3f(0.0, 0.0, 1.0);
				glVertex2f(
					blue.x_pos-0.1*cos(blue.angle+0.17),
					blue.y_pos-0.1*sin(blue.angle+0.17)
				);
				glColor3f(1.0, 0.0, 1.0);
				glVertex2f(
					blue.x_pos-0.1*cos(blue.angle-0.17),
					blue.y_pos-0.1*sin(blue.angle-0.17)
				);
		glEnd();
*/
	}
	else
	{
		glColor3f(0.0,0.0,1.0);
		glPointSize(2.0);
		glBegin(GL_POINTS);
		for (deathCount1 = 0; deathCount1 < 8; deathCount1++)
		{
			glVertex2f(blue.x_pos+deathTick1*cos(deathAngle1[deathCount1]/100),blue.y_pos+deathTick1*sin(deathAngle1[deathCount1]/100));
		}
		glEnd();
	}
}

void displayShip2(void)
{
	if (!red.is_dead)
	{
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_QUADS);
			glVertex2f(red.x_pos,red.y_pos);
			glVertex2f(red.x_pos-0.1*cos(red.angle+0.17),red.y_pos-0.1*sin(red.angle+0.17));
			glVertex2f(red.x_pos-0.07*cos(red.angle),red.y_pos-0.07*sin(red.angle));
			glVertex2f(red.x_pos-0.1*cos(red.angle-0.17),red.y_pos-0.1*sin(red.angle-0.17));
		glEnd();

		if (red.down)
		{
			glBegin(GL_LINES);
				glColor3f(0.2,1.0,0.0);
				glVertex2f(red.x_pos-0.08*cos(red.angle),red.y_pos-0.08*sin(red.angle));
				glColor3f(1.0,0.2,0.0);
				glVertex2f(red.x_pos-0.15*cos(red.angle),red.y_pos-0.15*sin(red.angle));
			glEnd();
		}
	/*	if (red.down)
		{
			glBegin(GL_LINES);
				glColor3f(0.2,1.0,0.0);
				glVertex2f(red.x_pos-0.07*cos(red.angle+0.17),red.y_pos-0.07*sin(red.angle+0.17));
				glColor3f(1.0,0.2,0.0);
				glVertex2f(red.x_pos-0.02*cos(red.angle+0.511),red.y_pos-0.02*sin(red.angle+0.511));

				glColor3f(0.2,1.0,0.0);
				glVertex2f(red.x_pos-0.07*cos(red.angle-0.17),red.y_pos-0.07*sin(red.angle-0.17));
				glColor3f(1.0,0.2,0.0);
				glVertex2f(red.x_pos-0.02*cos(red.angle-0.511),red.y_pos-0.02*sin(red.angle-0.511));
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
			glVertex2f(red.x_pos+deathTick2*cos(deathAngle2[deathCount2]/100),red.y_pos+deathTick2*sin(deathAngle2[deathCount2]/100));
		}
		glEnd();
	}
}

void displayLaser1(void)
{
	if (blue.is_shooting)
	{
		glColor3f(0.0,1.0,0.0);
		glBegin(GL_LINES);
			glVertex2f(blue.x_pos_laser,blue.y_pos_laser);
			glVertex2f(blue.x_pos_laser-0.02*cos(blue.angle_laser),blue.y_pos_laser-0.02*sin(blue.angle_laser));
		glEnd();
	}
}

void displayLaser2(void)
{
	if (red.is_shooting)
	{
		glColor3f(0.0,1.0,0.0);
		glBegin(GL_LINES);
			glVertex2f(red.x_pos_laser,red.y_pos_laser);
			glVertex2f(red.x_pos_laser-0.02*cos(red.angle_laser),red.y_pos_laser-0.02*sin(red.angle_laser));
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
//	gravitblue.y_pos_laser();
//	gravitred.y_pos_laser();
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
