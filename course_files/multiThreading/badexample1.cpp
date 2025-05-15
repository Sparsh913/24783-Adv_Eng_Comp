#include <thread>
#include <chrono>
#include <iostream>

class SubThread
{
public:
    enum
    {
        TASK_NONE,
        TASK_PRINT,
        TASK_QUIT,
    };
    int task=TASK_NONE;
    void Run(void);
};

void SubThread::Run(void)
{
    for(;;)
    {
        if(TASK_PRINT==task)
        {
            task=TASK_NONE;
            std::cout << "Do task!" << std::endl;
        }
        else if(TASK_QUIT==task)
        {
            task=TASK_NONE;
            break;
        }
    }
}

int main(void)
{
    class SubThread subThr;
    std::thread thr(&SubThread::Run,&subThr);
    for(int i=0; i<10; ++i)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if(SubThread::TASK_NONE==subThr.task)
        {
            subThr.task=SubThread::TASK_PRINT;
        }
    }
    subThr.task=SubThread::TASK_QUIT;
    thr.join();
    return 0;
}

