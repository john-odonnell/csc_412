/**
 * URI CSC 412 :: Version 1 of Program 5
 * 
 * DISPATCHER PROGRAM
 * Responsibilites
 * - receive lists of tasks in the form of job files
 * - dispatch jobs to utility programs
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * MAIN FUNCTION
 * 
 * ARGUMENTS
 * - path to file containing list of tasks
 * - path to image files to operate on
 * - path to output folder
 *
 * RETURN
 * - 0 if success
 * - ERROR_CODE if failure
 */
int main(int argc, char* argv[]) {
    // print usage statement if used incorrectly
    if (argc < 4) {
        std::cout << "Usage:\t./<executable> <filepath>" << std::endl;
    }

    // print arguments : debug
    for (int i = 0; i < argc; i++) {
        std::cout << "Arg" << i << ": " << argv[i] << std::endl;
    }

    // open file
    std::ifstream tasksFile;
    tasksFile.open(argv[1]);

    // process each line, unless line is end command
    std::string task;
    std::string imageDir = argv[2];
    std::string outDir = argv[3];


    while (std::getline(tasksFile, task)) {

        std::cout << task << std::endl;

        if (task == "end") {
            std::cout << "END received. Terminating." << std::endl;
            exit(0);
        } else {
            // turn the task string into a stream
            std::istringstream stream(task);

            std::string utility;
            std::string image;
            std::string imagePath;

            // handle each task based on it's command
            // rotate
            if (task.substr(0, 6) == "rotate") {
                std::string flag;
                if (!(stream >> utility >> flag >> image)) { break; }

                imagePath = imageDir + image;

                std::cout << imagePath << " " << flag << std::endl;

                int p = fork();
                if (p == 0) {
                    std::cout << "Fork successful. Rotating..." << std::endl;
                    execl("../rotate.o", "../rotate.o", flag.c_str(), imagePath.c_str(), outDir.c_str());
                } else if (p < 0) {
                    std::cout << "Error while forking for Rotation process. Exiting." << std::endl;
                    exit(1);
                }
            }

            // crop
            else if (task.substr(0, 4) == "crop") {
                int x, y, w, h;
                std::string xS, yS, wS, hS;
                if (!(stream >> utility >> image >> x >> y >> w >> h)) { break; }
                xS = std::to_string(x);
                yS = std::to_string(y);
                wS = std::to_string(w);
                hS = std::to_string(h);

                imagePath = imageDir + image;

                std::cout << imagePath << " " << x << " " << y << " " << w << " " << h << std::endl;

                int p = fork();
                if (p == 0) {
                    std::cout << "Fork successful. Cropping..." << std::endl;
                    execl("../crop.o", "../crop.o", imagePath.c_str(), outDir.c_str(), xS.c_str(), yS.c_str(), wS.c_str(), hS.c_str());
                } else if (p < 0) {
                    std::cout << "Error while forking for Cropping process. Exiting." << std::endl;
                    exit(1);
                }
            }

            // flipV
            else if (task.substr(0, 5) == "flipV") {
                if (!(stream >> utility >> image)) { break; }

                imagePath = imageDir + image;

                std::cout << imagePath << std::endl;

                int p = fork();
                if (p == 0) {
                    std::cout << "Fork successful. Flipping vertically..." << std::endl;
                    execl("../flipV.o", "../flipV.o", imagePath.c_str(), outDir.c_str());
                } else if (p < 0) {
                    std::cout << "Error while forking for Vertical Flip process. Exiting." << std::endl;
                    exit(1);
                }
            }

            // flipH
            else if (task.substr(0, 5) == "flipH") {
                if (!(stream >> utility >> image)) { break; }

                imagePath = imageDir + image;

                std::cout << imagePath << std::endl;

                int p = fork();
                if (p == 0) {
                    std::cout << "Fork successful. Flipping horizontally..." << std::endl;
                    execl("../flipH.o", "../flipH.o", imagePath.c_str(), outDir.c_str());
                } else if (p < 0) {
                    std::cout << "Error while forking for Horizontal Flip process. Exiting." << std::endl;
                    exit(1);
                }
            }

            // gray
            else if (task.substr(0, 4) == "gray") {
                if (!(stream >> utility >> image)) { break; }

                imagePath = imageDir + image;

                std::cout << imagePath << std::endl;

                int p = fork();
                if (p == 0) {
                    std::cout << "Fork successful. Grayscale..." << std::endl;
                    execl("../gray.o", "../gray.o", imagePath.c_str(), outDir.c_str());
                } else if (p < 0) {
                    std::cout << "Error while forking for Gray process. Exiting." << std::endl;
                    exit(1);
                }
            }
        }
    }

    return 0;
}