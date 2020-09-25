#include <string>

#include <Drone.h>

#include "CustomMissionHandler.h"
#include "Mission/CustomCoreMission.h"
#include "Mission/CustomConcreteMission.h"

using std::string;





void CustomMissionHandler::processCommand() {
    // action code for command goes here...
}





// public
CustomMissionHandler::CustomMissionHandler(int argc, char** argv)
: MissionHandler(argc, argv, "a17_empty_node") {
    // initializing drone code here...
}
