/************************************************************/
/*    NAME:                                               */
/*    ORGN: Project Ladon                                    */
/*    FILE: MuxBlock.hpp                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef MUXBLOCK_H
#define MUXBLOCK_H

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "MuxInput.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <string>
#include <list>

class MuxBlockBase {
    public:
        MuxBlockBase() {};
        static std::unique_ptr<MuxBlockBase> muxBlockFactory (rapidjson::Value &d);
        bool procMail (CMOOSMsg &msg);
        bool subscribe(Mux *m) {return (switchItem->subscribe(m) && trueVal->subscribe(m) && falseVal->subscribe(m));};
        std::list<std::string> buildReportHeader();
        virtual std::list<std::string> buildReportLines() = 0;
        virtual bool transmit(Mux *m) = 0;
        bool isValid() {return (switchItem && trueVal && falseVal);};
        ~MuxBlockBase() {};
    protected:
        std::string output;
        std::unique_ptr<MuxInputBase> switchItem;
        std::unique_ptr<MuxInputBase> trueVal;
        std::unique_ptr<MuxInputBase> falseVal;
        bool fresh;
};

class MuxBlockString : public MuxBlockBase {
    public:
        MuxBlockString(rapidjson::Value &d);
        std::list<std::string> buildReportLines();
        bool transmit(Mux *m);
        ~MuxBlockString() {};
    private:

};

class MuxBlockDouble : public MuxBlockBase {
    public:
        MuxBlockDouble(rapidjson::Value &d);
        std::list<std::string> buildReportLines();
        bool transmit(Mux *m);
        ~MuxBlockDouble() {};
};

class MuxBlockBinary : public MuxBlockBase {
    public:
        MuxBlockBinary(rapidjson::Value &d);
        std::list<std::string> buildReportLines();
        bool transmit(Mux *m);
        ~MuxBlockBinary() {};
};

#endif
