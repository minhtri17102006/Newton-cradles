/* Functions for handeling camera movement.


*/
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "vmath.h"

class Camera
{
private:
// Variables to save current state
float xpos;
float ypos;
float zpos;
float xrot;
float yrot;
float lastx, lasty;

public:
    Camera();
    void movement (void);
    void key (unsigned char key, int x, int y);
    void mouseClick(int button, int state, int x, int y);
    void mouseMovement(int x, int y);
    Vector3f getPosition();

};
#endif
