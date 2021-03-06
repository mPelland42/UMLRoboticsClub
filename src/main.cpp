//#include "drive.h"
//#include "pins.h"
//#include "motor.h"
//#include "point.h"
//#include "gpio.h"
//#include "lidar.h"
//#include "MPU6050.h"

//#include <stdexcept>
//#include <chrono>
//#include <unistd.h>
//#include <iostream>
//#include <cmath>

#include "robot.h"
#include "logger.h"

#include <pigpiod_if2.h>
#include <csignal>

void signalHandler(int){
    Robot::done = true;
    Logger::log("\nSignal caught, aborting...", Logger::HIGH);
}

int main() {
    signal(SIGINT , signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGFPE , signalHandler);
    signal(SIGILL , signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGHUP , signalHandler);

    Robot r;
    r.start();
}
//garbage
//while(running){
//    deque<int> scan = l.scan();
//    for(int i = 0; i < scan.size(); i++)
//        cout << scan[i] << ", ";
//    cout << endl;
//}
//Drive::mpu.initialize();
//Drive::OF.init();
//    signal(SIGINT , signalHandler);
//    signal(SIGABRT, signalHandler);
//    signal(SIGFPE , signalHandler);
//    signal(SIGILL , signalHandler);
//    signal(SIGSEGV, signalHandler);
//    signal(SIGTERM, signalHandler);
//    signal(SIGHUP , signalHandler);

//rotate();
// while(1){
// std::cin >> a >> b;
// drive(a, b);
//}
//Drive::drive(DoublePoint(1,0));
// Drive::rotate(60);
//drive(1, 0);
//drive(0, 1);
//drive(-1, 0);
//drive(0, -1);
// rotate();
//}
