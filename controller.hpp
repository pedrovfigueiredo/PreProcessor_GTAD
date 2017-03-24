//
//  controller.hpp
//  VideoShots
//
//  Created by Pedro Figueirêdo on 15/02/17.
//  Copyright © 2017 Lavid. All rights reserved.
//

#ifndef controller_hpp
#define controller_hpp

#define SHOTS_SRT 0
#define SHOTS_TXT 1
#define SCENES_SRT 2
#define SCENES_TXT 3

#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "VideoCuts.hpp"


class Controller
{
    
    // Paths
    std::string shotDetectionScriptPath_ = "";
    std::string shotGenerationScriptPath_ = "";
    std::string keyframesExtractionScriptPath_ = "";
    
    //Internal Methods:
    bool ComputeVideoCuts(std::vector<VideoCuts> &videoCuts, std::string srt_path, std::string txt_path);
    bool getTimes(std::vector<Time> &start_time, std::vector<Time> &end_time, std::string path);
    bool getFrames(std::vector<int> &start_frame, std::vector<int> &end_frame,
                   std::vector< std::vector<int> >&keyframes, std::string path);
    std::string getScriptFilesPath(std::string inputPath, int fileType);
    std::string retrieveFileName(std::string inputPath);
    float getTimeInSeconds(int hours, int minutes, int seconds, int miliseconds);
    
    // Script Calls
    bool ExecuteShotsDetectionScript(std::string videoPath);
    bool ExecuteShotsGenerationScript(std::string inputPath,std::string outputPath, float initialTime, float endTime);
    
public:
    
    Controller();
    Controller(std::string shotDetectionScriptPath, std::string shotGenerationScriptPath, std::string keyframesExtractionScriptPath);
    ~Controller();
    
    //Available External Methods:
    bool GenerateKeyFrames(std::string inputPath, std::string outputPath, int framesPerSecond);
    bool GenerateShots(std::string inputPath, std::string outputPath);
    bool GenerateShots(std::string inputPath, std::string outputPath, int shotnumber);
    bool GenerateShots(std::string inputPath, std::string outputPath, int inicialShot, int finalShot);
};

#endif /* controller_hpp */
