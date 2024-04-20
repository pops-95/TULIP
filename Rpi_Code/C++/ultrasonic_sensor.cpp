#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <eigen3/Eigen/Dense>

#define TRIG_PIN 6  // GPIO pin for trigger
#define ECHO_PIN 5  // GPIO pin for echo

using namespace std;



class KalmanFilter {
private:
    // State variables
    Eigen::VectorXd x;  // State vector
    Eigen::MatrixXd P;  // State covariance matrix

    // Kalman filter parameters
    Eigen::MatrixXd A;  // State transition matrix
    Eigen::MatrixXd H;  // Measurement matrix
    Eigen::MatrixXd Q;  // Process noise covariance matrix
    Eigen::MatrixXd R;  // Measurement noise covariance matrix
    Eigen::MatrixXd I;  // Identity matrix

public:
    KalmanFilter(double initialReading, double initialVariance, double processNoise, double measurementNoise) {
        // Initialize state vector
        x = Eigen::VectorXd(2);
        x << initialReading, 0;

        // Initialize state covariance matrix
        P = Eigen::MatrixXd(2, 2);
        P << initialVariance, 0,
             0, initialVariance;

        // Initialize Kalman filter parameters
        A = Eigen::MatrixXd(2, 2);
        A << 1, 1,
             0, 1;

        H = Eigen::MatrixXd(1, 2);
        H << 1, 0;

        Q = Eigen::MatrixXd(2, 2);
        Q << processNoise, 0,
             0, processNoise;

        R = Eigen::MatrixXd(1, 1);
        R << measurementNoise;

        I = Eigen::MatrixXd::Identity(2, 2);
    }

    double filter(double measurement) {
  // Prediction
     // Prediction
    x = A * x;
    P = A * P * A.transpose() + Q;

    // Kalman gain calculation
    Eigen::MatrixXd K = P * H.transpose() * (H * P * H.transpose() + R).inverse();

    // Update
    Eigen::VectorXd hx = H * x;  // Compute predicted measurement
    double residual = measurement - hx(0); // Compute measurement residual
    x = x + K * residual;
    P = (I - K * H) * P;

    return x(0); // Return filtered reading
    }
};


// Function to set GPIO pin direction
void setDirection(int pin, bool isOutput) {
    ofstream directionFile("/sys/class/gpio/gpio" + to_string(pin) + "/direction");
    if (isOutput)
        directionFile << "out";
    else
        directionFile << "in";
    directionFile.close();
}

// Function to write value to GPIO pin
void writeValue(int pin, int value) {
    ofstream valueFile("/sys/class/gpio/gpio" + to_string(pin) + "/value");
    valueFile << value;
    valueFile.close();
}

// Function to read value from GPIO pin
int readValue(int pin) {
    ifstream valueFile("/sys/class/gpio/gpio" + to_string(pin) + "/value");
    int value;
    valueFile >> value;
    valueFile.close();
    return value;
}

// Function to get distance from ultrasonic sensor
float getDistance() {
    // Initialize GPIO pins
    setDirection(TRIG_PIN, true);  // TRIG pin as output
    setDirection(ECHO_PIN, false); // ECHO pin as input

    // Trigger ultrasonic sensor
    writeValue(TRIG_PIN, 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    writeValue(TRIG_PIN, 1);
    std::this_thread::sleep_for(std::chrono::microseconds(10));
    writeValue(TRIG_PIN, 0);

    // Wait for echo
    auto start = std::chrono::steady_clock::now();
    while (readValue(ECHO_PIN) == 0) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() > 1000) {
            cout << "Timeout occurred!" << endl;
            return -1.0; // Return -1 if timeout occurs
        }
    }

    auto startTime = std::chrono::steady_clock::now();

    // Wait for echo end
    while (readValue(ECHO_PIN) == 1) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count() > 1000) {
            cout << "Timeout occurred!" << endl;
            return -1.0; // Return -1 if timeout occurs
        }
    }

    auto endTime = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);

    // Speed of sound: 343m/s = 34300cm/s = 0.0343cm/microsecond
    // Distance = (Time elapsed * Speed of sound) / 2
    float distance = (duration.count() * 0.0350) / 2;

    return distance;
}

int main() {
    int total_point=10;
    float total_dis[total_point];
    float sum=0;
    float prev_dis=0;
    double initialReading = 10.0;
    double initialVariance = 1.0;
    double processNoise = 0.01;
    double measurementNoise = 5.0;
    float filtered_dis[total_point];
    KalmanFilter filter(initialReading, initialVariance, processNoise, measurementNoise);


    while (true) {
        for(int i=0;i<=(total_point-1);i++){
            total_dis[i] = getDistance();
            
         }

        
        
        
        // float avg_dis=getDistance();
        
        // if (avg_dis != -1.0) {
        //     cout << "Distance: " << avg_dis<< " cm" << "\t" << " Diff : " <<(avg_dis-prev_dis) << "\n";
        // }

         for (double reading : total_dis) {
        double filteredReading = filter.filter(reading);
        // std::cout << ", Filtered reading: " << filteredReading << std::endl;
        sum=sum+reading;
         }

     float avg_dis=sum/total_point;
    std::cout << ", Filtered reading: " << avg_dis << std::endl;

        sum=0;
        // prev_dis=avg_dis;
        // std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay 1 second
    }
    return 0;
}

