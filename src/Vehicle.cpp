/*
 *  Vehicle.cpp
 *  plantShakeWalkerSerial
 *
 *  Created by Jonathan Chomko on 13-03-06.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Vehicle.h"


Vehicle::Vehicle( int _x, int _y, int _fx, int _fy, int _life)
{
	acceleration.set(0,0);
    velocity.set(0,0);
    location.set(_x,_y);
	
	fearloc.set(_fx, _fy);
	
    r = 6;
    wandertheta = 0;
    maxspeed = 2;
    maxforce = 0.08;
	borderSpacer = 20;
	atEdge = false;
	life = _life;
    lifespan = 0;
	stop = false;
	pause = false;
	
	variablespeed = 0;
	n = 0;
    
}

void Vehicle::checkLife(){
    
    if(life > 0){
		life -= 1;
		
	} 
	if (life <= 0 && atEdge) {
	    stop = true;
		
	}
	

}


void Vehicle::checkPause(){

	if (pause == true && ofGetElapsedTimeMillis() - getTime > waitTime) {
		pause = false;
	}
	
}
void Vehicle::run() {
	
    update();
	//borders();
    checkEdges();
	display();

}


// Method to update location
void Vehicle::update() {
    
    // Update velocity
	if(!stop){
        
        float m = 0.1;

        if(life > 0){
            m = (float)lifespan/life;
        }
       // cout << m;
        
        velocity += acceleration;
        
        // Limit speed
        velocity.limit(maxspeed/m);
        
        // cout << maxspeed*m;
        location += velocity;
        
        // Reset accelertion to 0 each cycle
        acceleration *= 0;
		
	}
    
}



void Vehicle::wander() {
	
	n += 0.01;
	float ns = ofNoise(n);
	float r = ofRandom(0,1);
	
	/*
	
	if (r < 0.01 && !pause) {
		pause = true;
		getTime = ofGetElapsedTimeMillis();
		waitTime = ofRandom(500,2000);
	}
	 */
	
	
	
    float wanderR = 25;         // Radius for our "wander circle"
    float wanderD = 60*ns;          // Distance for our "wander circle"
    float change = 0.6;
    
    wandertheta += ofRandom(-change,change);     // Randomly change wander theta
	
    // Now we have to calculate the new location to steer towards on the wander circle
	ofVec2f circleloc = velocity;    // Start with velocity
    circleloc.normalize();            // Normalize to get heading
    circleloc *= wanderD;          // Multiply by distance
    circleloc += location;               // Make it relative to boid's location
	
	float angle = (float)atan2(velocity.x, velocity.y);
	
	float h = angle;
   // float h = velocity.heading2D();        // We need to know the heading to offset wandertheta
	
    ofVec2f circleOffSet(wanderR*cos(wandertheta+h),wanderR*sin(wandertheta+h));
    ofVec2f target = circleloc + circleOffSet;
	
    seek(target);
	
    // Render wandering circle, etc. 
    drawWanderStuff(location,circleloc,target,wanderR);
} 

void Vehicle::avoid(ofVec2f target){
	
    ofVec2f desired = target - location;
	float d = desired.length();
	
	
	desired.normalize();
	desired *= - maxspeed; //*5
	
	
	ofVec2f steer = desired - velocity;
    
     
   
    steer.limit(maxforce);
	
	
    
    
    applyForce(steer);
	

}

void Vehicle::applyForce(ofVec2f force) {
    // We could add mass here if we want A = F / M
	
	acceleration += force;

}



void Vehicle::seek(ofVec2f target) {
    ofVec2f desired = target - location;  // A vector pointing from the location to the target
	
    // Normalize desired and scale to maximum speed
    desired.normalize();
    desired *= maxspeed/2;
	
    // Steering = Desired minus Velocity
    ofVec2f steer = desired - velocity;
    
    steer.limit(maxforce);  // Limit to maximum steering force
	
    applyForce(steer);
}



void Vehicle::display() {
    // Draw a triangle rotated in the direction of velocity
	ofPushStyle();
	ofFill();
	float theta = (float)atan2(velocity.x, velocity.y);
    ofPushMatrix();
    ofTranslate(location.x,location.y);
    ofRotateZ(theta);

   
	ofTriangle(0, -r*2, -r, r*2,r, r*2);
   // ofVertex(0, -r*2);
   // ofVertex(-r, r*2);
   // ofVertex(r, r*2);
   // ofEndShape();
    ofPopMatrix();
	ofPopStyle();
	
}

// Wraparound
void Vehicle::borders() {
    if (location.x < -r) location.x = ofGetWidth()+r;
    if (location.y < -r) location.y = ofGetHeight()+r;
    if (location.x > ofGetWidth()+r) location.x = -r;
    if (location.y > ofGetHeight()+r) location.y = -r;

}

void Vehicle::checkEdges()
{
	ofVec2f desired(0,0);
	
	if(location.x < topLeftCorner.x + borderSpacer){ // 
        desired.set(maxspeed,velocity.y);
       
		if(life <= 0){
			stop = true;
		}

    }
	
	else if(location.x > bottomRightCorner.x - borderSpacer){ // 
		desired.set(-maxspeed,velocity.y);
		
		if(life <= 0){
			stop = true;
		}
		
	}
	
	if(location.y < topLeftCorner.y + borderSpacer){
		desired.set(velocity.x,maxspeed);
       
		if(life <= 0){
			stop = true;
		}
    }
		else if(location.y > bottomRightCorner.y - borderSpacer){ // 
		desired.set(velocity.x,-maxspeed);
        
		if(life <= 0){
			stop = true;
		}
		
    }
	
	if(desired.x != 0 && desired.y != 0){
		desired.normalize();
		desired *= maxspeed;
		ofVec2f steer = desired - velocity;
		steer.limit(maxforce);
		applyForce(steer);
		//velocity = desired;
	}
}



// A method just to draw the circle associated with wandering
void Vehicle::drawWanderStuff(ofVec2f location, ofVec2f circle, ofVec2f target, float rad) {
	
	ofEllipse(circle.x,circle.y,rad*2,rad*2);
	ofEllipse(target.x,target.y,4,4);
	ofLine(location.x,location.y,circle.x,circle.y);
	ofLine(circle.x,circle.y,target.x,target.y);
}

