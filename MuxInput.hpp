/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                    */
/*    FILE: MuxInput.hpp                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef MUXINPUT_H
#define MUXINPUT_H

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <vector>
#include <memory>

class Mux; // forward declaration to prevent generating circular referances

class MuxInputBase {
    public:
        MuxInputBase() {};
        static std::unique_ptr<MuxInputBase> muxInputFactory (rapidjson::Value &d);
        virtual bool isTrue() = 0;
        std::string getVar() {return var;};
        virtual bool procMail (CMOOSMsg &msg) = 0;
        virtual bool subscribe(Mux *m) = 0;
        ~MuxInputBase() {};
    protected:
        std::string var;
};

class MuxInputString : public MuxInputBase {
    public:
        MuxInputString() {};
        static std::unique_ptr<MuxInputString> muxInputStringFactory (rapidjson::Value &d);
        const std::string &getVal() {return val;};
        bool isTrue();
        virtual bool procMail (CMOOSMsg &msg) = 0;
        virtual bool subscribe(Mux *m) = 0;
        ~MuxInputString() {};
    protected:
        std::string val;
};

class MuxInputStringVar : public MuxInputString {
    public:
        MuxInputStringVar(rapidjson::Value &d);
        bool procMail(CMOOSMsg &msg);
        bool subscribe(Mux *m);
        ~MuxInputStringVar() {};
};

class MuxInputStringConst : public MuxInputString {
    public:
        MuxInputStringConst(rapidjson::Value &d);
        bool procMail (CMOOSMsg &msg) {return false;};
        bool subscribe(Mux *m) {return true;};
        ~MuxInputStringConst() {};
};

class MuxInputDouble : public MuxInputBase {
    public:
        static std::unique_ptr<MuxInputDouble> muxInputDoubleFactory (rapidjson::Value &d);
        const double &getVal() {return val;};
        bool isTrue() {return (val > threshold);};
        virtual bool procMail (CMOOSMsg &msg) = 0;
        virtual bool subscribe(Mux *m) = 0;
    protected:
        double val = 0;
        double threshold = 0.5;
};

class MuxInputDoubleVar : public MuxInputDouble {
    public:
        MuxInputDoubleVar(rapidjson::Value &d);
        bool procMail(CMOOSMsg &msg);
        bool subscribe(Mux *m);
        ~MuxInputDoubleVar() {};
};

class MuxInputDoubleConst : public MuxInputDouble{
    public:
        MuxInputDoubleConst(rapidjson::Value &d);
        bool procMail (CMOOSMsg &msg) {return false;};
        bool subscribe(Mux *m) {return true;};
        ~MuxInputDoubleConst() {};
};

class MuxInputBinary : public MuxInputBase {
    public:
        static std::unique_ptr<MuxInputBinary> muxInputBinaryFactory (rapidjson::Value &d);
        const std::vector<uint8_t> &getVal() {return val;};
        bool isTrue();
        virtual bool procMail (CMOOSMsg &msg) = 0;
        virtual bool subscribe(Mux *m) = 0;
    protected:
        bool loadString(rapidjson::Value &d);
        bool loadArray(rapidjson::Value &d);
        std::vector<uint8_t> val;
};

class MuxInputBinaryVar : public MuxInputBinary {
    public:
        MuxInputBinaryVar(rapidjson::Value &d);
        bool procMail(CMOOSMsg &msg);
        bool subscribe(Mux *m);
        ~MuxInputBinaryVar() {};
};

class MuxInputBinaryConst : public MuxInputBinary {
    public:
        MuxInputBinaryConst(rapidjson::Value &d);
        bool procMail (CMOOSMsg &msg) {return false;};
        bool subscribe(Mux *m) {return true;};
        ~MuxInputBinaryConst() {};
};

#endif
