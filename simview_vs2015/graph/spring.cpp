/* CSC 473 Fall 2014
 * Ryan Guy
 */

#include "spring.h"

Spring::Spring(float springConstant, float restLength,
               PointMass *mass1, PointMass *mass2)
{
    k = springConstant;
    //kd = 0.1;
	kd = 0.5;
    rl = restLength;
    m1 = mass1;
    m2 = mass2;
    dscale = 10;
}

void Spring::setSpringConstant(float springConstant) {
    k = springConstant;
}

void Spring::setDampingConstant(float dampingConstant) {
    kd = dampingConstant;
}

void Spring::setRestLength(float restLength) {
    rl = restLength;
}

void Spring::setDistanceScale(float scale) {
    dscale = scale;
}

// calculates force vector acting upon refMass
// Force is calculated according to Hooke's law and includes damping
Vector Spring::getForce(PointMass *refMass) {
    if (!(refMass == m1 || refMass == m2)) {
        return Vector(0, 0, 0);
    }

    // spring force
	Vector v_12 = m1->position - m2->position;
	float dist = v_12.Length(); 
    float scalar = dscale*k*(dist - rl);
	Vector v_21 = m2->position - m1->position;
	Vector dir = Normalize(v_21);
		//glm::normalize(m2->position - m1->position);

    // find speed of contraction/expansion for damping force
	float s1 = Dot(m1->velocity, dir); // glm::dot(m1->velocity, dir);
	float s2 = Dot(m2->velocity, dir); // glm::dot(m2->velocity, dir);
    float dampingScalar = -kd * (s1 + s2);

    if (refMass == m1) {
		return dir * (scalar + dampingScalar);
    } else{
        return dir * (-scalar + dampingScalar);
    }

}

void Spring::draw() {
    glLineWidth(1.0);
    glColor4f(0.0, 0.3, 1.0, 0.3);

    glBegin(GL_LINES);
    glVertex3f(m1->position.x, m1->position.y, m1->position.z);
    glVertex3f(m2->position.x, m2->position.y, m2->position.z);
    glEnd();

}

void Spring::draw(float scaleX, float scaleY, float scaleZ)
{
	glLineWidth(1.0);
	glColor4f(0.0, 0.3, 1.0, 0.3);

	glBegin(GL_LINES);
	glVertex3f(scaleX * m1->position.x, scaleY * m1->position.y, scaleZ * m1->position.z);
	glVertex3f(scaleX * m2->position.x, scaleY * m2->position.y, scaleZ * m2->position.z);
	glEnd();

}