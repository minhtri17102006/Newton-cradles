#include "pendulum.h"


#include "windows.h"
#include "vmath.h"

#include <math.h>
#include <iostream>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

Pendulum::Pendulum()//DefaultConstructor
{
    play = false;

    initTheta = -90.0; theta = initTheta; //Angle

    omega = 0; //Angular Velocity

    rcolor = 0.5;
    gcolor = 0.5;
    bcolor = 0.5;

    velocity = 0;
    position.x = 0; position.y = 0; position.z = 0;
    prevPos.x = 0; prevPos.y = 0; prevPos.z = 0;

    ropeLength = 15;
    radius = 1.2f;
    mass = 10.0;

    airDrag = (0.47/(2*mass))*0.0011839*M_PI*(radius*radius)*ropeLength; //0.47 = dragCoefficient for cradles, 0.0011839 is air density at 25Celcius

    initHeight = ropeLength*(1-cos(DEG2RAD(theta)));
    height = 0;

    nr = 0.0;

}

Pendulum::Pendulum(float theX, float theY, float theZ, int theNr, float theAngle)//Constructor
{
    play = false;

    rcolor = 0.5;
    gcolor = 0.5;
    bcolor = 0.5;

    initTheta = theAngle; theta = initTheta; //Angle

    omega = 0; //Angular Velocity

    velocity = 0;
    position.x = theX; position.y = theY; position.z = theZ;
    prevPos.x = theX; prevPos.y = theY; prevPos.z = theZ;

    ropeLength = 15;
    radius = 1.2f;
    mass = 10.0;

    airDrag = (0.47/(2*mass))*0.0011839*M_PI*(radius*radius)*ropeLength; //0.47 = dragCoefficient of sphere, 0.0011839 density of air at 25Celcius



    initHeight = ropeLength*(1-cos(DEG2RAD(theta)));
    height = 0;

    nr = theNr;
}

//How is the Euler key calculated for each frame?
void Pendulum::calculatePosition()
{

    prevPos.x = position.x;
    prevPos.y = position.y;

    //Euler'method from matlab
    //Calculate omega and theta
    //Calculate angular velocity and angular acceleration :D

    if(play)
    {
        //F?r sm? theta approximeras sin(theta) med theta.
        if(theta < 0.05 && theta > -0.05)
            omega -= timeStep*(((GRAVITY/ropeLength)*theta) + airDrag*omega*abs(omega));

        else
            omega -= timeStep*((GRAVITY/ropeLength)*sin(DEG2RAD(theta)) + airDrag*omega*abs(omega));


        theta += timeStep*omega;
    }

    //Calculate height

    height = ropeLength*(1-cos(DEG2RAD(theta)));

    //Calculate pace
    velocity = sqrt(2*GRAVITY*(initHeight-height)); //v = sqrt(2g*deltaH)


    //Update cradle's position
    position.y = -ropeLength*cos(DEG2RAD(theta));
    position.x = (ropeLength*sin(DEG2RAD(theta)) + (radius*nr));
}

//Function draw cradles.
//Update cradles's position according to place we got from CalculatePosition and rotate rope which connect to the ball like it's stuck.
void Pendulum::draw()
{

    calculatePosition();

    glPushMatrix();
    glColor3f(rcolor, gcolor, bcolor);
    glTranslatef(prevPos.x,prevPos.y, 0.0);
    glRotatef(theta, 0.0, 0.0, 1.0);
    glutSolidSphere(radius, 30.0f, 30.0f);

    glRotatef(-70.0f,1.0f,0.0f,0.0f);
    glTranslatef(0.0, 0.0, 0.0);
    glColor3f(0.9f, 0.9f, 0.8f);
    glutSolidCone(0.05f, ropeLength, 15.0f, 15.0f);


    glRotatef(-45.0f,1.0f,0.0f,0.0f);
    glTranslatef(0.0, 0.0, 0.0);
    glColor3f(0.9f, 0.9f, 0.8f);
    glutSolidCone(0.05f, ropeLength, 15.0f, 15.0f);

    glPopMatrix();
}

//Insert newest angular velocity in collision.
void Pendulum::insertNewOmega(float newOmega)
{
    omega = newOmega;
}

//Insert new angle, using when teleporting and when choosing the original angle
void Pendulum::insertNewTheta(float newTheta)
{
    theta = newTheta;
}

//Function to look for position cradles moving
//Approximeate at limit 0,3 because angular velocity can't be zero. It should be better like this:))
int Pendulum::calculateDirection()
{
    if(omega < -0.3)
    {
        return -1;
    }
    else if(omega > 0.3)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//Cradles dont update their position except u click button "p";
void Pendulum::setPlay(bool p)
{
    play = p;
}

//Function to change color of rope. It called setActive because of the ruins of old thoughts.
void Pendulum::setActive(float r, float g, float b)
{
    rcolor = r;
    gcolor = g;
    bcolor = b;
}
