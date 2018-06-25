#ifndef PS_THREAD_H
#define PS_THREAD_H

#include <thread>
#include <string>
#include <iostream>

namespace ps{
    class threadGuard{
    private:
        std::thread& t;
        std::string id;

    public:
        //暗黙呼び出しの禁止
        explicit threadGuard(std::thread& t_,std::string id_) :
            t(t_),
            id(id_)
        {}

        //join()の呼び出しミス防止
        ~threadGuard() {
            if(t.joinable()){
                t.join();
                std::cout << "thread " << id << " join" << std::endl;
            }
        }
        //コピーコンストラクタ
        threadGuard(threadGuard const&) = delete;
        //代入演算子
        threadGuard& operator=(threadGuard const&) = delete;
    };
}

#endif
