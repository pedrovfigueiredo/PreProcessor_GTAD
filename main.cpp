//
//  main.cpp
//  VideoShots
//
//  Created by Pedro Figueirêdo on 15/02/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#include <iostream>

#include "time.hpp"
#include "VideoCuts.hpp"
#include "controller.hpp"


int main(int argc, const char * argv[]) {
    
    pid_t pid;
    
    Controller controller("/Users/PedroFigueiredo/Downloads/testecortado/video_segmentation","/Users/PedroFigueiredo/Downloads/testecortado/generate_shots.sh", "/Users/PedroFigueiredo/Downloads/testecortado/extract_frames.sh");
    
    if (!controller.GenerateKeyFrames("/Users/PedroFigueiredo/Downloads/testecortado/testecortado.mp4", "/Users/PedroFigueiredo/Downloads/testecortado/", 3)){
        std::cout << "Algo ruim aconteceu no GenerateKeyFrames...\nEncerrando...\n";
        exit(1);
     }
    
    
    if (!controller.GenerateShots("/Users/PedroFigueiredo/Downloads/testecortado/testecortado.mp4", "/Users/PedroFigueiredo/Downloads/testecortado/")){
         std::cout << "Algo ruim aconteceu no GenerateShots...\nEncerrando...\n";
         exit(1);
     }
    
    
    while ((pid = waitpid(-1, NULL, 0))) {
        if (errno == ECHILD) {
            break;
        }
    }
    
    return 0;
}
