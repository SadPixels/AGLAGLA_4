#include <iostream>
#include <valarray>
#include <vector>
#include <iomanip>


#ifdef WIN32
#define GNUPLOT_NAME "E:\\gnuplot\\bin\\gnuplot -persist"
#else
#define GNUPLOT_NAME "gnuplot -persist"
#endif

using namespace std;


double calculate_v_t(double alpha_1, double alpha_2, double beta_1, double beta_2, double v_0, double k_0, double t) {
    v_0 -= alpha_2 / beta_2;
    k_0 -= alpha_1 / beta_1;
    double res =v_0 * cos(t * sqrt(alpha_1 * alpha_2))
    - (k_0 * sqrt(alpha_2) * beta_1 * sin(t * sqrt( alpha_1 * alpha_2)))/ (beta_2 * sqrt(alpha_1));
    res += alpha_2 / beta_2;
    return res;
}

double calculate_k_t(double alpha_1, double alpha_2, double beta_1, double beta_2, double v_0, double k_0, double t) {
    v_0 -= alpha_2 / beta_2;
    k_0 -= alpha_1 / beta_1;
    double res = v_0 * sqrt(alpha_1) * beta_2 * sin(t* sqrt(alpha_1 * alpha_2)) / (beta_1 * sqrt(alpha_2))
    + k_0 * cos(t * sqrt(alpha_1 * alpha_2));
    res += alpha_1 / beta_1;
    return res;
}

int main() {
#ifdef WIN32
    FILE* pipe = _popen(GNUPLOT_NAME, "w");
#else
    FILE* pipe = popen(GNUPLOT_NAME, "w");
#endif
    double alpha_1, alpha_2, beta_1, beta_2, t, step, cur_time;
    int v_0, k_0, n;
//    cin >> v_0 >> k_0 >> alpha_1 >> beta_1 >> alpha_2 >> beta_2 >> t >> n;
    alpha_1 = 0.5;
    alpha_2 = 0.45;
    beta_1 = 0.009;
    beta_2 = 0.01;
    v_0 = 80;
    k_0 = 60;
    t = 100;
    n = 800;
    step = t / n;
    fprintf(pipe, "v_0=%lf\nk_0=%lf\nalpha_1=%lf\nbeta_1=%lf\nalpha_2=%lf\nbeta_2=%lf\nT=%lf\nN=%lf\n",
            double(v_0), double(k_0), alpha_1, beta_1, alpha_2, beta_2, t, double(n));
    fprintf(pipe, "v(t) = (v_0 - alpha_2 / beta_2) * cos(t * sqrt(alpha_1 * alpha_2))"
                  " - (k_0 - alpha_1 / beta_1) * sqrt(alpha_2) * beta_1 * sin(t * sqrt(alpha_1 * alpha_2)) / beta_2 * sqrt(alpha_1)"
                  "+ alpha_2 / beta_2\n");
    fprintf(pipe, "k(t) = (k_0 - alpha_1 / beta_1) * cos(t * sqrt(alpha_1 * alpha_2))"
                  "+ (v_0 - alpha_2 / beta_2) * sqrt(alpha_1) * beta_2 * sin(t * sqrt(alpha_1 * alpha_2)) / beta_1 * sqrt(alpha_2)"
                  "+ alpha_1 / beta_1\n");
    fprintf(pipe, "plot [0 : 100] [0 : 100] v(x) title\"v(t)\", k(x) title\"k(t)\"\n");
    vector<double> time_values = *new vector<double>;
    time_values.push_back(0);
    vector<double> victim_values = *new vector<double>;
    victim_values.push_back(v_0);
    vector<double> killer_values = *new vector<double>;
    killer_values.push_back(k_0);
    cur_time = 0;
    for (int i = 0; i < n; i++) {
        cur_time += step;
        time_values.push_back(cur_time);
        victim_values.push_back(calculate_v_t(alpha_1, alpha_2, beta_1, beta_2, v_0, k_0, cur_time));
        killer_values.push_back(calculate_k_t(alpha_1, alpha_2, beta_1, beta_2, v_0, k_0, cur_time));
    }
    fprintf(pipe, "plot [0 : 100] [0 : 100] '-' with lines title\"v(k)\"\n");
    for (int i = 0; i <= n; ++i)
        fprintf(pipe, "%lf %lf\n", killer_values[i], victim_values[i]);
    fprintf(pipe, "e\n");
#ifdef WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
//    cout << "t:" << endl;
//    for (int i = 0; i <= n; i++) {
//        cout << fixed << setprecision(2) << time_values[i] << " ";
//    }
//    cout << endl;
//    cout << "v:" << endl;
//    for (int i = 0; i <= n; i++) {
//        cout << fixed << setprecision(2) << victim_values[i] << " ";
//    }
//    cout << endl;
//    cout << "k:" << endl;
//    for (int i = 0; i <= n; i++) {
//        cout << fixed << setprecision(2) << killer_values[i] << " ";
//    }
//    cout << endl;
    return 0;
}
