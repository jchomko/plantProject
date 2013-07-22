#include "testApp.h"


static char HEADER = 'H';
static char COORD_TAG = 'C';
static char DELAY_TAG = 'D';



//--------------------------------------------------------------
void testApp::setup(){
    
    
   
   
    ofSetBackgroundAuto(false);
    ofEnableAlphaBlending();
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofNoFill();
    
    //Vehicles
	Vehicle *myVehicle;
	
	myVehicle = new Vehicle(ofGetWidth()/2, ofGetHeight()/2,ofGetWidth()/2, ofGetHeight()/2 , 300);
	vehicles.push_back(*myVehicle);	
	
	
    //Grid Pos
    topLeftCorner.set(336,271);
    bottomRightCorner.set(726,457);
    
	
	for (int i = 0; i < 200; i ++){
		matrixTimers[i] = 0;
	}
    
	matrixX[17] = 34;
    matrixX[16] = 36;
    matrixX[15] = 38;
    matrixX[14] = 40;
    matrixX[13] = 42;
    matrixX[12] = 44;
    matrixX[11] = 46;
    matrixX[10] = 48;
    matrixX[9] = 50;
    matrixX[8] = 52;
	matrixX[7] = 32;
	matrixX[6] = 30;
	matrixX[5] = 28;
	matrixX[4] = 26;
	matrixX[3] = 24;
	matrixX[2] = 22;
	matrixX[1] = 31;
	matrixX[0] = 33;	
    
    
    matrixY[0] = 39;
    matrixY[1] = 41;
    matrixY[2] = 43;
    matrixY[3] = 45;
    matrixY[4] = 47;
    matrixY[5] = 49;
    matrixY[6] = 51;
    matrixY[7] = 53;
    matrixY[8] = 8;
    matrixY[9] = 9;
    
    
    //CV
    camWidth = 640;
    camHeight = 480;
    speed = 10;
    thresh = 12;
    vidGrabber.initGrabber(camWidth,camHeight);
    colorImg.allocate(camWidth, camHeight);
    grayImg.allocate(camWidth, camHeight);
    lastGrayImg.allocate(camWidth, camHeight);
    
    
    
    //Arduino
    
    serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
    serial.setup(0, 9600); //57600 //9600
    arduinoReady = true;
    bytesBack = "";
    
    //UI
    
    float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 320-xInit;
	
    gui = new ofxUICanvas(0,0,length+xInit*2.0,ofGetHeight());
    gui->addWidgetDown(new ofxUILabel("PHYSIS", OFX_UI_FONT_LARGE));
    gui->addSpacer(length, 2);
    gui->addSlider("RUN SPEED", 0, 10, speed);
    gui->addSlider("LIFE SPAN", 0, 500, lifespan );
	gui->addSlider("ARDUINO DELAY TIME", 0, 600, ardDelay);
	gui->addSlider("SOFTWARE DELAY TIME", 0, 1000, softDelay);
	gui->addSlider("DETECTION DISTANCE", 0, 500, detectDist);
	gui->addSlider("THRESHOLD", 0, 255, thresh);
	gui->addSlider("BLOB THRESHOLD", 0, 1000, blobThresh);
	gui->addSlider("MOTION PRESENT DELAY", 0, 30000, motionPresentDelay);
	gui->addSlider("MAX NUMBER OF VEHICLES", 0, 20, maxVehicles);
	gui->addSlider("MIN DIST",0,100,minDist);
	
    //gui->addTextArea("TESTMODE", testModeUI, 20);
	
    ofAddListener(gui->newGUIEvent,this,&testApp::guiEvent);
	gui->loadSettings("GUI/guiSettings.xml");
    memset(bytesReadString, 0, 5);
    
    serial.writeByte(HEADER);
    
    
    testMode = false;
	d = 0;
	
	delayChanged = false;
	motionPresent = false;
	motionPresentTimer = 0;
	trackingDist  = 0;
		
        
}

//--------------------------------------------------------------
void testApp::update(){
    
	vidGrabber.update();
	
	
	// if(serial.readByte() == 'R'){
    //    arduinoReady = true;
    //    cout << "arduinoReady";
    //	}
	
	
	if (delayChanged) {
		
		sendMessage(DELAY_TAG, ardDelay, 50);
		
		delayChanged = false;
	}
    
	if (ofGetElapsedTimeMillis() - motionPresentTimer > motionPresentDelay) {
		motionPresent = false;
	}
    
	mousePos.set(ofGetMouseX(), ofGetMouseY());
	
	if (vidGrabber.isFrameNew()) {
		
        colorImg.setFromPixels(vidGrabber.getPixels(), camWidth,camHeight);
        grayImg = colorImg;
        grayImg.absDiff(lastGrayImg);
        grayImg.threshold(thresh);
        contourFinder.findContours(grayImg, 0, camWidth*camHeight/3, 1, false);
        lastGrayImg = colorImg;

    }
    
    
    velocity = mousePos - lastMousePosition;
	lastMousePosition = mousePos;
	
	rect.set(topLeftCorner - detectDist, bottomRightCorner +detectDist);
	
	
	
	 for (int i = 0 ; i < contourFinder.blobs.size(); i++) 
		{
		 if((contourFinder.blobs[i].boundingRect.width  *  
				contourFinder.blobs[i].boundingRect.height ) > blobThresh)
				{
					scaledCentroid = contourFinder.blobs[i].centroid*1.66;
						
					
					for (int v = 0; v < vehicles.size(); v ++) 
					{	
						trackingDist = scaledCentroid.distance(vehicles[v].location);	
						
						if (trackingDist < detectDist) 
						{
					
							closeMotion = true;
							closeMotionTimer = ofGetElapsedTimeMillis();
							vehicles[v].life = lifespan;
                            vehicles[v].lifespan = lifespan;
							vehicles[v].stop = false;	
				
						
						}else{
                            
							if (motionPresent == false) {
								vehicles[v].life = lifespan*2;	
								vehicles[v].stop = false;	
                            }
                            
							motionPresent = true;
							motionPresentTimer = ofGetElapsedTimeMillis();
											
							}
					  }
				 }
			}
			
}



//--------------------------------------------------------------
void testApp::draw(){
       
    ofSetColor(255, 255, 255);
       
    
	lastGrayImg.draw(0,0,1066,800);
	
    contourFinder.draw(0,0,1066,800);
	
	ofNoFill();
	ofRect(rect);
	
	
  	for(int i = 0; i < vehicles.size(); i ++){
					
		vehicles[i].bottomRightCorner = bottomRightCorner;
		vehicles[i].topLeftCorner = topLeftCorner;
		
		vehicles[i].checkLife();
		
		if(vehicles[i].stop == false){
            
            vehicles[i].run();
            vehicles[i].avoid(scaledCentroid);
//            
//            if(trackingDist < detectDist+100){
//                
//                vehicles[i].avoid(scaledCentroid);
//                
//                }else{
//                    
//                vehicles[i].wander();
//            }
            
           		
        }
        
		vehicles[i].checkEdges();
		vehicles[i].display();
		
		
		vehiclePos.set(vehicles[i].location.x,vehicles[i].location.y);
		
		if (testMode) {
			ofLine(mousePos.x, mousePos.y, vehicles[i].location.x, vehicles[i].location.y);
		}
		
	
		
        int rectWidth = bottomRightCorner.x - topLeftCorner.x;
        int rectHeight = bottomRightCorner.y - topLeftCorner.y;
		
		int spacerX = rectWidth/17;
        int spacerY = rectHeight/10;
		
		ofRect(topLeftCorner.x, topLeftCorner.y, rectWidth, rectHeight);
		
		
		for (int x = 0; x <  18; x ++) {
			
            for (int y = 0; y <  10; y ++) {
				
				float matrixDist = ofDist(topLeftCorner.x + ((spacerX/2) + (x*spacerX)), 
								 topLeftCorner.y + ((spacerY/2) + (y*spacerY)), 
								 vehicles[i].location.x, 
								 vehicles[i].location.y);
				
				if( matrixDist < minDist && ofGetElapsedTimeMillis() - matrixTimers[x*y] > softDelay && vehicles[i].stop == false) { 
                    
					
					ofFill();
                    
					sendMessage(COORD_TAG, matrixX[x], matrixY[y]);
					matrixTimers[x*y] = ofGetElapsedTimeMillis();
					
					
				}else{
                    
                    ofNoFill();
					
                }
				
				ofRect( x*spacerX +  topLeftCorner.x, y*spacerY + topLeftCorner.y, spacerX, spacerY);
				
			}
			
		 }
		}
        
		
		
    if(serial.available()> 1){
        
        unsigned char bytesReturned[5];
        memset(bytesReadString, 0, 5);
        memset(bytesReturned, 0, 5);
    
        serial.readBytes(bytesReturned, 5);
        
        memcpy(bytesReadString, bytesReturned, 5);
       
    }
	
    
      
			
	
    string info = "";
    
	ofEllipse(scaledCentroid.x, scaledCentroid.y, 20, 20);

	
	info += "number of Vehicles : " + ofToString( vehicles.size() ) + "\n";
    info += "speed : " + ofToString(speed)+ "\n";
    info += "testMode : " + ofToString(testMode) + "\n";
	info += "top left corner : " + ofToString(topLeftCorner) + "\n";
	info += "bottom right corner : " + ofToString(bottomRightCorner) + "\n";
	info += "delayChanged : " + ofToString(delayChanged) + "\n";
	
    ofSetColor(255,0,0);
    ofDrawBitmapString(info, 300,10);

  
    if(!motionPresent){
		//ofFill();
		ofEllipse(500, 500, 300, 300);
	}
	
    ofEllipse(vehiclePos.x, vehiclePos.y, 10, 10);       
  
}


void testApp::sendMessage( char tag, int x, int y){
	
   
    if(arduinoReady){
    //cout << tag;
    serial.writeByte(HEADER);
    serial.writeByte(tag);
    serial.writeByte((char) (x / 256));
    serial.writeByte( x & 0xff);
    serial.writeByte((char) (y / 256));
    serial.writeByte(y & 0xff);
	
    }
    
    
}



void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	
    if(name == "RUN SPEED"){
        ofxUISlider *slider = (ofxUISlider *) e.widget;
		for (int i = 0; i < vehicles.size(); i ++) {
            vehicles[i].maxspeed  = slider->getScaledValue();
        }
    }
    
    if(name == "LIFE SPAN"){
        ofxUISlider *slider = (ofxUISlider *) e.widget;
		lifespan  = slider->getScaledValue();
     
    }
    
    if(name == "ARDUINO DELAY TIME"){
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        ardDelay = slider->getScaledValue();
		delayChanged = true;
	}
	
	if(name == "SOFTWARE DELAY TIME"){
		
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        softDelay = slider->getScaledValue();
       		
	}
	if(name == "DETECTION DISTANCE"){
		
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        detectDist = slider->getScaledValue();
		
	}
	
	if(name == "THRESHOLD"){
		
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        thresh = slider->getScaledValue();
		
	}
	
	if(name == "BLOB THRESHOLD"){
		
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        blobThresh = slider->getScaledValue();
		
	}
	if(name == "MOTION PRESENT DELAY"){
		
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        motionPresentDelay = slider->getScaledValue();
		
	}
	if(name == "MAX NUMBER OF VEHICLES"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        maxVehicles = slider->getScaledValue();
	
	}
	if(name == "MIN DIST"){
	
		ofxUISlider *slider = (ofxUISlider *) e.widget;
        minDist = slider->getScaledValue();
		
	}
	

    
        
    
}

void testApp::exit()
{
    gui->saveSettings("GUI/guiSettings.xml");
    delete gui;
}



//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    switch (key) {
       
    case OF_KEY_UP:
            thresh += 1;
            break;
    case OF_KEY_DOWN:
            thresh -= 1;
            break;
    case OF_KEY_RIGHT:
            speed -= 0.1;
            break;
    case OF_KEY_LEFT:
            speed += 0.1;
            break;
    case 'l':
            topLeftCorner.set(ofGetMouseX(),ofGetMouseY());
            break;
    case 'r':
            bottomRightCorner.set(ofGetMouseX(),ofGetMouseY());
            break;
    case 'g':
        {
            gui->toggleVisible();
        }
            break;
    
    case 't':
            testMode = !testMode;
			break;
            
    }
    
    
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(button == 0){
   
    }
    if(button == 1){
        cout << "middle";
   
    }
    if(button == 2){
        serial.flush();
    }
  /*  Mover *myMover;
    myMover = new Mover(x,y,0);
    movers.push_back(*myMover);
  */
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}