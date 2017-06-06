#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(0, 0, 0);                      // default background to black / LEDs off
    ofDisableAntiAliasing();                    // we need our graphics sharp for the LEDs
    ofSetVerticalSync(false);
    ofSetFrameRate(90);
    
    // SYSTEM SETTINGS
    //--------------------------------------
    stripWidth = 300;                            // pixel width of strip
    stripHeight = 1;                            // pixel height of strip
    stripsPerPort = 8;                          // total number of strips per port
    numPorts = 1;                               // total number of teensy ports?
    brightness = 200;                             // LED brightness

    drawModes = 0;                              // default is demo mode
    demoModes = 0;                              // default is draw white

    dir = 1;
    
    // setup our teensys
    teensy.setup(stripWidth, stripHeight, stripsPerPort, numPorts);
    
    /* Configure our teensy boards (portName, xOffset, yOffset, width%, height%, direction) */
    teensy.serialConfigure("ttyACM0", 0, 0, 100, 100, 0);
//    teensy.serialConfigure("ttyACM1", 0, 50, 100, 50, 0);

    
    // allocate our pixels, fbo, and texture
    fbo.allocate(stripWidth, stripHeight*stripsPerPort*numPorts, GL_RGB);
    
    setupMedia();
}

void ofApp::exit()
{
    /* turn all leds to black */
    fbo.begin();
    ofClear(0,0,0);
    fbo.end();
    fbo.readToPixels(teensy.pixels1);
    teensy.update();

}

//--------------------------------------------------------------
void ofApp::update()
{
    //ofSetWindowTitle("TeensyOctoExample - "+ofToString(ofGetFrameRate()));
    ballpos+=dir*1.0f;

    if (dirVid.size() > 0)
    {
        if (videoOn) vid[currentVideo].update();                  // update video when enabled
    }

    updateFbo();                                // update our Fbo functions
    teensy.update();                            // update our serial to teensy stuff

}

//--------------------------------------------------------------
void ofApp::updateFbo()
{    
    fbo.begin();                                // begins the fbo
    ofClear(0,0,0);                             // refreshes fbo, removes artifacts
    
    ofPushStyle();
    switch (drawModes)
    {
        case 0:            
            drawPong();
            break;
        case 1:
            drawVideos();
            break;
        case 2:
            drawImages();
            break;
        case 3:
            drawDemos();
        break;
        default:
            break;
    }
    ofPopStyle();
    
    fbo.end();                                  // closes the fbo
    
    fbo.readToPixels(teensy.pixels1);           // send fbo pixels to teensy

}

//--------------------------------------------------------------
void ofApp::draw()
{
    teensy.draw(20,300);

    ofSetColor(255);
    ofDrawBitmapString("// Controls //", ofGetWidth()-250, 20);
    ofDrawBitmapString("Brightness (up/down) == " + ofToString(brightness), ofGetWidth()-250, 80);
    ofDrawBitmapString("Videos # == " + ofToString(dirVid.size()), ofGetWidth()-250, 120);
    ofDrawBitmapString("Images # == " + ofToString(dirImg.size()), ofGetWidth()-250, 140);
}

void ofApp::drawPong()
{
    if(ballpos > 290) {
        ballpos = 290;
        dir = -1;
    }
    else if(ballpos < 0) {
        ballpos = 0;
        dir = 1;
    }
    ofDrawRectangle(ballpos,0,10,stripHeight*stripsPerPort*numPorts);

}

void ofApp::drawDemos()
{
    switch (demoModes) {
        case 0:
            teensy.drawTestPattern();
            break;
        case 1:
            teensy.drawWhite();
            break;
        case 2:
            teensy.drawRainbowH();
            break;
        case 3:
            teensy.drawRainbowV();
            break;
        case 4:
            teensy.drawWaves();
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::enableVideo()
{
    if (dirVid.size() > 0) {
        if (!videoOn) videoOn = true;           // enables video
        if (vid[currentVideo].isLoaded() == false) {
            vid[currentVideo].load(dirVid.getPath(currentVideo));
            vid[currentVideo].play();           // plays the video
        }
        else {
            if (vid[currentVideo].isPlaying()) {
                vid[currentVideo].stop();       // stops/pauses the video
            }
            else {
                vid[currentVideo].play();       // plays the video
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::disableVideo()
{
    if (dirVid.size() > 0) {
        videoOn = false;                        // disables video
        if (vid[currentVideo].isPlaying()) vid[currentVideo].stop();  // stops/pauses the video
    }
}

//--------------------------------------------------------------
void ofApp::drawVideos()
{
    //Play videos
    if (dirVid.size() > 0){
        ofSetColor(brightness);
        vid[currentVideo].setSpeed(5.0f);
        vid[currentVideo].draw(0, 0, stripWidth, stripHeight*stripsPerPort*numPorts);
    }
}

//--------------------------------------------------------------
void ofApp::drawImages()
{
    if (dirImg.size() > 0) {
        ofSetColor(brightness);
        img[currentImage].draw(0, 0, stripWidth, stripHeight*stripsPerPort*numPorts);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key)
    {
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

        case 'v':
            drawModes = 1;                          // video mode
            enableVideo();
            break;

        case 'i':
            drawModes = 2;                          // image mode
            disableVideo();

            img[currentImage].load(dirImg.getPath(currentImage));
            break;
        
        case '=':
            if (drawModes == 1) {
                vid[currentVideo].stop();
            }
            
            if (drawModes == 2) {
                currentImage++;
                if (currentImage > dirImg.size()-1) currentImage = 0;
                img[currentImage].load(dirImg.getPath(currentImage));
            }
            break;
            
        case '-':
            if (drawModes == 1) {
                vid[currentVideo].stop();
            }
            
            if (drawModes == 2) {
                currentImage--;
                if (currentImage < 0) currentImage = dirImg.size()-1;
                img[currentImage].load(dirImg.getPath(currentImage));
            }
            break;
            
        case 'd':
            disableVideo();
            
            demoModes++;
            if (drawModes != 3) drawModes = 3;      // switch the draw mode to display demo mode.
            if (demoModes > 4) demoModes = 0;       // tap through the demo modes on each press.
            break;

        case 't':
            disableVideo();

            if (drawModes != 0) drawModes = 0;      // switch the draw mode
            break;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    switch (key) {
        case '=':
            if (drawModes == 1)
            {
                currentVideo++;
                if (currentVideo > dirVid.size()-1) currentVideo = 0;
                vid[currentVideo].load(dirVid.getPath(currentVideo));
                vid[currentVideo].play();
            }                       // restart video at first frame
            break;
            
        case '-':
            if (drawModes == 1) {
                currentVideo--;
                if (currentVideo < 0) currentVideo = dirVid.size()-1;
                vid[currentVideo].load(dirVid.getPath(currentVideo));
                vid[currentVideo].play();
            }
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::setupMedia()
{
    dirVid.listDir("videos/");
    dirVid.sort();
    //allocate the vector to have as many ofVidePlayer as files
    if( dirVid.size() ){
        vid.assign(dirVid.size(), ofVideoPlayer());
    }
    videoOn = false;
    currentVideo = 0;

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
