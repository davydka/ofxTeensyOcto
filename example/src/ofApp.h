#pragma once

#include "ofxTeensyOcto.h"
#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        void updateFbo();
        void setupMedia();
        void enableVideo();
        void disableVideo();
        void drawPong();
    
    // LED - Teensy stuff
    //-----------------------------
    ofxTeensyOcto teensy;
    int stripWidth;
    int stripHeight;
    int rowHeight;
    int stripsPerPort;
    int numPorts;
    int brightness;
    
    // FBO stuff
    //-----------------------------
    ofFbo fbo;
    int fboRotate;
    bool fboFlip;
    ofTexture tex;
    ofPixels guiPixels;
    
    // Gui
    //-----------------------------
    void drawPanels();
    ofTrueTypeFont font;
    
    // Graphic functions
    //-----------------------------
    void drawDemos();
    void drawVideos();
    void drawImages();
    int drawModes;
    int demoModes;
    int rectWidth;
    float ballpos;
    int dir;

    // Video player
    //-----------------------------
    ofDirectory dirVid;
    vector<ofVideoPlayer> vid;
    int currentVideo;
    bool videoOn;
    
    // Image player
    //-----------------------------
    ofDirectory dirImg;
    vector<ofImage> img;
    int currentImage;
		
};
