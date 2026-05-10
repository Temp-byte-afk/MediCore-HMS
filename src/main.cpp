#include <iostream>
#include "MediCoreSystem.h"
#include "SFMLApp.h"
#include "FileNotFoundException.h"
#include "HospitalException.h"

int main()
{
    try
    {
        MediCoreSystem backend;
        backend.initialize();

        SFMLApp app(backend, "assets/Roboto-Regular.ttf");
        app.run();
    }
    catch (const FileNotFoundException& ex)
    {
        std::cout << "Startup file error: " << ex.what() << "\n";
    }
    catch (const HospitalException& ex)
    {
        std::cout << "Hospital error: " << ex.what() << "\n";
    }
    catch (const std::exception& ex)
    {
        std::cout << "Standard error: " << ex.what() << "\n";
    }
    catch (...)
    {
        std::cout << "Unknown error occurred.\n";
    }

    return 0;
}