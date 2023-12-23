#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <math.h>
#include <string>
#include <vector>
#include <SFML/Audio.hpp>
#define DEG2RAD(a) (a * 0.0174532925)

int WIDTH = 1527;
int HEIGHT = 791;

GLuint tex;
char title[] = "Lab Of The Dead";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;
float eyeX = 1.0f;
float eyeY = 1.0f;
float eyeZ = 1.0f;
float centerX = 0.0f;
float centerY = 0.0f;
float centerZ = 0.0f;
float upX = 0.0f;
float upY = 1.0f;
float upZ = 0.0f;
float movex = 0.0f;
float movey = 0.0f;
sf::Sound bulletFire;
sf::Sound zombieScream;
sf::Sound Reload;
sf::Sound playerHit;
sf::Sound backgroundS;



bool reloading = false;
bool room1EnterFlag = false;
bool room1KeyPressed = false;
float zTest = 19.0f;
bool flashLight = false;
bool haveTorch = false;
bool torchPrompVis = false;
bool room2EnterFlag = false;
int score = 0;

bool isJumping = false;
bool finishedJumping = false;
int jumpCtr = 0;
bool potionPrompVis = false;
bool finalBoss = false;
int bossCtr = 0;

int makhtutaCtr = 0;

int currView = 0; // 3rd person



bool test = false;

void checkForCheatCodes(const std::string& input);
std::string recentKeys;

void drawCustomCursor(int x, int y);


bool mainCorridor1 = true;
bool mainCorridor2 = false;
bool room1 = false;
bool room2 = false;
bool room3 = false;
bool room3EnterFlag = false;
bool  isTorchTaken = false;
bool menrit = false;
bool menritCheat = false;
int cheatCtr = 0;
bool isPotionTaken = false;
bool isKeyTaken = false;
bool bossDead = false;
bool gunPrompt = false;
bool gunTaken = false;
bool makhtutaSecret;


// global Variables

float bossX = 0.0f;
float bossY = 0.0f;
float bossZ = 0.0f;
int bossAngle = 0;
float bossScale = 3;

float playerXMainCorr1 = 0.0f;
float playerYMainCorr1 = 0.0f;
float playerZMainCorr1 = 19.0f;
float playerZMainCorr1Prev = 19.0;

float playerXRoom1 = 0.0f;
float playerYRoom1 = 0.0f;
float playerZRoom1 = 19.0f;

float playerXRoom2 = 0.0f;
float playerYRoom2 = 0.0f;
float playerZRoom2 = 19.0f;


float playerXRoom3 = 0.0f;
float playerYRoom3 = 0.0f;
float playerZRoom3 = 19.0f;


float playerXFinal = 0.0f;
float playerYFinal = 0.0f;
float playerZFinal = 12;

int playerAngle = 180;

float playerXMainCorr2 = 0.0f;
float playerYMainCorr2 = 0.0f;
float playerZMainCorr2 = 19.0f;
float playerZMainCorr2Prev = 19.0;

float sunX = 0;
float sunZ = 0;


void drawBattery(float x, float y, float width, float height, int totalBars);
int remBattery = 5;


int mouseX = 0;
int mouseY = 0;

int bulletsFired = 0;
int reloadCtr;


bool finalHealthTaken = false;
bool finalHealthPrompt = false;
bool gameWin = false;
bool gameLoss = false;

bool shieldPrompt = false;
bool shieldTaken = false;
bool shieldUse = false;
bool returnCorr2Prompt = false;
bool makhtutaPrompt = false;
bool makhtutaTaken = false;

bool zombieDetected = false;
sf::SoundBuffer bulletFire_buffer;
sf::SoundBuffer zombieScream_buffer;
sf::SoundBuffer reload_buffer;
sf::SoundBuffer playerHit_buffer;
sf::SoundBuffer background_buffer;
class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}
	float magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}
	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};


Vector3f screenToWorld(int x, int y);





void renderBitmapString(float x, float y, void* font, const char* string) {
	const char* c;
	glRasterPos2f(x, y);

	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void showMessage(const char* string, float x, float y) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, WIDTH, 0, HEIGHT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);  // Disable writing to depth buffer
	glDepthFunc(GL_ALWAYS); // Change depth function to always pass

	glColor3d(1, 1, 1);
	renderBitmapString(x, y, GLUT_BITMAP_HELVETICA_18, string);

	glDepthFunc(GL_LESS);   // Reset depth function
	glDepthMask(GL_TRUE);   // Re-enable writing to depth buffer
	glEnable(GL_DEPTH_TEST);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


bool lightningActive = false;
float lightningTimer = 0.0f;
float nextLightningIn = 5.0f; // Time until next lightning, in seconds
int healthRem = 5;
bool returnToMainCorr1 = false;
bool labDoorPrompt = false;
bool haveKey = false;
void updateLightningEffect() {
	// Use GL_LIGHT0 for lightning effect
	if (lightningActive) {
		GLfloat lightningDiffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
		GLfloat lightningPosition[] = { 0.0f, 10000.0f, 0.0f, 1.0f }; // High above to illuminate entire scene
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightningDiffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, lightningPosition);
	}
	else {
		// Turn off the lightning light
		GLfloat off[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, off);
	}
}

bool intersectionDetected(Vector3f& lineStart, Vector3f& lineEnd, Vector3f& circleCenter, float circleRadius) {
	Vector3f line = lineEnd - lineStart;
	
	
	Vector3f startToCenter = circleCenter - lineStart;

	float a = line.x * line.x + line.y * line.y + line.z * line.z;
	float b = 2 * (line.x * startToCenter.x + line.y * startToCenter.y + line.z * startToCenter.z);
	float c = startToCenter.x * startToCenter.x + startToCenter.y * startToCenter.y + startToCenter.z * startToCenter.z - circleRadius * circleRadius;

	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		// No intersection
		return false;
	}

	discriminant = sqrt(discriminant);
	float t1 = (-b - discriminant) / (2 * a);
	float t2 = (-b + discriminant) / (2 * a);

	// Check if at least one solution is within the segment
	return (t1 >= 0 && t1 <= 1) || (t2 >= 0 && t2 <= 1);
}


bool checkCollisionBulletWithZombie(Vector3f& bulletPos, Vector3f& zombiePos, float deltaTime , Vector3f& velocity, std::vector<std::pair<Vector3f, float>>& spheres) {
	Vector3f bulletStartPos = bulletPos;
	Vector3f bulletEndPos = bulletPos + velocity * deltaTime;

	for (auto& sphere : spheres) {
		Vector3f sphereCenter = zombiePos + sphere.first; // Adjusting sphere center relative to zombie position
		float sphereRadius = sphere.second;

		if (intersectionDetected(bulletStartPos, bulletEndPos, sphereCenter, sphereRadius)) {
			return true;
		}
	}

	return false;
}

void drawSunRay(float x, float y, float z, float length, float width) {
	GLfloat rayColor[] = { 1.0f, 1.0f, 0.8f, 0.3f }; // Soft yellow, semi-transparent
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	glColor4fv(rayColor);
	// Create a quad representing the sun ray
	glVertex3f(x - width, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y, z + length);
	glVertex3f(x - width, y, z + length);
	glEnd();

	glDisable(GL_BLEND);
}


void addWindowLight() {
	// Global ambient light (very dim)
/*GLfloat globalAmbient[] = { 0.08f, 0.08f, 0.08f, 1.0f };
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);*/

// Setup window light sources here
// Example: GL_LIGHT1 positioned near a window
	GLfloat windowLightDiffuse[] = { 1, 1, 1, 0.0f }; // Soft white light

	GLfloat windowLightPosition[] = { 2.5, 2.5, 2.5, 1.0f }; // Position near the window
	GLfloat spotDirection[] = { -1, -1, 0 };



	glLightfv(GL_LIGHT2, GL_DIFFUSE, windowLightDiffuse);
	//glLightfv(GL_LIGHT1, GL_AMBIENT, intensity);
	glLightfv(GL_LIGHT2, GL_POSITION, windowLightPosition);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDirection);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 75);
	drawSunRay(0,0,0, 5.0f, 0.1f);
}

class Bullet {
public:
	Vector3f position;
	Vector3f velocity;
	float speed;

	Bullet(Vector3f pos, Vector3f dir, float spd) : position(pos), speed(spd) {
		velocity = dir.unit() * spd;  // Normalize direction and multiply by speed
	}

	void update(float deltaTime) {
		position = position + velocity * deltaTime;  // Move the bullet
	}
};

std::vector<Bullet> bullets; // Global vector to store bullets

float bulletSpeed = 560;
class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = eyeX, float eyeY = eyeY, float eyeZ = eyeZ, float centerX = centerX, float centerY = centerY, float centerZ = centerZ, float upX = upX, float upY = upY, float upZ = upZ) {
		printf("%i", eyeX);
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);

	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}
	void printCameraValues() {
		printf("eyex %f\neyeY %f\neyeZ %f\ncenterX %f\ncenterY %f\ncenterZ %f\nupX %f\nupY %f\nupZ %f", eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
	}

	void changeView() {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
		moveX(movex);
		moveY(movey);
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}

};

Camera camera;



void drawHeart(float x, float y, float size, bool filled) {
	glPushMatrix();
	glRotatef(180, 1, 0, 0);
	if (filled) {
		glColor3f(1.0f, 0.0f, 0.0f); // Red for filled heart
		glBegin(GL_POLYGON);
	}
	else {
		glColor3f(0.5f, 0.5f, 0.5f); // Grey for empty heart
		glBegin(GL_POLYGON);
	}

	// The top left curve of the heart
	for (int i = 0; i <= 180; i++) {
		float rad = i * DEG2RAD(i);
		glVertex2f(x + cos(rad) * size - size, y + sin(rad) * size + size / 2);
	}

	// The top right curve of the heart
	for (int i = 0; i <= 180; i++) {
		float rad = i * DEG2RAD(i);
		glVertex2f(x + cos(rad) * size + size, y + sin(rad) * size + size / 2);
	}

	// The bottom triangle of the heart
	glVertex2f(x, y - size * 2);
	glVertex2f(x - size * 2, y + size / 2);
	glVertex2f(x + size * 2, y + size / 2);

	glColor3d(1.0, 1.0, 1.0);

	glEnd();
	glPopMatrix();
}





void UpdateCameraPosition() {
	float distanceBehindPlayer = 3.2f; // Distance behind the player
	float heightAbovePlayer = 3.0f; // Height above the player
	float eyeLevel = 2.4f;
	float addZ = playerAngle == 180 ? -1.2 : 1.2;


	if (mainCorridor1) {


		if (currView == 0) {
			float radianAngle = DEG2RAD(playerAngle);
			float camX = playerXMainCorr1 - distanceBehindPlayer * sin(radianAngle);
			float camY = playerYMainCorr1 + heightAbovePlayer;
			float camZ = playerZMainCorr1 - distanceBehindPlayer * cos(radianAngle);

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// Update camera's center to look at the player
			camera.center.x = playerXMainCorr1;
			camera.center.y = playerYMainCorr1 + 2.4;
			camera.center.z = playerZMainCorr1;
		}
		else {


			float radianAngle = DEG2RAD(playerAngle);
			camera.eye.x = playerXMainCorr1;
			camera.eye.y = playerYMainCorr1 + eyeLevel;
			camera.eye.z = playerZMainCorr1 + addZ;

			// Make the camera look in the direction the player is facing
			camera.center.x = camera.eye.x + sin(radianAngle);
			camera.center.y = camera.eye.y;
			camera.center.z = camera.eye.z + cos(radianAngle);

			camera.up.x = 0.0f;
			camera.up.y = 1.0f;
			camera.up.z = 0.0f;
		}
	}
	else if (room1) {


		if (currView == 0) {
			float radianAngle = DEG2RAD(playerAngle);
			float camX = playerXRoom1 - distanceBehindPlayer * sin(radianAngle);
			float camY = playerYRoom1 + heightAbovePlayer;
			float camZ = playerZRoom1 - distanceBehindPlayer * cos(radianAngle);

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// Update camera's center to look at the player
			camera.center.x = playerXRoom1;
			camera.center.y = playerYRoom1 + 2.4;
			camera.center.z = playerZRoom1;
		}
		else {


			float radianAngle = DEG2RAD(playerAngle);
			camera.eye.x = playerXRoom1;
			camera.eye.y = playerYRoom1 + eyeLevel;
			camera.eye.z = playerZRoom1 + addZ;

			// Make the camera look in the direction the player is facing
			camera.center.x = camera.eye.x + sin(radianAngle);
			camera.center.y = camera.eye.y;
			camera.center.z = camera.eye.z + cos(radianAngle);

			camera.up.x = 0.0f;
			camera.up.y = 1.0f;
			camera.up.z = 0.0f;
		}
	}


	else if (room3) {


		if (currView == 0) {
			float radianAngle = DEG2RAD(playerAngle);
			float camX = playerXRoom3 - distanceBehindPlayer * sin(radianAngle);
			float camY = playerYRoom3 + heightAbovePlayer;
			float camZ = playerZRoom3 - distanceBehindPlayer * cos(radianAngle);

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// Update camera's center to look at the player
			camera.center.x = playerXRoom3;
			camera.center.y = playerYRoom3 + 2.4;
			camera.center.z = playerZRoom3;
		}
		else {


			float radianAngle = DEG2RAD(playerAngle);
			camera.eye.x = playerXRoom3;
			camera.eye.y = playerYRoom3 + eyeLevel;
			camera.eye.z = playerZRoom3 + addZ;

			// Make the camera look in the direction the player is facing
			camera.center.x = camera.eye.x + sin(radianAngle);
			camera.center.y = camera.eye.y;
			camera.center.z = camera.eye.z + cos(radianAngle);

			camera.up.x = 0.0f;
			camera.up.y = 1.0f;
			camera.up.z = 0.0f;
		}
	}
	else if (mainCorridor2) {


		if (currView == 0) {
			float radianAngle = DEG2RAD(playerAngle);
			float camX = playerXMainCorr2 - distanceBehindPlayer * sin(radianAngle);
			float camY = playerYMainCorr2 + heightAbovePlayer;
			float camZ = playerZMainCorr2 - distanceBehindPlayer * cos(radianAngle);

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// Update camera's center to look at the player
			camera.center.x = playerXMainCorr2;
			camera.center.y = playerYMainCorr2 + 2.4;
			camera.center.z = playerZMainCorr2;
		}
		else {


			float radianAngle = DEG2RAD(playerAngle);
			camera.eye.x = playerXMainCorr2;
			camera.eye.y = playerYMainCorr2 + eyeLevel;
			camera.eye.z = playerZMainCorr2 + addZ;

			// Make the camera look in the direction the player is facing
			camera.center.x = camera.eye.x + sin(radianAngle);
			camera.center.y = camera.eye.y;
			camera.center.z = camera.eye.z + cos(radianAngle);

			camera.up.x = 0.0f;
			camera.up.y = 1.0f;
			camera.up.z = 0.0f;
		}
	}

	else if (room2) {


		if (currView == 0) {
			float radianAngle = DEG2RAD(playerAngle);
			float camX = playerXRoom2 - distanceBehindPlayer * sin(radianAngle);
			float camY = playerYRoom2 + heightAbovePlayer;
			float camZ = playerZRoom2 - distanceBehindPlayer * cos(radianAngle);

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// Update camera's center to look at the player
			camera.center.x = playerXRoom2;
			camera.center.y = playerYRoom2 + 2.4;
			camera.center.z = playerZRoom2;
		}
		else {


			float radianAngle = DEG2RAD(playerAngle);
			camera.eye.x = playerXRoom2;
			camera.eye.y = playerYRoom2 + eyeLevel;
			camera.eye.z = playerZRoom2 + addZ;

			// Make the camera look in the direction the player is facing
			camera.center.x = camera.eye.x + sin(radianAngle);
			camera.center.y = camera.eye.y;
			camera.center.z = camera.eye.z + cos(radianAngle);

			camera.up.x = 0.0f;
			camera.up.y = 1.0f;
			camera.up.z = 0.0f;
		}
	}

	else if (finalBoss) {


		// Calculate the camera position based on player's position and angle
		if (currView == 0) {
			float radianAngle = DEG2RAD(playerAngle);
			float camX = playerXFinal - distanceBehindPlayer * sin(radianAngle);
			float camY = playerYFinal + heightAbovePlayer;
			float camZ = playerZFinal - distanceBehindPlayer * cos(radianAngle);

			// Update camera position
			camera.eye.x = camX;
			camera.eye.y = camY;
			camera.eye.z = camZ;

			// Update camera's center to look at the player
			camera.center.x = playerXFinal;
			camera.center.y = playerYFinal + 2.4;
			camera.center.z = playerZFinal;
		}
		else {


			float radianAngle = DEG2RAD(playerAngle);
			camera.eye.x = playerXFinal;
			camera.eye.y = playerYFinal + eyeLevel;
			camera.eye.z = playerZFinal + addZ;

			// Make the camera look in the direction the player is facing
			camera.center.x = camera.eye.x + sin(radianAngle);
			camera.center.y = camera.eye.y;
			camera.center.z = camera.eye.z + cos(radianAngle);

			camera.up.x = 0.0f;
			camera.up.y = 1.0f;
			camera.up.z = 0.0f;
		}
	}


}

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

Vector Eye(20, 5, 20);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_zombie;
Model_3DS model_player;
Model_3DS model_boss;
Model_3DS model_key;
Model_3DS model_window;
Model_3DS model_door;
Model_3DS model_table;
Model_3DS model_wall;
Model_3DS model_torch;
Model_3DS model_rightWall;
Model_3DS model_labDoor;
Model_3DS model_potion;
Model_3DS model_arena;
Model_3DS model_moreDamage;
Model_3DS model_shield;
Model_3DS model_makhtuta;
Model_3DS model_bandits;
// Textures
GLTexture tex_ground;
GLTexture tex_ceiling;
GLTexture tex_FinalGround;
GLTexture tex_finalWall;




//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
//	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
//	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}


class Zombie {
public:
	Vector3f position;
	float speed;
	std::vector<std::pair<Vector3f, float>> collisionSpheres;
	bool alive;
	float angle;

	Zombie(Vector3f pos, float spd, std::vector<std::pair<Vector3f, float>>& spheres) : position(pos), speed(spd), collisionSpheres(spheres) {
		alive = 1;
		angle = 0;
	}

	void update() {
		if (position.z >= 19)
			return;
		if (alive == 0)
			angle++;
		if (angle == 90)
			position.z = 10000;
		position.z += speed;
	}
};

std::vector<Zombie> zombies; // Global vector to store bullets


void setupLightSources() {
	// Global ambient light (very dim)
	/*GLfloat globalAmbient[] = { 0.08f, 0.08f, 0.08f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);*/

	// Setup window light sources here
	// Example: GL_LIGHT1 positioned near a window
	GLfloat windowLightDiffuse[] = { 1, 1, 1, 1.0f }; // Soft white light
	float lightZ = 1;
	if (playerAngle == 0)
		lightZ = -1;
	else if (playerAngle == 180)
		lightZ = 1;
	else
		lightZ = 0;

	float lightX = 0;
	if (playerAngle == 90)
		lightX = -1;
	else if (playerAngle == 270)
		lightX = 1;
	else
		lightX = 0;
	GLfloat windowLightPosition[] = { lightX, 0, lightZ, 0.0f }; // Position near the window
	//GLfloat spotDirection[] = { 0, 0, -1 };



	glLightfv(GL_LIGHT1, GL_DIFFUSE, windowLightDiffuse);
	//glLightfv(GL_LIGHT1, GL_AMBIENT, intensity);
	glLightfv(GL_LIGHT1, GL_POSITION, windowLightPosition);
	//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);
	//glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45); // 45 degrees cutoff angle

	
}


// Global variables
GLfloat morningAmbient[] = { 0.3f, 0.3f, 0.5f, 1.0f };
GLfloat dayAmbient[] = { 0.6f, 0.6f, 0.8f, 1.0f };
GLfloat eveningAmbient[] = { 0.3f, 0.2f, 0.5f, 1.0f };
GLfloat nightAmbient[] = { 0.1f, 0.1f, 0.2f, 1.0f };

GLfloat morningDiffuse[] = { 0.7f, 0.7f, 0.5f, 1.0f };
GLfloat dayDiffuse[] = { 1.0f, 1.0f, 0.9f, 1.0f };
GLfloat eveningDiffuse[] = { 0.7f, 0.5f, 0.3f, 1.0f };
GLfloat nightDiffuse[] = { 0.2f, 0.2f, 0.3f, 1.0f };

// Simulate time with a variable
float timeOfDay = 0.0f; // Ranges from 0.0 (morning) to 1.0 (night)

void updateDayCycle(float deltaTime) {
	// Increment time of day
	timeOfDay += deltaTime;
	if (timeOfDay > 1.0f) timeOfDay -= 1.0f; // Reset after a full cycle

	// Determine the current phase of the day
	GLfloat* ambient;
	GLfloat* diffuse;

	if (timeOfDay < 0.25f) { // Morning to day
		ambient = morningAmbient;
		diffuse = morningDiffuse;
	}
	else if (timeOfDay < 0.5f) { // Day
		ambient = dayAmbient;
		diffuse = dayDiffuse;
	}
	else if (timeOfDay < 0.75f) { // Evening
		ambient = eveningAmbient;
		diffuse = eveningDiffuse;
	}
	else { // Night
		ambient = nightAmbient;
		diffuse = nightDiffuse;
	}

	// Set the light source parameters
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse);

	// Update background color to match time of day
	GLfloat backgroundColor[3];
	for (int i = 0; i < 3; ++i) {
		backgroundColor[i] = ambient[i] + diffuse[i]; // Simplified calculation
	}
	glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
}

// Call updateDayCycle in your game's update loop
// deltaTime is the time passed since the last update, which can be used to control the speed of the day/night cycle


//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{

}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	// Enable 2D texturing
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]); // Bind the ground texture

	// Enable Lighting
	//glEnable(GL_LIGHTING);

	flashLight || room3 ? glColor3d(0.3, 0.3, 0.3) : glColor3d(0.1, 0.1, 0.1);
	// Draw the ground
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0); // Normal pointing up
	glTexCoord2f(0, 0);
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();
	glColor3d(1, 1, 1);
	// Disable texturing
	glDisable(GL_TEXTURE_2D);
}


void RenderFinalGround()
{
	// Enable 2D texturing
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_FinalGround.texture[0]); // Bind the ground texture

	// Enable Lighting
	//glEnable(GL_LIGHTING);

	flashLight || room3 || finalBoss ? glColor3d(0.3, 0.3, 0.3) : glColor3d(0.1, 0.1, 0.1);
	// Draw the ground
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0); // Normal pointing up
	glTexCoord2f(0, 0);
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();
	glColor3d(1, 1, 1);
	// Disable texturing
	glDisable(GL_TEXTURE_2D);
}



void RenderWall()
{
	// Enable 2D texturing
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_ceiling.texture[0]); // Bind the ground texture

	// Enable Lighting
	//glEnable(GL_LIGHTING);

	flashLight ? glColor3d(0.7, 0.7, 0.7) : glColor3d(0.4, 0.4, 0.4);
	// Draw the ground
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 0); // Normal pointing up
	glTexCoord2f(0, 0);
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();
	glColor3d(1, 1, 1);
	// Disable texturing
	glDisable(GL_TEXTURE_2D);
}

void RenderFinalWall()
{
	// Enable 2D texturing
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_finalWall.texture[0]); // Bind the ground texture

	// Enable Lighting
	//glEnable(GL_LIGHTING);

	flashLight || finalBoss ? glColor3d(0.7, 0.7, 0.7) : glColor3d(0.4, 0.4, 0.4);
	// Draw the ground
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 0); // Normal pointing up
	glTexCoord2f(0, 0);
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();
	glColor3d(1, 1, 1);
	// Disable texturing
	glDisable(GL_TEXTURE_2D);
}


bool isMouseCollidingWithZombies(std::vector<Zombie>& zombies) {
	Vector3f mouseWorldPos = screenToWorld(mouseX, mouseY);

	for (auto& zombie : zombies) {
		for (auto& sphere : zombie.collisionSpheres) {
			Vector3f sphereCenter = zombie.position + sphere.first;
			float sphereRadius = sphere.second;

			if ((mouseWorldPos - sphereCenter).magnitude() <= sphereRadius) {
				printf("OIWUROEUROWE\n");
				return true; // Collision detected
			}
		}
	}

	return false; // No collision detected
}

void initZombies() {

	std::vector<std::pair<Vector3f, float>> zombieCollisionSpheres;
	// Example: head, torso, legs
	zombieCollisionSpheres.push_back(std::make_pair(Vector3f(0, 3, 0), 1));
	zombieCollisionSpheres.push_back(std::make_pair(Vector3f(0, 2, 0), 1.5));
	zombieCollisionSpheres.push_back(std::make_pair(Vector3f(0, 1, 0), 1.5));
	Vector3f zombie1Pos = { 0,0,0 };
	// Create a bullet at the camera position
	Zombie zombie1(zombie1Pos, 0.005, zombieCollisionSpheres);
	zombies.push_back(zombie1); // Assuming you have a list of bullets

	Vector3f zombie2Pos = { 2.5,0,-10 };
	// Create a bullet at the camera position
	Zombie zombie2(zombie2Pos, 0.008 , zombieCollisionSpheres);
	zombies.push_back(zombie2); // Assuming you have a list of bullets
}


std::vector<std::pair<Vector3f, float>> bossSpheres;





//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	if (score < 0)
		score = 0;
	setupCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT2);
	setupLightSources();



	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 0.7f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
	GLfloat ambient[] = { 0.7, 0.7, 0.7, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambient);
	


	

	//showMessage(0, 0, GLUT_BITMAP_HELVETICA_18, "ALLLOOOOO");


	if (mainCorridor1 == true) {
		

		printf("playerX : %f\nplayerZ: %f\n", playerXMainCorr1, playerZMainCorr1);


		//setupLightSources();

		std::vector<Zombie>::iterator zombieIt = zombies.begin();
		while (zombieIt != zombies.end()) {
			bool zombieHit = false;
			std::vector<Bullet>::iterator bulletIt = bullets.begin();
			while (bulletIt != bullets.end()) {
				//printf("ALO");
				if (checkCollisionBulletWithZombie(bulletIt->position, zombieIt->position , 0.016 , bulletIt->velocity , zombieIt->collisionSpheres)) {
					zombieHit = true;
					bulletIt = bullets.erase(bulletIt); // Erase bullet and advance iterator
					zombieScream.play();
					score += 5;
				}
				else if (bulletIt->position.x >= 10000 || bulletIt->position.x <= -10000 || bulletIt->position.z >= 10000 || bulletIt->position.z <= -10000) {
					bulletIt = bullets.erase(bulletIt);
				}
				
				else {
					glPushMatrix();
					glTranslatef(bulletIt->position.x, bulletIt->position.y, bulletIt->position.z);
					//printf("positionX LOOP %f\npostiionY LOOP %f\npositionZ LOOP %f\n", bullet.position.x, bullet.position.y, bullet.position.z);

					glScalef(0.01, 0.01, 0.01);
					glRotatef(-90, 0, 1, 0);
					glRotatef(90, 0, 0, 1);
					model_tree.Draw();
					glPopMatrix();
					++bulletIt; // Only increment if not erased
				}
			}

			if (zombieHit) {
				zombieIt->alive = 0;
				//zombieIt = zombies.erase(zombieIt); // Erase zombie and advance iterator
			}
			else {
				glPushMatrix();
				glTranslatef(zombieIt->position.x, zombieIt->position.y , zombieIt->position.z);
				glRotatef(zombieIt->angle * -1, 1, 0, 0);
				glScalef(0.02, 0.02, 0.02);
				model_zombie.Draw();
				glPopMatrix();

				if (zombieIt->position.x >= playerXMainCorr1 - 1 && zombieIt->position.x <= playerXMainCorr1 + 1 && zombieIt->position.z >= playerZMainCorr1 - 0.46 && zombieIt->position.z <= playerZMainCorr1 + 0.24 && zombieIt->alive == 1) {
					playerHit.play();
					if (!menritCheat) {
						healthRem--;
						score -= 2;
					}
					if (playerZMainCorr1 < 18) {

						if (playerZMainCorr1 < playerZMainCorr1Prev) {
							playerZMainCorr1 += 1;
							playerZMainCorr1Prev += 1;
							if (zombieIt->position.z - 1 > -18.5)
								zombieIt->position.z -= 1;

						}
						else {
							playerZMainCorr1 -= 1;
							playerZMainCorr1Prev -= 1;

						}
					}

				}

				++zombieIt; // Only increment if not erased
			}
		}
		// Draw Ground
		glPushMatrix();
		glScalef(1, 1, 1.5);
		RenderGround();
		glPopMatrix();




		// Draw Tree Model
		glPushMatrix();
		glTranslatef(10, 0, 0);
		glScalef(0.7, 0.7, 0.7);
		model_tree.Draw();
		glPopMatrix();

		// Draw house Model
		glPushMatrix();
		glScalef(10, 10.1, 10.1);
		glRotatef(90.f, 1, 0, 0);
		model_house.Draw();
		glPopMatrix();

		// Draw Player
		glPushMatrix();
		glTranslatef(playerXMainCorr1, playerYMainCorr1 + 1.3, playerZMainCorr1);
		glScalef(0.05, 0.05, 0.05);
		glRotatef(playerAngle, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		model_player.Draw();
		glPopMatrix();

		// 


		// Draw Door
		glPushMatrix();
		glTranslatef(-3.38, 0.1, 0);
		glScalef(0.7, 3.4, 0.5);
		glRotatef(90, 0, 0, 1);
		model_door.Draw();
		glPopMatrix();




		// Draw Walls


/*		glPushMatrix();
		glBegin(GL_QUADS);
		Vector3f normal = { 1,0,0 };
		// Set the normal for the first vertex
		glNormal3f(normal.x, normal.y, normal.z);
		// Define the first vertex
		glVertex3f(-5, 0, 19);

		// Set the same normal for the second vertex
		glNormal3f(normal.x, normal.y, normal.z);
		// Define the second vertex
		glVertex3f(-5, 0, 0);

		// Repeat for third vertex
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(-5, 10,0);

		// And for the fourth vertex
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(-5,10,19);

		glEnd();*/

		// Draw Left Wall
		glPushMatrix();
		glTranslatef(-3.5, 0, 0);
		glScalef(1, 2.5, 2.0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		//model_wall.shownormals = true;
		//model_wall.rot;
		RenderWall();
		glPopMatrix();

		// Draw Right Wall
		glPushMatrix();
		glTranslatef(3.5, 0, 18.8);
		glScalef(1, 2.5, 1.62);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		//model_wall.shownormals = true;
		//model_wall.rot;
		RenderWall();
		glPopMatrix();



		// Draw Last Wall

		glPushMatrix();
		glTranslatef(5, 0, -20);
		glScalef(1, 2.5, 3);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();


		// Draw First Wall

		glPushMatrix();
		glTranslatef(5, 15, 22.75);
		glScalef(1, 4.5, 3);
		glRotatef(270, 1, 0, 0);
		RenderWall();
		glPopMatrix();


		// Draw Second Wall

		glPushMatrix();
		glTranslatef(23.5, 15, -12);
		glScalef(1, 4.5, 3);
		glRotatef(270, 1, 0, 0);
		RenderWall();
		RenderWall();

		glPopMatrix();

		// draw door to get out of game
		glPushMatrix();
		glTranslatef(-0.4, -0.5, 22.25);
		glScalef(1.0, 2.3, 0.8);

		glRotatef(90, 1, 0, 0);


		model_door.Draw();
		glPopMatrix();

		// Draw Ceiling
		
		glPushMatrix();
		glTranslatef(8, 13.5, 0);
		glScalef(15, 2.5, 3);
		glRotatef(90, 0, 1, 0);
		RenderWall();
		glPopMatrix();
		

		//sky box
		glPushMatrix();
		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glTranslated(50, 0, 0);
		glRotated(90, 1, 0, 1);
		glBindTexture(GL_TEXTURE_2D, tex);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 100, 100, 100);
		gluDeleteQuadric(qobj);
		glPopMatrix();


		glEnable(GL_LIGHT0);
		drawCustomCursor(mouseX, mouseY);

		if (room1EnterFlag)
			showMessage("Press v To Enter", 700, 600);

		if (menrit) {
			showMessage("You are a friend of Menritooo, Nothing can damage you in this game", 550, 730);
			printf("HELLO MENRITOOOO");
		}


	}
	else if (room1 == true) {

		printf("playerX : %f\nplayerZ: %f\n", playerXRoom1, playerZRoom1);

		RenderGround();

		// draw torch 
		if (isTorchTaken == false) {
			glPushMatrix();
			glTranslatef(-2, 2.35, 4.7);
			glRotatef(90, 1, 0, 0);
			glScalef(0.1, 0.1, 0.1);
			model_torch.Draw();
			glPopMatrix();
		}

		//draw table
		glPushMatrix();
		glTranslatef(1, -1, 5);
		glRotatef(90, 1, 0, 0);
		glScalef(0.01, 0.01, 0.02);
		model_table.Draw();
		glPopMatrix();


		glPushMatrix();
		glTranslatef(playerXRoom1, playerYRoom1 + 1.3, playerZRoom1);
		glScalef(0.05, 0.05, 0.05);
		glRotatef(playerAngle, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		model_player.Draw();
		glPopMatrix();

		// Draw Left Wall

		glPushMatrix();
		glTranslatef(-7, 0, 0);
		glScalef(1, 2.5, 2.0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();

		// Draw Right Wall
		glPushMatrix();
		glTranslatef(5, 0, 3.8);
		glScalef(1, 2.5, 1.62);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();

		// Draw Last Wall

		glPushMatrix();
		glTranslatef(-4, 0, -10);
		glScalef(1, 2.5, 3);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();


		// Draw Door to return to mainCorr1

		glPushMatrix();
		glTranslatef(-1.1, -1.4, 22.4);
		glScalef(1.0, 2.3, 0.8);
		glRotatef(90, 1, 0, 0);
		model_door.Draw();
		glPopMatrix();


		// Draw First Wall
		glPushMatrix();
		glTranslatef(-4, 0, 22.7);
		glScalef(1, 2.5, 3);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();

		// Draw Ceiling

		glPushMatrix();
		glTranslatef(8, 13.5, 0);
		glScalef(15, 2.5, 3);
		glRotatef(90, 0, 1, 0);
		RenderWall();
		glPopMatrix();

		glEnable(GL_LIGHT0);
		if (torchPrompVis) {
			showMessage("Press m To Collect Torch", 700, 600);
		}

		if (returnToMainCorr1)
			showMessage("Press v To Return To Main Corridor", 700, 600);
		glDisable(GL_LIGHT0);
	}

	else if (room2 == true) {
		RenderGround();

		// draw potion 
		if (isPotionTaken == false) { /// <<<<<<<<<<<< EDIT
			glPushMatrix();
			glTranslatef(-2.4, 2.3, 4.7);
			glRotatef(90, 1, 0, 0);
			glScalef(0.03, 0.03, 0.03);
			model_bandits.Draw();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-1.5, 2.3, 4.7);
			glRotatef(90, 1, 0, 0);
			glScalef(0.03, 0.03, 0.03);
			model_bandits.Draw();
			glPopMatrix();


			glPushMatrix();
			glTranslatef(-0.5, 2.3, 4.7);
			glRotatef(90, 1, 0, 0);
			glScalef(0.03, 0.03, 0.03);
			model_bandits.Draw();
			glPopMatrix();


			glPushMatrix();
			glTranslatef(0.5, 2.3, 4.7);
			glRotatef(90, 1, 0, 0);
			glScalef(0.03, 0.03, 0.03);
			model_bandits.Draw();
			glPopMatrix();

		}

		//draw table
		glPushMatrix();
		glTranslatef(1, -1, 5);
		glRotatef(90, 1, 0, 0);
		glScalef(0.01, 0.01, 0.02);
		model_table.Draw();
		glPopMatrix();


		glPushMatrix();
		glTranslatef(playerXRoom2, playerYRoom2 + 1.3, playerZRoom2);
		glScalef(0.05, 0.05, 0.05);
		glRotatef(playerAngle, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		model_player.Draw();
		glPopMatrix();

		// Draw Left Wall

		glPushMatrix();
		glTranslatef(-7, 0, 0);
		glScalef(1, 2.5, 2.0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();

		// Draw Right Wall
		glPushMatrix();
		glTranslatef(5, 0, 3.8);
		glScalef(1, 2.5, 1.62);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();

		// Draw Last Wall

		glPushMatrix();
		glTranslatef(-4, 0, -10);
		glScalef(1, 2.5, 3);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();


		// Draw Door to return to mainCorr1

		glPushMatrix();
		glTranslatef(-1.1, -1.4, 22.4);
		glScalef(1.0, 2.3, 0.8);
		glRotatef(90, 1, 0, 0);
		model_door.Draw();
		glPopMatrix();


		// Draw First Wall
		glPushMatrix();
		glTranslatef(-4, 0, 22.7);
		glScalef(1, 2.5, 3);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();

		// Draw Ceiling

		glPushMatrix();
		glTranslatef(8, 13.5, 0);
		glScalef(15, 2.5, 3);
		glRotatef(90, 0, 1, 0);
		RenderWall();
		glPopMatrix();

		// Draw Hanged Man


		glEnable(GL_LIGHT0);


		if (potionPrompVis) {
			showMessage("Press m To Collect Health Potions", 700, 600);
		}

		if (returnToMainCorr1)
			showMessage("Press v To Return To Main Corridor", 700, 600);

		glDisable(GL_LIGHT0);

		glPushMatrix();
		glTranslatef(2, 9.4, 8);
		glScalef(3, 3.1, 3.1);
		glRotatef(180, 0, 1, 0);
		glRotatef(90.f, 1, 0, 0);
		model_house.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-2, 9.4, 8);
		glScalef(3, 3.1, 3.1);
		glRotatef(180, 0, 1, 0);
		glRotatef(90.f, 1, 0, 0);
		model_house.Draw();
		glPopMatrix();


	}


	else if (room3 == true) {

		printf("playerX : %f\nplayerZ: %f\n", playerXRoom3, playerZRoom3);

		addWindowLight();
		glEnable(GL_LIGHT2);

		RenderGround();

		// draw torch 
		if (isKeyTaken == false) {
			glPushMatrix();
			glTranslatef(-2, 2.35, 4.7);
			glRotatef(90, 1, 0, 0);
			glScalef(0.02, 0.02, 0.02);
			model_key.Draw();
			glPopMatrix();
		}

		//draw table
		glPushMatrix();
		glTranslatef(1, -1, 5);
		glRotatef(90, 1, 0, 0);
		glScalef(0.01, 0.01, 0.02);
		model_table.Draw();
		glPopMatrix();


		glPushMatrix();
		glTranslatef(playerXRoom3, playerYRoom3 + 1.3, playerZRoom3);
		glScalef(0.05, 0.05, 0.05);
		glRotatef(playerAngle, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		model_player.Draw();
		glPopMatrix();

		// Draw Left Wall

		glPushMatrix();
		glTranslatef(-7, 0, 0);
		glScalef(1, 2.5, 2.0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();

		// Draw Right Wall
		glPushMatrix();
		glTranslatef(5, 0, 3.8);
		glScalef(1, 2.5, 1.62);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();

		// Draw Last Wall

		glPushMatrix();
		glTranslatef(-4, 0, -10);
		glScalef(1, 2.5, 3);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();


		// Draw Door to return to mainCorr1

		glPushMatrix();
		glTranslatef(-1.1, -1.4, 22.4);
		glScalef(1.0, 2.3, 0.8);
		glRotatef(90, 1, 0, 0);
		model_door.Draw();
		glPopMatrix();


		// Draw First Wall
		glPushMatrix();
		glTranslatef(-4, 0, 22.7);
		glScalef(1, 2.5, 3);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();

		// Draw Ceiling

		glPushMatrix();
		glTranslatef(8, 13.5, 0);
		glScalef(15, 2.5, 3);
		glRotatef(90, 0, 1, 0);
		RenderWall();
		glPopMatrix();

		glEnable(GL_LIGHT0);

		glPushMatrix();
		glTranslatef(4.5, 4, 2.7);
		glScalef(1, 1.4, 1.62);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		model_window.Draw();
		glPopMatrix();


		if (potionPrompVis) {
			showMessage("Press m To Collect Key", 700, 600);
		}

		if (returnToMainCorr1)
			showMessage("Press v To Return To Main Corridor", 700, 600);
		glDisable(GL_LIGHT0);
	}

	if (mainCorridor2 == true) {


		printf("playerX : %f\nplayerZ: %f\n", playerXMainCorr2, playerZMainCorr2);


		//setupLightSources();


		// Draw Ground
		glPushMatrix();
		glScalef(1, 1, 1.5);
		RenderGround();
		glPopMatrix();




		// Draw Tree Model
		glPushMatrix();
		glTranslatef(10, 0, 0);
		glScalef(0.7, 0.7, 0.7);
		model_tree.Draw();
		glPopMatrix();

		// Draw house Model


		// Draw Player
		glPushMatrix();
		glTranslatef(playerXMainCorr2, playerYMainCorr2 + 1.3, playerZMainCorr2);
		glScalef(0.05, 0.05, 0.05);
		glRotatef(playerAngle, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		model_player.Draw();
		glPopMatrix();

		// 


		// Draw Door
		glPushMatrix();
		glTranslatef(-3.38, 0.1, 10);
		glScalef(0.7, 3.4, 0.5);
		glRotatef(90, 0, 0, 1);
		model_door.Draw();
		glPopMatrix();



		// Draw Door for key room
		glPushMatrix();
		glTranslatef(3.38, 0.1, 0);
		glScalef(0.7, 3.4, 0.5);
		glRotatef(90, 0, 0, 1);
		model_door.Draw();
		glPopMatrix();




		// Draw Walls


/*		glPushMatrix();
		glBegin(GL_QUADS);
		Vector3f normal = { 1,0,0 };
		// Set the normal for the first vertex
		glNormal3f(normal.x, normal.y, normal.z);
		// Define the first vertex
		glVertex3f(-5, 0, 19);

		// Set the same normal for the second vertex
		glNormal3f(normal.x, normal.y, normal.z);
		// Define the second vertex
		glVertex3f(-5, 0, 0);

		// Repeat for third vertex
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(-5, 10,0);

		// And for the fourth vertex
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(-5,10,19);

		glEnd();*/

		// Draw Left Wall
		glPushMatrix();
		glTranslatef(-3.5, 0, 0);
		glScalef(1, 2.5, 2.0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		//model_wall.shownormals = true;
		//model_wall.rot;
		RenderWall();
		glPopMatrix();

		// Draw Right Wall
		glPushMatrix();
		glTranslatef(3.5, 0, 18.8);
		glScalef(1, 2.5, 1.62);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		//model_wall.shownormals = true;
		//model_wall.rot;
		RenderWall();
		glPopMatrix();



		// Draw Last Wall

		glPushMatrix();
		glTranslatef(5, 0, -20);
		glScalef(1, 2.5, 3);
		glRotatef(90, 1, 0, 0);
		RenderWall();
		glPopMatrix();


		// Draw First Wall

		glPushMatrix();
		glTranslatef(5, 15, 22.75);
		glScalef(1, 4.5, 3);
		glRotatef(270, 1, 0, 0);
		RenderWall();
		glPopMatrix();


		// Draw Second Wall

		glPushMatrix();
		glTranslatef(23.5, 15, -12);
		glScalef(1, 4.5, 3);
		glRotatef(270, 1, 0, 0);
		RenderWall();
		RenderWall();

		glPopMatrix();

		// draw door to get out of game
		glPushMatrix();
		glTranslatef(-0.4, -0.5, 22.25);
		glScalef(1.0, 2.3, 0.8);

		glRotatef(90, 1, 0, 0);


		model_door.Draw();
		glPopMatrix();

		// Draw Ceiling

		glPushMatrix();
		glTranslatef(8, 13.5, 0);
		glScalef(15, 2.5, 3);
		glRotatef(90, 0, 1, 0);
		RenderWall();
		glPopMatrix();


		// Draw Lab Door

		glPushMatrix();
		glTranslatef(-0.4, 0, -12.25);
		glScalef(3.8, 5, 5);
		glRotatef(90, 1, 0, 0);
		model_labDoor.Draw();
		glPopMatrix();


		//sky box
		glPushMatrix();
		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glTranslated(50, 0, 0);
		glRotated(90, 1, 0, 1);
		glBindTexture(GL_TEXTURE_2D, tex);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 100, 100, 100);
		gluDeleteQuadric(qobj);


		glPopMatrix();
		glEnable(GL_LIGHT0);
		drawCustomCursor(mouseX, mouseY);

		if (room2EnterFlag)
			showMessage("Press v To Enter", 700, 600);

		if (room3EnterFlag)
			showMessage("Press v To Enter", 700, 600);


		if (labDoorPrompt) {
			if (isKeyTaken)
				showMessage("Press v To Enter", 700, 600);
			else
				showMessage("You must find Lab Door Key first", 700, 600);
		}

		if (menrit) {
			showMessage("You are a friend of Menritooo, Nothing can damage you in this game", 550, 730);
			printf("HELLO MENRITOOOO");
		}
		std::string timeRemaining = "Score: " + std::to_string(score);
		const char* timeRemainingChar = timeRemaining.c_str();
		showMessage(timeRemainingChar, 1450, 770);
		glDisable(GL_LIGHT0);

	}
	else if (finalBoss == true) {
		printf("playerX : %f\nplayerZ : %f\n", playerXFinal , playerZFinal);
		// Example: head, torso, legs
		bossSpheres.push_back(std::make_pair(Vector3f(0, 6, 0), 1.7));
		bossSpheres.push_back(std::make_pair(Vector3f(0, 4, 0), 2.2));
		bossSpheres.push_back(std::make_pair(Vector3f(0, 2, 0), 2.2));
		updateDayCycle(0.0016);
				
		glDisable(GL_LIGHT1);
		//glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT3);



		// Draw Ground
		RenderFinalGround();


		/*
		
				std::vector<Zombie>::iterator zombieIt = zombies.begin();
		while (zombieIt != zombies.end()) {
			bool zombieHit = false;
			std::vector<Bullet>::iterator bulletIt = bullets.begin();
			while (bulletIt != bullets.end()) {
				printf("ALO");
				if (checkCollisionBulletWithZombie(bulletIt->position, zombieIt->position , 0.016 , bulletIt->velocity , zombieIt->collisionSpheres)) {
					zombieHit = true;
					bulletIt = bullets.erase(bulletIt); // Erase bullet and advance iterator
					score += 5;
				}
				else if (bulletIt->position.x >= 10000 || bulletIt->position.x <= -10000 || bulletIt->position.z >= 10000 || bulletIt->position.z <= -10000) {
					bulletIt = bullets.erase(bulletIt);
				}
		*/


		bool zombieHit = false;
		Vector3f position = {bossX , bossY , bossZ};
		std::vector<Bullet>::iterator bulletIt = bullets.begin();
		while (bulletIt != bullets.end()) {
			//printf("ALO");
			if (checkCollisionBulletWithZombie(bulletIt->position, position, 0.016, bulletIt->velocity, bossSpheres)) {
				zombieHit = true;
				zombieScream.play();
				bulletIt = bullets.erase(bulletIt); // Erase bullet and advance iterator
				score += 5;
			}
			else if (bulletIt->position.x >= 1000 || bulletIt->position.x <= -1000 || bulletIt->position.z >= 1000 || bulletIt->position.z <= -1000) {
				bulletIt = bullets.erase(bulletIt);
			}
			else
				++bulletIt; // Only increment if not erased
		}

		if (zombieHit) {
			bossCtr++;
			if (gunTaken)
				bossCtr++;
		}
		if (bossCtr >= 5) {
			bossScale = 3.5;
		}
		else bossScale = 3;

		if (bossCtr >= 10 && !bossDead) {
			bossDead = true;
			score += 30;
		}

		if (bossZ >= playerZFinal - 0.5 && bossZ <= playerZFinal + 0.5) {
			if (!bossDead) {
				// x -1.75 >> 1.5

				if (playerXFinal >= -1.75 && playerXFinal <= 1.5 ) {
					playerHit.play();

					if (shieldTaken && !shieldUse) {
						shieldUse = true;
					}
					else if (!menritCheat)
						healthRem--;
					if (bossAngle == 0) {
						playerZFinal = min(12, playerZFinal + 1);
						bossAngle = 180;
					}
					else {
						playerZFinal = max(-3, playerZFinal - 1);
						bossAngle = 0;
					}
					int currAngle = playerAngle;
					playerAngle = 180;
					UpdateCameraPosition();
					playerAngle = currAngle;
				}
			}
		}



		// Draw house Model
		glPushMatrix();
		glScalef(0, 0, 0);
		glRotatef(90.f, 1, 0, 0);
		model_house.Draw();
		glPopMatrix();


		


		//sky box
		glPushMatrix();

		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glTranslated(50, 0, 0);
		glRotated(90, 1, 0, 1);
		glBindTexture(GL_TEXTURE_2D, tex);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 100, 100, 100);
		gluDeleteQuadric(qobj);
		glPopMatrix();


		if (!finalHealthTaken) {

			// 5.250000 >> 4.25 , -2.25 >>> -1.5 x
			glPushMatrix();
			glTranslatef(-2.4, 1.2, 4.7);
			glRotatef(90, 1, 0, 0);
			glScalef(0.03, 0.03, 0.03);
			model_potion.Draw();
			glPopMatrix();

		}

		if (!shieldTaken) {
			//1.500000 >  -1  , 3.25 > 0.75
			glPushMatrix();
			glTranslatef(2, 0.7, 0);
			glScalef(0.5, 0.5, 0.5);
			model_shield.Draw();
			glPopMatrix();
		}


		if (!bossDead) {
			glPushMatrix();
			glTranslatef(bossX, bossY, bossZ);
			glRotatef(bossAngle, 0, 1, 0);
			glRotatef(90, 1, 0, 0);
			glScalef(bossScale, bossScale, bossScale);
			model_boss.Draw();
			glPopMatrix();
			glDisable(GL_LIGHT0);
		}
		else if (!makhtutaTaken) {
			glPushMatrix();
			glTranslatef(bossX, bossY, bossZ);
			glRotatef(90, 1, 0, 0);
			glScalef(0.01, 0.01, 0.01);
			model_makhtuta.Draw();
			glPopMatrix();
		}


		// Draw Gun

		// 1.75 >> -1.25 z
		// -0.75 >> 0.25 x

		if (!gunTaken) {
			glPushMatrix();
			glTranslatef(0, 0.7, 0);
			glScalef(0.3, 0.3, 0.3);
			glRotatef(90, 0, 0, 1);
			glRotatef(90, 1, 0, 0);
			model_moreDamage.Draw();
			glPopMatrix();
		}

		// Draw Left Wall

		glPushMatrix();
		glTranslatef(-7, -35, 0);
		glScalef(1, 2.5, 2.0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		RenderFinalWall();
		glPopMatrix();

		// Draw Right Wall
		glPushMatrix();
		glTranslatef(5, -35, 3.8);
		glScalef(1, 2.5, 1.62);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		RenderFinalWall();
		glPopMatrix();

		// Draw Last Wall

		glPushMatrix();
		glTranslatef(-4, -35, -10);
		glScalef(1, 2.5, 3);
		glRotatef(90, 1, 0, 0);
		RenderFinalWall();
		glPopMatrix();


		// Draw Player
		glPushMatrix();
		glTranslatef(playerXFinal, playerYFinal + 1.3, playerZFinal);
		glScalef(0.05, 0.05, 0.05);
		glRotatef(playerAngle, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		model_player.Draw();
		glPopMatrix();


		glPushMatrix();
		glTranslatef(-0.4, 0, 19);
		glScalef(6, 7.8, 5);
		glRotatef(180, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		model_labDoor.Draw();
		glPopMatrix();

		glEnable(GL_LIGHT0);

		if (gunPrompt)
			showMessage("Press m To Collect Gun (Increased Damage)", 700, 600);

		if (finalHealthPrompt)
			showMessage("Press m To Collect Health Potion", 700, 600);
		if (shieldPrompt)
			showMessage("Press m To Collect Shield", 700, 600);

		if (returnCorr2Prompt)
			showMessage("Press v To Return To Main Corridor", 700, 600);

		if (makhtutaPrompt) {
			showMessage("Press m To Collect Makhtuta", 700, 600);
		}
		if (makhtutaSecret) {
			showMessage("The Secret Of This Lab Was That It Ran Secret", 650, 600);
			showMessage("Experments On Humans and killed and hanged anyone ", 650, 570);
			showMessage("who didn't fit their wicked expermints, these ", 650, 540);
			showMessage("experments led to creating zombie-like creatures ", 650, 510);
			showMessage("that can only move forward and in rare cases backwards ", 650, 480);
			showMessage("and they damage anything they touch, ", 650, 450);
			showMessage("We Hope You Liked Our Game :) ", 650, 420);
			showMessage("Sincerly, ", 650, 390);
			showMessage("Lab Of The Dead Team ", 650, 360);


		}
		glDisable(GL_LIGHT0);

	}

	else if (gameWin == true) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, WIDTH, 0, HEIGHT);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);  // Disable writing to depth buffer
		glDepthFunc(GL_ALWAYS); // Change depth function to always pass

		glColor3d(1, 1, 1);
		renderBitmapString(600, 450, GLUT_BITMAP_HELVETICA_18, "Congrats, You Won");

		glDepthFunc(GL_LESS);   // Reset depth function
		glDepthMask(GL_TRUE);   // Re-enable writing to depth buffer
		glEnable(GL_DEPTH_TEST);

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	else if (gameLoss == true) {
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, WIDTH, 0, HEIGHT);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);  // Disable writing to depth buffer
		glDepthFunc(GL_ALWAYS); // Change depth function to always pass

		glColor3d(1, 1, 1);
		renderBitmapString(600, 450, GLUT_BITMAP_HELVETICA_18, "Sorry, You Lost");

		glDepthFunc(GL_LESS);   // Reset depth function
		glDepthMask(GL_TRUE);   // Re-enable writing to depth buffer
		glEnable(GL_DEPTH_TEST);

		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	
	/*
	glPushMatrix();

	glTranslatef(0, 0, 0);
	glRotatef(90, 1, 0, 0);
	glScalef(0.5, 0.5, 0.5);
	model_player.Draw();
	glPopMatrix();
	*/

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, WIDTH, HEIGHT, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Disable depth test so that hearts are always on top
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_LIGHT0);
	drawCustomCursor(mouseX, mouseY);

	// Draw the health hearts
	for (int i = 0; i < 5; i++) {
		glPushMatrix();
		glTranslatef(23, 13, 0);
		//glRotatef(90, 1, 0, 0);
		glScalef(5, 5, 5);
		drawHeart(0 + i* 10,0, 1, healthRem >= i + 1);
		glPopMatrix();
	}
	std::string timeRemaining = "Score: " + std::to_string(score);
	const char* timeRemainingChar = timeRemaining.c_str();
	showMessage(timeRemainingChar, 1420, 770);
	if (haveTorch)
		drawBattery(10, 30, 100, 50, 5); // Example usage, adjust x, y, width, height, and totalBars as needed
	glDisable(GL_LIGHT0);

	// Re-enable depth test
	glEnable(GL_DEPTH_TEST);

	// Restore the previous projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	glutSwapBuffers();
}


//=======================================================================
// More Functions
//=======================================================================

void checkForTorch() {
	if (room1 == true) {
		if (playerZRoom1 < 6.25 && playerZRoom1 >1.75 && playerXRoom1 <2 && playerXRoom1>-4.25) {
			if (torchPrompVis) {
				isTorchTaken = true;
				haveTorch = true;
				torchPrompVis = false;
			}
			else {
				torchPrompVis = true;
			}
		}
		else
			torchPrompVis = false;
	}
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{

	recentKeys.push_back(button);
	if (recentKeys.length() > 10) {  // Assuming max length of a cheat code is 10
		recentKeys.erase(recentKeys.begin());  // Keep only the most recent 10 keys
	}

	checkForCheatCodes(recentKeys);
	float d = 0.25;
	switch (button)
	{


	case 32 :
		if (isJumping == false && finishedJumping == false)
			isJumping = true;
		break;
	case 'c':
		if (currView == 0)
			currView = 1;
		else
			currView = 0;
		UpdateCameraPosition();
	case 'd':
		if (mainCorridor1 == true) {
			playerXMainCorr1 += d;
			playerAngle = 90;
			/*
					if (playerZMainCorr1 <= -14.75)
			*/
			if (playerXMainCorr1 >= 2.75) {
				if (playerZMainCorr1 <= -14.75) {
					mainCorridor2 = true;
					mainCorridor1 = false;
					playerAngle = 180;
					UpdateCameraPosition();
				}
				else
					playerXMainCorr1 -= d;
			}
		}
		else if (mainCorridor2 == true) {
			playerXMainCorr2 += d;
			playerAngle = 90;
			/*
					if (playerZMainCorr1 <= -14.75)
			*/
			if (playerXMainCorr2 >= 2.75) {
				if (playerZMainCorr2 <= -14.75) {
					mainCorridor2 = true;
					mainCorridor1 = false;
					playerAngle = 180;
					UpdateCameraPosition();
				}
				else
					playerXMainCorr2 -= d;
			}
		}

		else if (finalBoss == true) {
			playerXFinal += d;
			playerAngle = 90;
			/*
					if (playerZMainCorr1 <= -14.75)
			*/
			if (playerXFinal >= 4) {
					playerXFinal -= d;
			}
		}
		else if (room1 == true) {
			playerXRoom1 += d;
			if (playerXRoom1 > 3.75)
				playerXRoom1 -= d;

			/*
			
			playerX : -3.75
playerZ: 4.75
playerZ 2.5
x 1.75
			*/

			if (playerXRoom1 >= -3.75 && playerXRoom1 <= 1.75 && playerZRoom1 >= 2.5 && playerZRoom1 <= 4.75)
				playerXRoom1 -= d;
			playerAngle = 90;

		}

		else if (room2 == true) {
			playerXRoom2 += d;
			if (playerXRoom2 > 3.75)
				playerXRoom2 -= d;

			/*

			playerX : -3.75
playerZ: 4.75
playerZ 2.5
x 1.75
			*/

			if (playerXRoom2 >= -3.75 && playerXRoom2 <= 1.75 && playerZRoom2 >= 2.5 && playerZRoom2 <= 4.75)
				playerXRoom2 -= d;
			playerAngle = 90;

		}
		else if (room3 == true) {
			playerXRoom3 += d;
			if (playerXRoom3 > 3.75)
				playerXRoom3 -= d;

			/*

			playerX : -3.75
playerZ: 4.75
playerZ 2.5
x 1.75
			*/

			if (playerXRoom3 >= -3.75 && playerXRoom3 <= 1.75 && playerZRoom3 >= 2.5 && playerZRoom3 <= 4.75)
				playerXRoom3 -= d;
			playerAngle = 90;

		}
		break;
	case 'a':
		if (mainCorridor1 == true) {
			playerXMainCorr1 -= d;
			playerAngle = 270;
			if (playerXMainCorr1 <= -2.5)
				playerXMainCorr1 += d;

		}
		else if (finalBoss == true) {
			playerXFinal -= d;
			playerAngle = 270;
			if (playerXFinal <= -5.5)
				playerXFinal += d;

		}
		else if (mainCorridor2 == true) {
			playerXMainCorr2 -= d;
			playerAngle = 270;
			if (playerXMainCorr2 <= -2.5)
				playerXMainCorr2 += d;

		}

		else if (room1 == true) {
			playerXRoom1 -= d;
			if (playerXRoom1 <-5.5)
				playerXRoom1 += d;
			if (playerZRoom1 <= 5.5 && playerZRoom1 >= 2.75 && playerXRoom1 <= 1.5 && playerXRoom1 >= -3.75)
				playerXRoom1 += d;

				/*
				playerX : 1.500000
playerZ: 5.500000
				*/

			playerAngle = 270;
		}

		else if (room2 == true) {
			playerXRoom2 -= d;
			if (playerXRoom2 < -5.5)
				playerXRoom2 += d;
			if (playerZRoom2 <= 5.5 && playerZRoom2 >= 2.75 && playerXRoom2 <= 1.5 && playerXRoom2 >= -3.75)
				playerXRoom2 += d;

			/*
			playerX : 1.500000
playerZ: 5.500000
				*/

			playerAngle = 270;
		}

		else if (room3 == true) {
			playerXRoom3 -= d;
			if (playerXRoom3 < -5.5)
				playerXRoom3 += d;
			if (playerZRoom3 <= 5.5 && playerZRoom3 >= 2.75 && playerXRoom3 <= 1.5 && playerXRoom3 >= -3.75)
				playerXRoom3 += d;

			/*
			playerX : 1.500000
playerZ: 5.500000
				*/

			playerAngle = 270;
		}
		break;
	case 'w':
		if (mainCorridor1 == true) {
			printf("AA");
			playerZMainCorr1Prev = playerZMainCorr1;
			playerZMainCorr1 -= d;
			zTest -= d;
			if (playerZMainCorr1 <= -16.75) {
				playerZMainCorr1 += d;
				playerZMainCorr1Prev += d;
				zTest += d;

			}
		}
		else if (finalBoss == true) {
			printf("AA");
			playerZFinal -= d;
			zTest -= d;
			if (playerZFinal <= -6.75) {
				playerZFinal += d;
				zTest += d;

			}
		}
		else if (mainCorridor2 == true) {
			printf("AA");
			playerZMainCorr2Prev = playerZMainCorr1;
			playerZMainCorr2 -= d;
			zTest -= d;
			if (playerZMainCorr2 <= -16.75) {
				playerZMainCorr2 += d;
				playerZMainCorr2Prev += d;
				zTest += d;

			}
			if (playerZMainCorr2 <= -8.5) {
				playerZMainCorr2 += d;
				playerZMainCorr2Prev += d;
				labDoorPrompt = true;
			}
			else
				labDoorPrompt = false;
		}
		else if (room1 == true){
			playerZRoom1 -= d;
			if (playerZRoom1 < -6.75)
				playerZRoom1 += d;
			//playerX : 1.250000
			//playerZ: 5.500000
			//playerX : -3.500000
			//playerZ: 5.500000
			if (playerXRoom1 >= -3.5 && playerXRoom1 <= 1.25 && playerZRoom1 <= 5.5 && playerZRoom1 >= 3 )
				playerZRoom1 += d;

		}

		else if (room2 == true) {
			playerZRoom2 -= d;
			if (playerZRoom2 < -6.75)
				playerZRoom2 += d;
			//playerX : 1.250000
			//playerZ: 5.500000
			//playerX : -3.500000
			//playerZ: 5.500000
			if (playerXRoom2 >= -3.5 && playerXRoom2 <= 1.25 && playerZRoom2 <= 5.5 && playerZRoom2 >= 3)
				playerZRoom2 += d;

		}
		else if (room3 == true) {
			playerZRoom3 -= d;
			if (playerZRoom3 < -6.75)
				playerZRoom3 += d;
			//playerX : 1.250000
			//playerZ: 5.500000
			//playerX : -3.500000
			//playerZ: 5.500000
			if (playerXRoom3 >= -3.5 && playerXRoom3 <= 1.25 && playerZRoom3 <= 5.5 && playerZRoom3 >= 3)
				playerZRoom3 += d;

		}
		playerAngle = 180;
		break;
	case 's':
		if (mainCorridor1 == true) {
			playerZMainCorr1Prev = playerZMainCorr1;
			playerZMainCorr1 += d;
			playerAngle = 0;
			zTest += d;
			if (playerZMainCorr1 >= 19) {
				playerZMainCorr1 -= d;
				playerZMainCorr1Prev -= d;
				zTest -= d;
			}
		}
		else if (finalBoss == true) {
			//playerZFinalPrev = playerZFinal;
			playerZFinal += d;
			playerAngle = 0;
			zTest += d;
			if (playerZFinal >= 12) {
				playerZFinal -= d;
				//playerZFinalPrev -= d;
				zTest -= d;


			}
		}
		else if (mainCorridor2 == true) {
			playerZMainCorr2Prev = playerZMainCorr1;
			playerZMainCorr2 += d;
			playerAngle = 0;
			zTest += d;
			if (playerZMainCorr2 >= 19) {
				playerZMainCorr2 -= d;
				playerZMainCorr2Prev -= d;
				zTest -= d;
			}
		}
		else if (room1 == true) {

			/*
			
			playerX : 1.500000
playerZ: 3.250000
playerX : -3.500000
playerZ: 5.750000
			*/
			playerZRoom1 += d;
			if(playerZRoom1 > 19 || (playerXRoom1 >= -3.5 && playerXRoom1 <= 1.5 && playerZRoom1 >= 2.5 && playerZRoom1 <= 5.5))
				playerZRoom1 -= d;
			playerAngle = 0;

		}

		else if (room2 == true) {

			/*

			playerX : 1.500000
playerZ: 3.250000
playerX : -3.500000
playerZ: 5.750000
			*/
			playerZRoom2 += d;
			if (playerZRoom2 > 19 || (playerXRoom2 >= -3.5 && playerXRoom2 <= 1.5 && playerZRoom2 >= 2.5 && playerZRoom2 <= 5.5))
				playerZRoom2 -= d;
			playerAngle = 0;

		}
		else if (room3 == true) {

			/*

			playerX : 1.500000
playerZ: 3.250000
playerX : -3.500000
playerZ: 5.750000
			*/
			playerZRoom3 += d;
			if (playerZRoom3 > 19 || (playerXRoom3 >= -3.5 && playerXRoom3 <= 1.5 && playerZRoom3 >= 2.5 && playerZRoom3 <= 5.5))
				playerZRoom3 -= d;
			playerAngle = 0;

		}

		break;
	case 'y':
		camera.moveY(d);
		break;
	case 'h':
		camera.moveY(-d);
		break;
	case 'g':
		camera.moveX(d);
		break;
	case 'j':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;
	case 27:
		exit(0);
		break;
	case 't':
		if (haveTorch && remBattery > 0) {
			if (flashLight) {
				flashLight = false;
				glDisable(GL_LIGHT1);
			}
			else {
				flashLight = true;
				glEnable(GL_LIGHT1);
			}
		}
	default:
		break;
	}

	if (mainCorridor1 == true) {
		if (playerXMainCorr1 >= -2.5 && playerZMainCorr1 >= -1 && playerZMainCorr1 <= 0.25 && playerXMainCorr1 <= -2.25) {
			if (!room1EnterFlag) {
				room1EnterFlag = true;
			}
			else if (button == 'v' && room1EnterFlag) {
				room1EnterFlag = false;
				mainCorridor1 = false;
				room1 = true;
				playerAngle = 180;
				UpdateCameraPosition();
				return;
			}
		}
		else
			room1EnterFlag = false;

	}

	else if (finalBoss == true) {
		// 1.75 >> -1.25 z
// -0.75 >> 0.25 x

		if (playerXFinal >= -0.75 && playerXFinal <= 0.25 && playerZFinal >= -0.5 && playerZFinal <= 1.75) {
			if (!gunPrompt && !gunTaken)
				gunPrompt = true;
			else if (gunPrompt && button == 'm') {
				gunPrompt = false;
				gunTaken = true;
			}
		}
		else
			gunPrompt = false;
		// 5.250000 >> 4.25 , -2.25 >>> -1.5 x

		if (playerXFinal >= -3.00 && playerXFinal <= -1.75 && playerZFinal >= 4.0 && playerZFinal <= 5.5) {
			if (!finalHealthPrompt && !finalHealthTaken)
				finalHealthPrompt = true;
			else if (finalHealthPrompt && button == 'm') {
				finalHealthTaken = true;
				finalHealthPrompt = false;
				healthRem++;
			}
		}
		else
			finalHealthPrompt = false;


		//1.500000 >  -1  , 3.25 > 0.75 x
		if (playerXFinal >= 0.75 && playerXFinal <= 3.25 && playerZFinal >= -1 && playerZFinal <= 1.5) {
			if (!shieldTaken && !shieldPrompt)
				shieldPrompt = true;
			else if (shieldPrompt && button == 'm') {
				shieldPrompt = false;
				shieldTaken = true;
			}
		}
		else
			shieldPrompt = false;


		if (playerZFinal >= 11.75) {
			if (!returnCorr2Prompt) {
				returnCorr2Prompt = true;
			}
			else if (button == 'v') {
				mainCorridor2 = true;
				finalBoss = false;
			}
		}
		else
			returnCorr2Prompt = false;
		if (bossZ >= playerZFinal - 0.5 && bossZ <= playerZFinal + 0.5) {
			if (bossDead && !makhtutaPrompt && !makhtutaTaken)
				makhtutaPrompt = true;
			else if (makhtutaPrompt && button == 'm') {
				makhtutaPrompt = false;
				makhtutaTaken = true;
			}

		}
		else
			makhtutaPrompt = false;

	}

	else if (mainCorridor2 == true) {
		// playerX : -2.250000
		if (playerXMainCorr2 <= -2.25 && playerZMainCorr2 >= 9.25 && playerZMainCorr2 <= 10.5) {
			if (!room2EnterFlag) {
				room2EnterFlag = true;
			}
			else if (button == 'v' && room2EnterFlag) {
				room2EnterFlag = false;
				mainCorridor2 = false;
				room2 = true;
				playerAngle = 180;
				UpdateCameraPosition();
				return;
			}
		}
		else
			room2EnterFlag = false;

		/*playerX : 2.500000
playerZ: -0.250000*/
		if (playerXMainCorr2 >= 2.5 && playerZMainCorr2 >= -1.75 && playerZMainCorr2 <= -0.25) {
			if (!room3EnterFlag) {
				room3EnterFlag = true;
			}
			else if (button == 'v' && room3EnterFlag) {
				room3EnterFlag = false;
				mainCorridor2 = false;
				room3 = true;
				playerAngle = 180;
				UpdateCameraPosition();
				return;
			}
		}
		else
			room3EnterFlag = false;
	}

	else if (room1 == true) {
		if (playerZRoom1 < 6.25 && playerZRoom1 >1.75 && playerXRoom1 <2 && playerXRoom1>-4.25 && !isTorchTaken) {
			if (torchPrompVis && button == 'm') {
				isTorchTaken = true;
				haveTorch = true;
				torchPrompVis = false;
			}
			else {
				torchPrompVis = true;
			}
		}
		else
			torchPrompVis = false;

		if (playerZRoom1 >= 19 && playerXRoom1 >= -1.5 && playerXRoom1 <= 1.25)
			returnToMainCorr1 = true;
		else
			returnToMainCorr1 = false;

		if (returnToMainCorr1 == true && button == 'v') {
			room1 = false;
			mainCorridor1 = true;
			returnToMainCorr1 = false;
			playerAngle = 180;
			UpdateCameraPosition();
			playerAngle = 90;
		}
	}

	/*
		playerZRoom1 19
		playerX : -1.500000
		playerX : 1.250000
	*/
	else if (room2 == true) {
		if (playerZRoom2 < 6.25 && playerZRoom2 >1.75 && playerXRoom2 <2 && playerXRoom2>-4.25 && !isPotionTaken) {
			if (potionPrompVis && button == 'm') {
				isPotionTaken = true;
				healthRem = 5;
				potionPrompVis = false;
			}
			else {
				potionPrompVis = true;
			}
		}
		else
			potionPrompVis = false;


		if (playerZRoom2 >= 19 && playerXRoom2 >= -1.5 && playerXRoom2 <= 1.25)
			returnToMainCorr1 = true;
		else
			returnToMainCorr1 = false;

		if (returnToMainCorr1 == true && button == 'v') {
			room2 = false;
			mainCorridor2 = true;
			returnToMainCorr1 = false;
			playerAngle = 180;
			UpdateCameraPosition();
			playerAngle = 90;
		}

	}

	else if (room3 == true) {
		if (playerZRoom3 < 6.25 && playerZRoom3 >1.75 && playerXRoom3 <2 && playerXRoom3>-4.25 && !isKeyTaken) {
			if (potionPrompVis && button == 'm') {
				isKeyTaken = true;
				potionPrompVis = false;
			}
			else {
				potionPrompVis = true;
			}
		}
		else
			potionPrompVis = false;


		if (playerZRoom3 >= 19 && playerXRoom3 >= -1.5 && playerXRoom3 <= 1.25)
			returnToMainCorr1 = true;
		else
			returnToMainCorr1 = false;

		if (returnToMainCorr1 == true && button == 'v') {
			room3 = false;
			mainCorridor2 = true;
			returnToMainCorr1 = false;
			playerAngle = 180;
			UpdateCameraPosition();
			playerAngle = 90;
		}

	}

	if (button == 'w' || button == 's')
		UpdateCameraPosition();
	else if (button == 'a' || button == 'd') {
		if (currView == 0) {
			float currAngle = playerAngle;
			playerAngle = 180;
			UpdateCameraPosition();
			playerAngle = currAngle;
		}
		else
			UpdateCameraPosition();

	}
	if (labDoorPrompt && button == 'v' && (haveKey || isKeyTaken)) {
		finalBoss = true;
		mainCorridor2 = false;
		UpdateCameraPosition();
	}

	if (playerZMainCorr2 > -8.25)
		labDoorPrompt = false;

	glutPostRedisplay();
}


void checkForCheatCodes(const std::string& input) {
	if (input == "menrit") {
		printf("HELLO MENRITO");
		menrit = true;
		menritCheat = true;
		recentKeys.erase(recentKeys.begin());
	}
	else if (input == "IDKFA") {
		printf("NOOO");
	}
	else {
		printf("ALO");
	}
	// Add more cheat codes as needed
}


void onKeyPress(char key) {
	recentKeys.push_back(key);
	if (recentKeys.length() > 10) {  // Assuming max length of a cheat code is 10
		recentKeys.erase(recentKeys.begin());  // Keep only the most recent 10 keys
	}

	checkForCheatCodes(recentKeys);
}


Vector3f screenToWorld(int x, int y) {
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return Vector3f(posX, posY, posZ);
}




void mouseMove(int x, int y) {
	float xM = screenToWorld(x, y).x;
	float yM = screenToWorld(x, y).y;
	mouseX = x;
	mouseY = HEIGHT - y;
	// You may want to convert mouseY based on your coordinate system
	// For example: mouseY = windowHeight - y;
	glutPostRedisplay();
}



void drawCustomCursor(int x, int y) {
	const int cursorSize = 10; // Size of the cursor
	const int numSegments = 32; // Number of segments for the circle

	// Switch to orthographic projection to draw 2D overlay
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, WIDTH, 0, HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Set color based on the flag


	glColor3f(1.0f, 1.0, 1.0); // White otherwise
	// Draw the circle cursor
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < numSegments; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
		float dx = cursorSize * cosf(theta);
		float dy = cursorSize * sinf(theta);
		glVertex2f(x + dx, y + dy);
	}
	glEnd();

	glBegin(GL_POLYGON);
	for (int i = 0; i < numSegments; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
		float dx = cursorSize * cosf(theta);
		float dy = cursorSize * sinf(theta);
		glVertex2f(x + dx, y + dy);
	}
	glEnd();

	// Restore previous projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glColor3d(0.2, 0.2, 0.2);
}


void drawBattery(float x, float y, float width, float height, int totalBars) {
	// Draw battery outline
	glColor3f(0.0f, 0.0f, 0.0f); // Black color for battery border
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();



	float headWidth = width * 0.2f; 
	float headHeight = height * 0.15f; 
	glBegin(GL_QUADS);

	glVertex2f(x + width, y + (height - headWidth) / 2);
	glVertex2f(x + width, y + (height + headWidth) / 2);
	glVertex2f(x + (width + headHeight), y + (height + headWidth) / 2);
	glVertex2f(x + (width + headHeight), y +(height - headWidth) / 2);
	glEnd();

	// Calculate the width of each bar
	float barWidth = width / totalBars - 2; // -2 for some spacing between bars
	float barHeight = height - 4; // -4 for spacing from the top and bottom border

	// Draw filled bars based on remBattery
	for (int i = 0; i < remBattery; i++) {
		float barX = x + 2 + i * (width / totalBars);
		float barY = y + 2;
		if (remBattery > 3)
			glColor3f(0.0f, 1.0f, 0.0f); // Green color for battery bars
		else if (remBattery >= 2)
			glColor3f(1, 1, 0);
		else
			glColor3f(1, 0, 0);
		glBegin(GL_QUADS);
		glVertex2f(barX, barY);
		glVertex2f(barX + barWidth, barY);
		glVertex2f(barX + barWidth, barY + barHeight);
		glVertex2f(barX, barY + barHeight);
		glEnd();
	}
	glColor3f(1, 1, 1);
}



void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		camera.rotateX(a);
		break;
	case GLUT_KEY_DOWN:
		camera.rotateX(-a);
		break;
	case GLUT_KEY_LEFT:
		camera.rotateY(a);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotateY(-a);
		break;
	}

	glutPostRedisplay();
}

//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	//GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}



void onMouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (reloading)
			return;
		bulletsFired++;
		if (bulletsFired >= 5) {
			reloading = true;
			Reload.play();
			bulletsFired = 0;
		}
		printf("HELLO");
		Vector3f worldPos = screenToWorld(x, y);
		Vector3f bulletDirection = (worldPos - camera.eye).unit(); // Direction from camera to clicked point
		bulletFire.play();
		// Create a bullet at the camera position
		Bullet newBullet(camera.eye, bulletDirection, bulletSpeed);
		bullets.push_back(newBullet); // Assuming you have a list of bullets
	}
}

//=======================================================================
// Assets Loading Function
//=======================================================================


int flashLightCtr = 0;
void update(int value) {
	if (menrit) {
		cheatCtr++;
		if (cheatCtr == 360)
			menrit = false;
	}
	float deltaTime = 0.016; // Assuming 60 updates per second

	if (reloading) {
		reloadCtr++;
		if (reloadCtr >= 90) {
			reloading = false;
			reloadCtr = 0;
		}
	}
	if (flashLight) {
		flashLightCtr++;

		if (flashLightCtr % 180 == 0)
			remBattery--;
		if (remBattery == 0) {
			glDisable(GL_LIGHT1);
			flashLight = false;
		}
	}

	if (makhtutaTaken) {
		makhtutaCtr++;
		if (makhtutaCtr <= 600)
			makhtutaSecret = true;
		else {
			makhtutaSecret = false;
			finalBoss = false;
			gameWin = true;
		}
	}
	if (!bossDead) {
		if (bossAngle == 0) {
			bossZ += 0.05;
			if (bossZ >= 14)
				bossAngle = 180;
		}
		if (bossAngle == 180) {
			bossZ -= 0.05;
			if (bossZ <= -6)
				bossAngle = 0;
		}
	}
	
	if (healthRem <= 0) {
		mainCorridor1 = false;
		mainCorridor2 = false;
		room1 = false;
		room2 = false;
		room3 = false;
		finalBoss = false;
		gameLoss = true;
	}

	if (isJumping || finishedJumping) {
		// edit rest of rooms
		jumpCtr++;
		if (isJumping) {
			playerYMainCorr1 += 0.05;
			playerYMainCorr2 += 0.05;
			playerYRoom1 += 0.05;
			playerYRoom2 += 0.05;
			playerYRoom3 += 0.05;
			playerYFinal += 0.05;
		}

		else {
			playerYMainCorr1 -= 0.05;
			playerYMainCorr2 -= 0.05;
			playerYRoom1 -= 0.05;
			playerYRoom2 -= 0.05;
			playerYRoom3 -= 0.05;
			playerYFinal -= 0.05;
		}

		if (jumpCtr == 10) {
			isJumping = false;
			finishedJumping = true;
		}

		if (jumpCtr == 20) {
			finishedJumping = false;
			jumpCtr = 0;
		}


	}
	// Update each bullet
	for (auto& bullet : bullets) {
		bullet.update(deltaTime);
	}


	for (auto& zombie : zombies) {
		zombie.update();
	}

	// Remove bullets that have gone off-screen or collided

	// Redraw the scene
	glutPostRedisplay();

	// Set up the next timer call
	glutTimerFunc(16, update, 0); // 16 milliseconds ~ 60 FPS
}

void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/zombie4/hanging.3ds");
	model_tree.Load("Models/bullet/Bullet/bullet.3ds");
	model_zombie.Load("models/zombie3/zombie_farmer.3ds");
	model_player.Load("models/player1/player.3ds");
	model_boss.Load("models/finalBoss1/finalBoss.3ds");
	model_key.Load("models/Key/LabKey.3ds");
	model_window.Load("models/window/window.3ds");
	model_door.Load("models/door/door.3ds");
	model_table.Load("models/table/table.3ds");
	model_wall.Load("models/wall/wall.3ds");
	model_torch.Load("models/flashlight/torch.3ds");
	model_labDoor.Load("models/labDoor/labDoor.3ds");
	model_potion.Load("models/potion/potion.3ds");
	model_arena.Load("models/finalBossArena/arena.3ds");
	model_moreDamage.Load("models/moreDamage/gun.3ds");
	model_shield.Load("models/shield/shield.3ds");
	model_makhtuta.Load("models/makhtuta/makhtuta.3ds");
	model_bandits.Load("models/bandits/bandits.3ds");
	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);

	tex_finalWall.Load("Textures/finalWall.bmp");
	tex_FinalGround.Load("Textures/groundFinal.bmp");
	tex_ceiling.Load("Textures/wall.bmp");
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	srand(static_cast<unsigned int>(time(nullptr)));


	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(0, 0);

	glutCreateWindow(title);

	initZombies();

	/*
	eyex 0.266057
eyeY 2.739923
eyeZ 23.463078
centerX 0.270441
centerY 2.701188
centerZ 22.463837
upX 0.000170
upY 0.999250
upZ -0.038734
	*/
	bulletFire_buffer.loadFromFile("bullet.wav");

	bulletFire.setBuffer(bulletFire_buffer);
	bulletFire.setPitch(1.0f);
	bulletFire.setVolume(10.0f);

	reload_buffer.loadFromFile("Gun Reload sound effect.wav");
	zombieScream_buffer.loadFromFile("scream zombie.wav");
	playerHit_buffer.loadFromFile("playerHit.wav");
	background_buffer.loadFromFile("background.wav");

	backgroundS.setBuffer(background_buffer);
	backgroundS.setPitch(1.0f);
	backgroundS.setVolume(12.0f);
	backgroundS.setLoop(true);
	backgroundS.play();
	
	Reload.setBuffer(reload_buffer);
	Reload.setPitch(1.0f);
	Reload.setVolume(10.0f);
	

	playerHit.setBuffer(playerHit_buffer);
	playerHit.setPitch(1.0f);
	playerHit.setVolume(20.0f);


	zombieScream.setBuffer(zombieScream_buffer);
	zombieScream.setPitch(1.0f);
	zombieScream.setVolume(10.0f);

	camera.eye = { 0.258790  , 3.120505 , 25.120016 };
	camera.center = { 0.263176  ,3.116668, 24.120033 };
	camera.up = { 0.000017  ,0.999993, -0.003837 };

	UpdateCameraPosition();
	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(Special);
	glutPassiveMotionFunc(mouseMove);  // Register mouse move callback
	glutSetCursor(GLUT_CURSOR_NONE);   // Hide default cursor

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);
	glutMouseFunc(onMouseClick);
	glutTimerFunc(16, update, 0);

	glutReshapeFunc(myReshape);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	//InitLightSource();

	//InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);


	LoadAssets();
	glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);	
	//glEnable(GL_LIGHT0);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}