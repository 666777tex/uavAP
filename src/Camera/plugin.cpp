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
    float yaw;
};

static XPLMDataRef gLocalX = nullptr;
static XPLMDataRef gLocalY = nullptr;
static XPLMDataRef gLocalZ = nullptr;
static XPLMDataRef gPitch = nullptr;
static XPLMDataRef gRoll = nullptr;
static XPLMDataRef gYaw = nullptr;

static void InitDatarefs(){
    gLocalX = XPLMFindDataRef('sim/flightmodel/position/local_x');
    gLocalY = XPLMFindDataRef('sim/flightmodel/position/local_y');
    gLocalZ = XPLMFindDataRef('sim/flightmodel/position/local_z');
    gPitch = XPLMFindDataRef('sim/flightmodel/position/theta');
    gRoll = XPLMFindDataRef('sim/flightmodel/position/phi');
    gYaw = XPLMFindDataRef('sim/flightmodel/position/psi');
}

static PlanePose ReadPlanePose (){
    PlanePose p;
    p.x = XPLMGetDatad_f(gLocalX);
    p.y = XPLMGetDatad_f(gLocalY);
    p.z = XPLMGetDatad_f(gLocalZ);
    p.pitch = XPLMGetDatad_f(gPitch);
    p.roll = XPLMGetDatad_f(gRoll);
    p.yaw = XPLMGetDatad_f(gYaw);
    return p;
}

static int CamPosition(
    XPLMCameraPosition_t *outCameraPosition,
    int losingControl,
    void* inRefCon
){
    if (losingControl == 0 || outCameraPosition == nullptr)
        return 0;
    
    PlanePose plane = ReadPlanePose();

    outCameraPosition->x = static_cast<float>(plane.x);
    outCameraPosition->y = static_cast<float>(plane.y); // minus the difference, differ from models 
    outCameraPosition->z = static_cast<float>(plane.z);

}