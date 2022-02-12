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
	double death_tick, death_angle[8];
	int death_count;

	// for laser
	double x_pos_laser, y_pos_laser;
	double x_vel_laser, y_vel_laser;
	double angle_laser;

	// for color
	double red, green, blue;

	// for score
	unsigned int score;

} blue, red;

double xStar[35], yStar[35], starColor[35]; int starCount;

double colorPulsar = 0.0;

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
void moveShip(struct Ship *);
void shipAI(struct Ship *, struct Ship *);
void moveLaser(struct Ship *);
void gravityShip(struct Ship *);
void gravityLaser(struct Ship *);

void detectLaserCollision(void);
void detectShipCollision(void);
void detectPulsarCollision(void);

void deathShip(struct Ship *, struct Ship *);

/* DISPLAY FUNCTIONS */
void displayStars(void);
void displayPulsar(void);

void displayShip(struct Ship *);
void displayLaser(struct Ship *);

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

	blue.red = 0.0;
	blue.green = 1.0;
	blue.blue = 1.0;

	blue.score = 0;

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

	red.red = 1.0;
	red.green = 0.0;
	red.blue = 1.0;

	red.score = 0;

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

void moveShip(struct Ship *ship)
{
	if (ship->left && !ship->is_dead)
		ship->angle += 0.03;
	if (ship->right && !ship->is_dead)
		ship->angle -= 0.03;

	if (ship->angle > 2*M_PI)
		ship->angle = 0.0;
	else if (ship->angle < 0.0)
		ship->angle = 2*M_PI;

	if (ship->down && !ship->is_dead)
	{
		ship->x_vel += 0.00005 * cos(ship->angle);
		ship->y_vel += 0.00005 * sin(ship->angle);
	}

	ship->x_pos += ship->x_vel;
	ship->y_pos += ship->y_vel;

	if (ship->x_pos >= 1.0)
		ship->x_pos = -0.99;
	if (ship->x_pos <= -1.0)
		ship->x_pos = 0.99;
	if (ship->y_pos >= 1.0)
		ship->y_pos = -0.99;
	if (ship->y_pos <= -1.0)
		ship->y_pos = 0.99;
}

void shipAI(struct Ship *ship, struct Ship *other_ship)
{
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
			pow(- 2.*ship->x_pos*other_ship->x_vel - 2.*ship->y_pos*other_ship->y_vel + 2.*other_ship->x_vel*other_ship->x_pos + 2.*other_ship->y_vel*other_ship->y_pos, 2)
			- 4.*(- v_l*v_l + other_ship->x_vel*other_ship->x_vel + other_ship->y_vel*other_ship->y_vel)*(ship->x_pos*ship->x_pos
			- 2.*ship->x_pos*other_ship->x_pos + ship->y_pos*ship->y_pos
			- 2.*ship->y_pos*other_ship->y_pos + other_ship->x_pos*other_ship->x_pos + other_ship->y_pos*other_ship->y_pos) // checks out
		)
		+ 2.*ship->x_pos*other_ship->x_vel + 2.*ship->y_pos*other_ship->y_vel - 2.*other_ship->x_vel*other_ship->x_pos - 2.*other_ship->y_vel*other_ship->y_pos // checks out
	)
	/ (
		2.*(- v_l*v_l + other_ship->x_vel*other_ship->x_vel + other_ship->y_vel*other_ship->y_vel) // checks out
	);

	x_prime = other_ship->x_vel * delta_t + other_ship->x_pos;
	y_prime = other_ship->y_vel * delta_t + other_ship->y_pos;

	// adjust for center of hitbox
	x_prime -= 0.05*cos(other_ship->angle);
	y_prime -= 0.05*sin(other_ship->angle);

	// check location of target
/*	glColor3f(1.0,0.0,1.0);
	glBegin(GL_POINTS);
	glVertex2d(x_prime, y_prime);
	glEnd();

	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES);
	glVertex2d(ship->x_pos, ship->y_pos);
	glVertex2d(x_prime, y_prime);
	glEnd();
*/
	if (ship->x_pos < 0.0)
		gravityAngle = atan(ship->y_pos/ship->x_pos);
	else
		gravityAngle = atan(ship->y_pos/ship->x_pos) + M_PI;

	angleFact = sin(ship->angle - gravityAngle);

	if (ship->x_pos - x_prime < 0.0)
		shipAngle = atan((ship->y_pos - y_prime) / (ship->x_pos - x_prime));
	else
		shipAngle = atan((ship->y_pos - y_prime) / (ship->x_pos - x_prime)) + M_PI;

	angleFactShip = sin(ship->angle - shipAngle);

	shipDist = sqrt(pow(ship->x_pos,2)+pow(ship->y_pos,2));

	if (shipDist < 0.75)
	{
		if (angleFact > 0.0)
		{
			ship->left = 1;
			ship->right = 0;
		}
		else
		{
			ship->left = 0;
			ship->right = 1;
		}
		ship->down = 1;
	}
	else
	{
		ship->down = 0;
		if (angleFactShip > 0.0)
		{
			ship->left = 0;
			ship->right = 1;
		}
		else
		{
			ship->left = 1;
			ship->right = 0;
		}
	}

	if (!ship->is_dead && !other_ship->is_dead && !ship->is_shooting && angleFactShip < 0.2 && angleFactShip > -0.2)
	{
		ship->is_shooting = 1;
		ship->x_pos_laser = ship->x_pos;
		ship->y_pos_laser = ship->y_pos;
		ship->angle_laser = ship->angle;
		ship->x_vel_laser = 0.03*cos(ship->angle_laser);
		ship->y_vel_laser = 0.03*sin(ship->angle_laser);
	}
}

void moveLaser(struct Ship *ship)
{
	if (ship->is_shooting)
	{
		ship->x_pos_laser += ship->x_vel_laser;
		ship->y_pos_laser += ship->y_vel_laser;

		if (ship->x_pos_laser <= -1.0 || ship->x_pos_laser >= 1.0 || ship->y_pos_laser <= -1.0 || ship->y_pos_laser >= 1.0)
			ship->is_shooting = 0;
	}
	else
	{
		ship->x_pos_laser = 0.0;
		ship->y_pos_laser = 0.0;
		ship->x_vel_laser = 0.0;
		ship->y_vel_laser = 0.0;
	}
}

void gravityShip(struct Ship *ship)
{
	double gravityAngle;

	if (ship->x_pos < 0.0)
		gravityAngle = atan(ship->y_pos/ship->x_pos);
	else
		gravityAngle = atan(ship->y_pos/ship->x_pos)+M_PI;

	if (!ship->is_dead)
	{
		ship->x_vel += 0.000008*cos(gravityAngle)/(pow(ship->x_pos,2)+pow(ship->y_pos,2));
		ship->y_vel += 0.000008*sin(gravityAngle)/(pow(ship->x_pos,2)+pow(ship->y_pos,2));
	}
	else
	{
		ship->x_vel = 0.0;
		ship->y_vel = 0.0;
	}
}

void gravityLaser(struct Ship *ship)
{
	double gravityAngle;

	if (ship->x_pos_laser < 0.0)
		gravityAngle = atan(ship->y_pos_laser/ship->x_pos_laser);
	else
		gravityAngle = atan(ship->y_pos_laser/ship->x_pos_laser)+M_PI;

	ship->x_vel_laser += 0.001*cos(gravityAngle)/(pow(ship->x_pos_laser,2)+pow(ship->y_pos_laser,2));
	ship->y_vel_laser += 0.001*sin(gravityAngle)/(pow(ship->x_pos_laser,2)+pow(ship->y_pos_laser,2));
}

void detectLaserCollision(void)
{
	// use 1/2 of width = 0.017 for small side of rectangular hitbox
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
	if (shipDist < 0.0025 && !blue.is_dead && !red.is_dead)
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

void deathShip(struct Ship *ship, struct Ship *other_ship)
{
	if (ship->is_dead)
	{
		if (ship->death_tick == 0.0)
		{
			other_ship->score++;
			for (ship->death_count = 0; ship->death_count < 8; ship->death_count++)
			{
				ship->death_angle[ship->death_count] = rand() % 1256 - 628;
			}
		}

		ship->death_tick += 0.001;
		if (ship->death_tick >= 0.15)
		{
			ship->x_vel = 0.0;
			ship->y_vel = 0.0;

			do
			{
				ship->x_pos = rand() % 200 - 99;
				ship->x_pos /= 100;
				ship->y_pos = rand() % 200 - 99;
				ship->y_pos /= 100;
			} while (ship->x_pos <= 0.5 && ship->x_pos >= -0.5 || ship->y_pos <= 0.5 && ship->y_pos >= -0.5);

			ship->is_dead = 0;
			ship->death_tick = 0.0;
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
	if (colorPulsar >= 2. * M_PI)
		colorPulsar = 0.0;
}

void displayShip(struct Ship *ship)
{
	if (!ship->is_dead)
	{
		glColor3f(ship->red,ship->green,ship->blue);
		glBegin(GL_QUADS);
			glVertex2f(ship->x_pos,ship->y_pos);
			glVertex2f(ship->x_pos-0.1*cos(ship->angle+0.17),ship->y_pos-0.1*sin(ship->angle+0.17));
			glVertex2f(ship->x_pos-0.07*cos(ship->angle),ship->y_pos-0.07*sin(ship->angle));
			glVertex2f(ship->x_pos-0.1*cos(ship->angle-0.17),ship->y_pos-0.1*sin(ship->angle-0.17));
		glEnd();

		if (ship->down)
		{
			glBegin(GL_LINES);
				glColor3f(0.2,1.0,0.0);
				glVertex2f(ship->x_pos-0.08*cos(ship->angle),ship->y_pos-0.08*sin(ship->angle));
				glColor3f(1.0,0.2,0.0);
				glVertex2f(ship->x_pos-0.15*cos(ship->angle),ship->y_pos-0.15*sin(ship->angle));
			glEnd();
		}
	}
	else
	{
		glColor3f(ship->red,ship->green,ship->blue);
		glPointSize(2.0);
		glBegin(GL_POINTS);
		for (ship->death_count = 0; ship->death_count < 8; ship->death_count++)
		{
			glVertex2f(ship->x_pos+ship->death_tick*cos(ship->death_angle[ship->death_count]/100),ship->y_pos+ship->death_tick*sin(ship->death_angle[ship->death_count]/100));
		}
		glEnd();
	}
}

void displayLaser(struct Ship *ship)
{
	if (ship->is_shooting)
	{
		glColor3f(0.0,1.0,0.0);
		glBegin(GL_LINES);
			glVertex2f(ship->x_pos_laser,ship->y_pos_laser);
			glVertex2f(ship->x_pos_laser-0.02*cos(ship->angle_laser),ship->y_pos_laser-0.02*sin(ship->angle_laser));
		glEnd();
	}
}

void displayScores(void)
{
	char cscore1[5], cscore2[5];

	sprintf(cscore1, "%u", blue.score);
	sprintf(cscore2, "%u", red.score);

	glColor3f(blue.red,blue.green,blue.blue);
	glRasterPos2f(-0.9,0.9);
	for (char* c = cscore1; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*c);

	glColor3f(red.red,red.green,red.blue);
	glRasterPos2f(-0.7,0.9);
	for (char* c = cscore2; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*c);
}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// move the ships
	moveShip(&blue);
	moveShip(&red);

	// AI function
	shipAI(&blue, &red);
	shipAI(&red, &blue);

	// move the lasers
	moveLaser(&blue);
	moveLaser(&red);

	// apply gravity
	gravityShip(&blue);
	gravityShip(&red);

	// collision detection
	detectLaserCollision();
	detectShipCollision();
	detectPulsarCollision();

	// sequences if ships are dead
	deathShip(&blue, &red);
	deathShip(&red, &blue);

	// display the field
	displayStars();
	displayPulsar();

	// display the ships
	displayShip(&blue);
	displayShip(&red);

	// display the lasers
	displayLaser(&blue);
	displayLaser(&red);

	displayScores();

	glFlush();
}
