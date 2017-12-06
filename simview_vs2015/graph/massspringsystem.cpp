/* CSC 473 Fall 2014
 * Ryan Guy
 */

#include "massspringsystem.h"
#include <QDebug>

MassSpringSystem::MassSpringSystem()
{
    dscale = 10;
}

MassSpringSystem::~MassSpringSystem()
{
    for (uint i=0; i<masses.size(); i++) {
        delete masses[i];
    }
    for (uint i=0; i<springs.size(); i++) {
        delete springs[i];
    }
}

// insert a mass into the system
// mass in kilograms, position is according to openGL coordinate system
PointMass* MassSpringSystem::addMass(float mass, float x, float y, float z, Vector f_ext){
	PointMass *m = new PointMass(mass, x, y, z, f_ext);
	m->setDistanceScale(dscale);
	masses.push_back(m);
	return m;
}


PointMass* MassSpringSystem::addMass(float mass, float x, float y, float z){
    PointMass *m = new PointMass(mass, x, y, z);
    m->setDistanceScale(dscale);
    masses.push_back(m);
    return m;
}

// Connect two masses with a spring
Spring* MassSpringSystem::addSpring(float springConstant, float restLength,
                                    PointMass *mass1, PointMass *mass2) {
    Spring *s = new Spring(springConstant, restLength, mass1, mass2);
    s->setDistanceScale(dscale);
    mass1->addSpring(s);
    mass2->addSpring(s);
    springs.push_back(s);
    return s;
}

// distance scale is in units/meter
void MassSpringSystem::setDistanceScale(float scale) {
    dscale = scale;

    for (uint i=0; i<masses.size(); i++) {
        masses[i]->setDistanceScale(dscale);
    }

    for (uint i=0; i<springs.size(); i++) {
        springs[i]->setDistanceScale(dscale);
    }
}

void MassSpringSystem::update(float dt, bool applyExtForce) {
    // update mass objects
    for (uint i=0; i<masses.size(); i++) {
		masses[i]->update(dt, applyExtForce);
    }

}

void MassSpringSystem::draw() {
    // draw masses
    for (uint i=0; i<masses.size(); i++) {
        masses[i]->draw();
    }

    // draw springs
    for (uint i=0; i<springs.size(); i++) {
        springs[i]->draw();
    }
}

void MassSpringSystem::draw(float scaleX, float scaleY, float scaleZ)
{
	// draw masses
	for (uint i = 0; i<masses.size(); i++) {
		masses[i]->draw(scaleX, scaleY, scaleZ);
	}

	// draw springs
	for (uint i = 0; i<springs.size(); i++) {
		springs[i]->draw(scaleX, scaleY, scaleZ);
	}
}









