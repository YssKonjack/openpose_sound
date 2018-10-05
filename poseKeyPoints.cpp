#include <iostream>
#include <string>
#include "poseKeyPoints.hpp"

namespace pkp{

    //jrt = 0:PoseKeyPoints jrt = 1:JRT
    void PoseKeyPoints::readData(int jrt){
        std::string input;
        int dataCount = 0;
        int personCount = 0;

        while(1){
            std::cin >> input;
            if("start" == input) startFlg = true;
            if("quit" == input) {
                startFlg = false;
                break;
            }

            if("reset" == input){
                dataCount = 0;
                personCount = 0;
                frameCnt++;

                calcJRT();

                if (1 == jrt){
                    std::cout << std::endl;
                    printJRT();

                    // int x = jrtPositiveX();
                    // int y = jrtPositiveY();
                    // std::cout << x << " " << y << std::endl;

                }else if(0 == jrt){
                    print();
                }

                reset();

            }else{
                if ("Person" == input){
                    personCount += 1;
                }else {
                    if (1 == personCount){//一人分のデータのみを使用する
                        setValue(input, dataCount / varNum(), dataCount % varNum() );
                        dataCount++;
                    }
                }
            }
        }
    }

    void PoseKeyPoints::print() const {
        std::cout << " POSE_COCO_BODY_PARTS {" << std::endl <<
            "{0,  Nose}," << std::endl <<
            "{1,  Neck}," << std::endl <<
            "{2,  RShoulder}," << std::endl <<
            "{3,  RElbow}," << std::endl <<
            "{4,  RWrist}," << std::endl <<
            "{5,  LShoulder}," << std::endl <<
            "{6,  LElbow}," << std::endl <<
            "{7,  LWrist}," << std::endl <<
            "{8,  RHip}," << std::endl <<
            "{9,  RKnee}," << std::endl <<
            "{10, RAnkle}," << std::endl <<
            "{11, LHip}," << std::endl <<
            "{12, LKnee}," << std::endl <<
            "{13, LAnkle}," << std::endl <<
            "{14, REye}," << std::endl <<
            "{15, LEye}," << std::endl <<
            "{16, REar}," << std::endl <<
            "{17, LEar}," << std::endl <<
            "{18, Background}," << std::endl <<
        "}" << std::endl << std::endl;

        std::cout << "(x, y, score)" << std::endl;
        for (int i = 0; i < keyPointNum; i++){
            std::cout << i << " " << std::to_string(kpArray[i][0]) << ", " << std::to_string(kpArray[i][1]) << ", " << std::to_string(kpArray[i][2]) << std::endl;
        }
        std::cout << std::endl;
    }

    void PoseKeyPoints::reset(){
        for(int i = 0; i < keyPointNum; i++){
            for(int j = 0; j < variableNum; j++){
                kpArray[i][j] = 0.0;
            }
        }
    }

    void PoseKeyPoints::calcJRT(){
        for(int i = 0; i < keyPointNum-1; i++){
            for(int j = i+1; j < keyPointNum; j++){
                if(0.0 == kpArray[i][0] || 0.0 == kpArray[j][0]){
                    jrtArray[i][j] = 0;
                }else{
                    jrtArray[i][j] = kpArray[i][0] > kpArray[j][0] ? 1 : -1;
                }
                if(0.0 == kpArray[j][1] || 0.0 == kpArray[i][1]){
                    jrtArray[j][i] = 0.0;
                }else{
                    jrtArray[j][i] = kpArray[j][1] > kpArray[i][1] ? 1 : -1;
                }
            }
        }
    }


    void PoseKeyPoints::printJRT() const {
        using std::cout;
        using std::endl;

        cout << "x-coordinate" << endl;
        for(int i=0; i < keyPointNum-1; i++){
            for(int k=0; k < i+1; k++){
                cout << "   ";
            }
            for(int j = i+1; j < keyPointNum; j++){
                if(jrtArray[i][j] > -1){
                    cout << " " << jrtArray[i][j] << " ";
                }else{
                    cout << jrtArray[i][j] << " ";
                }
            }
            cout << endl;
        }
        cout << endl;

        cout << "y-coordinate" << endl;
        for(int i=0; i < keyPointNum-1; i++){
            for(int k=0; k < i+1; k++){
                cout << "   ";
            }
            for(int j = i+1; j < keyPointNum; j++){
                if(jrtArray[j][i] > -1){
                    cout << " " << jrtArray[j][i] << " ";
                }else{
                    cout << jrtArray[j][i] << " ";
                }
            }
            cout << endl;
        }
    }

    int PoseKeyPoints::jrtPositiveX(){
        int cnt = 0;
        for(int i=0; i < keyPointNum-1; i++){
            for(int j = i+1; j < keyPointNum; j++){
                if(1 == jrtArray[i][j]){
                    cnt += 1;
                }
            }
        }
        return cnt;
    }


    int PoseKeyPoints::jrtPositiveY(){
        int cnt;
        for(int i=0; i < keyPointNum-1; i++){
            for(int j = i+1; j< keyPointNum; j++){
                if(1 == jrtArray[j][i]){
                    cnt +=1;
                }
            }
        }
        return cnt;
    }

}
