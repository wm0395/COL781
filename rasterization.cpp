#include <bits/stdc++.h>

using namespace std;

float isLeft(float ax, float ay, float bx, float by, float px, float py)
{
    float val = (py-ay)*(bx-ax) + (px-ax)*(ay-by);
    return val;
}

bool inTriangle(float ax, float ay, float bx, float by, float cx, float cy, float px, float py)
{
    if ((isLeft(ax,ay,bx,by,px,py)>=0) && (isLeft(bx,by,cx,cy,px,py)>=0) && (isLeft(cx,cy,ax,ay,px,py)>=0)) return true;
    if ((isLeft(ax,ay,bx,by,px,py)<=0) && (isLeft(bx,by,cx,cy,px,py)<=0) && (isLeft(cx,cy,ax,ay,px,py)<=0)) return true;
    return false;
}

void fillTriangle1(float ax, float ay, float bx, float by, float cx, float cy)
{
    float right = max(ax,bx);
    right = max(right, cx);
    int rt = ceil(right);

    float top = max(ay,by);
    top = max(top, cy);
    int tp = ceil(top);

    vector<vector<int>> grid(tp, vector<int>(rt,0));

    for (int i = 0; i<tp; i++)
    {
        for (int j = 0; j<rt; j++)
        {
            float py = (float(i) + float(i+1))/2;
            float px = (float(j) + float(j+1))/2;
            if (inTriangle(ax,ay,bx,by,cx,cy,px,py)) grid[i][j] = 1;
        }
    }

    for (int i = 0; i<tp; i++)
    {
        for (int j = 0; j<rt; j++)
        {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void fillTriangle2(float ax, float ay, float bx, float by, float cx, float cy)
{
    float right = max(ax,bx);
    right = max(right, cx);
    int rt = ceil(right);

    float top = max(ay,by);
    top = max(top, cy);
    int tp = ceil(top);

    float left = min(ax,bx);
    left = min(left, cx);
    int lft = left;

    float bottom = min(ay,by);
    bottom = min(bottom, cy);
    int btm = bottom;

    vector<vector<int>> grid(tp, vector<int>(rt,0));

    for (int i = btm; i<tp; i++)
    {
        for (int j = lft; j<rt; j++)
        {
            float py = (float(i) + float(i+1))/2;
            float px = (float(j) + float(j+1))/2;
            if (inTriangle(ax,ay,bx,by,cx,cy,px,py)) grid[i][j] = 1;
        }
    }

    // for (int i = 0; i<tp; i++)
    // {
    //     for (int j = 0; j<rt; j++)
    //     {
    //         cout << grid[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
}

float lineCut(float ax, float ay, float bx, float by, float py)
{
    if (ay==by)
    {
        if (py==ay) return ax;
        return -1;
    }
    float val = (py-ay)*(bx-ax)/(by-ay);
    val += ax;
    return val;
}

void fillTriangle3(float ax, float ay, float bx, float by, float cx, float cy)
{
    float right = max(ax,bx);
    right = max(right, cx);
    int rt = ceil(right);

    float top = max(ay,by);
    top = max(top, cy);
    int tp = ceil(top);

    float left = min(ax,bx);
    left = min(left, cx);
    int lft = left;

    float bottom = min(ay,by);
    bottom = min(bottom, cy);
    int btm = bottom;


    vector<vector<int>> grid(tp, vector<int>(rt,0));

    for (int i = btm; i<tp; i++)
    {
        float py = (float(i) + float(i+1))/2;
        float first = lineCut(ax,ay,bx,by,py);
        float second = lineCut(bx,by,cx,cy,py);
        float third = lineCut(cx,cy,ax,ay,py);
        float start = -1.0;
        float end = -1.0;
        if (min(ax,bx)<=first && first<=max(ax,bx)) 
        {
            start = first;
        }
        if (min(bx,cx) <= second && second <= max(bx,cx))
        {
            if (start == -1) start = second;
            else end = second;
        }
        if (min(cx,ax) <= third && third <= max(cx,ax))
        {
            end = third;
        }

        if (end==-1) continue;
        if (end<start) swap(start,end);

        cout << start << " " << end << endl;

        int beg = int(start);
        int fin = int(end);
        float px1 = (float(beg)+float(beg+1))/2;
        float px2 = (float(fin)+float(fin+1))/2;
        if (inTriangle(ax,ay,bx,by,cx,cy,px1,py)) grid[i][beg] = 1;
        if (inTriangle(ax,ay,bx,by,cx,cy,px2,py)) grid[i][fin] = 1;
        for (int j = start+1; j<=end-1; j++) grid[i][j] = 1;
        for (int j = lft; j<rt; j++)
        {
            float py = (float(i) + float(i+1))/2;
            float px = (float(j) + float(j+1))/2;
            if (inTriangle(ax,ay,bx,by,cx,cy,px,py)) grid[i][j] = 1;
        }
    }

    // for (int i = 0; i<tp; i++)
    // {
    //     for (int j = 0; j<rt; j++)
    //     {
    //         cout << grid[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
}

int main()
{

    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the range for the random floating-point numbers
    float lower_bound = 0.0;
    float upper_bound = 10000.0;

    // Create a uniform distribution for the specified range
    std::uniform_real_distribution<float> distribution(lower_bound, upper_bound);

    // Generate and print a random floating-point number
    float ax = distribution(gen);
    float ay = distribution(gen);
    float bx = distribution(gen);
    float by = distribution(gen);
    float cx = distribution(gen);
    float cy = distribution(gen);

    cout << ax << " ," << ay << ", " << bx << ", " << by << ", " << cx << ", " << cy << endl;

    ax = 1.9;
    ay = 0.1;
    bx = 0.1;
    by = 1.9;
    cx = 29;
    cy = 29;

    auto start_time = std::chrono::high_resolution_clock::now();
    fillTriangle1(ax,ay,bx,by,cx,cy);
    auto time1 = std::chrono::high_resolution_clock::now();
    fillTriangle2(ax,ay,bx,by,cx,cy);
    auto time2 = std::chrono::high_resolution_clock::now();
    fillTriangle3(ax,ay,bx,by,cx,cy);
    auto end_time = std::chrono::high_resolution_clock::now();


    // Calculate the duration
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - start_time);
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1);
    auto duration3 = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - time2);

    // Print the time taken
    std::cout << "Time taken: " << duration1.count()<< " " << duration2.count() <<" " << duration3.count() << " milliseconds" << std::endl;
    return 0;
}