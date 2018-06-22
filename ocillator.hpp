#ifndef OCILLATOR_H
#define OCILLATOR_H


namespace oci{
    class Ocillator{
    public:
        Ocillator() :
            sampleFreq(44100),
            Amp(10000),
            Freq(0),
            finFlag(1)

        {}

        void player();
        void ocillator();

    private:
        const double sampleFreq;
        double Amp;
        double Freq;

        static FILE *fp;
        int finFlag;


    };
}

#endif
