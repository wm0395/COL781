#include <vector>

using namespace std;

std::vector<std::vector<float>> product(std::vector<std::vector<float>> M, std::vector<std::vector<float>> N){
    int x = M.size();
    int y = N[0].size();
    int inner_dim = M[0].size();

    std::vector<std::vector<float>> result(x, std::vector<float>(y, 0));

    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            for(int k = 0; k < inner_dim; k++){
                result[i][j] += M[i][k] * N[k][j];
            }
        }
    }

    return result;
}

std::vector<float> product(std::vector<std::vector<float>> M, std::vector<float> N){
    int x = M.size();
    int y = N.size();
    int inner_dim = M[0].size();

    std::vector<float> result(x, 0);

    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            result[i] += M[i][j] * N[j];
        }
    }

    return result;
}

std::vector<float> product(std::vector<float> M, std::vector<std::vector<float>> N){
    int x = M.size();
    int y = N[0].size();
    int inner_dim = N.size();

    std::vector<float> result(y, 0);

    for(int i = 0; i < y; i++){
        for(int j = 0; j < x; j++){
            result[i] += M[j] * N[j][i];
        }
    }

    return result;
}

float product(std::vector<float> M, std::vector<float> N){
    int x = M.size();
    float result = 0;

    for(int i = 0; i < x; i++){
        result += M[i] * N[i];
    }

    return result;
}

std::vector<float> sum(std::vector<float> M, std::vector<float> N){
    int x = M.size();
    std::vector<float> result(x, 0);

    for(int i = 0; i < x; i++){
        result[i] = M[i] + N[i];
    }

    return result;
}

std::vector<float> diff(std::vector<float> M, std::vector<float> N){
    int x = M.size();
    std::vector<float> result(x, 0);

    for(int i = 0; i < x; i++){
        result[i] = M[i] - N[i];
    }

    return result;
}

std::vector<float> scale(std::vector<float> M, float s){
    int x = M.size();
    std::vector<float> result(x, 0);

    for(int i = 0; i < x; i++){
        result[i] = M[i] * s;
    }

    return result;
}