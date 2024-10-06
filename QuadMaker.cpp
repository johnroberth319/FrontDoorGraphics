#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

vector<float> bottomLeft, topLeft, topRight, bottomRight;
bool sliceMode = true; // True = Slice Bottom Right to Top Left;

bool checkPrecision(float var)
{
    int checkInt = (int)(var);
    float difference = ((var*1000.0) - (float(checkInt)*1000.0));
    if (difference == 0) {
        return false;
    } else {
        return true;
    }
}

bool loadValues() {
    ifstream file("values.txt");
    if (!file.is_open()) {
        cout << "Failed to open file" << endl;
        return false;
    }
    string line;
    bottomLeft.clear();
    topLeft.clear();
    topRight.clear();
    bottomRight.clear();
    int count = 0;
    int vector = 0;
    while (getline(file, line)) {
        if (vector == 0) {
            bottomLeft.push_back(stof(line));
            count++;
        } else if (vector == 1) {
            topLeft.push_back(stof(line));
            count++;
        } else if (vector == 2) {
            topRight.push_back(stof(line));
            count++;
        } else if (vector == 3) {
            bottomRight.push_back(stof(line));
            count++;
        }

        if (count >= 3) {
            vector++;
            count = 0;
        }
    }
    return true;
}

bool saveValues() {
    ofstream file("values.txt");
    if (!file.is_open()) {
        cout << "Failed to open file" << endl;
        return false;
    }
    
    for (int i = 0; i < bottomLeft.size(); i++) {
        file << bottomLeft[i] << endl;
    }
    for (int i = 0; i < topLeft.size(); i++) {
        file << topLeft[i] << endl;
    }
    for (int i = 0; i < topRight.size(); i++) {
        file << topRight[i] << endl;
    }
    for (int i = 0; i < bottomRight.size(); i++) {
        file << bottomRight[i] << endl;
    }

    /*
    for (int i = 0; i < bottomLeft.size(); i++) {
        file << fixed << setprecision(3) << bottomLeft[i] << endl;
    }
    for (int i = 0; i < topLeft.size(); i++) {
        file << fixed << setprecision(3) << topLeft[i] << endl;
    }
    for (int i = 0; i < topRight.size(); i++) {
        file << fixed << setprecision(3) << topRight[i] << endl;
    }
    for (int i = 0; i < bottomRight.size(); i++) {
        file << fixed << setprecision(3) << bottomRight[i] << endl;
    }
    */
    return true;
}

bool verifyFloat(string text) {
    try {
        float flPoint = stof(text);
        return true;
    } catch (const invalid_argument& e) {
        cout << "Invalid Float" << endl;
        return false;
    }

}

bool enterValues(int ID) {
    float x, y, z;
    string current;
    cout << "Enter x:" << endl;
    getline(cin, current);
    while (!verifyFloat(current)) {
        cout << "Enter x:" << endl;
        getline(cin, current);
    }
    x = stof(current);

    cout << "Enter y:" << endl;
    getline(cin, current);
    while (!verifyFloat(current)) {
        cout << "Enter y:" << endl;
        getline(cin, current);
    }
    y = stof(current);

    cout << "Enter z:" << endl;
    getline(cin, current);
    while (!verifyFloat(current)) {
        cout << "Enter z:" << endl;
        getline(cin, current);
    }
    z = stof(current);

    switch(ID) {
        case 0:
            bottomLeft.clear();
            bottomLeft.push_back(x);
            bottomLeft.push_back(y);
            bottomLeft.push_back(z);
            break;
        case 1:
            topLeft.clear();
            topLeft.push_back(x);
            topLeft.push_back(y);
            topLeft.push_back(z);
            break;
        case 2:
            topRight.clear();
            topRight.push_back(x);
            topRight.push_back(y);
            topRight.push_back(z);
            break;
        case 3:
            bottomRight.clear();
            bottomRight.push_back(x);
            bottomRight.push_back(y);
            bottomRight.push_back(z);
            break;
    }

    return true;
}

void outValues(){
    if (sliceMode) {
        cout << endl;
        for (int i = 0; i < bottomLeft.size(); i++) {
            if (!checkPrecision(bottomLeft[i])) {
                if (i < bottomLeft.size() - 1) {
                    cout << fixed << setprecision(3) << bottomLeft[i] << "f, ";
                } else {
                    cout << fixed << setprecision(3) << bottomLeft[i] << "f, 0.0f, 0.0f, " << endl;
                }
            } else {
                if (i < bottomLeft.size() - 1) {
                    cout << bottomLeft[i] << "f, ";
                } else {
                    cout << bottomLeft[i] << "f, 0.0f, 0.0f, " << endl;
                }
            }
        }
        for (int i = 0; i < topLeft.size(); i++) {
            if (!checkPrecision(topLeft[i])) {
                if (i < topLeft.size() - 1) {
                    cout << fixed << setprecision(3) << topLeft[i] << "f, ";
                } else {
                    cout << fixed << setprecision(3) << topLeft[i] << "f, 0.0f, 1.0f, " << endl;
                }
            } else {
                if (i < topLeft.size() - 1) {
                    cout << topLeft[i] << "f, ";
                } else {
                    cout << topLeft[i] << "f, 0.0f, 1.0f, " << endl;
                }
            }
        }
        for (int i = 0; i < topRight.size(); i++) {
            if (!checkPrecision(topRight[i])) {
                if (i < topRight.size() - 1) {
                    cout << fixed << setprecision(3) << topRight[i] << "f, ";
                } else {
                    cout << fixed << setprecision(3) << topRight[i] << "f, 1.0f, 1.0f, " << endl;
                }
            } else {
                if (i < topRight.size() - 1) {
                    cout << topRight[i] << "f, ";
                } else {
                    cout << topRight[i] << "f, 1.0f, 1.0f, " << endl;
                }
            }
        }
        cout << endl;
        for (int i = 0; i < bottomLeft.size(); i++) {
            if (!checkPrecision(bottomLeft[i])) {
                if (i < bottomLeft.size() - 1) {
                    cout << fixed << setprecision(3) << bottomLeft[i] << "f, ";
                } else {
                    cout << fixed << setprecision(3) << bottomLeft[i] << "f, 1.0f, 0.0f, " << endl;
                }
            } else {
                if (i < bottomLeft.size() - 1) {
                    cout << bottomLeft[i] << "f, ";
                } else {
                    cout << bottomLeft[i] << "f, 0.0f, 0.0f, " << endl;
                }
            }
        }
        for (int i = 0; i < bottomRight.size(); i++) {
            if (!checkPrecision(bottomRight[i])) {
                if (i < bottomRight.size() - 1) {
                    cout << fixed << setprecision(3) << bottomRight[i] << "f, ";
                } else {
                    cout << fixed << setprecision(3) << bottomRight[i] << "f, 0.0f, 0.0f, " << endl;
                }
            } else {
                if (i < bottomRight.size() - 1) {
                    cout << bottomRight[i] << "f, ";
                } else {
                    cout << bottomRight[i] << "f, 1.0f, 0.0f, " << endl;
                }
            }
        }
        for (int i = 0; i < topRight.size(); i++) {
            if (!checkPrecision(topRight[i])) {
                if (i < topRight.size() - 1) {
                    cout << fixed << setprecision(3) << topRight[i] << "f, ";
                } else {
                    cout << fixed << setprecision(3) << topRight[i] << "f, 1.0f, 1.0f, " << endl;
                }
            } else {
                if (i < topRight.size() - 1) {
                    cout << topRight[i] << "f, ";
                } else {
                    cout << topRight[i] << "f, 1.0f, 1.0f, " << endl;
                }
            }
        }
    } else {
        cout << endl;
        for (int i = 0; i < bottomLeft.size(); i++) {
            if (!checkPrecision(bottomLeft[i])) {
                if (i < bottomLeft.size() - 1) {
                    cout << fixed << setprecision(3) << bottomLeft[i] << "f, ";
                } else {
                    cout << fixed << setprecision(3) << bottomLeft[i] << "f, 1.0f, 0.0f, " << endl;
                }
            } else {
                if (i < bottomLeft.size() - 1) {
                    cout << bottomLeft[i] << "f, ";
                } else {
                    cout << bottomLeft[i] << "f, 0.0f, 0.0f, " << endl;
                }
            }
        }
        for (int i = 0; i < topLeft.size(); i++) {
            if (!checkPrecision(topLeft[i])) {
                if (i < topLeft.size() - 1) {
                    cout << fixed << setprecision(3) << topLeft[i] << "f, ";
                } else {
                    cout << fixed << setprecision(3) << topLeft[i] << "f, 0.0f, 1.0f, " << endl;
                }
            } else {
                if (i < topLeft.size() - 1) {
                    cout << topLeft[i] << "f, ";
                } else {
                    cout << topLeft[i] << "f, 0.0f, 1.0f, " << endl;
                }
            }
        }
        for (int i = 0; i < bottomRight.size(); i++) {
            if (!checkPrecision(bottomRight[i])) {
                if (i < bottomRight.size() - 1) {
                    cout << fixed << setprecision(3) << bottomRight[i] << "f, ";
                } else {
                    cout << fixed << setprecision(3) << bottomRight[i] << "f, 0.0f, 0.0f, " << endl;
                }
            } else {
                if (i < bottomRight.size() - 1) {
                    cout << bottomRight[i] << "f, ";
                } else {
                    cout << bottomRight[i] << "f, 1.0f, 0.0f, " << endl;
                }
            }
        }
        cout << endl;
        for (int i = 0; i < topLeft.size(); i++) {
            if (!checkPrecision(topLeft[i])) {
                if (i < topLeft.size() - 1) {
                    cout << fixed << setprecision(3) << topLeft[i] << "f, ";
                } else {
                    cout << fixed << setprecision(3) << topLeft[i] << "f, 0.0f, 1.0f, " << endl;
                }
            } else {
                if (i < topLeft.size() - 1) {
                    cout << topLeft[i] << "f, ";
                } else {
                    cout << topLeft[i] << "f, 0.0f, 1.0f, " << endl;
                }
            }
        }
        for (int i = 0; i < topRight.size(); i++) {
            if (!checkPrecision(topRight[i])) {
                if (i < topRight.size() - 1) {
                    cout << fixed << setprecision(3) << topRight[i] << "f, ";
                } else {
                    cout << fixed << setprecision(3) << topRight[i] << "f, 1.0f, 1.0f, " << endl;
                }
            } else {
                if (i < topRight.size() - 1) {
                    cout << topRight[i] << "f, ";
                } else {
                    cout << topRight[i] << "f, 1.0f, 1.0f, " << endl;
                }
            }
        }
        for (int i = 0; i < bottomRight.size(); i++) {
            if (!checkPrecision(bottomRight[i])) {
                if (i < bottomRight.size() - 1) {
                    cout << fixed << setprecision(3) << bottomRight[i] << "f, ";
                } else {
                    cout << fixed << setprecision(3) << bottomRight[i] << "f, 0.0f, 0.0f, " << endl;
                }
            } else {
                if (i < bottomRight.size() - 1) {
                    cout << bottomRight[i] << "f, ";
                } else {
                    cout << bottomRight[i] << "f, 1.0f, 0.0f, " << endl;
                }
            }
        }
    }
    
    /*
    for (int i = 0; i < bottomLeft.size(); i++) {
        if (i < bottomLeft.size() - 1) {
            cout << fixed << setprecision(3) << bottomLeft[i] << "f, ";
        } else {
            cout << fixed << setprecision(3) << bottomLeft[i] << "f, 0.0f, 0.0f, " << endl;
        }
    }
    */
}

int main() {

    bool exit = false;
    string input;

    while (!exit) {
        cout << "1: Enter Values" << endl <<
            "2: Output Values" << endl <<
            "3: Load Values" << endl <<
            "4: Save Values" << endl <<
            "5: Toggle Slice Mode" << endl <<
            "6: Exit" << endl;
        getline(cin, input);
        if (input == "1") {
            cout << "Which vector?" << endl <<
                "1: Bottom Left" << endl <<
                "2: Bottom Right" << endl <<
                "3: Top Left" << endl <<
                "4: Top Right" << endl;
            getline(cin, input);
            if (input == "1") {
                enterValues(0);
            } else if (input == "2") {
                enterValues(3);
            } else if (input == "3") {
                enterValues(1);
            } else if (input == "4") {
                enterValues(2);
            } else {
                outValues();
            }
        } else if (input == "2") {
            outValues();
        } else if (input == "3") {
            loadValues();
        } else if (input == "4") {
            saveValues();
        } else if (input == "5") {
            sliceMode = !sliceMode;
        } else if (input == "6") {
            exit = true;
        }
    }

    return 0;
}