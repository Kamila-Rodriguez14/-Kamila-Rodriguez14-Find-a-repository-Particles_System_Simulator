#pragma once
#include "ofMain.h"

enum particleMode{
	PARTICLE_MODE_ATTRACT = 0,
	PARTICLE_MODE_REPEL,
	PARTICLE_MODE_NEAREST_POINTS,
	PARTICLE_MODE_NOISE
};

class Particle{

	public:
		Particle();
		
		void setMode(particleMode newMode);	
		void setAttractPoints( vector <glm::vec3> * attract );
		void attractToPoint(int, int);
		void repelFromPoint(int, int);
		void reset();
		void update();
		void draw();	
		void setup() {
		}

		glm::vec3 pos;
		glm::vec3 vel; //use for the increase and decrease velocity
		glm::vec3 frc;
		
		float drag; 
		float uniqueVal;
		float scale;
		
		particleMode mode;

		int particleShape;
		
		//Added functions to alter particle velocity
		void doubleVelocity(); 
		void halfVelocity();
		void normalVelocity();
		double changeVelocity;

		//Added functions to magnify particles in rectangle
		void magnifyPartcle(ofRectangle& rect);
		bool insideRectangle;
		int increaseScale;

		vector <glm::vec3> * attractPoints; 

				
};
