//
//  Mover.cpp
//  plantShake
//
//  Created by fabrica197 on 11/01/13.
//
//

#include "Mover.h"


Mover::Mover( int _x, int _y, float _vel)
{
    x = _x;
    y = _y;
    location.set(x,y);
    life = 0;
    atEdge = false;
    stop = false;
    fearLoc.set(0,0);
    inc = 10;
    lifespan = 100;
    n = 0;
    runMult = 10;
    runDistribution = 0.2;
	pause = false;
	
	
	
    
}

void Mover::update(){
    
    //int inc = 10;
    n += 1;
    
    //ofVec2f center(300, 200);
    center =  topLeftCorner - ((topLeftCorner - bottomRightCorner)/2); // topLeftCorner.bottomRightCorner/;
	
	pause = false;
    
    ofVec2f fearNorm = fearLoc;
    fearNorm -= center;
    fearNorm.normalize();
	
    
    if (life < lifespan) {
        stop = false;
    }
    
	//if(ofGetElapsedTimeMillis()-getTime < 300){
	//	pause = true;
	//}else {
	//	pause = false;
	//}

    
    life += 1;
    
    
    if (!stop) {
    float otherDist = (1-runDistribution)/5;
	
    float r = ofRandom(1);
    
    if(r < runDistribution){
		
	   location -= fearNorm*runMult;
		
    }
        
    else if(r < otherDist*2 ){

        location.x -= inc;
    
    }
    else if(r < otherDist*3){
    
        location.y += inc;
    
    }else if(r < otherDist*4){
    
        location.y -= inc;
    
    }else if(r < otherDist*5){
		getTime = ofGetElapsedTimeMillis();
		pause = true;
    }
	else{
        
        location.x += inc;
        
        }
		
    }
	
    checkEdges();
    
     
     
}



void Mover::draw(){
    
    /*
    ofRect(location.x,location.y, 10,10);
    ofNoFill();
    ofRect(0, 0, 600, 400);
    
    int spacer = 200;
    for (int x = 0 ; x < 600/spacer; x ++) {
        for (int y = 0; y < 400/spacer; y ++) {
            if (location.x > x*spacer &&
                location.x < spacer+ x*spacer &&
                location.y > y*spacer &&
                location.y < spacer+y*spacer) {
                ofFill();
                
                
            }else{
                ofNoFill();
            }
            ofRect(x*spacer, y*spacer, spacer, spacer);
        }
    }
     */
    
   //ofPoint(location.x,location.y);

}


void Mover::checkEdges()

{
    
    
    if(location.x > bottomRightCorner.x){
        location.x = bottomRightCorner.x;
        if (life > lifespan) {
            stop = true;
        }
    }else if(location.x < topLeftCorner.x){
        location.x = topLeftCorner.x;
        if (life > lifespan) {
            stop = true;
        }
    }
    if(location.y > bottomRightCorner.y){
        location.y = bottomRightCorner.y;
        if (life > lifespan) {
            stop = true;
        }
    }else if(location.y < topLeftCorner.y){
        location.y = topLeftCorner.y;
        if (life > lifespan) {
            stop = true;
        }
    }


}

