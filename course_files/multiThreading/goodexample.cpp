#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>
#include <condition_variable>

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
    std::mutex mtx;
    std::condition_variable cond;
    int counter=0;
    void Run(void);
};

void SubThread::Run(void)
{
    for(;;)
    {
		std::unique_lock <std::mutex> lock(mtx);

		cond.wait(lock,[&]{return TASK_NONE!=task;});

        if(TASK_PRINT==task)
        {
            task=TASK_NONE;
            std::cout << "Do task!" << counter << std::endl;
            cond.notify_one();
        }
        else if(TASK_QUIT==task)
        {
            task=TASK_NONE;
            cond.notify_one();
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
		subThr.mtx.lock();
		subThr.counter=i;
        subThr.task=SubThread::TASK_PRINT;
        subThr.mtx.unlock();

		subThr.cond.notify_one();

		std::unique_lock <std::mutex> lock(subThr.mtx);
		subThr.cond.wait(lock,[&]{return subThr.task==SubThread::TASK_NONE;});
    }

	subThr.mtx.lock();
    subThr.task=SubThread::TASK_QUIT;
	subThr.mtx.unlock();
	subThr.cond.notify_one();
    thr.join();

    return 0;
}

