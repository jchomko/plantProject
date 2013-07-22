#pragma once

#include "ofMain.h"
#include "Vehicle.h"
#include "ofxOpenCv.h"
#include "ofxUI.h"



class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void sendMessage( char tag, int x, int y);
        
    
        vector<ofVec2f> mousePositions;
       
		vector <Vehicle> vehicles;
        ofVec2f lastMousePosition;
        ofVec2f velocity;
        
    
        ofVideoGrabber vidGrabber;
        int camWidth;
        int camHeight;
        int thresh;
        float speed;
    
        ofxCvColorImage colorImg;
        ofxCvGrayscaleImage grayImg;
        ofxCvGrayscaleImage lastGrayImg;
        ofxCvContourFinder contourFinder;
        ofVec2f topLeftCorner;
        ofVec2f bottomRightCorner;
		ofVec2f largestCentroid;
    
        ofSerial	serial;
        long waitUntil;
		
		bool        arduinoReady;
		bool		bSendSerialMessage;			// a flag for sending serial
		char		bytesRead[5];				// data from serial, we will be trying to read 3
		char		bytesReadString[6];			// a string needs a null terminator, so we need 3 + 1 bytes
		int			nBytesRead;					// how much did we read?
		int			nTimesRead;					// how many times did we read?
		float		readTime;					// when did we last read?
		string bytesBack;
    
		bool		bSetupArduino;			// flag variable for setting up arduino once
		bool		addMover;
    
		ofVec2f mousePos;
		ofVec2f scaledCentroid;
		float d;
    
		int ardDelay;
		int softDelay;
		
		bool delayChanged;

		int lifespan;
	
		bool testMode;
		int blobThresh;
		
		ofxUICanvas *gui;
		void guiEvent(ofxUIEventArgs &e);
		
		float runSpeed;
		float runMult;
		float runDistribution;
		
		
		int detectDist;
    
		int matrixX[20];
		int matrixY[10];
		long matrixTimers[200];
	
		bool motionPresent;
		bool runTowards;
		long motionPresentTimer;
		int motionPresentDelay;
		
		bool closeMotion;
		long closeMotionTimer;
		int closeMotionDelay;
		
		
		ofRectangle rect;
		int maxVehicles;
		ofVec2f vehiclePos;
		int minDist;
		float trackingDist;
        string serialDevice;
  
    
    
};
