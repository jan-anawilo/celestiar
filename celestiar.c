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
	double death_tick, death_angle[16];
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

struct StarField {
	double xStar[35], yStar[35], starColor[35], starSize[35];
	int starCount;
	double colorPulsar;
} star_field;

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
void generateStars(struct StarField *);

/* MECHANICS AND GAMEPLAY FUNCTIONS */
void moveShip(struct Ship *);
void shipAI(struct Ship *, const struct Ship *);
void moveLaser(struct Ship *);
void gravityShip(struct Ship *);
void gravityLaser(struct Ship *);

void detectLaserCollision(struct Ship *, struct Ship *);
void detectShipCollision(struct Ship *, struct Ship *);
void detectPulsarCollision(struct Ship *, struct Ship *);

void deathShip(struct Ship *, struct Ship *);

/* DISPLAY FUNCTIONS */
void displayStars(struct StarField *);
void displayPulsar(struct StarField *);

void displayShip(struct Ship *);
void displayLaser(struct Ship *);

void displayScores(const struct Ship *, const struct Ship *);

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

	generateStars(&star_field);

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
		case 'd' :
			blue.right = 1;
			break;
		case 'w' :
            blue.down = 1;
			break;
		case 's' :
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
		case 'd' :
			blue.right = 0;
			break;
		case 'w' :
			blue.down = 0;
			break;
		case 's' :
			blue.down = 0;
			break;
	}
}

void generateStars(struct StarField *starfield)
{
	srand(time(NULL));
	for (starfield->starCount = 0; starfield->starCount <= 34; starfield->starCount++)
	{
		starfield->starColor[starfield->starCount] = rand() % 9 + 2;
		starfield->starColor[starfield->starCount] /=  10.;

		starfield->xStar[starfield->starCount] = rand() % 200 - 99;
		starfield->xStar[starfield->starCount] /= 100.;
		starfield->yStar[starfield->starCount] = rand() % 200 - 99;
		starfield->yStar[starfield->starCount] /= 100.;

		starfield->starSize[starfield->starCount] = rand() % 150 + 50;
		starfield->starSize[starfield->starCount] /= 100.;
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

void shipAI(struct Ship *ship, const struct Ship *other_ship)
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
//		printf("%g\n", angleFact);
//		if (angleFact < -0.9 || angleFact > 0.9)
//			ship->down = 0;
//		else
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

	if (!ship->is_dead && !other_ship->is_dead && !ship->is_shooting && angleFactShip < 0.05 && angleFactShip > -0.05)
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

void detectLaserCollision(struct Ship *ship, struct Ship *other_ship)
{
	// use 1/2 of width = 0.017 for small side of rectangular hitbox
	if (triangle_collison(other_ship->x_pos_laser, other_ship->y_pos_laser,
		ship->x_pos-0.017*cos(ship->angle-M_PI/2.), ship->y_pos-0.017*sin(ship->angle-M_PI/2.),
		ship->x_pos-0.1*cos(ship->angle-0.17), ship->y_pos-0.1*sin(ship->angle-0.17),
		ship->x_pos-0.1*cos(ship->angle+0.17), ship->y_pos-0.1*sin(ship->angle+0.17)))
	{
		other_ship->is_shooting = 0;
		ship->is_dead = 1;
	}
	else if (triangle_collison(other_ship->x_pos_laser, other_ship->y_pos_laser,
		ship->x_pos-0.017*cos(ship->angle-M_PI/2.), ship->y_pos-0.017*sin(ship->angle-M_PI/2.),
		ship->x_pos-0.017*cos(ship->angle+M_PI/2.), ship->y_pos-0.017*sin(ship->angle+M_PI/2.),
		ship->x_pos-0.1*cos(ship->angle+0.17), ship->y_pos-0.1*sin(ship->angle+0.17)))
	{
		other_ship->is_shooting = 0;
		ship->is_dead = 1;
	}

	if (triangle_collison(ship->x_pos_laser, ship->y_pos_laser,
		other_ship->x_pos-0.017*cos(other_ship->angle-M_PI/2.), other_ship->y_pos-0.017*sin(other_ship->angle-M_PI/2.),
		other_ship->x_pos-0.1*cos(other_ship->angle-0.17), other_ship->y_pos-0.1*sin(other_ship->angle-0.17),
		other_ship->x_pos-0.1*cos(other_ship->angle+0.17), other_ship->y_pos-0.1*sin(other_ship->angle+0.17)))
	{
		ship->is_shooting = 0;
		other_ship->is_dead = 1;
	}
	else if (triangle_collison(ship->x_pos_laser, ship->y_pos_laser,
		other_ship->x_pos-0.017*cos(other_ship->angle-M_PI/2.), other_ship->y_pos-0.017*sin(other_ship->angle-M_PI/2.),
		other_ship->x_pos-0.017*cos(other_ship->angle+M_PI/2.), other_ship->y_pos-0.017*sin(other_ship->angle+M_PI/2.),
		other_ship->x_pos-0.1*cos(other_ship->angle+0.17), other_ship->y_pos-0.1*sin(other_ship->angle+0.17)))
	{
		ship->is_shooting = 0;
		other_ship->is_dead = 1;
	}
}

void detectShipCollision(struct Ship *ship, struct Ship *other_ship)
{
	double shipDist;
	double x_prime0, y_prime0, x_prime1, y_prime1;

	x_prime0 = ship->x_pos;
	y_prime0 = ship->y_pos;

	x_prime1 = other_ship->x_pos;
	y_prime1 = other_ship->y_pos;

	// adjust for center of hitbox
	x_prime0 -= 0.05*cos(ship->angle);
	y_prime0 -= 0.05*sin(ship->angle);

	x_prime1 -= 0.05*cos(other_ship->angle);
	y_prime1 -= 0.05*sin(other_ship->angle);

	shipDist = pow(x_prime0 - x_prime1, 2) + pow(y_prime0 - y_prime1, 2);
	if (shipDist < 0.0025 && !ship->is_dead && !other_ship->is_dead)
	{
//		printf("Ship collision\n");
		ship->is_dead = 1;
		other_ship->is_dead = 1;
	}
}

void detectPulsarCollision(struct Ship *ship, struct Ship *other_ship)
{
	if (ship->x_pos >= -0.05 && ship->x_pos <= 0.05
		&& ship->y_pos >= -0.05 && ship->y_pos <= 0.05)
		ship->is_dead = 1;

	if (other_ship->x_pos >= -0.05 && other_ship->x_pos <= 0.05
		&& other_ship->y_pos >= -0.05 && other_ship->y_pos <= 0.05)
		other_ship->is_dead = 1;

	if (ship->x_pos_laser <= 0.05 && ship->x_pos_laser >= -0.05
		&& ship->y_pos_laser <= 0.05 && ship->y_pos_laser >= -0.05)
		ship->is_shooting = 0;

	if (other_ship->x_pos_laser <= 0.05 && other_ship->x_pos_laser >= -0.05
		&& other_ship->y_pos_laser <= 0.05 && other_ship->y_pos_laser >= -0.05)
		other_ship->is_shooting = 0;
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

void displayStars(struct StarField *starfield)
{
	for (starfield->starCount = 0; starfield->starCount <= 34; starfield->starCount++)
	{	
		starfield->xStar[starfield->starCount] += 0.0005;
		starfield->yStar[starfield->starCount] += 0.0001;
		if (starfield->xStar[starfield->starCount] >= 1.)
			starfield->xStar[starfield->starCount] = -1.;
		if (starfield->yStar[starfield->starCount] >= 1.)
			starfield->yStar[starfield->starCount] = -1.;

		glPointSize(starfield->starSize[starfield->starCount]);
		glBegin(GL_POINTS);
		glColor3f(starfield->starColor[starfield->starCount],starfield->starColor[starfield->starCount],starfield->starColor[starfield->starCount]);
		glVertex2f(starfield->xStar[starfield->starCount],starfield->yStar[starfield->starCount]);
		glEnd();
	}
}

void displayPulsar(struct StarField *starfield)
{
	glColor3f(0.5*sin(starfield->colorPulsar)+0.5,0.5*sin(starfield->colorPulsar)+0.5,0.5*sin(starfield->colorPulsar)+0.5);
	glBegin(GL_POLYGON);
	for (double angle = 0.0; angle < 6.28; angle += 0.8)
	{
		glVertex2f(0.025*cos(angle),0.025*sin(angle));
	}
	glEnd();

	starfield->colorPulsar += 0.2;
	if (starfield->colorPulsar >= 2. * M_PI)
		starfield->colorPulsar = 0.0;
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
		for (ship->death_count = 0; ship->death_count < 16; ship->death_count++)
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

void displayScores(const struct Ship *ship, const struct Ship *other_ship)
{
	char cscore1[5], cscore2[5];

	snprintf(cscore1, sizeof(cscore1), "%u", ship->score);
	snprintf(cscore2, sizeof(cscore2), "%u", other_ship->score);

	glColor3f(ship->red,ship->green,ship->blue);
	glRasterPos2f(-0.9,0.9);
	for (char* c = cscore1; *c != '\0'; c++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*c);

	glColor3f(other_ship->red,other_ship->green,other_ship->blue);
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
//	shipAI(&blue, &red);
	shipAI(&red, &blue);

	// move the lasers
	moveLaser(&blue);
	moveLaser(&red);

	// apply gravity
	gravityShip(&blue);
	gravityShip(&red);

	// collision detection
	detectLaserCollision(&blue, &red);
	detectShipCollision(&blue, &red);
	detectPulsarCollision(&blue, &red);

	// sequences if ships are dead
	deathShip(&blue, &red);
	deathShip(&red, &blue);

	// display the field
	displayStars(&star_field);
	displayPulsar(&star_field);

	// display the ships
	displayShip(&blue);
	displayShip(&red);

	// display the lasers
	displayLaser(&blue);
	displayLaser(&red);

	displayScores(&blue, &red);

	glFlush();
}
