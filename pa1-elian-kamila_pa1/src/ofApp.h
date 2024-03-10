#pragma once

#include "ofMain.h"
#include "Particle.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void resetParticles();
		
		void keyPressed  (int key);
		void keyPressedActions(int key);//copy of keyPressed that makes it so you cannot press anything during replay.
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		particleMode currentMode;
		
		string currentModeStr; 
		
		vector <Particle> p; 
		vector <glm::vec3> attractPoints;
		vector <glm::vec3> attractPointsWithMovement;
		
		bool drawingARectangle; //checks if a rectangle is beign drawn.
		int keyTcount; //variable that keeps count of the amount of times the key 'T' has been pressed.
		int startXrectangle, startYrectangle, rectWidth, rectHeight; //dimensions of the rectangle to be drawn.
		
		ofRectangle drawnRectangle; //a rectangle type variable to store said dimensions.
		
		bool pauseParticles; //checks if you want to stop the particles.
		int pauseCounter; //keeps count of the times the pause button is pressed.		

		string displayInformation;//string 
	
		//vector for recording key presses and other related variables.
		vector<char> recording = {};
		int counterR, counterP;
		bool isRecording;
		bool isReplaying;

		char currentKey;//holds current key in replay.
		int recordIndex;

		bool bonusKeyPressed; // variable that refers to the key that will be pressed for bonus
		int bonusIndex; 
};
