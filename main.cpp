```cpp
#include <iostream>   // Allows the program to display output using cout
#include "capacitor.h" // Includes the Capacitor struct definition

using namespace std;

int main()
{
    // Define the simulation time step and total simulation duration.
    // dt = 0.1 nanoseconds; final_time = 5 microseconds.
    const double dt = 1e-10;
    const double final_time = 5e-6;

    // Calculate the number of time steps needed to reach final_time.
    // static_cast<int> converts the result to an integer.
    int steps = static_cast<int>(final_time / dt);

    // Initial conditions for the constant-current source circuit.
    double Ccurrent_I0 = 0.01; // Initial current: 0.01 A
    double Ccurrent_V0 = 0;    // Initial capacitor voltage: 0 V

    // Initial conditions and resistance for the constant-voltage source circuit.
    double Cvoltage_V0 = 10;   // Voltage supplied by the source: 10 V
    double Cvoltage_R = 1000;  // Resistance: 1000 ohms

    // Calculate the initial current using Ohm's law: I = V / R.
    double Cvoltage_I0 = (Cvoltage_V0 / Cvoltage_R);

    // Create two Capacitor objects to store the simulation results.
    Capacitor Ccurrent; // Circuit powered by a constant-current source
    Capacitor Cvoltage; // Circuit powered by a constant-voltage source

    // Dynamically allocate arrays to store time, voltage, and current
    // at every time step. steps + 1 includes the initial time at index 0.
    Ccurrent.time = new double[steps + 1];
    Ccurrent.voltage = new double[steps + 1];
    Ccurrent.current = new double[steps + 1];

    Cvoltage.time = new double[steps + 1];
    Cvoltage.voltage = new double[steps + 1];
    Cvoltage.current = new double[steps + 1];

    // Set the initial values for the constant-current circuit.
    Ccurrent.time[0] = 0;
    Ccurrent.voltage[0] = Ccurrent_V0;
    Ccurrent.current[0] = Ccurrent_I0;

    // Set the initial values for the constant-voltage circuit.
    Cvoltage.time[0] = 0;
    Cvoltage.voltage[0] = 0; // Capacitor starts uncharged
    Cvoltage.current[0] = Cvoltage_I0;

    // Set the capacitance of both capacitors to 100 picofarads.
    Ccurrent.C = 100e-12;
    Cvoltage.C = 100e-12;

    // Simulate the circuit powered by a constant-current source.
    for (int i = 1; i <= steps; ++i)
    {
        // Calculate the current simulation time.
        double timeStep = i * dt;
        Ccurrent.time[i] = timeStep;

        // Update the capacitor voltage using I = C * dV/dt.
        // Rearranged: new voltage = previous voltage + (current * dt / C).
        Ccurrent.voltage[i] = Ccurrent.voltage[i-1] + ((Ccurrent.current[i-1] * dt) / Ccurrent.C);

        // The current remains constant throughout this simulation.
        Ccurrent.current[i] = Ccurrent_I0;
    }

    // Simulate the circuit powered by a constant-voltage source
    // connected in series with a resistor.
    for (int i = 1; i <= steps; ++i)
    {
        // Calculate the current simulation time.
        double timeStep = i * dt;
        Cvoltage.time[i] = timeStep;

        // Update the current as the capacitor charges.
        // The current decreases because the voltage across the
        // resistor decreases as the capacitor voltage increases.
        Cvoltage.current[i] = Cvoltage.current[i-1] - (Cvoltage.current[i-1] / (Cvoltage_R * Cvoltage.C)) * dt;

        // Use Kirchhoff's voltage law: source voltage =
        // capacitor voltage + resistor voltage.
        // Rearranged using Ohm's law: Vc = Vs - (I * R).
        Cvoltage.voltage[i] = Cvoltage_V0 - (Cvoltage.current[i] * Cvoltage_R);
    }

    // Print column headings for the simulation results.
    cout << "Time|\tConstant V Source|\tConstant C Source|"
         << "\tConstant V Current|\tConstant C Current\n";

    // Loop through every stored time step to display selected results.
    for (int i = 0; i <= steps; ++i)
    {
        // Print only every 200th time step to keep the output manageable.
        // The % operator gives the remainder; a remainder of 0 means
        // that i is divisible by 200. This also prints the initial state.
        if (i % 200 == 0)
        {
            cout << Cvoltage.time[i] << "s           |\t";
            cout << Cvoltage.voltage[i] << "V             |\t";
            cout << Ccurrent.voltage[i] << "V             |\t";
            cout << Cvoltage.current[i] << "A                |\t";
            cout << Ccurrent.current[i] << "A\n";
        }
    }

    // Release the dynamically allocated memory for the first circuit.
    delete[] Ccurrent.time;
    delete[] Ccurrent.voltage;
    delete[] Ccurrent.current;
    delete[] Ccurrent;

    // Release the dynamically allocated memory for the second circuit.
    delete[] Cvoltage.time;
    delete[] Cvoltage.voltage;
    delete[] Cvoltage.cu6rrent;
    delete[] Cvoltage;

    // End of main; returning 0 indicates successful program execution.
    return 0;
}
```
