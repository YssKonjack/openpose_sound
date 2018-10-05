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

        bool startFlg;

        int frameCnt;

    public:
        PoseKeyPoints() :
            kpArray(),
            jrtArray(),
            startFlg(false),
            frameCnt(0)
        {}

        inline void setValue(std::string value, int kpoint, int var){
            kpArray[kpoint][var] = std::stof(value);
        }
        inline float getKpVal(int joint, int var) const {
            return kpArray[joint][var];
        }
        inline int kpNum() const {return keyPointNum;}
        inline int varNum() const {return variableNum;}
        inline int stFlg() const {return startFlg;}
        inline int frmCnt() const {return frameCnt;}

        void readData(int jrt);
        void print() const;
        void reset();

        void calcJRT();
        void printJRT() const;

        int jrtPositiveX();
        int jrtPositiveY();




    };
}


#endif
