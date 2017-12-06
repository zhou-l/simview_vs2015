
#ifndef POINTMASS_H
#define POINTMASS_H
#include <vector>
#include "spring.h"
#include "geometry.h"
#include <QOpenGLFunctions_3_3_Compatibility>

class Spring;

// A point mass is a mass at a position
// This object controls motion of the mass under the force of gravity
// and connected springs
class PointMass : public QOpenGLFunctions_3_3_Compatibility
{
public:
	PointMass();
    PointMass(float mass, float x, float y, float z);
	PointMass(float mass, float x, float y, float z, Vector external_force);

    void setPosition(float x, float y, float z);
    void setMass(float m);
    void setDistanceScale(float scale);
    void setFixedPosition(bool r);
    void addSpring(Spring *s);

    Vector calculateForces();  // calculate all forces acting upon the mass
                                  // (gravity and springs
    void update(float dt, bool applyExtForce = true);
    void draw();
	void draw(float scaleX, float scaleY, float scaleZ);

    float mass;     // in kilograms;
    float dscale;   // distance scale in units/metre
	Vector position;
    Vector velocity;
    Vector gravity;
	Vector f_external; // the applied force
    bool isFixedPosition;    // masses in fixed position do not move

    std::vector<Spring*> springs;  // holds all springs connected to a mass

};

#endif // POINTMASS_H
