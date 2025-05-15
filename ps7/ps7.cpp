#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "fssimplewindow.h"
#include "ysdelaunaytri.h"

const double YsPI=3.1415927;
const int nThreads=8;

// Thread synchronization variables
std::mutex mtx;
std::condition_variable cv_start, cv_done;
bool ready_to_start = false;
int threads_done = 0;

class BubbleForcePair
{
public:
    size_t i; // Index to a bubble.
    YsVec2 f; // Force to be added.
};

class BubblePacking
{
public:
    const double h=0.03;
    const double m=1.0;
    const double overlap=1.1;
    const double eps=1e-6;
    const double C=4.0; // Damping coefficient
    const double K=0.5; // Spring coefficient

    class Bubble
    {
    public:
        YsVec2 pos,vel,f;
        double rad;
        bool fixed=false;
        int groupId=0;
    };

private:
    std::vector<Bubble> bubbles;
    std::vector<std::vector<BubbleForcePair>> deferredForces;

public:
    BubblePacking() {
        // Initialize deferred forces vectors for each thread
        deferredForces.resize(nThreads);
    }

    void PackOnCorners(void);
    void PackOnBoundary(void);
    void PackInside(void);

    void AssignGroupId(void);

    void ClearForce(void);
    void ComputeInterBubbleForce(void);
    void ComputeInterBubbleForce(int groupId, std::vector<BubbleForcePair> &forcePairs);
    void ApplyBubbleForcePairs(void);
    void ComputeDampingForce(void);
    void Move(double dt);

    void MakeDelaunay(YsUnconstrainedDelaunayTriangulation &tri);
    void Render(void) const;
    static void DrawCircle(YsVec2 cen,double rad);

    // Thread function
    static void ThreadFunc(BubblePacking *packing, int threadId);
};

void BubblePacking::PackOnCorners(void)
{
    Bubble bub;
    bub.rad=h/2;
    bub.vel=YsVec2::Origin();
    bub.f=YsVec2::Origin();
    bub.fixed=true;

    bub.pos.Set(-1.0,-1.0);
    bubbles.push_back(bub);
    bub.pos.Set( 1.0,-1.0);
    bubbles.push_back(bub);
    bub.pos.Set( 1.0, 1.0);
    bubbles.push_back(bub);
    bub.pos.Set(-1.0, 1.0);
    bubbles.push_back(bub);
}

void BubblePacking::PackOnBoundary(void)
{
    YsVec2 corner[5]=
    {
        YsVec2(-1.0,-1.0),
        YsVec2( 1.0,-1.0),
        YsVec2( 1.0, 1.0),
        YsVec2(-1.0, 1.0),
        YsVec2(-1.0,-1.0),
    };
    for(int i=0; i<4; ++i)
    {
        double d=(corner[i+1]-corner[i]).GetLength();
        int n=int(d*overlap/h);
        for(int j=1; j<n; ++j)
        {
            double t=double(j)/double(n);
            auto pos=corner[i]*(1.0-t)+corner[i+1]*t;

            Bubble bub;
            bub.pos=pos;
            bub.rad=h/2;
            bub.vel=YsVec2::Origin();
            bub.f=YsVec2::Origin();
            bub.fixed=true;

            bubbles.push_back(bub);
        }
    }
}

void BubblePacking::PackInside(void)
{
    int n=int(2.0*overlap/h);
    n*=n;
    for(int i=0; i<n; ++i)
    {
        double x=rand();
        double y=rand();
        x/=double(RAND_MAX);
        y/=double(RAND_MAX);
        x*=2.0;
        y*=2.0;
        x-=1.0;
        y-=1.0;
        x*=(1.0-h);
        y*=(1.0-h);

        Bubble bub;
        bub.pos.Set(x,y);
        bub.rad=h/2;
        bub.vel=YsVec2::Origin();
        bub.f=YsVec2::Origin();
        bub.fixed=false;

        bubbles.push_back(bub);
    }
}

void BubblePacking::AssignGroupId(void)
{
    // Assign group IDs based on X coordinate (0-7)
    for(auto &bub : bubbles)
    {
        // Map x from [-1.0,1.0] to [0,nThreads-1]
        double normalizedX = (bub.pos.x() + 1.0) / 2.0; // [0.0,1.0]
        int groupId = (int)(normalizedX * nThreads);
        
        // Ensure the groupId is within range [0,nThreads-1]
        if(groupId >= nThreads)
            groupId = nThreads - 1;
        if(groupId < 0)
            groupId = 0;
            
        bub.groupId = groupId;
    }
}

void BubblePacking::ClearForce(void)
{
    for(auto &bub : bubbles)
    {
        bub.f=YsVec2::Origin();
    }
    
    // Clear deferred forces
    for(auto &forcePairs : deferredForces)
    {
        forcePairs.clear();
    }
}

void BubblePacking::ComputeInterBubbleForce(void)
{
    // Create threads
    std::thread threads[nThreads];
    
    // Reset thread synchronization
    ready_to_start = false;
    threads_done = 0;
    
    // Start worker threads
    for(int i = 0; i < nThreads; ++i)
    {
        threads[i] = std::thread(ThreadFunc, this, i);
    }
    
    // Notify threads to start computation
    {
        std::unique_lock<std::mutex> lock(mtx);
        ready_to_start = true;
        cv_start.notify_all();
    }
    
    // Wait for all threads to finish
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv_done.wait(lock, []{return threads_done >= nThreads;});
    }
    
    // Apply deferred forces
    ApplyBubbleForcePairs();
    
    // Join all threads
    for(int i = 0; i < nThreads; ++i)
    {
        threads[i].join();
    }
}

void BubblePacking::ThreadFunc(BubblePacking *packing, int threadId)
{
    // Wait for signal to start
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv_start.wait(lock, []{return ready_to_start;});
    }
    
    // Compute forces for assigned group
    packing->ComputeInterBubbleForce(threadId, packing->deferredForces[threadId]);
    
    // Signal completion
    {
        std::unique_lock<std::mutex> lock(mtx);
        threads_done++;
        if(threads_done >= nThreads)
        {
            cv_done.notify_one();
        }
    }
}

void BubblePacking::ComputeInterBubbleForce(int groupId, std::vector<BubbleForcePair> &forcePairs)
{
    for(size_t i=0; i<bubbles.size(); ++i)
    {
        auto &bubA=bubbles[i];
        
        // Skip if bubble doesn't belong to this thread's group
        if(bubA.groupId != groupId)
            continue;
            
        for(size_t j=i+1; j<bubbles.size(); ++j)
        {
            auto &bubB=bubbles[j];
            if(true!=bubB.fixed || true!=bubA.fixed)
            {
                auto dist=(bubA.pos-bubB.pos).GetLength();
                auto refDist=bubA.rad+bubB.rad;
                if(dist<refDist)
                {
                    double x=dist/refDist;
                    double y=K*(1.25*x*x*x-2.375*x*x+1.125);

                    YsVec2 u=bubB.pos-bubA.pos;
                    double l=u.GetLength();
                    if(l<eps)
                    {
                        u.SetX(1);
                    }
                    else
                    {
                        u/=l;
                    }
                    
                    // Add force to bubA (belongs to this thread's group)
                    bubA.f-=u*y;
                    
                    // If bubB belongs to same group, add force directly
                    if(bubB.groupId == groupId)
                    {
                        bubB.f+=u*y;
                    }
                    else
                    {
                        // Defer the force addition to bubB
                        BubbleForcePair pair;
                        pair.i = j;
                        pair.f = u*y;
                        forcePairs.push_back(pair);
                    }
                }
            }
        }
    }
}

void BubblePacking::ApplyBubbleForcePairs(void)
{
    // Apply all deferred forces
    for(int i = 0; i < nThreads; ++i)
    {
        for(const auto &pair : deferredForces[i])
        {
            bubbles[pair.i].f += pair.f;
        }
    }
}

void BubblePacking::ComputeDampingForce(void)
{
    for(auto &bub : bubbles)
    {
        bub.f-=C*bub.vel;
    }
}

void BubblePacking::Move(double dt)
{
    for(auto &bub : bubbles)
    {
        if(true!=bub.fixed)
        {
            bub.pos+=bub.vel*dt;
            bub.vel+=(bub.f/m)*dt;
        }
    }
}

void BubblePacking::Render(void) const
{
    for(auto &bub : bubbles)
    {
        DrawCircle(bub.pos,bub.rad);
    }
}

void BubblePacking::DrawCircle(YsVec2 cen,double rad)
{
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<32; ++i)
    {
        double a=YsPI*double(i)/16.0;
        double c=cos(a);
        double s=sin(a);
        glVertex2d(cen.x()+c*rad,cen.y()+s*rad);
    }
    glEnd();
}

void BubblePacking::MakeDelaunay(YsUnconstrainedDelaunayTriangulation &tri)
{
    tri.CleanUp();
    const int iniNode[4]=
    {
        tri.AddInitialNode(YsVec2(-2.0,-2.0)),
        tri.AddInitialNode(YsVec2( 2.0,-2.0)),
        tri.AddInitialNode(YsVec2( 2.0, 2.0)),
        tri.AddInitialNode(YsVec2(-2.0, 2.0)),
    };
    const int iniTri[2][3]=
    {
        {iniNode[0],iniNode[1],iniNode[2]},
        {iniNode[2],iniNode[3],iniNode[0]}
    };
    tri.AddInitialTri(iniTri[0]);
    tri.AddInitialTri(iniTri[1]);
    for(auto &bub : bubbles)
    {
        tri.InsertNode(bub.pos);
    }
}

int main(void)
{
    FsOpenWindow(0,0,720,720,1);

    srand(time(nullptr));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.1,1.1,-1.1,1.1,-1.1,1.1);

    bool pause=false;

    YsUnconstrainedDelaunayTriangulation tri;

    BubblePacking packing;
    packing.PackOnCorners();
    packing.PackOnBoundary();
    packing.PackInside();
    packing.AssignGroupId(); // Assign group IDs initially

    for(;;)
    {
        FsPollDevice();
        auto key=FsInkey();
        if(FSKEY_ESC==key)
        {
            break;
        }
        if(FSKEY_SPACE==key)
        {
            packing.MakeDelaunay(tri);
            pause=!pause;
        }

        if(true!=pause)
        {
            packing.AssignGroupId(); // Update group IDs as bubbles move
            for(int i=0; i<10; ++i)
            {
                packing.ClearForce();
                packing.ComputeInterBubbleForce();
                packing.ComputeDampingForce();
                packing.Move(0.01);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        if(true!=pause)
        {
            packing.Render();
        }
        else
        {
            // If paused, show Delaunay triangulation.
            glBegin(GL_LINES);
            for(int i=0; i<tri.GetNumTri(); ++i)
            {
                YsVec2 p[3];
                tri.GetTriangleNodePos(p,i);
                glVertex2dv(p[0]);
                glVertex2dv(p[1]);

                glVertex2dv(p[1]);
                glVertex2dv(p[2]);

                glVertex2dv(p[2]);
                glVertex2dv(p[0]);
            }
            glEnd();
        }
        FsSwapBuffers();
    }

    return 0;
}