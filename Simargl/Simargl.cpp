#include <iostream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
extern "C" {
#include <modbus.h> 
}

using std::cout;
using std::cin;

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

int main()
{
    modbus_t* activeController = CreateController(); // Give our modbus server
    if (activeController == NULL)
        return 0;  // We can't received modbus server
}
