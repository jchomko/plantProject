/*
 *  Vehicle.h
 *  plantShakeWalkerSerial
 *
 *  Created by Jonathan Chomko on 13-03-06.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef VEHICLE
#define VEHICLE

#include <iostream>
#include "ofMain.h"


class Vehicle{
public:
    
	Vehicle(int x, int y, int fx, int fy, int life);
	void run();
	void update();
	void wander();
	void applyForce(ofVec2f force);
	void seek(ofVec2f target);
	void display();
	void borders();
	void drawWanderStuff(ofVec2f location,ofVec2f circle, ofVec2f target, float rad);
	void checkEdges();
	void avoid(ofVec2f target);
	void checkLife();
	void checkPause();
	
	
	ofVec2f location;
	ofVec2f velocity;
	ofVec2f acceleration;
	float r;
	float wandertheta;
	float maxforce;    // Maximum steering force
	float maxspeed;    // Maximum speed
	
	float variablespeed;
	
	int life;
	int lifespan;
	int borderSpacer;
	bool stop;
	bool pause;
	long getTime;
	int waitTime;
	bool atEdge;
	float n;
	
	
	ofVec2f fearloc;
	ofVec2f topLeftCorner;
    ofVec2f bottomRightCorner;
	
    
private:
	
};

#endif /* defined(__plantShake__Mover__) */


