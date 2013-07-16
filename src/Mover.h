//
//  Mover.h
//  plantShake
//
//  Created by fabrica197 on 11/01/13.
//
//

#ifndef __plantShake__Mover__
#define __plantShake__Mover__

#include <iostream>
#include "ofMain.h"

class Mover{
public:
    
    ofVec2f topLeftCorner;
    ofVec2f bottomRightCorner;
    ofVec2f center;
    Mover(int x, int y, float inc);
    
    float inc;
    int lifespan;
    float runMult;
    float runDistribution;
    int x,y;
    
    float n;
   
    void update();
    void seek();
    void draw();
    void checkEdges();
       
    ofVec2f location;
    ofVec2f velocity;
    ofVec2f acceleration;
    ofVec2f fearLoc;
    int life;
    bool atEdge;
    bool stop;
	long getTime;
	bool pause;
    
	
	

    
private:

};

#endif /* defined(__plantShake__Mover__) */


