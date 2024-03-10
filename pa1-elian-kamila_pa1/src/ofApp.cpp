#include "ofApp.h"

float incMult, decMult;
//--------------------------------------------------------------
void ofApp::setup()
{

	ofSetVerticalSync(true);
	int num = 1500;
	p.assign(num, Particle());
	currentMode = PARTICLE_MODE_ATTRACT;
	currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
	resetParticles();

	// initialize the variables used throughout ofApp
	drawingARectangle = false;
	pauseParticles = false;
	pauseCounter = 0;

	isRecording = false;
	recordIndex = 0;
	counterR = 0;

	isReplaying = false;
	counterP = 0;
}

//--------------------------------------------------------------
void ofApp::resetParticles()
{

	// these are the attraction points used in the fourth demo
	attractPoints.clear();
	for (int i = 0; i < 4; i++) {
		attractPoints.push_back(glm::vec3(ofMap(i, 0, 4, 100, ofGetWidth() - 100), ofRandom(100, ofGetHeight() - 100), 0));
	}

	attractPointsWithMovement = attractPoints;

	for (unsigned int i = 0; i < p.size(); i++) {
		p[i].setMode(currentMode);
		p[i].setAttractPoints(&attractPointsWithMovement);
		;
		p[i].reset();
	}
}

//--------------------------------------------------------------
void ofApp::update()
{

	// if statement that decides to magnify the particles inside of the rectangle if they are not paused
	if (pauseParticles == false) {
		for (unsigned int i = 0; i < p.size(); i++) {
			p[i].setMode(currentMode);
			p[i].magnifyPartcle(drawnRectangle);
			p[i].update();
		}

		// lets add a bit of movement to the attract points
		for (unsigned int i = 0; i < attractPointsWithMovement.size(); i++) {
			attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
			attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
		}
	}

	// lets add a bit of movement to the attract points
	for (unsigned int i = 0; i < attractPointsWithMovement.size(); i++) {
		attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
		attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
	}

	// if statements to decide which color to set the Particles depending on the times the key 'T' was pressed.
	if (keyTcount == 0) {
		ofFill(); // Refills particles if they were turned into bubbles.

		// sets the default color depending on which mode is currently activated.
		if (currentMode == PARTICLE_MODE_ATTRACT) {
			ofSetColor(255, 63, 180);
		}
		else if (currentMode == PARTICLE_MODE_REPEL) {
			ofSetColor(208, 255, 63);
		}
		else if (currentMode == PARTICLE_MODE_NEAREST_POINTS) {
			ofSetColor(103, 160, 237);
		}
		else if (currentMode == PARTICLE_MODE_NOISE) {
			ofSetColor(99, 63, 255);
		}
	}
	else if (keyTcount == 1) {
		ofSetColor(ofColor::blue);
	}
	else if (keyTcount == 2) {
		ofSetColor(ofColor::yellow);
	}
	else if (keyTcount == 3) {
		ofSetColor(ofColor::red);
	}
	else if (keyTcount == 4) {
		ofSetColor(ofColor::hotPink);
		ofNoFill(); // turn the particles into bubbles when the custom color is set.
	}

	// When the bonus key is pressed the color of the particles will change between random greens and blues 
	// particles will also change into triangles instead of circles.
	if (bonusKeyPressed) {
		ofFill();

		ofSetColor(ofColor(0, ofRandom(255), ofRandom(255)));
		for (unsigned int i = 0; i < p.size(); i++) {
			if (p[i].particleShape == 0) {
				p[i].particleShape = 1;
			}
		}

	} else if (bonusKeyPressed == false) {
		for (unsigned int i = 0; i < p.size(); i++) {
			if (p[i].particleShape == 1) {
				p[i].particleShape = 0;
			}
		}
	}

	if (isReplaying == true) {
		if (ofGetFrameNum() % 175 == 0) {
			keyPressedActions(recording[recordIndex]);
			currentKey = recording[recordIndex];
			recordIndex++;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackgroundGradient(ofColor(60, 60, 60), ofColor(10, 10, 10));

	for (unsigned int i = 0; i < p.size(); i++) {
		p[i].draw();
	}

	ofSetColor(190);
	if (currentMode == PARTICLE_MODE_NEAREST_POINTS) {
		for (unsigned int i = 0; i < attractPoints.size(); i++) {
			ofNoFill();
			ofDrawCircle(attractPointsWithMovement[i], 10);
			ofFill();
			ofDrawCircle(attractPointsWithMovement[i], 4);
		}
	}

	ofSetColor(230);
	ofDrawBitmapString(currentModeStr + "\n \nSpacebar to reset. \nKeys 1-4 to change mode. \nPress t to change colors. \nPress s to pause the particles. \nPress d to increase velocity. \nPress a to decrease the velocity. \nPress r to record. \nPress p to replay. \nPress c to cancel replay. \n Press b for bonus mode (warning flashing colors)\n" + displayInformation + " " + currentKey, 10, 20);

	// checks if you are trying to draw a rectangle and draws it on the screen.
	ofSetColor(137, 207, 240, 100);
	if (drawingARectangle == true) {
		ofFill();
		ofDrawRectangle(drawnRectangle);
	}
	ofSetColor(ofColor(ofRandom(200, 255), ofRandom(200, 255), ofRandom(200, 255)));
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

	// sets status to replaying
	if (key == 'p' || key == 'P') {
		if (counterP == 0) {
			displayInformation = "\nReplaying...";
			isReplaying = true;
			counterP = 1;
			recordIndex = 0;
		}
		else if (counterP == 1) {
			displayInformation = " ";
			isReplaying = false;
			counterP = 0;
			currentKey = ' ';
		}
	}

	// cancels replaying
	if (key == 'c' || key == 'C') {
		isReplaying = false;
		displayInformation = " ";
		counterP = 0;
		recordIndex = 0;
		currentKey = ' ';
	}

	// allows key inputs only if replay is off
	if (isReplaying == false) {

		// sets status to Recording.
		if (key == 'r' || key == 'R') {
			if (counterR == 0) {
				recording.clear();
				displayInformation = "\nRecording in progress...";
				isRecording = true;
				counterR++;
			}
			else if (counterR == 1) {
				displayInformation = " ";
				isRecording = false;
				counterR--;
			}
		}

		// changes modes
		if (key == '1') {
			currentMode = PARTICLE_MODE_ATTRACT;
			currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
			if (isRecording == true) {
				recording.push_back(key);
			}
		}
		if (key == '2') {
			currentMode = PARTICLE_MODE_REPEL;
			currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
			if (isRecording == true) {
				recording.push_back(key);
			}
		}
		if (key == '3') {
			currentMode = PARTICLE_MODE_NEAREST_POINTS;
			currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force";
			if (isRecording == true) {
				recording.push_back(key);
			}
		}
		if (key == '4') {
			currentMode = PARTICLE_MODE_NOISE;
			currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation";
			if (isRecording == true) {
				recording.push_back(key);
			}
			resetParticles();
		}

		if (key == ' ') {
			resetParticles();
			keyTcount = 0; // returns the particles to their default color if they are reseted. <--cheeck if must do this!!!!
			if (isRecording == true) {
				recording.push_back(key);
			}
			for (unsigned int i = 0; i < p.size(); i++) {
				p[i].normalVelocity();
			}
		}

		// updates the int keyTcount to add +1 everytime the key 'T' is pressed.
		if (key == 't' || key == 'T') {
			if (isRecording == true) {
				recording.push_back(key);
			}
			if (keyTcount == 0) {
				keyTcount++;
			}
			else if (keyTcount == 1) {
				keyTcount++;
			}
			else if (keyTcount == 2) {
				keyTcount++;
			}
			else if (keyTcount == 3) {
				keyTcount++;
			}
			else if (keyTcount == 4) {
				keyTcount = 0;
			}
		}
		// Change into Party Mode
		if (key == 'b' || key == 'B') {
			if (bonusKeyPressed == false) {
				bonusKeyPressed = true;
				displayInformation = "\n\nPARTY MODE";
			}
			else if (bonusKeyPressed == true) {
				bonusKeyPressed = false;
				displayInformation = " ";
			}
		}

		// Using the int keyCounter to pause the particles
		if (key == 's' || key == 'S') {
			if (isRecording == true) {
				recording.push_back(key);
			}

			if (pauseCounter == 0) {
				pauseParticles = true;
				pauseCounter++;
			}
			else if (pauseCounter == 1) {
				pauseParticles = false;
				pauseCounter--;
			}
		}
		// Increases velocity
		if (key == 'd' || key == 'D') {
			for (unsigned int i = 0; i < p.size(); i++) {
				p[i].doubleVelocity();
			}
			if (isRecording == true) {
				recording.push_back(key);
			}
		}
		// Decreases velocity
		if (key == 'a' || key == 'A') {
			for (unsigned int i = 0; i < p.size(); i++) {
				p[i].halfVelocity();
			}
			if (isRecording == true) {
				recording.push_back(key);
			}
		}
	}
}
//--------------------------------------------------------------
void ofApp::keyPressedActions(int key)
{
	// changes modes
	if (key == '1') {
		currentMode = PARTICLE_MODE_ATTRACT;
		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
	}
	if (key == '2') {
		currentMode = PARTICLE_MODE_REPEL;
		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
	}
	if (key == '3') {
		currentMode = PARTICLE_MODE_NEAREST_POINTS;
		currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force";
	}
	if (key == '4') {
		currentMode = PARTICLE_MODE_NOISE;
		currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation";
		resetParticles();
	}

	if (key == ' ') {
		resetParticles();
		keyTcount = 0; // returns the particles to their default color if they are reseted. <--cheeck if must do this!!!!

		for (unsigned int i = 0; i < p.size(); i++) {
			p[i].normalVelocity();
		}
	}

	// updates the int keyTcount to add +1 everytime the key 'T' is pressed.
	if (key == 't' || key == 'T') {

		if (keyTcount == 0) {
			keyTcount++;
		}
		else if (keyTcount == 1) {
			keyTcount++;
		}
		else if (keyTcount == 2) {
			keyTcount++;
		}
		else if (keyTcount == 3) {
			keyTcount++;
		}
		else if (keyTcount == 4) {
			keyTcount = 0;
		}
	}

	// Using the int keyCounter to pause the particles
	if (key == 's' || key == 'S') {

		if (pauseCounter == 0) {
			pauseParticles = true;
			pauseCounter++;
		}
		else if (pauseCounter == 1) {
			pauseParticles = false;
			pauseCounter--;
		}
	}
	if (key == 'd' || key == 'D') {
		for (unsigned int i = 0; i < p.size(); i++) {
			p[i].doubleVelocity();
		}
	}
	if (key == 'a' || key == 'A') {
		for (unsigned int i = 0; i < p.size(); i++) {
			p[i].halfVelocity();
		}
	}
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

	// updates the drawn rectangle in real time when moving the mouse around the screen.
	if (button == OF_MOUSE_BUTTON_LEFT) {
		rectWidth = x - startXrectangle;
		rectHeight = y - startYrectangle;

		drawnRectangle.setPosition(startXrectangle, startYrectangle);
		drawnRectangle.setWidth(rectWidth);
		drawnRectangle.setHeight(rectHeight);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// When clicking the left mouse button, sets the starting coordinates and the width and height.
	if (button == OF_MOUSE_BUTTON_LEFT) {
		startXrectangle = x;
		startYrectangle = y;
		rectWidth = 0;
		rectHeight = 0;

		drawingARectangle = true;
	}

	// When clicking the right mouse button, eliminates the rectangle only if your cursor coordinates are inside of the rectangle.
	else if (button == OF_MOUSE_BUTTON_RIGHT) {
		if (drawnRectangle.inside(x, y)) {
			drawingARectangle = false;

			rectWidth = 0;
			rectHeight = 0;

			drawnRectangle.setPosition(startXrectangle, startYrectangle);
			drawnRectangle.setWidth(rectWidth);
			drawnRectangle.setHeight(rectHeight);
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	// Checks if the left mouse button is released when you are drawing a rectangle and sets the final rectangle dimensions.
	if (button == (OF_MOUSE_BUTTON_LEFT) && drawingARectangle == true) {
		rectWidth = x - startXrectangle;
		rectHeight = y - startYrectangle;

		drawnRectangle.setPosition(startXrectangle, startYrectangle);
		drawnRectangle.setWidth(rectWidth);
		drawnRectangle.setHeight(rectHeight);

		drawingARectangle = true;
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}
//--------------------------------------------------------------
