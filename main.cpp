#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
//#include <ostringstream>

#include "camera.h"
#include "pendulum.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;



Camera camera;

const int nrOfPendulums = 6;
Pendulum pendlar[nrOfPendulums];

int material = 1;

//Funktioner
void collide(Pendulum &pendel1, Pendulum &pendel2);
void detectCollision(Pendulum pendlars[]);

void drawText(float x, float y, char *textString)
{
    int i, len;



    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glRasterPos2f(x, y);
    glColor4f(1.0f ,1.0f, 1.0f, 1.0f);
    for (i = 0, len = strlen(textString); i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)textString[i]);
    }


}

void init()
{
    for(int i = 0; i < nrOfPendulums; i++)
    {
        pendlar[i] = Pendulum(0.0,0.0,0.0,i*2,0.0);
    }

    pendlar[0].setActive(1.0, 0.0, 0.0);

}


void changeSize(int w, int h) 
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void setupLight()
{
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {50.0};
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    glClearColor (0.7, 0.7, 0.8, 0.5);
    glShadeModel (GL_SMOOTH);

    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable ( GL_COLOR_MATERIAL );

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}


//Function to search the collision
void detectCollision(Pendulum pendlars[])
{
    float limit = 2.0 * pendlars[0].getRadius();

    //Loopar igenom listan med pendlar.
    for(int i = 0; i < nrOfPendulums; i++)
    {

        if(i < nrOfPendulums-1)
        {
            //If two cradles from two head with the same angle and height, it will bounce reversally
            if(i > 0 && abs(pendlars[0].getXpos() - pendlars[1].getXpos()) < limit && abs(pendlars[nrOfPendulums-2].getXpos() - pendlars[nrOfPendulums-1].getXpos()) < limit && pendlars[0].getTheta() == -pendlars[nrOfPendulums-1].getTheta())
            {
                collide(pendlars[0], pendlars[nrOfPendulums-1]);
            }

            //Check continuosly in the list. i located in collied radius of cradles i+1
            if(abs(pendlars[i].getXpos() - pendlars[i+1].getXpos()) < limit)
            {
                collide(pendlars[i], pendlars[i+1]);
            }

        }


    }
}

//When detecting collision, two cradles will be there:v
void collide(Pendulum &pendel1, Pendulum &pendel2)
{


        float tempOmega1 = pendel1.getOmega();
        float tempOmega2 = pendel2.getOmega();

        //If u drop two cradles from the same height, not always collide
        if(pendel1.getTheta() == pendel2.getTheta() && pendel1.getOmega() == pendel2.getOmega() && pendel1.calculateDirection() == pendel2.calculateDirection())
        {
            return;
        }

        else
        {

            if(pendel1.calculateDirection() != pendel2.calculateDirection()) //If two pendulums move towards each other in opposite directions or if one pendulum is at rest.
            {

                if(pendel1.calculateDirection() != 0 && pendel2.calculateDirection() != 0) //If they come towards
                {
                    float tempOmega111 = tempOmega1*pendel1.calculateDirection();
                    float tempOmega222 = tempOmega2*pendel2.calculateDirection();

                    pendel1.insertNewTheta(pendel1.getTheta()+(pendel2.getTheta() - pendel1.getTheta()));//Move the two pendulums apart so they don't get stuck.


                    pendel1.insertNewOmega(tempOmega222*pendel1.getCOR(material));
                    pendel2.insertNewOmega(tempOmega111*pendel2.getCOR(material));

                }

                else if(pendel1.calculateDirection() == 0)//Om den ena pendeln st?r still
                {
                    pendel1.insertNewTheta(pendel1.getTheta()+(pendel2.getTheta() - pendel1.getTheta()));//Move the two pendulums apart so they don't get stuck.

                    pendel1.insertNewOmega(tempOmega2*pendel1.getCOR(material));
                    pendel2.insertNewOmega(tempOmega1*pendel2.getCOR(material));
                }

                else if(pendel2.calculateDirection() == 0)//If the second cradles remain the same position
                {
                    pendel2.insertNewTheta(pendel2.getTheta()+(pendel1.getTheta() - pendel2.getTheta()));//Move the two pendulums apart so they don't get stuck.

                    pendel2.insertNewOmega(tempOmega1*pendel2.getCOR(material));
                    pendel1.insertNewOmega(tempOmega2*pendel1.getCOR(material));
                }

            }

            else if(pendel1.calculateDirection() == pendel2.calculateDirection()) //If they go in the same direction and collide. The algorithm is pretty much the same as before.
            {
                if(pendel1.calculateDirection() != 0 && pendel2.calculateDirection() != 0)
                {
                    pendel1.insertNewTheta(pendel1.getTheta()+(pendel2.getTheta() - pendel1.getTheta()));


                    float tempOmega11 = pendel1.getOmega();
                    float tempOmega22 = pendel2.getOmega();

                    pendel2.insertNewOmega(tempOmega11*pendel2.getCOR(material));
                    pendel1.insertNewOmega(tempOmega22*pendel1.getCOR(material));
                }

                else if(pendel1.calculateDirection() == 0)
                {
                    pendel1.insertNewTheta(pendel1.getTheta()+(pendel2.getTheta() - pendel1.getTheta()));


                    pendel1.insertNewOmega(tempOmega2*pendel2.getCOR(material));
                    pendel2.insertNewOmega(tempOmega1*pendel1.getCOR(material));
                }

                else if(pendel2.calculateDirection() == 0)
                {
                    pendel2.insertNewTheta(pendel2.getTheta()+(pendel1.getTheta() - pendel2.getTheta()));


                    pendel2.insertNewOmega(tempOmega1*pendel2.getCOR(material));
                    pendel1.insertNewOmega(tempOmega2*pendel1.getCOR(material));
                }

            }

        }
}


void setupScene()
{
    //Draw all cradles
    for(int i = 0; i < nrOfPendulums; i++)
    {
        pendlar[i].draw();
    }

    //Run the function to search the collision everywhere
    detectCollision(pendlar);
}


void renderScene(void)
{
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	camera.movement();

    setupLight();

    setupScene();
    char text[] = "Use keys '4' and '6' to choose pendulum.";
    char text2[] = "Use ''Shift +' and '-' to change the initial angle.'Shift +' raises the pendulum to the right, and '-' to the left.";
    char text3[] = "Press 'p' to start the simulation and 't' to pause.";
    char text4[] = "Press '1' for steel balls, '2' for wooden balls and '3' for glass balls.";
    char text5[] = "Press '4' to decrease the order of the cradles and '6' to increase the order of the cradles.";
    char text6[] = "Made By Hoang Minh Tri, Nguyen Huu Phong. 12/4/2025";

    drawText(-3.0,-1.2, text);
    drawText(-3.0,-1.3, text2);
    drawText(-3.0,-1.4, text3);
    drawText(-3.0,-1.5, text4);
    drawText(-3.0,-1.6, text5);
    drawText(-3.0,-1.8, text6);

	glutSwapBuffers();
}

float angle = 0.0;
int active = 0;
float materialR = 0.5;
float materialG = 0.5;
float materialB = 0.5;

//Control function.
void key (unsigned char key, int x, int y)
{
    if(key == '6')
    {
        if(active >= nrOfPendulums)
            active = nrOfPendulums-1;

        pendlar[active].setActive(materialR, materialG, materialB);
        active++;

        if(active >= nrOfPendulums)
            active = nrOfPendulums-1;

        angle = pendlar[active].getTheta();
        pendlar[active].setActive(1.0, 0.0, 0.0);
    }

    if(key == '4')
    {
        if(active <= 0)
            active = 0;

        pendlar[active].setActive(materialR, materialG, materialB);
        active--;

        if(active <= 0)
            active = 0;

        angle = pendlar[active].getTheta();
        pendlar[active].setActive(1.0, 0.0, 0.0);
    }

    if(key == '+')
    {
        pendlar[active].insertNewTheta(angle);
        angle--;
    }

    if(key == '-')
    {
        pendlar[active].insertNewTheta(angle);
        angle++;
    }

    if(key == 'p')
        {
            for(int i = 0; i < nrOfPendulums; i++)
            {
                pendlar[i].setActive(materialR, materialG, materialB);
                pendlar[i].setPlay(true);
            }
        }

    if(key == 't')
    {
        for(int i = 0; i < nrOfPendulums; i++)
        {
                pendlar[i].setPlay(false);
        }
    }

    if(key == '1')
    {
        material = 1;
        materialR = 0.5;
        materialG = 0.5;
        materialB = 0.5;

        for(int i = 0; i < nrOfPendulums; i++)
            pendlar[i].setActive(materialR, materialG, materialB);
    }

    if(key == '2')
    {
        material = 2;
        materialR = 0.5;
        materialG = 0.3;
        materialB = 0.2;

        for(int i = 0; i < nrOfPendulums; i++)
            pendlar[i].setActive(materialR, materialG, materialB);
    }

    if(key == '3')
    {
        material = 3;

        materialR = 1.0;
        materialG = 1.0;
        materialB = 1.0;

        for(int i = 0; i < nrOfPendulums; i++)
            pendlar[i].setActive(materialR, materialG, materialB);
    }
}


int main(int argc, char **argv) 
{


    //Create cradles
    init();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1152,648);
	glutCreateWindow("Newton's Cradle");

	// register callbackspendlar[i]
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(key);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle

	glutMainLoop();
	
	system("CLS");
	return 0;
}
