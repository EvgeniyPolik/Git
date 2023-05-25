#include <iostream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <map>
extern "C" {
#include <modbus.h> 
}
uint8_t inputRegisters[8];

using std::cout;
using std::cin;
using std::string;
using std::map;

modbus_t* CreateController() {  // method make modbus server
    modbus_t* controller;
    std::string ipAddress;


    do {
        cout << "Input correct ip address your Controller or \"q\" by exit\n";
        cin >> ipAddress;
        if (ipAddress == "q")
        {
            cout << "Thank you, goodbye";
            return NULL;
        }
        controller = modbus_new_tcp(ipAddress.c_str(), 502);
        if (controller == NULL) {  // There are any problems, may be error in libmodbus
            fprintf(stderr, "Can't using libmodbus, try again or exit");
            modbus_free(controller);
        }
        else if (modbus_connect(controller) == -1) {  // Can't established connection
            fprintf(stderr, "Cannection failed: %s\n", modbus_strerror(errno));
        }
    } while (controller == NULL || modbus_connect(controller) == -1);

    cout << "Connection established succesfull\n";
    return controller;
}


bool ExecuteCommand(string command, modbus_t* controller) {
    int resSwitchOn, resSwitchOff;
    map<string, int> comAddress = { {"1", 8256}, {"2", 8257} };
    resSwitchOn = modbus_write_bit(controller, comAddress[command], 1);
    resSwitchOff = modbus_write_bit(controller, comAddress[command], 0);
    return resSwitchOn == 1 && resSwitchOff == 1;
}

int ReadInputRegisters(modbus_t* controller) { // Discrite input registers read method
    int queryResult = 0;
    queryResult = modbus_read_input_bits(controller, 0, 8, inputRegisters);
    if (queryResult == -1) {
        fprintf(stderr, "Filed read status from modbus server %s\n", modbus_strerror(errno));
        return -1;
    }
    return queryResult;
}

void PrintStatusRegisters(int lengthAnswer) {
    for (int i = 0; i < lengthAnswer; i++)
        cout << (int)inputRegisters[i] << " ";
    cout << std::endl;
}

void Welcome(modbus_t* controller) {
    
    string userInp = "";
    while (true) {
        cout << "Input comand:\n" << "1) Open water\n" << "2) Close water\n" << "3) Get input status\n" << "Any comand to exit\n" << std::endl;
        cin >> userInp;
        if (userInp == "1" || userInp == "2")
            if (ExecuteCommand(userInp, controller))
                cout << "Succes";
            else
                cout << "Warning! command don't work";
        else if (userInp == "3") {
            int queryResult = ReadInputRegisters(controller);
            if (queryResult != -1)
                PrintStatusRegisters(queryResult);
        }
        else
            break;
    }
}

int main()
{
    modbus_t* activeController = CreateController(); // Give our modbus server
    if (activeController == NULL)
        return 0;  // We can't received modbus server
    Welcome(activeController);  // Start user dialog
}
