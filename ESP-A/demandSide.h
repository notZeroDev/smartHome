#ifndef DEMANDSIDE
#define DEMANDSIDE

// Potentiometer pins
extern const int potAC;
extern const int potHeater;
extern const int potLight;

// Load states
extern bool acOn;
extern bool heaterOn;
extern bool lightOn;

// Load values
extern int valAC;
extern int valHeater;
extern int valLight;

// Load LED states
extern bool pot1;
extern bool pot2;
extern bool pot3;

// Total load threshold
extern const int totalLoadThreshold;

void checkLoad();
void load_shed();



// Cascading Cutoff Function: AC → Heater → Light
bool cutOffLoad(int , int , int , int);
// Restoration Function: Light → Heater → AC
void restoreLoad(int , int , int , int ) ;
#endif
