#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>

using std::cout;
using std::cin;
using std::fixed;
using std::setprecision;

class WeightConverter {
private:
    double kilograms;

public:
    WeightConverter() : kilograms(0.0) {}

    WeightConverter(double kg) {
        setKilograms(kg); 
    }

    void setKilograms(double kg) {
        if (kg >= 0) {
            kilograms = kg;
        }
        else {
            kilograms = 0.0;
            cout << "Warning: Weight cannot be negative. Set to 0 kg.\n";
        }
    }

    double getKilograms() const { 
        return kilograms;
    }

    double getPharmacyPounds() const { 
        return kilograms * 2.679;
    }

    double getPoods() const { 
        return kilograms / 16.38;
    }

    void print() const { 
        cout << fixed << setprecision(3);
        cout << "Weight: " << kilograms << " kg\n";
        cout << "In pharmacy pounds: " << getPharmacyPounds() << " lb\n";
        cout << "In poods: " << getPoods() << " pood\n";
    }
};

int main() {
    int a;
    double kg;
    WeightConverter weight;

    cout << "=== WEIGHT CONVERTER ===\n";
    cout << "(Pharmacy pounds and poods)\n\n";

    do {
        cout << "Menu:\n";
        cout << "1. Set weight in kilograms\n";
        cout << "2. Show weight in kilograms\n";
        cout << "3. Show weight in all units\n";
        cout << "0. Exit\n";
        cout << "Your choice: ";
        cin >> a;

        cout << '\n';

        switch (a) {
        case 1:
            cout << "Enter weight in kilograms: ";
            cin >> kg;
            weight.setKilograms(kg);
            cout << "Weight set successfully!\n";
            break;

        case 2:
            cout << "Weight in kilograms: " << weight.getKilograms() << " kg\n";
            break;

        case 3:
            weight.print();
            break;

        case 0:
            cout << "Goodbye!\n";
            break;

        default:
            cout << "Wrong choice! Please select 0-3.\n";
        }

        cout << '\n';
    } while (a != 0);

    return 0;
}