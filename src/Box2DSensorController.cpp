#include "Box2DSensorController.h"

void Box2DSensorController::SetSensor(b2Body* bodyToTrack)
{
    this->bodyToTrack = bodyToTrack;
}

bool Box2DSensorController::IsBeingTriggered()
{
    return bodiesInside!=0;
}

bool Box2DSensorController::OnTriggerEnter(b2Body& bodyToEnter)
{
    if (lastBodyEnter != nullptr) {
        bodyToEnter = *lastBodyEnter;
        lastBodyEnter = nullptr;
        return true;
    }
    return false;
}

bool Box2DSensorController::OnTriggerExit(b2Body& bodyToExit)
{
    if (lastBodyExit != nullptr) {
        bodyToExit = *lastBodyExit;
        lastBodyExit = nullptr;
        return true;
    }
    return false;
}

void Box2DSensorController::BeginContact(b2Contact* contact)
{
    // Get the two fixtures involved in the contact
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    // Get the bodies involved in the contact
    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    // Check if the tracked body is involved
    if (bodyA == bodyToTrack || bodyB == bodyToTrack) {
        // Determine which fixture is the sensor (if any)
        if (fixtureA->IsSensor() || fixtureB->IsSensor()) {
            bodiesInside++;
            lastBodyEnter = GetDifferentBody(bodyA, bodyB, bodyToTrack);
        }
    }
}

void Box2DSensorController::EndContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();

    // Check if the tracked body is involved
    if (bodyA == bodyToTrack || bodyB == bodyToTrack) {
        // Determine which fixture is the sensor (if any)
        if (fixtureA->IsSensor() || fixtureB->IsSensor()) {
            bodiesInside--;
            lastBodyExit = GetDifferentBody(bodyA, bodyB, bodyToTrack);
        }
    }
}

b2Body* Box2DSensorController::GetDifferentBody(b2Body* body1, b2Body* body2, b2Body* bodyToBeDifferentFrom)
{
    if (body1 != bodyToBeDifferentFrom)
        return body1;
    return body2;
}


