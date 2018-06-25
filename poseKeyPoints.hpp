#ifndef POSEKEYPOINTS_H
#define POSEKEYPOINTS_H

namespace pkp{
    class PoseKeyPoints {
    private:
        //18個の関節
        const static int keyPointNum = 18;
        //x,y,scoreの3つを記録
        const static int variableNum = 3;

        float kpArray[keyPointNum][variableNum];

        int jrtArray[keyPointNum][keyPointNum];

    public:
        PoseKeyPoints() :
            kpArray(),
            jrtArray()
        {}

        inline void setValue(std::string value, int kpoint, int var){
            kpArray[kpoint][var] = std::stof(value);
        }
        inline int kpNum() const {return keyPointNum;}
        inline int varNum() const {return variableNum;}
        void readData(int jrt);
        void print() const;
        void reset();

        void calcJRT();
        void printJRT() const;

    };
}


#endif

// POSE_COCO_BODY_PARTS {
//     {0,  "Nose"},
//     {1,  "Neck"},
//     {2,  "RShoulder"},
//     {3,  "RElbow"},
//     {4,  "RWrist"},
//     {5,  "LShoulder"},
//     {6,  "LElbow"},
//     {7,  "LWrist"},
//     {8,  "RHip"},
//     {9,  "RKnee"},
//     {10, "RAnkle"},
//     {11, "LHip"},
//     {12, "LKnee"},
//     {13, "LAnkle"},
//     {14, "REye"},
//     {15, "LEye"},
//     {16, "REar"},
//     {17, "LEar"},
//     {18, "Background"},
// }
