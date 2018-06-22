
namespace oci{
    class threadGuard{
    private:
        std::thread& t;
    public:
        //暗黙呼び出しの禁止
        explicit threadGuard(std::thread& t_) :
            t(t_)
        {}

        //join()の呼び出しミス防止
        ~threadGuard() {
            if(t.joinable()){
                t.join();
            }
        }
        //コピーコンストラクタ
        threadGuard(threadGuard const&) = delete;
        //代入演算子
        threadGuard& operator=(threadGuard const&) = delete;
    };
}
