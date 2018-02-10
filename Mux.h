/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                    */
/*    FILE: Mux.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef Mux_HEADER
#define Mux_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "MuxBlock.hpp"
#include <vector>
#include <memory>

class muxtest;  // Forward declaration to enable hooking in the test framework

class Mux : public AppCastingMOOSApp {
    friend muxtest;
    public:
        Mux() {};
        ~Mux() {};
        bool notify(const std::string &var, const std::string &val) {
     	   return Notify(var, val);
        };
        bool notify(const std::string &var, const double &val) {
     	   return Notify(var, val);
        };
        bool notify(const std::string &var, const std::vector<uint8_t> &val) {
     	   return Notify(var, val);
        };
        bool registerVar(const std::string &var) {return Register(var);};

    protected: // Standard MOOSApp functions to overload
        bool OnNewMail(MOOSMSG_LIST &NewMail);
        bool Iterate();
        bool OnConnectToServer();
        bool OnStartUp();

        // Standard AppCastingMOOSApp function to overload
        bool buildReport();
        void registerVariables();

        // Configuration variables

        // State variables
        std::vector<std::unique_ptr<MuxBlockBase>> blocks;
};

#endif
