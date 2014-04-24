#include "testApp.h"

using namespace ofxCv;

void testApp::setup() {
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    
    cam.initGrabber(640, 480);
    
    mov.loadMovie("ironman_640.mp4");
//    mov.setSpeed(0.2);
	mov.play();

    tracker.setup();
    imgTracker.setup();
}


void testApp::update() {
    mov.update();
    cam.update();

    if(cam.isFrameNew()) {
        imgTracker.update(toCv(mov));
        tracker.update(toCv(cam));
    }

}


void testApp::draw() {
    ofSetColor(255);

//---sub frames on the side
    cam.draw(640, 0, 320, 240);
    mov.draw(640,180, 320, 180);

//---main frame
    mov.draw(0,0,640,360);


//---draw framerate
    ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);

/*TODO: ofSetupScreenOrtho is deprecated*/
    //ofSetOrientation(OF_ORIENTATION_DEFAULT, true);
    ofSetupScreenOrtho(640, 480, OF_ORIENTATION_DEFAULT, true, -1000, 1000);



/*-----------------------------------------------
 TACKING CAMERA INPUT
 --------------------------------------------------*/
    ofMesh camFaceMesh;
    ofVec2f camFacePosition;
    float camFaceScale;
    ofVec3f camFaceOrientation;
    
    if(tracker.getFound()){
        camFaceMesh = tracker.getObjectMesh();

//---get the face's position, scale, orientation
        camFacePosition = tracker.getPosition();
        camFaceScale = tracker.getScale();
        camFaceOrientation = tracker.getOrientation();

//---draw a wireframe on the cam face
        ofPushMatrix();
        ofTranslate(640 + camFacePosition.x/2, camFacePosition.y/2);
        ofScale(camFaceScale/2, camFaceScale/2, camFaceScale/2);
        ofRotateX(camFaceOrientation.x * 45.0f);
        ofRotateY(camFaceOrientation.y * 45.0f);
        ofRotateZ(camFaceOrientation.z * 45.0f);
        ofSetColor(255, 0, 0, 200);
        cam.getTextureReference().bind();
        camFaceMesh.drawWireframe();
        cam.getTextureReference().unbind();
        ofPopMatrix();
    }

    

/*-----------------------------------------------
 TACKING MOVIE INPUT
 --------------------------------------------------*/
    ofMesh imgFaceMesh;
    ofVec2f imgFacePosition;
    float imgFaceScale;
    ofVec3f imgFaceOrientation;
    
    if(imgTracker.getFound()){
        imgFaceMesh = imgTracker.getObjectMesh();

//---get the face's position, scale, orientation
        int imgFaceOriginalW = mov.width;
        int imgFaceOriginalH = mov.height;
        imgFacePosition = imgTracker.getPosition();
        imgFaceScale = imgTracker.getScale();
        imgFaceOrientation = imgTracker.getOrientation();
        
//---draw a wireframe on the image face
        ofPushMatrix();
        ofTranslate(640 + imgFacePosition.x/2, 180 + imgFacePosition.y/2);
        ofScale(imgFaceScale/2, imgFaceScale/2, imgFaceScale/2);
        ofRotateX(imgFaceOrientation.x * 45.0f);
        ofRotateY(imgFaceOrientation.y * 45.0f);
        ofRotateZ(imgFaceOrientation.z * 45.0f);
        ofSetColor(0, 0, 255, 200);
        faceImage.getTextureReference().bind();
        imgFaceMesh.drawWireframe();
        faceImage.getTextureReference().unbind();
        ofPopMatrix();
    }


    
/*-----------------------------------------------
 MERGING TWO INPUTS
 --------------------------------------------------*/
    if(tracker.getFound() && imgTracker.getFound()) {
//---allocate each vertex from a mesh to another mesh
        for (int i = 0; i < camFaceMesh.getNumVertices(); i++) {
            imgFaceMesh.setVertex(i, camFaceMesh.getVertex(i));
        }

//---mapping the mesh
        ofPushMatrix();
        ofTranslate(imgFacePosition.x, imgFacePosition.y);
        ofScale(imgFaceScale, imgFaceScale, imgFaceScale);
        ofRotateX(imgFaceOrientation.x * 45.0f);
        ofRotateY(imgFaceOrientation.y * 45.0f);
        ofRotateZ(imgFaceOrientation.z * 45.0f);

//---overlay face of mine
        ofSetColor(255, 255, 255, 210);
        cam.getTextureReference().bind();
        camFaceMesh.draw();
        cam.getTextureReference().unbind();
        
        ofPopMatrix();
    }
}


void testApp::keyPressed(int key) {
//---reset trackers
    if(key == 'r') {
        tracker.reset();
        imgTracker.reset();
    }
}

ofImage testApp::resizeImage(ofImage img){
    int areaW = 320;
    int areaH = 240;
    int imgW = img.width;
    int imgH = img.height;
    int newImgW, newImgH;
    float resizeRatio;
    resizeRatio = (float)areaH / imgH;
    newImgW = (int)imgW * resizeRatio;
    newImgH = areaH;
    img.resize(newImgW, newImgH);
    img.crop(newImgW/2-areaW/2, 0, areaW, areaH);
    return img;
};



