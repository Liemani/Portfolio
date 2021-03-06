#include "MissionHandler_CrossMove.h"

#define state_landed 0
#define state_takingOff 1
#define state_hovering 2
#define state_flying 3
#define state_landing 4
#define state_emergency 5
#define state_userTakeOff 6
#define state_motorRamping 7
#define state_emergencyLanding 8

#define state_noMission 0
#define state_onMission 1





using std::string;





// member function
MissionPlanner::MissionPlanner(int argc, char** argv, ros::NodeHandle* pNodeHandle, int* pCommand) {
    ros::init(argc, argv, "a15_mission_planner_node2");    // node name
    this->pNodeHandle = pNodeHandle;

    this->pCommand = pCommand;

    Drone* drone1 = new Drone(pNodeHandle, "bebop1");
    Drone* drone2 = new Drone(pNodeHandle, "bebop2");
    Drone* drone3 = new Drone(pNodeHandle, "bebop3");

    pDrone_vector.push_back(drone1);
    pDrone_vector.push_back(drone2);
    pDrone_vector.push_back(drone3);

    missionState = state_noMission;
    mission = NULL;
}

double MissionPlanner::getTargetLatitude() {
    return targetGlobalPosition.latitude;
}

double MissionPlanner::getTargetLongitude() {
    return targetGlobalPosition.longitude;
}

double MissionPlanner::getTargetAltitude() {
    return targetGlobalPosition.altitude;
}



// return:
//  true: key did used
//  false: key didn't used
void MissionPlanner::processCommand() {
    switch(*pCommand) {
        case 122:    // save current position of first drone to savedGlobalPosition
            targetGlobalPosition = pDrone_vector[0]->getGlobalPosition();
            printf("================ \n");
            printf("Position Saved!! \n");
            printf("  Latitude : %0.12f \n", targetGlobalPosition.latitude);
            printf("  Longitude: %0.12f \n", targetGlobalPosition.longitude);
            printf("  Altitude : %0.12f \n", targetGlobalPosition.altitude);
            printf("Cartesian Coordinate \n");
            printf("  x: %0.12f \n", pDrone_vector[0]->getMatchingX());
            printf("  y: %0.12f \n", pDrone_vector[0]->getMatchingY());
            printf("  z: %0.12f \n", pDrone_vector[0]->getMatchingZ());
            printf("------------------------- \n");

            *pCommand = 0;
            break;
        case 120:    // active cross move mission
            if (mission) delete mission;
            mission = new Mission_CrossMove(pDrone_vector[1]->getAltitude() + 0.5);
            
            missionState = state_onMission;
            pDrone_vector[0]->setMission(mission);
            pDrone_vector[1]->setMission(mission);
            pDrone_vector[2]->setMission(mission);

            *pCommand = 0;
            break;
        case 99:    // print debug description
            debugDescription();

            *pCommand = 0;
            break;
        case 113:    // reset drone[0]
            pDrone_vector[0]->reset();

            *pCommand = 0;
            break;
        default:
            break;
    }
}

void MissionPlanner::doMission() {
    if (missionState == state_onMission) {
        if (mission->perform(pDrone_vector)) missionState = state_noMission;
    }
}

void MissionPlanner::debugDescription() {
    for (int i = 0; i < pDrone_vector.size(); ++i) {
        printf("=========================== \n");
        printf("Mission planner description \n");
        printf("Name: %s \n", pDrone_vector[i]->getName().c_str());
        printf("Global Position \n");
        printf("  Latitude  : %.12f \n", pDrone_vector[i]->getLatitude());
        printf("  Longitude : %.12f \n", pDrone_vector[i]->getLongitude());
        printf("  Altitude  : %.12f \n", pDrone_vector[i]->getAltitude());
        printf("  Bearing   : %.12f \n", pDrone_vector[i]->getBearing());
        printf("Cartesian coordinate \n");
        printf("  x : %.12f \n", pDrone_vector[i]->getCalculatedX());
        printf("  y : %.12f \n", pDrone_vector[i]->getCalculatedY());
        printf("  z : %.12f \n", pDrone_vector[i]->getCalculatedZ());
        printf("Flying state: %d \n", pDrone_vector[i]->getFlyingState());
        printf("----------------------- \n");
    }

    if (mission) mission->debugDescription();
    // if (pDrone_vector[0]) pDrone_vector[0]->debugDescription();
}

void MissionPlanner::loop() {
    this->processCommand();
    this->doMission();
}
