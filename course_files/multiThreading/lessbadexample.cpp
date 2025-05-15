#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>

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
    int counter=0;
    void Run(void);
};

void SubThread::Run(void)
{
    for(;;)
    {
		mtx.lock();
        if(TASK_PRINT==task)
        {
            task=TASK_NONE;
            std::cout << "Do task!" << counter << std::endl;
        }
        else if(TASK_QUIT==task)
        {
            task=TASK_NONE;
	        mtx.unlock();
            break;
        }
        mtx.unlock();
    }
}

int main(void)
{
    class SubThread subThr;
    std::thread thr(&SubThread::Run,&subThr);
    for(int i=0; i<10; )
    {
		subThr.mtx.lock();
        if(SubThread::TASK_NONE==subThr.task)
        {
            subThr.task=SubThread::TASK_PRINT;
            ++subThr.counter;
            ++i;
        }
		subThr.mtx.unlock();
    }

	for(;;)
    {
		subThr.mtx.lock();
		auto taskCopy=subThr.task;
		subThr.mtx.unlock();
		if(SubThread::TASK_NONE==taskCopy)
		{
			break;
		}
	}

	subThr.mtx.lock();
    subThr.task=SubThread::TASK_QUIT;
	subThr.mtx.unlock();
    thr.join();

    return 0;
}

