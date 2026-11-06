#include "plugin.h"

#include <cstring>
#include <uavEE/extern/xPlane/CHeaders/XPLM/XPLMCamera.h>
#include <uavEE/extern/xPlane/CHeaders/XPLM/XPLMDataAccess.h>


// get the position of the plane, x,y,z,roll,pitch,yaw, then use that to find a good place to put the camera at, using the json
// probably have a json for each model. 

// how did he write it to connect to xplane

struct PlanePose{
    double x,y,z;
    float pitch;
    float roll;
    float heading;
};

static XPLMDataRef gLocalX = nullptr;
static XPLMDataRef gLocalY = nullptr;
static XPLMDataRef gLocalZ = nullptr;
static XPLMDataRef gPitch = nullptr;
static XPLMDataRef gRoll = nullptr;
static XPLMDataRef gHeading = nullptr;

static void InitDatarefs(){
    gLocalX = XPLMFindDataRef("sim/flightmodel/position/local_x");
    gLocalY = XPLMFindDataRef("sim/flightmodel/position/local_y");
    gLocalZ = XPLMFindDataRef("sim/flightmodel/position/local_z");
    gPitch = XPLMFindDataRef("sim/flightmodel/position/theta");
    gRoll = XPLMFindDataRef("sim/flightmodel/position/phi");
    gHeading = XPLMFindDataRef("sim/flightmodel/position/psi");
}

static PlanePose ReadPlanePose (){
    PlanePose p;
    p.x = XPLMGetDataf(gLocalX);
    p.y = XPLMGetDataf(gLocalY);
    p.z = XPLMGetDataf(gLocalZ);
    p.pitch = XPLMGetDataf(gPitch);
    p.roll = XPLMGetDataf(gRoll);
    p.heading = XPLMGetDataf(gHeading);
    return p;
}

static int CamPosition(
    XPLMCameraPosition_t *outCameraPosition,
    int losingControl,
    void* inRefCon
){
    if (losingControl != 0 || outCameraPosition == nullptr)
        return 0;
    
    PlanePose plane = ReadPlanePose();

    outCameraPosition->x = plane.x;
    outCameraPosition->y = plane.y - 5.0f; // minus the difference, differ from models 
    outCameraPosition->z = plane.z;
    outCameraPosition->pitch = plane.pitch - 90.0f;
    outCameraPosition->roll = plane.roll;
    outCameraPosition->heading = plane.heading;
    outCameraPosition->zoom = 1.0f;

    return 1;
}