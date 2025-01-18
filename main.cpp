#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

// Scene 1 Code
float cloudOffset = 0.0f;
float birdOffset = 0.0f;
float submarineOffset = 0.0f;
float grassWaveOffset = 0.0f;
float waveOffset = 0.0f; // Offset for wave animation
float humanOffset = -1.0f; // Initial position of the human
bool humanWalking = true; // Flag to control walking
bool humanJumping = false; // Flag to control jumping
float jumpHeight = 0.0f; // Height of the jump
float jumpAngle = 0.0f; // Angle for the jump
bool humanInSubmarine = false; // Flag to indicate if the human is in the submarine
float propellerAngle = 0.0f; // Angle for propeller animation
float sunAngle = 1.5707963f; // Angle for the sun's position (set to a fixed value)
float sunSize = 0.1f; // Size of the sun
bool isDayMode = true; // Flag to track day/night mode
bool isSunSetting = false; // Flag to track if the sun is setting
bool isSubmarineSubmerging = false; // Flag to track if the submarine is submerging
bool isSubmarineMoving = false; // Flag to control submarine movement

// Seed the random number generator
void initRandom() {
    std::srand(std::time(0));
}

// Generate a random float between min and max
float randomFloat(float min, float max) {
    return min + static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void drawRectangle(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void drawWaves() {
    glColor3f(0.0f, 0.4f, 0.7f); // Darker blue for waves
    glBegin(GL_QUADS);
    for (float x = -1.0f; x <= 1.0f; x += 0.01f) {
        float y1 = 0.03f * sinf(15 * x + waveOffset) + 0.02f * sinf(30 * x + 2 * waveOffset);
        float y2 = 0.02f * sinf(15 * x + waveOffset + 3.1415926f / 2) + 0.01f * sinf(30 * x + 2 * waveOffset + 3.1415926f / 2);
        glVertex2f(x, y1 - 0.4f);
        glVertex2f(x, -1.0f);
        glVertex2f(x + 0.01f, -1.0f);
        glVertex2f(x + 0.01f, y1 - 0.4f);
    }
    glEnd();

    glBegin(GL_QUADS);
    for (float x = -1.0f; x <= 1.0f; x += 0.01f) {
        float y1 = 0.02f * sinf(15 * x + waveOffset + 3.1415926f / 2) + 0.01f * sinf(30 * x + 2 * waveOffset + 3.1415926f / 2);
        float y2 = 0.01f * sinf(15 * x + waveOffset) + 0.005f * sinf(30 * x + 2 * waveOffset);
        glVertex2f(x, y1 - 0.42f);
        glVertex2f(x, -1.0f);
        glVertex2f(x + 0.01f, -1.0f);
        glVertex2f(x + 0.01f, y1 - 0.42f);
    }
    glEnd();
}

void drawDetailedMountains() {
    // Base layer
    glColor3f(0.0f, 0.6f, 0.0f); // Vibrant green
    glBegin(GL_POLYGON);
    for (float x = -1.0f; x <= 1.0f; x += 0.01f) {
        float y = 0.2f * sinf(3.0f * 3.1415926f * x) + 0.1f * sinf(6.0f * 3.1415926f * x);
        glVertex2f(x, y + 0.2f);
    }
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    // Second layer with more details
    glColor3f(0.0f, 0.8f, 0.0f); // More vibrant green
    glBegin(GL_POLYGON);
    for (float x = -1.0f; x <= 1.0f; x += 0.01f) {
        float y = 0.15f * sinf(4.0f * 3.1415926f * x) + 0.08f * sinf(8.0f * 3.1415926f * x);
        glVertex2f(x, y + 0.25f);
    }
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    // Third layer with even more details
    glColor3f(0.0f, 1.0f, 0.0f); // Even more vibrant green
    glBegin(GL_POLYGON);
    for (float x = -1.0f; x <= 1.0f; x += 0.01f) {
        float y = 0.1f * sinf(5.0f * 3.1415926f * x) + 0.05f * sinf(10.0f * 3.1415926f * x);
        glVertex2f(x, y + 0.3f);
    }
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    // Fourth layer with even more details
    glColor3f(0.2f, 1.0f, 0.2f); // Lighter vibrant green
    glBegin(GL_POLYGON);
    for (float x = -1.0f; x <= 1.0f; x += 0.01f) {
        float y = 0.07f * sinf(6.0f * 3.1415926f * x) + 0.03f * sinf(12.0f * 3.1415926f * x);
        glVertex2f(x, y + 0.35f);
    }
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();
}

void drawWindTurbine(float x, float y, float scale) {
    static float propellerAngle = 0.0f; // Propeller rotation angle
    static float towerWidth = 0.02f + 0.01f * sinf(propellerAngle * 0.05f); // Dynamic width for the tower
    float bladeLength = 0.1f * scale; // Blade length (dynamic based on scale)

    // Draw Tower (center part) - slim and dynamic width
    glColor3f(0.8f, 0.8f, 0.8f); // Gray color for the tower
    glBegin(GL_POLYGON);
    glVertex2f(x - towerWidth, y);               // Bottom left
    glVertex2f(x + towerWidth, y);               // Bottom right
    glVertex2f(x + towerWidth, y + 0.3f * scale); // Top right
    glVertex2f(x - towerWidth, y + 0.3f * scale); // Top left
    glEnd();

    // Add purple color for the base of the tower
    glColor3f(0.5f, 0.0f, 0.5f); // Purple color for base
    glBegin(GL_POLYGON);
    glVertex2f(x - towerWidth - 0.01f, y - 0.05f);    // Bottom left of the base
    glVertex2f(x + towerWidth + 0.01f, y - 0.05f);    // Bottom right of the base
    glVertex2f(x + towerWidth + 0.01f, y);            // Top right of the base
    glVertex2f(x - towerWidth - 0.01f, y);            // Top left of the base
    glEnd();

    // Draw Blades (using glVertex2f for each point on the blades)
    glColor3f(1.0f, 1.0f, 1.0f); // White color for blades
    glPushMatrix();
    glTranslatef(x, y + 0.3f * scale, 0.0f); // Move to the top of the tower

    for (int i = 0; i < 3; i++) {
        glPushMatrix();
        glRotatef(propellerAngle + i * 120.0f, 0.0f, 0.0f, 1.0f); // Rotate each blade by 120 degrees

        glBegin(GL_POLYGON);  // Draw blade as a triangle
        glVertex2f(0.0f, 0.0f); // Center of rotation
        glVertex2f(bladeLength, 0.02f);  // Tip of the blade
        glVertex2f(bladeLength, -0.02f); // Bottom of the blade
        glEnd();

        glPopMatrix();
    }

    glPopMatrix();

    // Update propeller angle for animation
    propellerAngle += 0.6f; // Adjust the speed of rotation
    if (propellerAngle >= 360.0f) {
        propellerAngle = 0.0f; // Reset angle to keep it in bounds
    }
}

void drawTree(float x, float y, float height) {
    // Adjust the height to create medium-sized trees
    height *= 0.8f; // Scale the height to 80% of the original

    // Trunk
    glColor3f(0.55f, 0.27f, 0.07f); // Brown color for trunk
    float trunkWidth = 0.02f; // Slimmer trunk width
    glBegin(GL_QUADS);
    glVertex2f(x - trunkWidth, y);                      // Bottom left
    glVertex2f(x + trunkWidth, y);                      // Bottom right
    glVertex2f(x + trunkWidth, y + height * 0.6f);      // Top right, increased trunk height
    glVertex2f(x - trunkWidth, y + height * 0.6f);      // Top left, increased trunk height
    glEnd();

    // Canopy
    glColor3f(0.0f, 0.8f, 0.0f); // Green color for foliage
    float foliageHeight = y + height * 0.6f;    // Start of foliage, adjusted for longer trunk

    // Main foliage circle (medium size)
    float foliageRadius = height * 0.2f; // Medium foliage radius based on new height
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, foliageHeight + foliageRadius * 0.3f);  // Center of top foliage
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x + cos(angle) * foliageRadius, foliageHeight + foliageRadius * 0.3f + sin(angle) * foliageRadius);
    }
    glEnd();

    // Additional foliage circles to give a more rounded and full appearance
    float additionalFoliageRadius1 = height * 0.15f; // Slightly smaller additional foliage radius
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x - height * 0.05f, foliageHeight + height * 0.15f);
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x - height * 0.05f + cos(angle) * additionalFoliageRadius1, foliageHeight + height * 0.15f + sin(angle) * additionalFoliageRadius1);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + height * 0.05f, foliageHeight + height * 0.15f);
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x + height * 0.05f + cos(angle) * additionalFoliageRadius1, foliageHeight + height * 0.15f + sin(angle) * additionalFoliageRadius1);
    }
    glEnd();

    float additionalFoliageRadius2 = height * 0.1f; // Even smaller additional foliage radius
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, foliageHeight + height * 0.2f);
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x + cos(angle) * additionalFoliageRadius2, foliageHeight + height * 0.2f + sin(angle) * additionalFoliageRadius2);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, foliageHeight + height * 0.25f);
    for (int i = 0; i <= 360; i++) {
        float angle = i * M_PI / 180.0f;
        glVertex2f(x + cos(angle) * additionalFoliageRadius2, foliageHeight + height * 0.25f + sin(angle) * additionalFoliageRadius2);
    }
    glEnd();
}

void drawWoodenHome(float x, float y, float scale) {
    // Main structure of the home
    glColor3f(0.6f, 0.3f, 0.1f); // Brown color for the walls
    glBegin(GL_QUADS);
    glVertex2f(x - 0.1f * scale, y);               // Bottom left
    glVertex2f(x + 0.1f * scale, y);               // Bottom right
    glVertex2f(x + 0.1f * scale, y + 0.15f * scale); // Top right
    glVertex2f(x - 0.1f * scale, y + 0.15f * scale); // Top left
    glEnd();

    // Roof
    glColor3f(0.8f, 0.4f, 0.0f); // Darker brown color for the roof
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.12f * scale, y + 0.15f * scale); // Left bottom
    glVertex2f(x + 0.12f * scale, y + 0.15f * scale); // Right bottom
    glVertex2f(x, y + 0.25f * scale);                // Top
    glEnd();

    // Door
    glColor3f(0.3f, 0.1f, 0.0f); // Dark brown color for the door
    glBegin(GL_QUADS);
    glVertex2f(x - 0.03f * scale, y);               // Bottom left
    glVertex2f(x + 0.03f * scale, y);               // Bottom right
    glVertex2f(x + 0.03f * scale, y + 0.08f * scale); // Top right
    glVertex2f(x - 0.03f * scale, y + 0.08f * scale); // Top left
    glEnd();

    // Window
    glColor3f(0.8f, 0.8f, 1.0f); // Light blue color for the window
    glBegin(GL_QUADS);
    glVertex2f(x + 0.05f * scale, y + 0.05f * scale); // Bottom left
    glVertex2f(x + 0.08f * scale, y + 0.05f * scale); // Bottom right
    glVertex2f(x + 0.08f * scale, y + 0.08f * scale); // Top right
    glVertex2f(x + 0.05f * scale, y + 0.08f * scale); // Top left
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f); // Black color for the window frame
    glBegin(GL_LINES);
    glVertex2f(x + 0.05f * scale, y + 0.05f * scale); // Bottom left
    glVertex2f(x + 0.08f * scale, y + 0.05f * scale); // Bottom right
    glVertex2f(x + 0.08f * scale, y + 0.05f * scale); // Bottom right
    glVertex2f(x + 0.08f * scale, y + 0.08f * scale); // Top right
    glVertex2f(x + 0.08f * scale, y + 0.08f * scale); // Top right
    glVertex2f(x + 0.05f * scale, y + 0.08f * scale); // Top left
    glVertex2f(x + 0.05f * scale, y + 0.08f * scale); // Top left
    glVertex2f(x + 0.05f * scale, y + 0.05f * scale); // Bottom left
    glEnd();
}

void drawMountainsWithTrees() {
    drawDetailedMountains();

    // Draw the first Wind Turbine (normal size) close to the left corner
    drawWindTurbine(-0.80f, 0.2f, 1.0f);

    // Draw the second Wind Turbine (smaller) beside the first one
    drawWindTurbine(-0.65f, 0.2f, 0.7f);

    // Adjust the positions of the remaining trees to ensure they are well-spaced and within the frame
    // Left big tree

    // Adjust the positions of the remaining trees to ensure they are well-spaced and within the frame
    drawTree(0.64f, 0.155f, 0.5f);  // Second small tree (larger, spaced)
    drawTree(0.87f, 0.18f, 0.45f);// Third small tree (medium-sized)
    drawWoodenHome(0.7f, 0.1f, 1.0f);
    // Add bushes and flowers around the trees

}

void drawDock() {
    // Dock base
    glColor3f(0.4f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.5f);
    glVertex2f(-0.2f, -0.5f);
    glVertex2f(-0.2f, -0.4f);
    glVertex2f(-1.0f, -0.4f);
    glEnd();

    // Dock planks
    for (float x = -1.0f; x < -0.2f; x += 0.15f) {
        glColor3f(0.5f, 0.3f, 0.1f);
        drawRectangle(x, -0.4f, x + 0.1f, -0.35f);
    }

    // Dock supports extended to ground bottom
    glColor3f(0.3f, 0.15f, 0.05f);
    for (float x = -1.0f; x < -0.2f; x += 0.15f) {
        drawRectangle(x + 0.03f, -0.5f, x + 0.07f, -1.0f);
    }
}

// Function to draw a partial circle
void drawPartialCircle(float cx, float cy, float r, int start_angle, int end_angle, int num_segments) {
    glBegin(GL_POLYGON);
    for (int i = start_angle; i <= end_angle; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / 360.0f; // Convert angle to radians
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

// Function to draw the propeller
void drawPropeller(float cx, float cy, float r, float angle) {
    glColor3f(1.0f, 0.6f, 0.0f); // Orange
    for (int i = 0; i < 3; i++) { // Draw 3 blades
        float bladeAngle = angle + i * 120.0f; // 120-degree separation
        float theta = bladeAngle * 3.1415926f / 180.0f; // Convert to radians
        glBegin(GL_TRIANGLES);
        glVertex2f(cx, cy); // Center
        glVertex2f(cx + r * cosf(theta), cy + r * sinf(theta)); // Tip of the blade
        glVertex2f(cx + 0.05f * cosf(theta + 1.5708f), cy + 0.05f * sinf(theta + 1.5708f)); // Width of the blade
        glEnd();
    }
}

void drawSubmarine() {
    glPushMatrix();
    glTranslatef(0.3f + submarineOffset, -0.4f, 0.0f); // Adjust the position of the submarine to be lower and move to the right
    glScalef(0.3f, 0.3f, 1.0f); // Scale the submarine to make it bigger

    // Submarine body
    glColor3f(1.0f, 0.6f, 0.0f); // Orange
    glBegin(GL_POLYGON);
    glVertex2f(-0.6f, -0.3f); // Bottom-left corner
    glVertex2f(0.2f, -0.3f);  // Bottom-right straight edge
    glVertex2f(0.2f, 0.3f);   // Top-right straight edge
    glVertex2f(-0.6f, 0.3f);  // Top-left corner
    glVertex2f(-0.7f, 0.0f);  // Left-most tail
    glEnd();

    // Wide rounded front (right side)
    glColor3f(1.0f, 0.6f, 0.0f); // Orange
    drawPartialCircle(0.2f, 0.0f, 0.3f, 0, 360, 50); // Full circle on the right

    // Windows
    glColor3f(0.0f, 0.2f, 0.6f); // Blue border
    drawPartialCircle(-0.25f, 0.0f, 0.14f, 0, 360, 50); // Left window border
    drawPartialCircle(0.1f, 0.0f, 0.15f, 0, 360, 50);   // Right window border
    glColor3f(0.6f, 0.8f, 1.0f); // Light blue inner
    drawPartialCircle(-0.25f, 0.0f, 0.12f, 0, 360, 50); // Left window inner
    drawPartialCircle(0.1f, 0.0f, 0.12f, 0, 360, 50);   // Right window inner

    // Periscope base
    glColor3f(0.0f, 0.2f, 0.6f); // Blue
    glBegin(GL_POLYGON);
    glVertex2f(-0.05f, 0.3f);
    glVertex2f(0.05f, 0.3f);
    glVertex2f(0.05f, 0.55f);
    glVertex2f(-0.05f, 0.55f);
    glEnd();

    // Periscope top arm
    glColor3f(0.0f, 0.2f, 0.6f); // Blue
    glBegin(GL_POLYGON);
    glVertex2f(0.05f, 0.5f);
    glVertex2f(0.15f, 0.5f);
    glVertex2f(0.15f, 0.55f);
    glVertex2f(0.05f, 0.55f);
    glEnd();

    // Periscope viewfinder
    glColor3f(1.0f, 0.6f, 0.0f); // Orange
    glBegin(GL_POLYGON);
    glVertex2f(0.15f, 0.48f);
    glVertex2f(0.2f, 0.48f);
    glVertex2f(0.2f, 0.57f);
    glVertex2f(0.15f, 0.57f);
    glEnd();

    // Tail
    glColor3f(0.0f, 0.2f, 0.6f); // Blue tail
    glBegin(GL_POLYGON);
    glVertex2f(-0.7f, 0.1f);
    glVertex2f(-0.9f, 0.0f);
    glVertex2f(-0.7f, -0.1f);
    glEnd();

    // Propeller blades with rotation
    drawPropeller(-0.9f, 0.0f, 0.15f, propellerAngle);

    glPopMatrix();
}

void drawHumanSitting(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(0.8f, 0.8f, 1.0f); // Scale down the human to fit on the submarine

    // Head
    glColor3f(0.8f, 0.6f, 0.4f); // Brown skin color
    drawCircle(0.0f, 0.08f, 0.04f, 50);

    // Body (sitting)
    glColor3f(0.8f, 0.6f, 0.4f); // Brown skin color
    drawRectangle(-0.03f, 0.0f, 0.03f, 0.06f);

    // Arms (sitting)
    glColor3f(0.8f, 0.6f, 0.4f); // Brown skin color
    drawRectangle(-0.06f, 0.03f, -0.03f, 0.06f);
    drawRectangle(0.03f, 0.03f, 0.06f, 0.06f);

    // Legs (sitting)
    glColor3f(1.0f, 1.0f, 1.0f); // White color for legs
    drawRectangle(-0.03f, -0.06f, -0.01f, 0.0f);
    drawRectangle(0.01f, -0.06f, 0.03f, 0.0f);

    // Feet (sitting)
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for shoes
    drawRectangle(-0.04f, -0.08f, -0.01f, -0.06f);
    drawRectangle(0.01f, -0.08f, 0.04f, -0.06f);

    glPopMatrix();
}

void drawHumanWalking(float x, float y) {
    // Head
    glColor3f(0.8f, 0.6f, 0.4f); // Brown skin color
    drawCircle(x, y + 0.1f, 0.05f, 50);

    // Eyes
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(x - 0.02f, y + 0.11f, 0.01f, 50);
    drawCircle(x + 0.02f, y + 0.11f, 0.01f, 50);

    // Nose
    glColor3f(0.8f, 0.6f, 0.4f); // Brown skin color
    drawCircle(x, y + 0.09f, 0.005f, 50);

    // Mouth
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.015f, y + 0.08f);
    glVertex2f(x + 0.015f, y + 0.08f);
    glEnd();

    // Body (slimmed down)
    glColor3f(0.8f, 0.6f, 0.4f); // Brown skin color
    drawRectangle(x - 0.03f, y + 0.05f, x + 0.03f, y);

    // Arms
    glColor3f(0.8f, 0.6f, 0.4f); // Brown skin color
    drawRectangle(x - 0.06f, y + 0.05f, x - 0.03f, y + 0.025f);
    drawRectangle(x + 0.03f, y + 0.05f, x + 0.06f, y + 0.025f);

    // Hands
    glColor3f(0.8f, 0.6f, 0.4f); // Brown skin color
    drawCircle(x - 0.06f, y + 0.025f, 0.02f, 50);
    drawCircle(x + 0.06f, y + 0.025f, 0.02f, 50);

    // Legs
    glColor3f(1.0f, 1.0f, 1.0f); // White color for legs
    drawRectangle(x - 0.03f, y - 0.1f, x - 0.01f, y);
    drawRectangle(x + 0.01f, y - 0.1f, x + 0.03f, y);

    // Feet
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for shoes
    drawRectangle(x - 0.04f, y - 0.12f, x - 0.01f, y - 0.1f);
    drawRectangle(x + 0.01f, y - 0.12f, x + 0.04f, y - 0.1f);
}

void drawSoilGround() {
    // Draw a gradient soil ground
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.3f, 0.1f); // Darker brown at the bottom
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glColor3f(0.7f, 0.4f, 0.2f); // Lighter brown at the top
    glVertex2f(1.0f, -0.6f);
    glVertex2f(-1.0f, -0.6f);
    glEnd();

    // Draw grass on the soil ground
    glColor3f(0.0f, 0.8f, 0.0f); // Green color for grass
    for (float x = -1.0f; x <= 1.0f; x += 0.02f) {
        float grassHeight = 0.02f + 0.01f * sinf(grassWaveOffset * 10.0f + x * 10.0f);
        drawRectangle(x, -0.6f, x + 0.01f, -0.6f + grassHeight);
    }
}

void drawWater() {
    glColor3f(0.0f, 0.3f, 0.7f);  // Water blue color
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.6f);   // Bottom left corner
    glVertex2f(1.0f, -0.6f);    // Bottom right corner
    glVertex2f(1.0f, -0.4f);    // Top right corner (closer to the bottom)
    glVertex2f(-1.0f, -0.4f);   // Top left corner (closer to the bottom)
    glEnd();
}

void drawSunAndMoon() {
    float sunY = isDayMode ? 0.7f : -0.7f;  // Sun's vertical position
    float moonY = isDayMode ? -0.7f : 0.7f; // Moon's vertical position

    // Sun's color changes for sunrise/sunset
    float red = 1.0f, green = 0.9f, blue = 0.0f;
    if (isDayMode && sunY < 0.1f) {
        red = 1.0f;
        green = 0.5f;
        blue = 0.0f;
    }

    // Draw the sun
    glColor3f(red, green, blue);
    drawCircle(0.0f, sunY, sunSize, 50);

    // Draw the moon
    glColor3f(0.9f, 0.9f, 1.0f);
    drawCircle(0.0f, moonY, sunSize * 0.8f, 50);
}

void drawMoon() {
    // Calculate the moon's position (opposite side of the sun)
    float moonX = -0.7f * cosf(sunAngle); // Mirror sun's X position
    float moonY = -0.7f * sinf(sunAngle) + 0.2f; // Mirror sun's Y position

    // Set moon color to a pale white/gray
    glColor3f(0.9f, 0.9f, 1.0f);
    drawCircle(moonX, moonY, sunSize * 0.8f, 50); // Moon size slightly smaller than sun
}

void drawStars() {
    if (!isDayMode) {
        glColor3f(1.0f, 1.0f, 1.0f); // White stars
        glBegin(GL_POINTS);
        for (int i = 0; i < 100; ++i) { // Number of stars
            float x = randomFloat(-1.0f, 1.0f); // Random x position
            float y = randomFloat(0.0f, 1.0f);  // Random y position (upper half of the scene)
            glVertex2f(x, y);
        }
        glEnd();
    }
}

void drawCloud(float cx, float cy) {
    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircle(cx, cy, 0.05f, 50);
    drawCircle(cx + 0.06f, cy, 0.06f, 50);
    drawCircle(cx + 0.12f, cy, 0.05f, 50);
    drawCircle(cx + 0.08f, cy + 0.04f, 0.04f, 50);
}

void drawClouds() {
    if (isDayMode) {
        glPushMatrix();
        glTranslatef(cloudOffset, 0.0f, 0.0f);
        drawCloud(-0.8f, 0.8f);
        drawCloud(-0.3f, 0.9f);
        drawCloud(0.1f, 0.85f);
        drawCloud(0.6f, 0.75f);
        glPopMatrix();
    }
}

void renderScene1() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Sky color based on day/night mode
    float skyRed = 0.5f, skyGreen = 0.8f, skyBlue = 0.92f; // Day sky color
    if (!isDayMode) {
        // Night sky color
        skyRed = 0.0f;
        skyGreen = 0.0f;
        skyBlue = 0.2f;
    }

    glColor3f(skyRed, skyGreen, skyBlue);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    // Draw the sun and moon
    drawSunAndMoon();

    // Other scene elements
    drawMountainsWithTrees();
    drawWater();
    drawWaves();
    drawSoilGround();
    drawDock();
    drawSubmarine();
    drawClouds();

    // Human on the dock or in the submarine
    if (humanInSubmarine) {
        drawHumanSitting(0.3f + submarineOffset, -0.35f);
    } else {
        drawHumanWalking(humanOffset, -0.3f + jumpHeight);
    }

    // Stars at night
    if (!isDayMode) {
        drawStars();
    }

    glutSwapBuffers();
}

void updateScene1(int value) {
    waveOffset += 0.01f;
    propellerAngle += 5.0f;
    cloudOffset += 0.001f;

    if (cloudOffset > 1.5f) cloudOffset = -1.5f;

    // Removed birdOffset update

    grassWaveOffset += 0.01f;
    if (grassWaveOffset > 1.0f) grassWaveOffset = -1.0f;

    waveOffset += 0.02f; // Update wave offset for animation
    if (waveOffset > 2.0f * 3.1415926f) waveOffset = 0.0f;

    propellerAngle += 5.0f; // Update propeller angle for animation
    if (propellerAngle > 360.0f) propellerAngle = 0.0f;

    if (humanWalking) {
        humanOffset += 0.005f; // Update human position, slowed down
        if (humanOffset > -0.2f) { // Stop at the end of the dock
            humanWalking = false;
            humanJumping = true; // Start jumping
        }
    }

    if (humanJumping) {
        jumpAngle += 0.1f; // Update jump angle
        jumpHeight = 0.1f * sinf(jumpAngle); // Calculate jump height
        if (jumpAngle > 3.1415926f) { // Stop jumping when the jump is complete
            humanJumping = false;
            jumpAngle = 0.0f;
            jumpHeight = 0.0f;
            humanInSubmarine = true; // Human is now in the submarine
        }
    }

    if (isSunSetting) {
        sunAngle -= 0.01f; // Decrease the sun angle to simulate setting
        if (sunAngle < -1.5707963f) { // Stop when the sun is below the horizon
            isSunSetting = false;
            isDayMode = false;
        }
    }

    if (isSubmarineSubmerging) {
        submarineOffset -= 0.005f; // Decrease the submarine offset to simulate submerging
        if (submarineOffset < -0.4f) { // Stop when the submarine is fully submerged
            isSubmarineSubmerging = false;
        }
    }

    // Move the submarine to the right only if the flag is set
    if (isSubmarineMoving) {
        submarineOffset += 0.005f; // Move the submarine to the right
        if (submarineOffset >= 0.6f) {
            isSubmarineMoving = false;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(32, updateScene1, 0); // Slowed down the timer to 32ms
}

void keyboardScene1(unsigned char key, int x, int y) {
    switch (key) {
        case 'd': // Toggle day/night mode
        case 'D':
            isDayMode = !isDayMode; // Invert the mode
            if (isDayMode) {
                sunAngle = 1.5707963f; // Reset sun angle for day mode
            } else {
                sunAngle = -1.5707963f; // Reset sun angle for night mode
            }
            glutPostRedisplay();   // Request a redisplay
            break;

        case 'g': // Start moving the submarine to the right
        case 'G':
            isSubmarineMoving = true; // Set the flag to start moving the submarine
            glutPostRedisplay();   // Request a redisplay
            break;

        case 27: // Exit the program when 'Esc' is pressed
            exit(0);
            break;

        default:
            break; // Ignore other key presses
    }
}

// Scene 2 Code
struct Bubble {
    float x;
    float y;
    float radius;
    float speed;
    bool active; // Flag to check if the bubble is active
};

const int numBubbles = 10;
Bubble bubbles[numBubbles];

// Submarine position and speed
float submarineX = -0.9f;
float submarineY = 0.9f;
float submarineSpeed = 0.01f;
//float propellerAngle = 0.0f; // Angle for propeller rotation

// Fish positions and speeds
struct Fish {
    float x;
    float y;
    float scale;
    float speed;
};

const int numFish = 6;
Fish fish[numFish] = {
    {-0.4f, 0.2f, 1.4f, 0.005f},
    {-0.8f, -0.5f, 1.2f, 0.005f},
    {0.5f, 0.4f, 1.2f, 0.005f},
    {0.8f, -0.5f, 1.3f, 0.005f},
    {-0.2f, -0.7f, 1.3f, 0.005f},
    {0.0f, 0.0f, 1.0f, 0.005f}
};

// Seaweed structure
struct Seaweed {
    float x;
    float y;
    float scale;
    float width;
    float swayAngle; // Angle for swaying motion
    float swaySpeed; // Speed of swaying motion
};

const int numSeaweed = 8;
Seaweed seaweed[numSeaweed] = {
    {-0.9f, -1.0f, 0.8f, 0.04f, 0.0f, 0.005f},
    {-0.95f, -1.0f, 0.7f, 0.04f, 0.0f, 0.005f},
    {-0.86f, -1.0f, 0.9f, 0.04f, 0.0f, 0.005f},
    {-0.75f, -1.0f, 0.8f, 0.04f, 0.0f, 0.005f},
    {0.8f, -1.0f, 0.9f, 0.04f, 0.0f, 0.005f},
    {0.85f, -1.0f, 0.8f, 0.04f, 0.0f, 0.005f},
    {0.93f, -1.0f, 0.7f, 0.04f, 0.0f, 0.005f},
    {0.95f, -1.0f, 0.9f, 0.04f, 0.0f, 0.005f}
};

// Initialize bubbles with fixed positions and properties
void initBubbles() {
    for (int i = 0; i < numBubbles; ++i) {
        bubbles[i].active = false; // Initially, no bubbles are active
    }
}

// Draw a single bubble as a ring
void drawBubble(const Bubble& bubble) {
    if (!bubble.active) return; // Only draw active bubbles
    glColor4f(0.7f, 0.9f, 1.0f, 0.5f); // Light blue semi-transparent color
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.14159f / 180.0f;
        float dx = bubble.radius * cos(theta);
        float dy = bubble.radius * sin(theta);
        glVertex2f(bubble.x + dx, bubble.y + dy);
    }

    glEnd();
}

// Function to draw realistic, short, thin wavy seaweed with a sharp tip
void drawRealisticSeaweed(const Seaweed& seaweed) {
    glPushMatrix();
    glTranslatef(seaweed.x, seaweed.y, 0.0f);
    glRotatef(seaweed.swayAngle, 0.0f, 0.0f, 1.0f); // Apply swaying motion

    glColor3f(0.0f, 0.7f, 0.2f); // Seaweed color

    // Draw main body
    glBegin(GL_TRIANGLE_STRIP);
    for (float t = 0.0f; t <= 1.0f; t += 0.02f) {
        float offset = 0.02f * sin(12 * t); // Wavy effect
        float taper = 1.0f - t; // Taper for sharp tip
        glVertex2f(offset * seaweed.scale - seaweed.width * taper * 0.6f, t * seaweed.scale); // Thinner width
        glVertex2f(offset * seaweed.scale + seaweed.width * taper * 0.6f, t * seaweed.scale);
    }
    // Sharp tip of the seaweed
    glVertex2f(0.0f, seaweed.scale);
    glEnd();

    // Add shading for realism
    glColor3f(0.0f, 0.5f, 0.2f);
    glBegin(GL_TRIANGLE_STRIP);
    for (float t = 0.0f; t <= 1.0f; t += 0.02f) {
        float offset = 0.02f * sin(12 * t);
        float taper = 1.0f - t;
        glVertex2f(offset * seaweed.scale, t * seaweed.scale);
        glVertex2f(offset * seaweed.scale + seaweed.width * taper * 0.3f, t * seaweed.scale); // Thinner shading
    }
    glVertex2f(0.0f, seaweed.scale);
    glEnd();

    glPopMatrix();
}

// Function to draw a simple fish with fixed smaller size
void drawFish(float x, float y, float scale) {
    // Body of the fish
    glColor3f(1.0f, 0.5f, 0.0f); // Orange color
    glBegin(GL_POLYGON);
    glVertex2f(x + scale * (-0.075f), y + scale * 0.0f);
    glVertex2f(x + scale * (-0.06f), y + scale * 0.015f);
    glVertex2f(x + scale * (-0.045f), y + scale * 0.03f);
    glVertex2f(x + scale * (-0.03f), y + scale * 0.045f);
    glVertex2f(x + scale * (-0.015f), y + scale * 0.0525f);
    glVertex2f(x + scale * 0.0f, y + scale * 0.045f);
    glVertex2f(x + scale * 0.015f, y + scale * 0.0375f);
    glVertex2f(x + scale * 0.03f, y + scale * 0.0225f);
    glVertex2f(x + scale * 0.045f, y + scale * 0.0f);
    glVertex2f(x + scale * 0.03f, y + scale * (-0.0225f));
    glVertex2f(x + scale * 0.015f, y + scale * (-0.0375f));
    glVertex2f(x + scale * 0.0f, y + scale * (-0.045f));
    glVertex2f(x + scale * (-0.015f), y + scale * (-0.0525f));
    glVertex2f(x + scale * (-0.03f), y + scale * (-0.045f));
    glVertex2f(x + scale * (-0.045f), y + scale * (-0.03f));
    glVertex2f(x + scale * (-0.06f), y + scale * (-0.015f));
    glEnd();

    // Tail of the fish
    glColor3f(0.8f, 0.3f, 0.0f); // Darker orange
    glBegin(GL_TRIANGLES);
    glVertex2f(x + scale * (-0.075f), y + scale * 0.0f);
    glVertex2f(x + scale * (-0.105f), y + scale * 0.03f);
    glVertex2f(x + scale * (-0.105f), y + scale * (-0.03f));
    glEnd();

    // Eye of the fish
    glColor3f(0.0f, 0.0f, 0.0f); // Black eye
    glBegin(GL_POLYGON);
    glVertex2f(x + scale * 0.0225f, y + scale * 0.015f);
    glVertex2f(x + scale * 0.0255f, y + scale * 0.018f);
    glVertex2f(x + scale * 0.0285f, y + scale * 0.015f);
    glVertex2f(x + scale * 0.0255f, y + scale * 0.012f);
    glEnd();

    // Decorative vertical lines inside the fish body
    glColor3f(1.0f, 1.0f, 1.0f); // White lines
    glBegin(GL_LINES);
    glVertex2f(x + scale * (-0.045f), y + scale * 0.03f);
    glVertex2f(x + scale * (-0.045f), y + scale * (-0.03f));
    glVertex2f(x + scale * (-0.015f), y + scale * 0.0375f);
    glVertex2f(x + scale * (-0.015f), y + scale * (-0.0375f));
    glVertex2f(x + scale * 0.015f, y + scale * 0.0225f);
    glVertex2f(x + scale * 0.015f, y + scale * (-0.0225f));
    glEnd();
}

void drawSunlight() {
    int numSegments = 100; // Smoothness of the gradient
    float radius = 0.8f;   // Radius of the sunlight glow
    glBegin(GL_TRIANGLE_FAN);

    // Center of the sunlight (bright yellow)
    glColor4f(1.0f, 1.0f, 0.6f, 0.5f);
    glVertex2f(0.0f, 1.0f);

    // Outer edges of the sunlight (fading to transparent)
    for (int i = 0; i <= numSegments; ++i) {
        float theta = i * 2.0f * 3.14159f / numSegments;
        float x = radius * cos(theta);
        float y = 1.0f + radius * sin(theta);
        glColor4f(1.0f, 1.0f, 0.6f, 0.0f); // Transparent at the edges
        glVertex2f(x, y);
    }
    glEnd();
}

// Function to draw sunlight rays
void drawSunlightEffect() {
    int numSegments = 100; // Smoothness of the gradient
    float radius = 0.8f;   // Radius of the sunlight glow
    glBegin(GL_TRIANGLE_FAN);

    // Center of the sunlight (bright yellow)
    glColor4f(1.0f, 1.0f, 0.8f, 0.6f); // Yellowish color with transparency
    glVertex2f(0.0f, 1.0f);

    // Outer edges of the sunlight (fading to transparent)
    for (int i = 0; i <= numSegments; ++i) {
        float theta = i * 2.0f * 3.14159f / numSegments;
        float x = radius * cos(theta);
        float y = 1.0f + radius * sin(theta); // Positioned at the top
        glColor4f(1.0f, 1.0f, 0.8f, 0.0f); // Transparent at the edges
        glVertex2f(x, y);
    }
    glEnd();
}

void drawLightRays() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int numRays = 10; // Number of rays
    float rayWidth = 0.3f; // Width of each ray
    float rayHeight = 1.0f; // Length of the rays

    for (int i = 0; i < numRays; ++i) {
        float startAngle = -0.5f + (i * rayWidth); // Positioning of rays
        glBegin(GL_TRIANGLES);
        glColor4f(1.0f, 1.0f, 1.0f, 0.2f); // Bright and slightly translucent
        glVertex2f(0.0f, 1.0f); // Top-center (origin of the rays)

        glColor4f(1.0f, 1.0f, 1.0f, 0.0f); // Fading to transparency
        glVertex2f(startAngle, 0.0f); // Bottom left of the ray
        glVertex2f(startAngle + rayWidth, 0.0f); // Bottom right of the ray
        glEnd();
    }

    glDisable(GL_BLEND);
}

// Function to draw the background
void drawBackground() {
    // Gradient background
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.5f); // Deep blue
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glColor3f(0.0f, 0.7f, 1.0f); // Light blue
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();
}

// Display callback
void renderScene2() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the background
    drawBackground();

    // Add sunlight effect
    //drawSunlightEffect();

    // Add light rays
    drawLightRays();

    // Draw the fish at specified positions and scaled sizes
    for (int i = 0; i < numFish; ++i) {
        drawFish(fish[i].x, fish[i].y, fish[i].scale);
    }

    glFlush();

    // Draw seaweed
    for (int i = 0; i < numSeaweed; ++i) {
        drawRealisticSeaweed(seaweed[i]);
    }

    // Draw bubbles
    for (int i = 0; i < numBubbles; ++i) {
        drawBubble(bubbles[i]);
    }

    // Submarine body
    glPushMatrix();
    glTranslatef(submarineX, submarineY, 0.0f); // Adjust the position of the submarine
    glScalef(0.3f, 0.3f, 1.0f); // Scale the submarine to make it smaller

    glColor3f(1.0f, 0.6f, 0.0f); // Orange
    glBegin(GL_POLYGON);
    glVertex2f(-0.6f, -0.3f); // Bottom-left corner
    glVertex2f(0.2f, -0.3f);  // Bottom-right straight edge
    glVertex2f(0.2f, 0.3f);   // Top-right straight edge
    glVertex2f(-0.6f, 0.3f);  // Top-left corner
    glVertex2f(-0.7f, 0.0f);  // Left-most tail
    glEnd();

    // Wide rounded front (right side)
    glColor3f(1.0f, 0.6f, 0.0f); // Orange
    drawPartialCircle(0.2f, 0.0f, 0.3f, 0, 360, 50); // Full circle on the right

    // Windows
    glColor3f(0.0f, 0.2f, 0.6f); // Blue border
    drawPartialCircle(-0.25f, 0.0f, 0.14f, 0, 360, 50); // Left window border
    drawPartialCircle(0.1f, 0.0f, 0.15f, 0, 360, 50);   // Right window border
    glColor3f(0.6f, 0.8f, 1.0f); // Light blue inner
    drawPartialCircle(-0.25f, 0.0f, 0.12f, 0, 360, 50); // Left window inner
    drawPartialCircle(0.1f, 0.0f, 0.12f, 0, 360, 50);   // Right window inner

    // Periscope base
    glColor3f(0.0f, 0.2f, 0.6f); // Blue
    glBegin(GL_POLYGON);
    glVertex2f(-0.05f, 0.3f);
    glVertex2f(0.05f, 0.3f);
    glVertex2f(0.05f, 0.55f);
    glVertex2f(-0.05f, 0.55f);
    glEnd();

    // Periscope top arm
    glColor3f(0.0f, 0.2f, 0.6f); // Blue
    glBegin(GL_POLYGON);
    glVertex2f(0.05f, 0.5f);
    glVertex2f(0.15f, 0.5f);
    glVertex2f(0.15f, 0.55f);
    glVertex2f(0.05f, 0.55f);
    glEnd();

    // Periscope viewfinder
    glColor3f(1.0f, 0.6f, 0.0f); // Orange
    glBegin(GL_POLYGON);
    glVertex2f(0.15f, 0.48f);
    glVertex2f(0.2f, 0.48f);
    glVertex2f(0.2f, 0.57f);
    glVertex2f(0.15f, 0.57f);
    glEnd();

    // Tail
    glColor3f(0.0f, 0.2f, 0.6f); // Blue tail
    glBegin(GL_POLYGON);
    glVertex2f(-0.7f, 0.1f);
    glVertex2f(-0.9f, 0.0f);
    glVertex2f(-0.7f, -0.1f);
    glEnd();

    // Propeller blades with rotation
    drawPropeller(-0.9f, 0.0f, 0.15f, propellerAngle);

    glPopMatrix();

    glutSwapBuffers();
}

// Function to update the scene
void updateScene2(int value) {
    // Move the submarine diagonally
    submarineX += submarineSpeed;
    submarineY -= submarineSpeed;
    if (submarineY < -0.9f) {
        submarineY = 0.9f;
        submarineX = -0.9f;
    }

    // Move the fish
    for (int i = 0; i < numFish; ++i) {
        fish[i].x += fish[i].speed;
        if (fish[i].x > 1.0f) {
            fish[i].x = -1.0f;
        }
    }

    // Update swaying motion for seaweed
    for (int i = 0; i < numSeaweed; ++i) {
        seaweed[i].swayAngle = 5.0f * sin(seaweed[i].swaySpeed * value);
        seaweed[i].swaySpeed += 0.0002f; // Slower sway speed
    }

    // Move the bubbles
    for (int i = 0; i < numBubbles; ++i) {
        if (bubbles[i].active) {
            bubbles[i].y += bubbles[i].speed;
            if (bubbles[i].y > 1.0f) {
                bubbles[i].active = false; // Deactivate the bubble when it goes out of the screen
            }
        }
    }

    // Update propeller rotation
    propellerAngle += 10.0f; // Increase the angle for rotation
    if (propellerAngle >= 360.0f) {
        propellerAngle = 0.0f;
    }

    // Create new bubbles from fish mouths
    for (int i = 0; i < numFish; ++i) {
        if (rand() % 100 < 5) { // 5% chance to create a new bubble
            for (int j = 0; j < numBubbles; ++j) {
                if (!bubbles[j].active) {
                    bubbles[j].x = fish[i].x + fish[i].scale * 0.045f; // Position at the fish's mouth
                    bubbles[j].y = fish[i].y;
                    bubbles[j].radius = 0.02f;
                    bubbles[j].speed = 0.005f;
                    bubbles[j].active = true;
                    break;
                }
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(100, updateScene2, value + 1);
}

// Special key function to handle arrow keys
void specialKeysScene2(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            submarineSpeed += 0.01f;
            break;
        case GLUT_KEY_DOWN:
            submarineSpeed -= 0.01f;
            if (submarineSpeed < 0.0f) submarineSpeed = 0.0f;
            break;
    }
    glutPostRedisplay();
}

// Keyboard function to handle regular keys
void keyboardScene2(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            submarineY += submarineSpeed;
            break;
        case 's':
            submarineY -= submarineSpeed;
            break;
        case 'a':
            submarineX -= submarineSpeed;
            break;
        case 'g':
            submarineX += submarineSpeed;
            break;
              case ' ': // Spacebar to stop
            submarineSpeed = 0.0f;

            break;
        case '\r': // Enter key to start

            submarineSpeed = 0.01f;
            break;
    }
    glutPostRedisplay();
}

// Submarine position and speed
float submarinX = 0.0f;
float submarinY = 0.9f;
float submarinSpeed = 0.01f;
float propelleAngle = 0.0f; // Angle for propeller rotation

// Fish positions and speeds
struct Fish1 {
    float x;
    float y;
    float scale;
    float speed;
};

const int numFish2 = 6;
Fish1 fish2[numFish2] = {
   {-0.4f, 0.8f, 1.4f, 0.01f},
    {-0.8f, 0.7f, 1.2f, 0.01f},
    {0.5f, 0.9f, 1.2f, 0.01f},
    {0.8f, 0.6f, 1.3f, 0.01f},
    {-0.2f, 0.85f, 1.3f, 0.01f},
    {0.0f, 0.75f, 1.0f, 0.01f}
};

// Shark variables
float sharkX1 = -1.2f, sharkY1 = 0.0f, sharkDir1 = 0.005f;
float sharkX2 = 1.2f, sharkY2 = 0.3f, sharkDir2 = -0.005f;
float submarinY2 = 0.0, submarinDir = 0.002;

// Fish shadow positions
struct FishShadow {
    float x;
    float y;
    float speed;
};

const int numFishShadows = 6;
FishShadow fishShadows[numFishShadows] = {
    {-0.8f, 0.8f, 0.01f},
    {-0.75f, 0.85f, 0.01f},
    {-0.2f, 0.9f, 0.01f},
    {-0.15f, 0.85f, 0.01f},
    {0.5f, 0.88f, 0.01f},
    {0.55f, 0.92f, 0.01f}
};
void drawShark(float x, float y, bool straight) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f); // Move to the shark's position

    if (!straight) {
        glScalef(-1.0f, 1.0f, 1.0f); // Flip horizontally
    }

    // Body
    glColor3f(0.0, 0.5, 0.7); // Blue
    glBegin(GL_POLYGON);
    glVertex2f(-0.15, 0.0);
    glVertex2f(0.15, 0.0);
    glVertex2f(0.13, 0.08);
    glVertex2f(-0.13, 0.08);
    glEnd();

    // Tail
    glBegin(GL_TRIANGLES);
    glVertex2f(0.15, 0.0);
    glVertex2f(0.2, -0.05);
    glVertex2f(0.2, 0.05);
    glEnd();

    // Top fin
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0, 0.08);
    glVertex2f(-0.03, 0.13);
    glVertex2f(0.03, 0.13);
    glEnd();

    // Eyes
    glColor3f(1.0, 1.0, 1.0);
    drawCircle(-0.1, 0.04, 0.015, 20);
    glColor3f(0.0, 0.0, 0.0);
    drawCircle(-0.1, 0.04, 0.008, 20);

    glPopMatrix();
}

// Initialize bubbles with fixed positions and properties
void initBubble() {
    for (int i = 0; i < numBubbles; ++i) {
        bubbles[i].active = false; // Initially, no bubbles are active
    }
}

// Draw a single bubble as a ring
void drawBubbl(const Bubble& bubble) {
    if (!bubble.active) return; // Only draw active bubbles
    glColor4f(0.7f, 0.9f, 1.0f, 0.5f); // Light blue semi-transparent color
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);

    for (int i = 0; i < 360; i += 10) {
        float theta = i * 3.14159f / 180.0f;
        float dx = bubble.radius * cos(theta);
        float dy = bubble.radius * sin(theta);
        glVertex2f(bubble.x + dx, bubble.y + dy);
    }

    glEnd();
}

// Function to draw the submarine
void drawSubmarin() {
    glPushMatrix();
    glTranslatef(submarinX, submarinY, 0.0f); // Adjust the position of the submarine
    glScalef(0.3f, 0.3f, 1.0f); // Scale the submarine to make it smaller

    // Submarine body
    glColor3f(1.0f, 0.6f, 0.0f); // Orange
    glBegin(GL_POLYGON);
    glVertex2f(-0.6f, -0.3f); // Bottom-left corner
    glVertex2f(0.2f, -0.3f);  // Bottom-right straight edge
    glVertex2f(0.2f, 0.3f);   // Top-right straight edge
    glVertex2f(-0.6f, 0.3f);  // Top-left corner
    glVertex2f(-0.7f, 0.0f);  // Left-most tail
    glEnd();

    // Wide rounded front (right side)
    glColor3f(1.0f, 0.6f, 0.0f); // Orange
    drawPartialCircle(0.2f, 0.0f, 0.3f, 0, 360, 50); // Full circle on the right

    // Windows
    glColor3f(0.0f, 0.2f, 0.6f); // Blue border
    drawPartialCircle(-0.25f, 0.0f, 0.14f, 0, 360, 50); // Left window border
    drawPartialCircle(0.1f, 0.0f, 0.15f, 0, 360, 50);   // Right window border
    glColor3f(0.6f, 0.8f, 1.0f); // Light blue inner
    drawPartialCircle(-0.25f, 0.0f, 0.12f, 0, 360, 50); // Left window inner
    drawPartialCircle(0.1f, 0.0f, 0.12f, 0, 360, 50);   // Right window inner

    // Periscope base
    glColor3f(0.0f, 0.2f, 0.6f); // Blue
    glBegin(GL_POLYGON);
    glVertex2f(-0.05f, 0.3f);
    glVertex2f(0.05f, 0.3f);
    glVertex2f(0.05f, 0.55f);
    glVertex2f(-0.05f, 0.55f);
    glEnd();

    // Periscope top arm
    glColor3f(0.0f, 0.2f, 0.6f); // Blue
    glBegin(GL_POLYGON);
    glVertex2f(0.05f, 0.5f);
    glVertex2f(0.15f, 0.5f);
    glVertex2f(0.15f, 0.55f);
    glVertex2f(0.05f, 0.55f);
    glEnd();

    // Periscope viewfinder
    glColor3f(1.0f, 0.6f, 0.0f); // Orange
    glBegin(GL_POLYGON);
    glVertex2f(0.15f, 0.48f);
    glVertex2f(0.2f, 0.48f);
    glVertex2f(0.2f, 0.57f);
    glVertex2f(0.15f, 0.57f);
    glEnd();

    // Tail
    glColor3f(0.0f, 0.2f, 0.6f); // Blue tail
    glBegin(GL_POLYGON);
    glVertex2f(-0.7f, 0.1f);
    glVertex2f(-0.9f, 0.0f);
    glVertex2f(-0.7f, -0.1f);
    glEnd();

    // Propeller blades with rotation
    drawPropeller(-0.9f, 0.0f, 0.15f, propellerAngle);

    glPopMatrix();
}

    // Body of the fish
void drawFish1(float x, float y, float scale) {
    // Body of the fish
    glColor3f(1.0f, 0.5f, 0.0f); // Orange color
    glBegin(GL_POLYGON);
    glVertex2f(x + scale * (-0.075f), y + scale * 0.0f);
    glVertex2f(x + scale * (-0.06f), y + scale * 0.015f);
    glVertex2f(x + scale * (-0.045f), y + scale * 0.03f);
    glVertex2f(x + scale * (-0.03f), y + scale * 0.045f);
    glVertex2f(x + scale * (-0.015f), y + scale * 0.0525f);
    glVertex2f(x + scale * 0.0f, y + scale * 0.045f);
    glVertex2f(x + scale * 0.015f, y + scale * 0.0375f);
    glVertex2f(x + scale * 0.03f, y + scale * 0.0225f);
    glVertex2f(x + scale * 0.045f, y + scale * 0.0f);
    glVertex2f(x + scale * 0.03f, y + scale * (-0.0225f));
    glVertex2f(x + scale * 0.015f, y + scale * (-0.0375f));
    glVertex2f(x + scale * 0.0f, y + scale * (-0.045f));
    glVertex2f(x + scale * (-0.015f), y + scale * (-0.0525f));
    glVertex2f(x + scale * (-0.03f), y + scale * (-0.045f));
    glVertex2f(x + scale * (-0.045f), y + scale * (-0.03f));
    glVertex2f(x + scale * (-0.06f), y + scale * (-0.015f));
    glEnd();

    // Tail of the fish
    glColor3f(0.8f, 0.3f, 0.0f); // Darker orange
    glBegin(GL_TRIANGLES);
    glVertex2f(x + scale * (-0.075f), y + scale * 0.0f);
    glVertex2f(x + scale * (-0.105f), y + scale * 0.03f);
    glVertex2f(x + scale * (-0.105f), y + scale * (-0.03f));
    glEnd();

    // Eye of the fish
    glColor3f(0.0f, 0.0f, 0.0f); // Black eye
    glBegin(GL_POLYGON);
    glVertex2f(x + scale * 0.0225f, y + scale * 0.015f);
    glVertex2f(x + scale * 0.0255f, y + scale * 0.018f);
    glVertex2f(x + scale * 0.0285f, y + scale * 0.015f);
    glVertex2f(x + scale * 0.0255f, y + scale * 0.012f);
    glEnd();

    // Decorative vertical lines inside the fish body
    glColor3f(1.0f, 1.0f, 1.0f); // White lines
    glBegin(GL_LINES);
    glVertex2f(x + scale * (-0.045f), y + scale * 0.03f);
    glVertex2f(x + scale * (-0.045f), y + scale * (-0.03f));
    glVertex2f(x + scale * (-0.015f), y + scale * 0.0375f);
    glVertex2f(x + scale * (-0.015f), y + scale * (-0.0375f));
    glVertex2f(x + scale * 0.015f, y + scale * 0.0225f);
    glVertex2f(x + scale * 0.015f, y + scale * (-0.0225f));
    glEnd();
}

// Function to draw a starfish
void drawStarfish(float cx, float cy, float scale) {
    glColor3f(0.9f, 0.5f, 0.0f); // Starfish color
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 10; i++) {
        float angle = i * 3.14159f / 5.0f; // 5 points of the starfish
        float radius = (i % 2 == 0) ? scale : scale / 2.5f;
        float x = cx + radius * cos(angle);
        float y = cy + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

// Function to draw a rock
void drawRock(float x, float y, float width, float height) {
    glColor3f(0.4f, 0.4f, 0.4f); // Gray color
    glBegin(GL_POLYGON);
    for (int i = 0; i < 30; i++) {
        float angle = 2.0f * 3.1415926f * i / 30;
        float xOffset = width * cos(angle);
        float yOffset = height * sin(angle);
        glVertex2f(x + xOffset, y + yOffset);
    }
    glEnd();
}

// Function to draw the ocean floor
void drawOceanFloor() {
    glColor3f(0.8f, 0.7f, 0.5f); // Sandy color
    glBegin(GL_POLYGON);
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, -0.85f);
    glVertex2f(-1.0f, -0.85f);
    glEnd();

    // Add rocks
    drawRock(-0.7f, -0.95f, 0.1f, 0.05f);
    drawRock(0.5f, -0.9f, 0.15f, 0.08f);

    // Add starfish
    drawStarfish(-0.3f, -0.92f, 0.05f);
    drawStarfish(0.7f, -0.88f, 0.06f);
}

// Function to draw smoke coming out of the ship's chimney
void drawSmoke(float x, float y) {
    glColor4f(0.5f, 0.5f, 0.5f, 0.6f); // Light gray with transparency
    for (int i = 0; i < 3; ++i) {      // Draw three smoke puffs
        glPushMatrix();
        glTranslatef(x, y + i * 0.05f, 0.0f); // Offset each puff
        glBegin(GL_POLYGON);
        for (int j = 0; j < 360; j += 30) { // Create circular puffs
            float angle = j * M_PI / 180.0f;
            glVertex2f(0.05f * cos(angle), 0.05f * sin(angle));
        }
        glEnd();
        glPopMatrix();
    }
}

// Function to draw a realistic ship
void drawShip() {
    glPushMatrix();                    // Save current transformation state
    glTranslatef(-0.3f, -0.8f, 0.0f); // Position the ship
    glRotatef(-10.0f, 0.0f, 0.0f, 1.0f); // Rotate the ship for realism

    // Main hull
    glBegin(GL_POLYGON);
    glColor3f(0.15f, 0.25f, 0.4f);    // Dark blue at the bottom
    glVertex2f(-0.4f, 0.0f);          // Bottom-left
    glVertex2f(0.4f, 0.0f);           // Bottom-right
    glColor3f(0.4f, 0.6f, 0.8f);      // Light blue at the top
    glVertex2f(0.35f, 0.1f);          // Top-right
    glVertex2f(-0.35f, 0.1f);         // Top-left
    glEnd();

    // Red stripe
    glColor3f(0.8f, 0.2f, 0.2f); // Deep red stripe
    glBegin(GL_QUADS);
    glVertex2f(-0.35f, 0.05f);
    glVertex2f(0.35f, 0.05f);
    glVertex2f(0.35f, 0.07f);
    glVertex2f(-0.35f, 0.07f);
    glEnd();

    // Upper deck
    glColor3f(0.9f, 0.9f, 0.9f); // White deck
    glBegin(GL_QUADS);
    glVertex2f(-0.3f, 0.1f);
    glVertex2f(0.3f, 0.1f);
    glVertex2f(0.25f, 0.18f);
    glVertex2f(-0.25f, 0.18f);
    glEnd();

    // Additional layer
    glColor3f(0.85f, 0.85f, 0.85f); // Slightly darker white
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, 0.18f);
    glVertex2f(0.2f, 0.18f);
    glVertex2f(0.15f, 0.25f);
    glVertex2f(-0.15f, 0.25f);
    glEnd();

    // Broken windows on the main deck
    glColor3f(0.0f, 0.0f, 0.0f); // Black for window frames
    for (float wx = -0.25f; wx <= 0.25f; wx += 0.1f) {
        glBegin(GL_LINE_LOOP);
        glVertex2f(wx - 0.02f, 0.12f);
        glVertex2f(wx + 0.02f, 0.12f);
        glVertex2f(wx + 0.02f, 0.15f);
        glVertex2f(wx - 0.02f, 0.15f);
        glEnd();

        // Broken glass effect
        glBegin(GL_LINES);
        glVertex2f(wx, 0.12f);
        glVertex2f(wx + 0.015f, 0.14f);
        glVertex2f(wx, 0.12f);
        glVertex2f(wx - 0.015f, 0.14f);
        glEnd();
    }

    // Chimney
    glColor3f(0.7f, 0.1f, 0.1f); // Red chimney
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.2f);
    glVertex2f(0.05f, 0.2f);
    glVertex2f(0.04f, 0.35f);
    glVertex2f(-0.04f, 0.35f);
    glEnd();

    // Chimney outline
    glColor3f(0.0f, 0.0f, 0.0f); // Black outline
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.05f, 0.2f);
    glVertex2f(0.05f, 0.2f);
    glVertex2f(0.04f, 0.35f);
    glVertex2f(-0.04f, 0.35f);
    glEnd();

    // Draw smoke from the chimney
    drawSmoke(0.0f, 0.35f);

    glPopMatrix(); // Restore transformation state
}

// Function to draw a curved sail
void drawCurvedSail(float baseX, float baseY, float width, float height) {
    glBegin(GL_POLYGON);
    glVertex2f(baseX, baseY);
    glVertex2f(baseX + width, baseY);

    // Curved edge of the sail
    for (float t = 0.0f; t <= 1.0f; t += 0.05f) {
        float x = baseX + width * t;
        float y = baseY + height * (1 - t * t);  // Quadratic curve
        glVertex2f(x, y);
    }
    glEnd();
}

// Function to draw the shadow of the fish
void drawFishShadow(float centerX, float centerY, float scaleX, float scaleY) {
    glEnable(GL_BLEND); // Enable blending for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.0f, 0.0f, 0.0f, 0.15f); // Semi-transparent black for shadows
    glBegin(GL_POLYGON);
    for (int angle = 0; angle < 360; angle += 10) {
        float theta = angle * 3.14159f / 180.0f; // Convert degrees to radians
        float x = scaleX * cosf(theta);
        float y = scaleY * sinf(theta);
        glVertex2f(centerX + x, centerY + y);
    }
    glEnd();

    glDisable(GL_BLEND); // Disable blending
}

// Function to draw the shadow of the fish
void drawTopFishShadows() {
    // Draw a semi-transparent shadow at the top of the scene
    for (int i = 0; i < numFishShadows; ++i) {
        drawFishShadow(fishShadows[i].x, fishShadows[i].y, 0.1f, 0.03f);
    }
}

// Display callback
void renderScene3() {
    glClear(GL_COLOR_BUFFER_BIT);
      glPushMatrix();
    glTranslatef(submarinX, submarinY, 0.0f); // Adjust the position of the submarine
    glScalef(0.3f, 0.3f, 1.0f); // Scale the submarine to make it smaller

    // Submarine body
    glColor3f(1.0f, 0.6f, 0.0f); // Orange
    glBegin(GL_POLYGON);
    glVertex2f(-0.6f, -0.3f); // Bottom-left corner
    glVertex2f(0.2f, -0.3f);  // Bottom-right straight edge
    glVertex2f(0.2f, 0.3f);   // Top-right straight edge
    glVertex2f(-0.6f, 0.3f);  // Top-left corner
    glVertex2f(-0.7f, 0.0f);  // Left-most tail
    glEnd();

    // Wide rounded front (right side)
    glColor3f(1.0f, 0.6f, 0.0f); // Orange
    drawPartialCircle(0.2f, 0.0f, 0.3f, 0, 360, 50); // Full circle on the right

    // Windows
    glColor3f(0.0f, 0.2f, 0.6f); // Blue border
    drawPartialCircle(-0.25f, 0.0f, 0.14f, 0, 360, 50); // Left window border
    drawPartialCircle(0.1f, 0.0f, 0.15f, 0, 360, 50);   // Right window border
    glColor3f(0.6f, 0.8f, 1.0f); // Light blue inner
    drawPartialCircle(-0.25f, 0.0f, 0.12f, 0, 360, 50); // Left window inner
    drawPartialCircle(0.1f, 0.0f, 0.12f, 0, 360, 50);   // Right window inner

    // Periscope base
    glColor3f(0.0f, 0.2f, 0.6f); // Blue
    glBegin(GL_POLYGON);
    glVertex2f(-0.05f, 0.3f);
    glVertex2f(0.05f, 0.3f);
    glVertex2f(0.05f, 0.55f);
    glVertex2f(-0.05f, 0.55f);
    glEnd();

    // Periscope top arm
    glColor3f(0.0f, 0.2f, 0.6f); // Blue
    glBegin(GL_POLYGON);
    glVertex2f(0.05f, 0.5f);
    glVertex2f(0.15f, 0.5f);
    glVertex2f(0.15f, 0.55f);
    glVertex2f(0.05f, 0.55f);
    glEnd();

    // Periscope viewfinder
    glColor3f(1.0f, 0.6f, 0.0f); // Orange
    glBegin(GL_POLYGON);
    glVertex2f(0.15f, 0.48f);
    glVertex2f(0.2f, 0.48f);
    glVertex2f(0.2f, 0.57f);
    glVertex2f(0.15f, 0.57f);
    glEnd();

    // Tail
    glColor3f(0.0f, 0.2f, 0.6f); // Blue tail
    glBegin(GL_POLYGON);
    glVertex2f(-0.7f, 0.1f);
    glVertex2f(-0.9f, 0.0f);
    glVertex2f(-0.7f, -0.1f);
    glEnd();

    // Propeller blades with rotation
    drawPropeller(-0.9f, 0.0f, 0.15f, propellerAngle);

   glPopMatrix();

    // Background gradient
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.8f);
    glVertex2f(-1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 0.2f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    // Draw elements
    drawOceanFloor();
    drawShip();
    drawTopFishShadows();

    // Draw seaweed
    for (const auto& seaweedItem : seaweed) {
        drawRealisticSeaweed(seaweedItem);
    }

    drawShark(sharkX1, sharkY1, 0.0f);    // First shark, facing right (no rotation)
    drawShark(sharkX2, sharkY2, 180.0f);  // Second shark, facing left (180-degree rotation)

    drawSubmarin();

    // Draw fishes
    for (int i = 0; i < numFish2; ++i) {
        drawFish1(fish2[i].x, fish2[i].y, fish2[i].scale);
    }

    for (int i = 0; i < numBubbles; ++i) {
        drawBubble(bubbles[i]);
    }

    glutSwapBuffers();
}

// Function to update the scene
void updateScene3(int value) {
    // Move the submarine from top to bottom in the center
    submarinY -= submarinSpeed;

    // Stop the submarine near the ship
    if (submarinY < -0.7f) {
        submarinSpeed = 0.0f;
    }

    // Move the fish
    for (int i = 0; i < numFish2; ++i) {
        // Move fish only at the top
        fish2[i].x += fish[i].speed;
        if (fish2[i].x > 1.0f) {
            fish2[i].x = -1.0f;
        }
    }

    // Update swaying motion for seaweed
    for (int i = 0; i < numSeaweed; ++i) {
        seaweed[i].swayAngle = 5.0f * sin(seaweed[i].swaySpeed * value);
        seaweed[i].swaySpeed += 0.0002f; // Slower sway speed
    }

    // Move the bubbles
    for (int i = 0; i < numBubbles; ++i) {
        if (bubbles[i].active) {
            bubbles[i].y += bubbles[i].speed;
            if (bubbles[i].y > 1.0f) {
                bubbles[i].active = false; // Deactivate the bubble when it goes out of the screen
            }
        }
    }

    // Update propeller rotation
    propellerAngle += 10.0f; // Increase the angle for rotation
    if (propellerAngle >= 360.0f) {
        propellerAngle = 0.0f;
    }

    // Create new bubbles from shark mouths
    for (int i = 0; i < 2; ++i) {
        float sharkX = (i == 0) ? sharkX1 : sharkX2;
        float sharkY = (i == 0) ? sharkY1 : sharkY2;
        if (rand() % 100 < 5) { // 5% chance to create a new bubble
            for (int j = 0; j < numBubbles; ++j) {
                if (!bubbles[j].active) {
                    bubbles[j].x = sharkX + 0.045f; // Position at the shark's mouth
                    bubbles[j].y = sharkY;
                    bubbles[j].radius = 0.02f;
                    bubbles[j].speed = 0.005f;
                    bubbles[j].active = true;
                    break;
                }
            }
        }
    }

    // Create new bubbles from fish mouths
    for (int i = 0; i < numFish; ++i) {
        if (rand() % 100 < 5) { // 5% chance to create a new bubble
            for (int j = 0; j < numBubbles; ++j) {
                if (!bubbles[j].active) {
                    bubbles[j].x = fish[i].x + 0.045f; // Position at the fish's mouth
                    bubbles[j].y = fish[i].y;
                    bubbles[j].radius = 0.02f;
                    bubbles[j].speed = 0.005f;
                    bubbles[j].active = true;
                    break;
                }
            }
        }
    }

    // Update shark 1 (moving from left to right)
    sharkX1 += sharkDir1;
    if (sharkX1 > 1.2f) sharkX1 = -1.2f;

    // Update shark 2 (moving from right to left)
    sharkX2 += sharkDir2;
    if (sharkX2 < -1.2f) sharkX2 = 1.2f;

    // Update submarine
    submarinY2 += submarinDir;
    if (submarinY2 > 0.2f || submarinY2 < -0.2f) submarinDir = -submarinDir;

    // Update fish shadows
    for (int i = 0; i < numFishShadows; ++i) {
        fishShadows[i].x += fishShadows[i].speed;
        if (fishShadows[i].x > 1.0f) {
            fishShadows[i].x = -1.0f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(100, updateScene3, value + 1);
}

// Special key function to handle arrow keys
void specialKeysScene3(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            submarinSpeed += 0.01f;
            break;
        case GLUT_KEY_DOWN:
            submarinSpeed -= 0.01f;
            if (submarinSpeed < 0.0f) submarinSpeed = 0.0f;
            break;
    }
    glutPostRedisplay();
}

// Keyboard function to start the animation
void keyboardScene3(unsigned char key, int x, int y) {

    switch (key) {
        case 'w':
            submarinY += submarinSpeed;
            break;
        case 's':
            submarinY -= submarinSpeed;
            break;
        case 'a':
            submarinX -= submarinSpeed;
            break;
        case 'g':
            submarinX += submarinSpeed;
            break;
              case ' ': // Spacebar to stop
            submarinSpeed = 0.0f;


            break;
        case '\r': // Enter key to start

            submarinSpeed = 0.01f;
            break;
    }
    glutPostRedisplay();
}

// Scene Manager
enum Scene {
    SCENE_1,
    SCENE_2,
    SCENE_3
};

Scene currentScene = SCENE_1;

void renderCurrentScene() {
    switch (currentScene) {
        case SCENE_1:
            renderScene1();
            break;
        case SCENE_2:
            renderScene2();
            break;
        case SCENE_3:
            renderScene3();
            break;
    }
}
void switchScene(Scene newScene) {
    currentScene = newScene;
    // Additional logic for transitioning between scenes
    if (newScene == SCENE_2) {
        // Initialize the submarine's position for Scene 3
        submarineX = -0.9f;
        submarineY = 0.9f;
        submarineSpeed = 0.01f;
    }



    currentScene = newScene;
    // Additional logic for transitioning between scenes
    if (newScene == SCENE_3) {
        // Initialize the submarine's position for Scene 3
        submarinX = 0.0f;
        submarinY = 0.9f;
        submarinSpeed = 0.01f;
    }
}

void handleInput(unsigned char key, int x, int y) {
    switch (key) {
        case '1':
            switchScene(SCENE_1);
            break;
        case '2':
            switchScene(SCENE_2);
            break;
        case '3':
            switchScene(SCENE_3);
            break;
        default:
            if (currentScene == SCENE_1) {
                keyboardScene1(key, x, y);
            } else if (currentScene == SCENE_2) {
                keyboardScene2(key, x, y);
            } else if (currentScene == SCENE_3) {
                keyboardScene3(key, x, y);
            }
            break;
    }
}

void specialKeys(int key, int x, int y) {
    if (currentScene == SCENE_2) {
        specialKeysScene2(key, x, y);
    } else if (currentScene == SCENE_3) {
        specialKeysScene3(key, x, y);
    }
    // Add similar cases for Scene 1 if needed
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Scene Manager");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glutDisplayFunc(renderCurrentScene);
    glutTimerFunc(25, updateScene1, 0);
    glutTimerFunc(25, updateScene2, 0);
    glutTimerFunc(25, updateScene3, 0);
    glutKeyboardFunc(handleInput);
    glutSpecialFunc(specialKeys);

    initBubbles();

    glutMainLoop();
    return 0;
}
