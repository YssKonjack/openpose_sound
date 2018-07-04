#include <iostream>
#include <string>
#include <poseKeyPoints.hpp>

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

                if (1 == jrt){
                    calcJRT();
                    printJRT();
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
        std::cout << "(x, y, score)" << std::endl;
        for (int i = 0; i < keyPointNum; i++){
            std::cout << std::to_string(kpArray[i][0]) << ", " << std::to_string(kpArray[i][1]) << ", " << std::to_string(kpArray[i][2]) << std::endl;
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
}
