
#include "pointmass.h"

PointMass::PointMass()
{
	mass = 1;
	dscale = 0.1;
	position = Vector(0, 0, 0);
	velocity = Vector(0.0, 0.0, 0.0);
	gravity = Vector(0.0, -9.81, 0.0);
	f_external = Vector(0, 0, 0);
	isFixedPosition = false;
}

PointMass::PointMass(float m, float x, float y, float z)
{
    mass = m;
    dscale = 0.1;
    position = Vector(x, y, z);
    velocity = Vector(0.0, 0.0, 0.0);
    gravity = Vector(0.0, -9.81, 0.0);
	f_external = Vector(0, 0, 0);
    isFixedPosition = false;
}

PointMass::PointMass(float m, float x, float y, float z, Vector external_force)
{
	mass = m;
	dscale = 0.1;
	position = Vector(x, y, z);
	velocity = Vector(0, 0, 0);
	//gravity = Vector(0.0, -9.81, 0.0);
	// no gravity
	gravity = Vector(0, 0, 0);
	f_external = external_force;
	//f_external = Vector(0,0,0);
	isFixedPosition = false;
}

void PointMass::setPosition(float x, float y, float z) {
    position = Vector(x, y, z);
}

// in kilograms
void PointMass::setMass(float m) {
    mass = m;
}

// in units/meter
void PointMass::setDistanceScale(float scale) {
    dscale = scale;
}

// a mass in fixed position does not move
void PointMass::setFixedPosition(bool r) {
    isFixedPosition = r;
}

// connect a spring to the mass
void PointMass::addSpring(Spring *s) {
    springs.push_back(s);
}

// force of gravity and springs
Vector PointMass::calculateForces() {
    Vector fg = gravity * mass;
    Vector fs = Vector(0.0, 0.0, 0.0);
	Vector fext = f_external;

    for (uint i=0; i<springs.size(); i++) {
        fs += springs[i]->getForce(this);
    }

    Vector force = fg + fs + fext;
    return force;
}

void PointMass::update(float dt, bool applyExtForce) {
    if (isFixedPosition) { return; }

    // F = m*a -> a = F / m
    Vector acc = (applyExtForce)? calculateForces() / mass : Vector(0,0,0);

    // vf = vi + a*dt
    velocity = velocity + acc * dt;

    // pf = pi + v*dt
    position = position + velocity * dscale * dt;
}

void PointMass::draw() {

    glColor4f(0.0, 0.5, 1.0, 1.0);
    glPointSize(2.0);
    glBegin(GL_POINTS);
    glVertex3f(position.x, position.y, position.z);
    glEnd();
}

void PointMass::draw(float scaleX, float scaleY, float scaleZ)
{
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glPointSize(10.0);
	glBegin(GL_POINTS);
	glVertex3f(scaleX * position.x, scaleY * position.y, scaleZ * position.z);
	glEnd();
}



















