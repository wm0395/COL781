#include <vector>
#include <utility>
#include <iostream>

#include "triangle_raster.hpp"

using namespace std;

// MAIN FUNCTIONS
// const int N = 10;
// template<typename T>

// bool isLeft(pair<T,T> &a, pair<T,T> &b, pair<T,T> &p){
//     pair<int,int> n = make_pair(a.second-b.second, b.first - a.first), d = p - a;
//     return (dot(n, d) >= 0);
// }

// void make_anti_clockwise(vector<pair<int,int>> &points){
//     if(!isLeft(points[0], points[1], points[2])){
//         swap(points[0], points[1]);
//     }
// }

// void make_ordered(vector<pair<int,int>> &points){
//     pair<int,int> a = points[0];
//     pair<int,int> b = points[1];
//     pair<int,int> c = points[2];

//     if(c.second>= a.second && c.second >= b.second){
//         return;
//     }
//     else if(b.second >= a.second){
//         cycle_aw(points[0], points[1], points[2]);
//     }
//     else{
//         cycle_cw(points[0], points[1], points[2]);
//     }
// }

// bool isInside(pair<int,int> &a, pair<int,int> &b, pair<int,int> &c, pair<int,int> p){
//     // cout<<"(" << isLeft(a, b, p) << isLeft(b, c, p) << isLeft(c, a, p)<<")";
//     return isLeft(a, b, p) && isLeft(b, c, p) && isLeft(c, a, p);
// }

// void print_inline(vector<pair<int,int>> &points){
//     for(int j = 0; j<N; j++){
//         for(int i = 0; i<N; i++){
//             pair<int,int> p = make_pair(i,j);
//             if(isInside(points[0], points[1], points[2], p)){
//                 cout << "[]";
//             }
//             else{
//                 cout << "..";
//             }
//         }
//         cout << endl;
//     }
// }

// TRIANGLE RASTERIZATION

class line{
    public:
    pair<float,float> a, b;
    line(){
        a = make_pair(0,0);
        b = make_pair(0,0);
    }
    line(pair<float,float> a, pair<float,float> b){
        this->a = a;
        this->b = b;
    }

    bool isLeft(pair<float,float> &p){
        pair<float,float> n = make_pair(a.second - b.second, b.first - a.first);
        pair<float,float> d = p - a;
        // cout<<"dot:"<<n<<d<<dot(n, d)<<endl;
        return (dot(n, d) >= 0);
    }

    float intercept_x(float y){
        if(a.second == b.second){
            return min(a.first,b.first)-1;
        }
        else{
            return a.first + (y - a.second)*(b.first - a.first)/(b.second - a.second);
        }
    }

    float intercept_y(float x){
        if(a.first == b.first){
            return min(a.second, b.second)-1;
        }
        else{
            return a.second + (x - a.first)*(b.second - a.second)/(b.first - a.first);
        }
    }
};

class triangle{
    public:
    pair<float,float> a, b, c;
    line A, B, C;

    triangle(){
        a = make_pair(0,0);
        b = make_pair(0,0);
        c = make_pair(0,0);
        A = line(a, b);
        B = line(b, c);
        C = line(c, a);
    }
    triangle(pair<float, float> a, pair<float, float> b, pair<float, float> c){
        this->a = a;
        this->b = b;
        this->c = c;
        make_anti_clockwise();
        A = line(this->a, this->b);
        B = line(this->b, this->c);
        C = line(this->c, this->a);
        cout<<"triangle: "<<this->a<<" "<<this->b<<" "<<this->c<<endl;
    }
    
    bool isInside(pair<float,float> p){
        return A.isLeft(p) && B.isLeft(p) && C.isLeft(p);
    }

    private:
    void make_anti_clockwise(){
        line A = line(this->a, this->b);
        if(!A.isLeft(this->c)){
            swap(this->a, this->b);
        }
    }
};

class screen{
    public:
    int width, height;
    vector<vector<float>> pixels;
    screen(int w, int h){
        width = w;
        height = h;
        pixels = vector<vector<float>>(h, vector<float>(w, 0));
    }

    // COLOUR
    // . : o # @
    const vector<char> colors = {' ', '.', ':', 'o', 'O', '0', '#', '@'};

    void print(){
        cout<<"Screen: "<<width<<"x"<<height<<endl;
        for(int i = -1; i<width; i++){
            cout<<"__";
        }
        cout<<endl;
        for(int j = 0; j<height; j++){
            cout<<'|';
            for(int i = 0; i<width; i++){
                int color = min(pixels[height-1-j][i]*colors.size(), colors.size()-1.0f); 
                cout<< colors[color] <<" ";
            }
            cout<<"|"<<endl;
        }
        for(int i = -1; i<width; i++){
            cout<<"--";
        }
        cout<<endl;
    }
    void clear(){
        for(int i = 0; i<height; i++){
            for(int j = 0; j<width; j++){
                pixels[i][j] = 0;
            }
        }
    }
    void rasterize(triangle &T, string cmp = "inline", float color = 1, int sr = 1){
        if(cmp == "inline"){
            rasterize_inline(T, color);
        }
        else if(cmp == "tile"){
            rasterize_tile(T, color);
        }
        else if(cmp == "incremental"){
            rasterize_incremental(T, color);
        }
        else if(cmp == "anti_alias"){
            rasterize_aa(T, sr, color);
        }
        else{
            throw runtime_error("\""+ cmp+ "\" is not a valid comparison method.");
        }
    }

    private:
    void set_pixel(int x, int y, float val){
        pixels[y][x] = val;
    }
    void rasterize_inline(triangle &T, float color = 1){
        for(int j = 0; j < height; j++){
            for(int i = 0; i < width; i++){
                if(T.isInside(make_pair(i,j))){
                    set_pixel(i, j, color);
                }
            }
        }
    }
    void rasterize_tile(triangle &T, float color = 1){
        int x_min = min(T.a.first, min(T.b.first, T.c.first));
        int x_max = max(T.a.first, max(T.b.first, T.c.first));
        int y_min = min(T.a.second, min(T.b.second, T.c.second));
        int y_max = max(T.a.second, max(T.b.second, T.c.second));

        for(int j = y_min; j <= y_max; j++){
            for(int i = x_min; i <= x_max; i++){
                pair<int,int> p = make_pair(i,j);
                if(T.isInside(p)){
                    set_pixel(i, j, color);
                }
            }
        }
    }
    void rasterize_incremental(triangle &T, float color = 1){
        int x_min, x_max;
        int y_min = min(T.a.second, min(T.b.second, T.c.second));
        int y_max = max(T.a.second, max(T.b.second, T.c.second));

        for(int j = y_min; j <= y_max; j++){
            int l1 = T.A.intercept_x(j), l2 = T.B.intercept_x(j), l3 = T.C.intercept_x(j);
            if(l1< min(T.a.first, T.b.first) || l1>max(T.a.first, T.b.first)){
                x_min = min(l2, l3);
                x_max = max(l2, l3);
            }
            else if(l2< min(T.b.first, T.c.first) || l2>max(T.b.first, T.c.first)){
                x_min = min(l1, l3);
                x_max = max(l1, l3);
            }
            else{
                x_min = min(l1, l2);
                x_max = max(l1, l2);
            }
            if(x_min > 0 && T.isInside(make_pair(x_min-1, j))){
                x_min--;
            }
            else if(!T.isInside(make_pair(x_min, j))){
                x_min++;
            }
            if(x_max < width-1 && T.isInside(make_pair(x_max+1, j))){
                x_max++;
            }
            else if(!T.isInside(make_pair(x_max, j))){
                x_max--;
            }
            for(int i = x_min; i <= x_max; i++){
                set_pixel(i, j, color);
            }
        }
    }
    
    float sample_aa(int i, int j, int sr, float &color, triangle &T){
            float sum = 0;
            for(int k = 0; k < sr; k++){
                for(int l = 0; l < sr; l++){
                    float x = i + (float)k/sr;
                    float y = j + (float)l/sr;
                    if(T.isInside(make_pair(x, y))){
                        sum += color;
                    }
                    else{
                        sum += pixels[y][x];
                    }
                }
            }
            return sum/(sr*sr);
        }

    void rasterize_aa(triangle &T, int sr = 1, float color = 1){
        int x_min, x_max;
        int y_min = min(T.a.second, min(T.b.second, T.c.second));
        int y_max = max(T.a.second, max(T.b.second, T.c.second));

        const float sample_centre = 0.5;

        for(int j = y_min; j <= y_max; j++){
            int l1 = T.A.intercept_x(j + sample_centre), l2 = T.B.intercept_x(j + sample_centre), l3 = T.C.intercept_x(j + sample_centre);
            if(l1< min(T.a.first, T.b.first) || l1>max(T.a.first, T.b.first)){
                x_min = min(l2, l3);
                x_max = max(l2, l3);
            }
            else if(l2< min(T.b.first, T.c.first) || l2>max(T.b.first, T.c.first)){
                x_min = min(l1, l3);
                x_max = max(l1, l3);
            }
            else{
                x_min = min(l1, l2);
                x_max = max(l1, l2);
            }
            if(x_min > 0 && T.isInside(make_pair(x_min-1, j))){
                x_min--;
            }
            // else if(!T.isInside(make_pair(x_min, j))){
            //     x_min++;
            // }
            if(x_max < width-1 && T.isInside(make_pair(x_max+1, j))){
                x_max++;
            }
            // else if(!T.isInside(make_pair(x_max, j))){
            //     x_max--;
            // }
            for(int i = x_min; i <= x_max; i++){
                float c = sample_aa(i, j, sr, color, T);
                set_pixel(i, j, c);
            }
        }
        
    }
};

int main(){
    // vector<pair<int,int>> points(3, pair<int,int>(0,0));
    // for(int i = 0; i < 3; i++){
    //     cin >> points[i].first >> points[i].second;
    // }

    // cout<<points[0].first<<" "<<points[0].second<<"\t"<<points[1].first<<" "<<points[1].second<<"\t"<<points[2].first<<" "<<points[2].second<<endl;
    // make_anti_clockwise(points);
    
    // cout<<points[0].first<<" "<<points[0].second<<"\t"<<points[1].first<<" "<<points[1].second<<"\t"<<points[2].first<<" "<<points[2].second<<endl;
    // print_inline(points);
    
    screen S(20, 20);
    triangle T1(make_pair(0,0), make_pair(11,19), make_pair(10,4));
    triangle T2(make_pair(11,18), make_pair(14,19), make_pair(19,4));

    // cout<<T.isInside(make_pair(0,0))<<"\n"<<T.isInside(make_pair(1,1))<<endl;

    // S.rasterize(T,"inline");
    // S.print();
    // S.clear();
    S.rasterize(T1,"incremental", 0.5f);
    S.rasterize(T2,"incremental", 0.75f);
    S.print();
    S.clear();
    S.rasterize(T1,"anti_alias",0.5f,4);
    S.rasterize(T2,"anti_alias",0.75f,4);
    S.print();
    return 0;
}