#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0, 0, 0);                      // default background to black / LEDs off
	ofDisableAntiAliasing();                    // we need our graphics sharp for the LEDs
	ofSetVerticalSync(false);
	ofSetFrameRate(60);

	// SYSTEM SETTINGS
	//--------------------------------------
	stripWidth = 16;                            // pixel width of strip
	stripHeight = 128;                            // pixel height of strip
	rowHeight = 16;
	stripsPerPort = 1;                          // total number of strips per port
	numPorts = 1;                               // total number of teensy ports?
	brightness = 20;                             // LED brightness


	drawModes = 0;                              // default mode

	rectWidth = 1;
	dir = 1;

	// setup our teensys
	teensy.setup(stripWidth, stripHeight, rowHeight, stripsPerPort, numPorts);

	/* Configure our teensy boards (portName, xOffset, yOffset, width%, height%, direction) */
	//teensy.serialConfigure("cu.usbmodem2809741", 0, 0, 100, 100, 0);
	//teensy.serialConfigure("ttyACM0", 0, 0, 100, 100, 0);
	teensy.serialConfigure("cu.usbmodem2733511", 0, 0, 100, 100, 0);
	teensy.setBrightness(brightness);


	// allocate our pixels, fbo, and texture
	fbo.allocate(stripWidth, stripHeight*stripsPerPort*numPorts, GL_RGB);

	setupMedia();
}

void ofApp::exit(){
	/* turn all leds to black */
	/*
	fbo.begin();
	ofClear(0,0,0);
	fbo.end();
	fbo.readToPixels(teensy.pixels1);
	teensy.update();
	*/
}

//--------------------------------------------------------------
void ofApp::update(){
	//ofSetWindowTitle("TeensyOctoExample - "+ofToString(ofGetFrameRate()));
	ballpos+=dir*1.0f;

	updateFbo();                                // update our Fbo functions
	teensy.update();                            // update our serial to teensy stuff
}

//--------------------------------------------------------------
void ofApp::updateFbo(){
	fbo.begin();                                // begins the fbo
	ofClear(0,0,0);                             // refreshes fbo, removes artifacts

	//ofSetColor(brightness);
	ofSetColor(255);

	ofPushStyle();
	switch (drawModes)
	{
		case 0:            
			drawPong();
			break;
		case 1:
			drawImages();
			break;
		default:
			break;
	}
	ofPopStyle();

	fbo.end();                                  // closes the fbo

	fbo.readToPixels(teensy.pixels1);           // send fbo pixels to teensy
}

//--------------------------------------------------------------
void ofApp::draw(){
	teensy.draw(120,300);

	ofSetColor(255);
	ofDrawBitmapString("// Controls //", ofGetWidth()-250, 20);
	ofDrawBitmapString("Brightness (up/down) == " + ofToString(brightness), ofGetWidth()-250, 80);
	ofDrawBitmapString("Images # == " + ofToString(dirImg.size()), ofGetWidth()-250, 140);
}

void ofApp::drawPong(){
	if(ballpos > stripWidth-rectWidth) {
		ballpos = stripWidth-rectWidth;
		dir = -1;
	}
	else if(ballpos < 0) {
		ballpos = 0;
		dir = 1;
	}
	ofSetColor(255, 0, 255);
	//ofSetColor(brightness);
	ofDrawRectangle(ballpos,0,rectWidth,stripHeight*stripsPerPort*numPorts);
}

//--------------------------------------------------------------
void ofApp::drawImages(){
	if (dirImg.size() > 0) {
		//img[currentImage].draw(0, 0, stripWidth, stripHeight*stripsPerPort*numPorts);
		img[currentImage].draw(0, 0, stripWidth, rowHeight);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){ 
	switch (key){
		//-----------------------------------------------
		case OF_KEY_UP:
			brightness += 2;
			if (brightness > 255) brightness = 255;
			teensy.setBrightness(brightness);
			break;
			
		case OF_KEY_DOWN:
			brightness -= 2;
			if (brightness < 0) brightness = 0;
			teensy.setBrightness(brightness);
			break;

		case 'i':
			drawModes = 1;                          // image mode

			img[currentImage].load(dirImg.getPath(currentImage));
			break;
		
		case '=':
			if (drawModes == 1) {
				currentImage++;
				if (currentImage > dirImg.size()-1) currentImage = 0;
				img[currentImage].load(dirImg.getPath(currentImage));
			}
			break;
			
		case '-':
			if (drawModes == 1) {
				currentImage--;
				if (currentImage < 0) currentImage = dirImg.size()-1;
				img[currentImage].load(dirImg.getPath(currentImage));
			}
			break;
			
		case 't':
			if (drawModes != 0) drawModes = 0;      // switch the draw mode
			break;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::setupMedia(){
	dirImg.listDir("images/");
	dirImg.sort();
	//allocate the vector to have as many ofImages as files
	if( dirImg.size() ){
		img.assign(dirImg.size(), ofImage());
	}
	currentImage = 0;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){}
